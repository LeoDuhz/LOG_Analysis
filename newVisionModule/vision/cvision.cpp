#include "cvision.h"
#include <ctime>
#include "messages_robocup_ssl_wrapper.pb.h"
#include <QElapsedTimer>
#include "parammanager.h"
#include <fstream>
#include <sys/time.h>
#include <QDataStream>
#include "staticparams.h"
#include "networkinterfaces.h"

namespace {
auto zpm = ZSS::ZParamManager::instance();
auto vpm = ZSS::VParamManager::instance();
static double timepre;
static double interupt;
struct timeval tv;
}

Datastore::Datastore(QObject *parent)
    : QObject(parent),
      ZSPlugin("datasender"),
      udpReceive(),
      networkinterface(1) {
    ZNetworkInterfaces::instance()->updateInterfaces();
    declare_publish("imgdata");
    declare_publish("refereebox");
    udpSocketConnect(2);
}

void Datastore::udpSocketConnect(int sim) {
//    zpm->loadParam(communicateport, "AlertPorts/Vision4Sim", 10005);
    switch (sim) {
    case 0: zpm->loadParam(communicateport, "AlertPorts/Vision4Sim", 10020);break;
    case 1: zpm->loadParam(communicateport, "AlertPorts/Vision4Real", 10005);break;
    case 2:
        zpm->loadParam(communicateport, "AlertPorts/Vision4Log", 10008);
        refereebox.bind(QHostAddress::AnyIPv4, 10003, QUdpSocket::ShareAddress);
        break;
    }
    udpReceive.bind(QHostAddress::AnyIPv4, communicateport, QUdpSocket::ShareAddress);
    /*czkdebug*///qDebug() << "VisionPort : " << communicateport<<"Port "<<ZNetworkInterfaces::instance()->getFromIndex(networkinterface);
    refereebox.joinMulticastGroup(QHostAddress("224.5.23.1"),ZNetworkInterfaces::instance()->getFromIndex(0));
    udpReceive.joinMulticastGroup(QHostAddress("224.5.23.2"),ZNetworkInterfaces::instance()->getFromIndex(0));
    connect(&refereebox, SIGNAL(readyRead()), this, SLOT(sendReferee()), Qt::DirectConnection);
    std::cout<<"connect referee thread"<<std::endl;
    connect(&udpReceive, SIGNAL(readyRead()), this, SLOT(sendData()), Qt::DirectConnection);
    /*czkdebug*/std::cout<<"udp connected: "<<communicateport<<std::endl;
}

void Datastore::udpSocketDisconnect() {
    disconnect(&udpReceive);
    udpReceive.abort();
}

void Datastore::setinterface(int interface_new) {
    networkinterface = interface_new;
}

void Datastore::sendData() {
    static QByteArray datagram;
    while(udpReceive.hasPendingDatagrams()) {
        datagram.resize(udpReceive.pendingDatagramSize());
        udpReceive.readDatagram(datagram.data(), datagram.size());
        publish("imgdata", (void*)datagram.data(), datagram.size());
//        std::cout<<"send image"<<std::endl;
    }
}

void Datastore::sendReferee(){
    static QByteArray referee;
    while(refereebox.hasPendingDatagrams()) {
//        std::cout<<"send referee"<<std::endl;
        referee.resize(refereebox.pendingDatagramSize());
        refereebox.readDatagram(referee.data(), referee.size());
        publish("refereebox", (void*)referee.data(), referee.size());
        CvisionModule::instance()->dealwithreferee((void*)referee.data(), referee.size());
//        std::cout<<"send referee"<<std::endl;
//        Referee ssl_referee;
//        ssl_referee.ParseFromArray((void*)referee.data(), referee.size());
//        if (ssl_referee.has_command()){
//            const auto& command = ssl_referee.command();
//            std::cout << "send command"<<command<<std::endl;
//        }
    }
}

/***** LOG ******/
Rec_loader::Rec_loader():ZSPlugin("log"){
    const QString filename("../newVisionModule-for-ssl-master/2019-07-07_06-34_ER-Force-vs-ZJUNlict.log");
    framecount = 0;
    if(load_log(filename)){
        declare_publish("imgdata");
        std::cout<<"load successfully"<<std::endl;
    }
    else{
        std::cout<<"fuck"<<std::endl;
    }
}

