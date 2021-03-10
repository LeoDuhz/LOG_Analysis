#ifndef Cvision_H
#define Cvision_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QFile>
#include "singleton.hpp"
#include "vision_detection.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"
#include "ssl_referee.pb.h"
#include "dataqueue.hpp"
#include "messageformat.h"
#include "zsplugin.hpp"
#include "kalmanfilter.h"

enum PlayMode{
    PMStop,PMStart,PMHalt,PMReady,
    PMFirstHalf,PMHalfTime,PMSecondHalf,
    PMOverTime1,PMOverTime2,PMPenaltyShootout,
    PMTimeoutYellow,PMTimeoutBlue,PMTimeoutEnd,
    PMCancel,PMGoalYellow,PMGoalBlue,PMSubGoalYellow,PMSubGoalBlue,
    PMYellowCardYellow,PMYellowCardBlue,PMRestart,
    PMKickoffYellow,PMKickoffBlue,PMPenaltyYellow,PMPenaltyBlue,
    PMDirectYellow,PMDirectBlue,PMIndirectYellow,PMIndirectBlue,
    PMBallPlacementYellow,PMBallPlacementBlue,
    PMNone
};

// 数据入口 UDP接收
class Datastore : public QObject, public ZSPlugin {
    Q_OBJECT
public:
    Datastore(QObject *parent = nullptr);
    void udpSocketConnect(int);
    void udpSocketDisconnect();
    void setinterface(int interface_new);
    void run(){}

public slots:
    void sendData();
    void sendReferee();

private:
    QUdpSocket udpReceive;
    QUdpSocket refereebox;
    int communicateport;
    int networkinterface;
};

//画图的缓存
class FilterFrame {
public:
    FilterFrame(){}
    Robot* robots[2][PARAM::ROBOTNUM]; // 0 for blue, 1 for yellow
    Ball* balls[PARAM::BALLNUM];
};

class Rec_loader :public ZSPlugin {
public:
    Rec_loader();
    bool load_log(QString filename);
    void run();
private:
    QFile* replayFile;
    QList <QByteArray> packets;
    Vision_DetectionFrame log_frame;
    int framecount;
};

//数据处理
class Cvision : public QObject, public ZSPlugin {
    Q_OBJECT
public:
    Cvision();
    void run();
    void dealwithdata(const void *ptr, int size);
    void dealwithreferee(const void *ptr, int size);
    void dataupgrade();
    void dataset(const SSL_DetectionFrame& detection);
    void kmmergeball(const SSL_DetectionFrame& detection);
    void normalMergeBall(const SSL_DetectionFrame& detection);
    void getoneball(const SSL_DetectionFrame& detection);
    void datasend();
    const Robot getrobotinformation(int color, int id);
    const Ball getballinformation(int id);
    int ballnum;
    bool checkupdate();
    double cameraweight(int id, CGeoPoint origin);
    void camerareset();
    void SetBallType(bool);
    DataQueue<FilterFrame>Frame;
    CVector PosTranfer();
    void save_data();
    bool get_oneball(){return oneball;}

private:
    int PosType;
    CGeoPoint* CameraCenter[PARAM::CAMERA];
    bool oneball;
    SSL_DetectionFrame paintingframe;
//    Vision_DetectionFrame Frame;
    Robot* robots[2][PARAM::ROBOTNUM]; // 0 for blue, 1 for yellow
    Ball* balls[PARAM::BALLNUM];
    RobotCommand command[PARAM::ROBOTNUM];
    Kalmanfilter *robotfilter[2][PARAM::ROBOTNUM], *ballfilter[PARAM::BALLNUM];
    int ballpaired[PARAM::BALLNUM];
    bool cameraupdate[PARAM::CAMERA];
    double updatetime[2][PARAM::CAMERA];
    PlayMode cmd;
};



typedef Singleton<Cvision> CvisionModule;
#endif // Cvision_H