bool Rec_loader::load_log(QString filename){
    replayFile = new QFile(filename);
    packets.clear();
    if(replayFile->open(QIODevice::ReadOnly)){
        QIODevice  *recIO = replayFile;
        QDataStream stream(recIO);
        while (!stream.atEnd()) {
            QByteArray packet;
            stream >> packet;
            packets.append(packet);
            framecount ++;
        }
        std::cout<<framecount<<std::endl;
        delete replayFile;
        return true;
    }
    return false;
}

void Rec_loader::run(){
    while(framecount){
        publish("imgdata", packets[framecount].data(), packets[framecount].size());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout<<"xiao fu I am coming!!!"<<std::endl;
        framecount --;
    }
}

/*****  数据处理 ******/
Cvision::Cvision() : ZSPlugin("datareciever") , ballnum(0), oneball(false) {
    for(int i=0;i<PARAM::ROBOTNUM;i++) {
        robots[0][i] = new Robot(CGeoPoint(-99999, -99999), 0);
        robotfilter[0][i] = new Kalmanfilter(CGeoPoint(-99999, -99999), 0);
        robotfilter[1][i] = new Kalmanfilter(CGeoPoint(-99999, -99999), 0);
        robots[1][i] = new Robot(CGeoPoint(-99999, -99999), 0);
    }
    for(int i=0;i<PARAM::BALLNUM;i++) {
        balls[i] = new Ball(CGeoPoint(-99999, -99999));
        ballfilter[i] = new Kalmanfilter(CGeoPoint(-99999, -99999));
        ballpaired[i] = 0;
    }
    switch (PARAM::CAMERA) {
    case 2:
        CameraCenter[0] = new CGeoPoint(2250, 0);
        CameraCenter[1] = new CGeoPoint(-2250, 0);
        break;
    default:
        break;
    }
    camerareset();
    timepre = tv.tv_usec;
    declare_receive("refereebox",false);
    declare_receive("imgdata",false);
    for(int i=0;i<PARAM::CAMERA;i++) updatetime[0][i] = 0;
}

void Cvision::run() {
    ZSData data;
    ZSData refe;
    while(true) {
//        std::cout<<try_receive("imgdata",data)<<std::endl;
        if(try_receive("refereebox",refe)) dealwithreferee(refe.data(), refe.size());
        if(try_receive("imgdata",data)) dealwithdata(data.data(), data.size());
    }
}

void Cvision::dealwithreferee(const void *ptr, int size){
    Referee ssl_referee;
//    std::cout<<"deal with referee"<<std::endl;
    ssl_referee.ParseFromArray(ptr,size);
    if (ssl_referee.has_command()){
        const auto& command = ssl_referee.command();
        unsigned long command_counter = ssl_referee.command_counter();
        const auto& yellow = ssl_referee.yellow();
        const auto& blue = ssl_referee.blue();
        unsigned char cmd_index = 0;
        std::cout<<"command"<<command<<std::endl;
        //command 对应
        switch(command) {
        case 0: cmd = PMHalt; break; // Halt
        case 1: cmd = PMStop; break; // Stop
        case 2: cmd = PMReady; break; // Normal start (Ready)
        case 3: cmd = PMStart; break; // Force start (Start)
        case 4: cmd = PMKickoffYellow; break; // Kickoff Yellow
        case 5: cmd = PMKickoffBlue; break; // Kickoff Blue
        case 6: cmd = PMPenaltyYellow; break; // Penalty Yellow
        case 7: cmd = PMPenaltyBlue; break; // Penalty Blue
        case 8: cmd = PMDirectYellow; break; // Direct Yellow
        case 9: cmd = PMDirectBlue; break; // Direct Blue
        case 10: cmd = PMIndirectYellow; break; // Indirect Yellow
        case 11: cmd = PMIndirectBlue; break; // Indirect Blue
        case 12: cmd = PMTimeoutYellow; break; // Timeout Yellow
        case 13: cmd = PMTimeoutBlue; break; // Timeout Blue
        case 14: cmd = PMGoalYellow; break; // Goal Yellow
        case 15: cmd = PMGoalBlue; break; // Goal Blue
        case 16: cmd = PMBallPlacementYellow; break; // Ball Placement Yellow
        case 17: cmd = PMBallPlacementBlue; break; // Ball Placement Blue
        default:
            std::cout << "refereebox is fucked !!!!! command : " << command << std::endl;
            cmd = PMHalt;break;
        }
        std::cout<<"refereebox update"<<cmd<<std::endl;
    }
}

void Cvision::dealwithdata(const void *ptr, int size) {
    static SSL_WrapperPacket packet;
    packet.ParseFromArray(ptr, size);
    if (packet.has_detection()) {
        const SSL_DetectionFrame& detection = packet.detection();
        /*czkdebug*///std::cout<<"camera number"<<detection.camera_id()<<std::endl;
        std::cout.setf(std::ios_base::fixed,std::ios_base::floatfield);
//        std::cout<<"capture time"<<detection.t_capture()<<std::endl;
        updatetime[1][detection.camera_id()] = detection.t_capture();
        dataset(detection); //设置原始全局图像信息并融合
        if(checkupdate()){ //一组信息收集后进行滤波
//            gettimeofday(&tv, NULL);
//            interupt = tv.tv_usec - timepre > 0? tv.tv_usec - timepre:1000000 + tv.tv_usec - timepre;
//            timepre = tv.tv_usec;
            /*czkdebug*///std::cout<<"time: "<<interupt<<" begin filtering"<<std::endl;
            interupt = updatetime[1][0]-updatetime[0][0];
            for(int i=0;i<PARAM::CAMERA;i++) updatetime[0][i] = updatetime[1][i];
            dataupgrade();
            camerareset();
            save_data();
        }
    }
}

void Cvision::save_data(){
    std::ofstream file ("dhzDATA",std::ios::app);
    if(file.is_open()){
        file.setf(std::ios::fixed, std::ios::floatfield);
        file<<"time: "<<updatetime[0][0]<<std::endl;
        file<<"referee: "<<cmd<<std::endl;
        for(int j=0;j<PARAM::TEAMS;j++){
            for(int i=0;i<PARAM::ROBOTNUM;i++){
                if(robots[j][i]->isobserved >= 1)
                    file<<"team: "<<j<<" num: "<<i<<" position: "<<robots[j][i]->filteredpos.x()<<" "<<robots[j][i]->filteredpos.y()<<" "<<robots[j][i]->filtereddir
                       <<" velocity: "<<robots[j][i]->filterVelocty.x()<<" "<<robots[j][i]->filterVelocty.y()<<" "<<robots[j][i]->filteredomega<<std::endl;
            }
        }
        file<<"ballpos: "<<balls[0]->filterdpos.x()<<" "<<balls[0]->filterdpos.y()<<" velocity "<<balls[0]->filtervelocity.x()<<" "<<balls[0]->filtervelocity.y()<<std::endl;
    }
}

void Cvision::SetBallType(bool type) {
    this->oneball = type;
}

void Cvision::dataupgrade() {
    FilterFrame newframe;
    int temp_ball;
    ballnum = 0;
    double robot_to_ball = 9999;
    /*czkdebugstd::cout<<ballpaired[0]<<" "<<balls[0]->isobserved<<std::endl*/;
    for(int i=0;i<PARAM::BALLNUM;i++){
        if(ballpaired[i]==0){
            if(!balls[i]->isobserved){
                newframe.balls[i] = new Ball(CGeoPoint(-9999,-9999));
                newframe.balls[i]->isobserved = false;
            }
            else{
                balls[i]->missingframe++;
                newframe.balls[i] = new Ball(balls[i]);
                if(balls[i]->missingframe>10){
                    balls[i]->missingframe = 0;
                    balls[i]->isobserved = 0;
                }
            }
        }
        else{
            ballnum++;
            balls[i]->update();
            /*czkdebug*///std::cout<<"ball rawposiiton "<<balls[i]->rawnowpos<<std::endl;
            /*czkdebug*///std::cout<<"ball rawvelocity "<<balls[i]->getrawvelocity()<<std::endl;
            /*czkdebug*///std::cout<<"ball filterd position"<<balls[i]->filterdpos<<std::endl;
            if(balls[i]->isobserved) {
                CGeoPoint ball_pos = ballfilter[i]->update(balls[i]->getrawposition(), balls[i]->getrawvelocity(), interupt);
                balls[i]->filtervelocity = (ball_pos - balls[i]->filterdpos)/balls[i]->DeltaT;
                balls[i]->filterdpos = ball_pos;
                balls[i]->missingframe = 0;
            }
            else {
                balls[i]->isobserved = true;
                ballfilter[i]->reset(balls[i]->rawnowpos);
                balls[i]->filterdpos = balls[i]->rawnowpos;
                balls[i]->filtervelocity = CVector(0,0);
            }
            /*czkdebug*///std::cout<<"ball raw_vel: "<<balls[i]->rawvelocity<<"ball filter vel: "<<balls[i]->filtervelocity<<std::endl;
            newframe.balls[i] = new Ball(balls[i]);
            ballpaired[i] = 0;
        }
    }

    for(int i=0; i<=1; i++){
        for(int j=0;j<PARAM::ROBOTNUM;j++) {
            switch(robots[i][j]->isobserved) {
            case 0:
                break;
            case 1:
                robots[i][j]->update();
                robots[i][j]->filteredpos = robots[i][j]->getrawnowpos();
                robots[i][j]->filtereddir = robots[i][j]->getrawdir();
                robots[i][j]->filterVelocty = CVector(0, 0);
                robots[i][j]->filteredomega = 0;
                newframe.robots[i][j] = new Robot(robots[i][j]->filteredpos, robots[i][j]->filtereddir);
                if(robots[i][j]->missingframe>10) {
                    robots[i][j]->isobserved--;
                    robots[i][j]->missingframe = 0;
                    break;
                }
                robots[i][j]->missingframe ++;
                if(robots[i][j]->missingframe==0) robotfilter[i][j]->reset(robots[i][j]->getrawnowpos(), robots[i][j]->getrawdir());
                //std::cout<<"team: "<<i<<"number"<<j;
                break;
            case 2:
                robots[i][j]->update();
                robots[i][j]->filteredpreviousdir = robots[i][j]->filtereddir;
                robots[i][j]->filteredpreviouspos = robots[i][j]->filteredpos;
                robotfilter[i][j]->update(robots[i][j]->getrawnowpos(), robots[i][j]->getrawdir(), RobotCommand(0, 0, 0), interupt, robots[i][j]->filteredpos, robots[i][j]->filtereddir);
                robots[i][j]->filterVelocty = (robots[i][j]->filteredpos - robots[i][j]->filteredpreviouspos)/interupt;
                robots[i][j]->filteredomega = (robots[i][j]->filtereddir - robots[i][j]->filteredpreviousdir)/interupt;
                if(oneball && robots[i][j]->filteredpos.dist(balls[0]->filterdpos) < robot_to_ball) robot_to_ball = robots[i][j]->filteredpos.dist(balls[0]->filterdpos);
                newframe.robots[i][j] = new Robot(robots[i][j]->filteredpos, robots[i][j]->filtereddir);
                robots[i][j]->missingframe = 0;
                robots[i][j]->isobserved --;
                /*czkdebug*///std::cout<<"car raw dir: "<<robots[i][j]->getrawdir()<<" filter dir: "<<robots[i][j]->filtereddir<<std::endl;
                break;
            }
            newframe.robots[i][j] = new Robot(robots[i][j]);
        }
    }
    /*czkdebug*///std::cout<<"robot_to_ball: "<<robot_to_ball<<std::endl;
    if(ballnum == 0) balls[0]->filterdpos = balls[0]->filterdpos + balls[0]->filtervelocity*interupt;
    if(oneball) balls[0]->evaluator->update(balls[0]->filtervelocity, robot_to_ball);
    /*czkdebug*///std::cout<<"after filtering the ball num is "<<ballnum<<std::endl;
    Frame.push(newframe);
    //datasend();
}

CVector Cvision::PosTranfer() {
    switch (PosType) {
    case 1:
        return CVector(4500,3000);
    default:
        return CVector(9000,6000);
    }
}

void Cvision::dataset(const SSL_DetectionFrame& detection){
    int cam_id = detection.camera_id();
//    std::cout<<"fuck cam_id"<<cam_id<<std::endl;
    int bluesize = detection.robots_blue_size();
    int yellowsize = detection.robots_yellow_size();
    /*czkdebug*///std::cout<<"camera number: "<<cam_id<<" blue: "<<bluesize<<" yellow: "<<yellowsize<<std::endl;
    for(int i=0; i<bluesize; i++) {
        const SSL_DetectionRobot& littleblue = detection.robots_blue(i);
        CGeoPoint blue_pos(littleblue.x(), littleblue.y());
        //设置原始滤波前的球的位置,各个摄像头加权
        robots[0][littleblue.robot_id()]->update(cameraweight(cam_id, blue_pos), blue_pos, littleblue.orientation(), interupt);
        if(robots[0][littleblue.robot_id()]->isobserved == 0){
            robots[0][littleblue.robot_id()]->isobserved = 1;
        }
        else robots[0][littleblue.robot_id()]->isobserved = 2;
    }
    for(int i=0; i<yellowsize; i++) {
        const SSL_DetectionRobot& littleyellow = detection.robots_yellow(i);
        CGeoPoint yellow_pos(littleyellow.x(), littleyellow.y());
        //设置原始滤波前的机器人的位置，各个摄像头加权
        robots[1][littleyellow.robot_id()]->update(cameraweight(cam_id, yellow_pos), yellow_pos, littleyellow.orientation(), interupt);
        if(robots[1][littleyellow.robot_id()]->isobserved == 0){
            robots[1][littleyellow.robot_id()]->isobserved = 1;
        }
        else robots[1][littleyellow.robot_id()]->isobserved = 2;
    }
    if(!oneball){
        normalMergeBall(detection);
    }
    else {
        getoneball(detection);
    }
    cameraupdate[cam_id] = true;
    /*czkdebug*///std::cout<<"set successful"<<std::endl;
}

void Cvision::getoneball(const SSL_DetectionFrame &detection) {
    int cam_id = detection.camera_id();
    int ballsize = detection.balls_size();
    if(ballsize<=0) return;
    if(ballnum == 0){
        const SSL_DetectionBall& ball = detection.balls(0);
        CGeoPoint ball_pos(ball.x(), ball.y());
        balls[0]->update(cameraweight(cam_id, ball_pos), ball_pos, interupt);
        ballnum++;
    }
    else{
        for(int i=0;i<ballsize;i++){
            const SSL_DetectionBall& ball = detection.balls(i);
            CGeoPoint ball_pos(ball.x(), ball.y());
            if(ball_pos.dist(balls[0]->filterdpos)<PARAM::BALLMERGEDISTANCE) {
                balls[0]->update(cameraweight(cam_id, ball_pos), ball_pos, interupt);
            }
        }
        /*czkdebugstd::cout<<"merge ball"<<std::endl;*/
    }
    ballpaired[0] = 1;
}

//利用KM算法进行数据关联
void Cvision::kmmergeball(const SSL_DetectionFrame &detection) {
    int cam_id = detection.camera_id();
    int ballsize = detection.balls_size();
    int ballcount = ballnum;
    for(int i=0;i<ballsize&&i<PARAM::BALLNUM;i++) {
        const SSL_DetectionBall& ball = detection.balls(i);
        CGeoPoint ball_pos(ball.x(), ball.y());
        if(ballnum == 0){
            balls[i]->update(cameraweight(cam_id, ball_pos), ball_pos, interupt);
            ballnum++;
            ballpaired[i] = 1;
        }
        else{
        }
    }

}

//利用特定距离匹配法进行关联
void Cvision::normalMergeBall(const SSL_DetectionFrame &detection){
    int cam_id = detection.camera_id();
    int ballsize = detection.balls_size();
    int ballcount = ballnum;
    /*czkdebug*///std::cout<<"before Merge ballnum is "<<ballnum<<std::endl;
    for(int i=0; i<ballsize; i++) {
        const SSL_DetectionBall& ball = detection.balls(i);
        CGeoPoint ball_pos(ball.x(), ball.y());
        if(ballnum == 0){    //上一帧没有球则不考虑关联
            balls[i]->update(cameraweight(cam_id, ball_pos), ball_pos, interupt);
            ballnum++;
            ballpaired[i] = 1;
            continue;
        }
        bool merge = false;   //看是否需要与其他球融合
        for(int j=0; j<PARAM::BALLNUM; j++) {
            /*czkdebug*///std::cout<<"distance: "<<ball_pos.dist(balls[j]->rawnowpos)<<" "<<balls[j]->isobserved<<std::endl;
            if(balls[j]->isobserved && ball_pos.dist(balls[j]->rawnowpos) < PARAM::BALLMERGEDISTANCE){
                balls[j]->update(cameraweight(cam_id,ball_pos), ball_pos, interupt);
                merge = true;
                ballpaired[j]=1;
                break;
            }
        }
        if(!merge){
            for(int k=0;k<PARAM::BALLNUM;k++) {
                if(!ballpaired[k]&&!balls[k]->isobserved) {
                    balls[k]->isobserved = false;
                    balls[k]->update(cameraweight(cam_id, ball_pos), ball_pos, interupt);
                    ballpaired[k]=1;
                    ballcount++;
                    break;
                }
            }
        }
    }
    ballnum = ballcount;
    /*czkdebug*///std::cout<<"detection ball num: "<<ballsize<<" after merge: "<<ballnum<<std::endl;
}

const Robot Cvision::getrobotinformation(int color, int id) {
    const Robot car(*robots[color][id]);
    return car;
}

const Ball Cvision::getballinformation(int id) {
    const Ball ball(*balls[id]);
    return ball;
}

double Cvision::cameraweight(int id, CGeoPoint origin) {

    return 1;
}

bool Cvision::checkupdate() {
    for(int i=0;i<PARAM::CAMERA;i++) {
        if(!cameraupdate[i]) return false;
    }
    return true;
}

void Cvision::camerareset() {
    for(int i=0;i<PARAM::CAMERA;i++) {
        cameraupdate[i] = false;
    }
}

// for Medusa
//    Vision_DetectionBall* ball;
//    for(int i=0;i<ballnum;i++){
//        ball = Frame.mutable_balls();
//        ball->set_x(float(balls[i]->filterdpos.x()));
//        ball->set_y(float(balls[i]->filterdpos.y()));
//        ball->set_valid(true);
//        ball->set_raw_x(float(balls[i]->getrawposition().x()));
//        ball->set_raw_y(float(balls[i]->getrawposition().y()));
//    }
//    Vision_DetectionRobot* Robot;
//    for(int j=0;j<2;j++){
//        for(int i=0;i<PARAM::ROBOTNUM;i++){
//            if(robots[j][i]->isobserved==1){
//                if(j==0) Robot = Frame.add_robots_blue();
//                else Robot = Frame.add_robots_yellow();
//                Robot->set_x(float(robots[j][i]->filteredpos.x()));
//                Robot->set_y(float(robots[j][i]->filteredpos.y()));
//                Robot->set_valid(true);
//                Robot->set_robot_id(i);
//                Robot->set_orientation(float(robots[j][i]->filtereddir));
//                Robot->set_raw_x(float(robots[j][i]->getrawnowpos().x()));
//                Robot->set_raw_y(float(robots[j][i]->getrawnowpos().y()));
//            }
//        }
//    }
//    int size = Frame.ByteSize();
//    data.resize(size);
//    Frame.SerializeToArray(data.ptr(), size);
//    publish("realdata",data);
//    Frame.clear_balls();
//    Frame.clear_robots_blue();
//    Frame.clear_robots_yellow();


//for drawing
void Cvision::datasend() {
    static ZSData data;
    SSL_DetectionFrame paintingframe;
    int bluenum = 0;
    int yellownum = 0;
    for(int i=0;i<ballnum;i++){
        SSL_DetectionBall* filterball = paintingframe.add_balls();
        filterball->set_x(float(balls[i]->filterdpos.x()));
        filterball->set_y(float(balls[i]->filterdpos.y()));
        filterball->set_pixel_x(0);
        filterball->set_pixel_y(0);
        filterball->set_confidence(100-5*ballnum);
    }
    SSL_DetectionRobot* filterrobot;
    for(int j=0;j<2;j++){
        for(int i=0;i<PARAM::ROBOTNUM;i++){
            if(robots[j][i]->isobserved==1){
                if(j==0) {
                    filterrobot = paintingframe.add_robots_blue();
                    bluenum ++;
                }
                else {
                    filterrobot = paintingframe.add_robots_yellow();
                    yellownum ++;
                }
                filterrobot->set_x(float(robots[j][i]->filteredpos.x()));
                filterrobot->set_y(float(robots[j][i]->filteredpos.y()));
                filterrobot->set_robot_id(i);
                filterrobot->set_orientation(float(robots[j][i]->filtereddir));
                filterrobot->set_pixel_x(0);
                filterrobot->set_pixel_y(0);
            }
        }
    }
    paintingframe.set_frame_number(0);
    paintingframe.set_t_sent(0);
    paintingframe.set_t_capture(0);
    paintingframe.set_camera_id(0);
    int size = paintingframe.ByteSize();
    data.resize(size);
    paintingframe.SerializeToArray(data.ptr(), size);
    publish("realdata",data);
    paintingframe.clear_balls();
    paintingframe.clear_robots_blue();
    paintingframe.clear_robots_yellow();
}


