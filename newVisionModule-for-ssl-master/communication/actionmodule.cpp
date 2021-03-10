#include <QDateTime>
#include "actionmodule.h"
#include "networkinterfaces.h"
#include "parammanager.h"

namespace  {
const int TRANSMIT_PACKET_SIZE = 25;
const int TRANSMIT_START_PACKET_SIZE = 6;
const QStringList radioSendAddress2choose=  {"10.12.225.142", "10.12.225.130","10.12.225.109","10.12.225.78"};
const QStringList radioReceiveAddress2choose =  {"10.12.225.142", "10.12.225.130","10.12.225.110","10.12.225.79"};
QString radioSendAddress[PARAM::TEAMS] = {"10.12.225.78","10.12.225.142"};
QString radioReceiveAddress[PARAM::TEAMS] = { "10.12.225.142","10.12.225.142"};
const int PORT_SEND = 1030;
const int PORT_RECEIVE = 1030;
auto zpm = ZSS::ZParamManager::instance();
std::thread* receiveThread = nullptr;
}

CAction::CAction(QObject *parent) :
    QObject(parent),
    tx(TRANSMIT_PACKET_SIZE, 0){
    tx[0] = 0x40;
    if(receiveDataSocket.bind(QHostAddress::AnyIPv4, PORT_RECEIVE, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        qDebug() << "****** start receive ! ******";
        receiveThread = new std::thread([ = ] {getsensor();});
        receiveThread->detach();
    }
    else {
        qDebug() << "Bind Error in action module !";
    }
}

void CAction::getsensor() {
    static QHostAddress address;
    static int color;
    static qint64 last_receive_time[PARAM::ROBOTNUM];
    static bool no_response[PARAM::ROBOTNUM];
    for (int i = 0; i < PARAM::ROBOTNUM; i++) {
        no_response[i] = false;
    }
    qDebug() << "receive data !!!"<<address;
    while(true) {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        if(!PARAM::ISSIM) {
            for (int color = PARAM::BLUE; color <= PARAM::YELLOW; color++) {
                for (int j = 0; j < PARAM::ROBOTNUM; j++ ) {
                    QDateTime UTC(QDateTime::currentDateTimeUtc());
                    qint64 current_time = UTC.toMSecsSinceEpoch();
                    if (current_time - last_receive_time[j] > 3000 && !no_response[j]) {
                        no_response[j] = true;
                    }
                }
            }
        }
        while (receiveDataSocket.state() == QUdpSocket::BoundState && receiveDataSocket.hasPendingDatagrams()) {
            qDebug() << "receive data !!!"<<address;
            rx.resize(receiveDataSocket.pendingDatagramSize());
            receiveDataSocket.readDatagram(rx.data(), rx.size(), &address);
            color = (address.toString() == radioReceiveAddress[0]) ? team[0] : team[1];
            if (color == -1) {
                qDebug() << "Receive Error Message from:" << address << "in actionmodule.cpp";
                break;
            }
            auto& data = rx;
            int id = 0;
            bool infrared = false;
            bool flat = false;
            bool chip = false;
            int battery = 0;
            int capacitance = 0;
            short wheelVel[4] = {0};
            double imu_dir;
            double imu_rotate_vel;

            if(data[0] == (char)0xff && data[1] == (char)0x02) {
                id       = (quint8)data[2];
                infrared = (quint8)data[3] & 0x40;
                flat     = (quint8)data[3] & 0x20;
                chip     = (quint8)data[3] & 0x10;
                battery  = (quint8)data[4];
                capacitance = (quint8)data[5];
                wheelVel[0] = (quint16)(data[6] << 8) + data[7];
                wheelVel[1] = 1 + (short)~(data[8] << 8) + data[9];
                wheelVel[2] = 1 + (short)~(data[10] << 8) + data[11];
                wheelVel[3] = (quint16)(data[12] << 8) + data[13];
                imu_dir = ((quint8)(data[14]) + 256 * (qint8)(data[15]));
                imu_rotate_vel = ((quint8)(data[16]) + 256 * (qint8)(data[17]));
                imu_dir = imu_dir > 32767 ? imu_dir - 65535 : imu_dir;
                imu_rotate_vel = imu_rotate_vel > 32767 ? imu_rotate_vel - 65535 : imu_rotate_vel;
                imu_dir = imu_dir / 100.0 * ZSS::Sim::PI / 180.0;
                imu_rotate_vel = imu_rotate_vel / 100.0 * ZSS::Sim::PI / 180.0 * 2;
            }
            qDebug()<<"czkdebug::id: "<<id<<"infrared"<<infrared<<"flat"<<flat;
            QDateTime UTC(QDateTime::currentDateTimeUtc());
            last_receive_time[id] = UTC.toMSecsSinceEpoch();
            no_response[id] = false;
        }
    }
}

bool CAction::connectRadio(int id, int frq) {
    bool color;
    if(id >= 0 && id < PARAM::TEAMS) {
        zpm->loadParam(color, "ZAlert/IsYellow", false);
        team[id] = color ? PARAM::YELLOW : PARAM::BLUE;
        qDebug() << "connectRadio : " << id << (color ? "YELLOW" : "BLUE") << frq;
        sendStartPacket(id, frq);
        return true;
    }
    else qDebug() << "ERROR id in connectRadio function!";
    return false;
}

CAction::~CAction() {
    sendCommandSocket.disconnectFromHost();
    receiveDataSocket.disconnectFromHost();
}

void CAction::sendStartPacket(int t, int frequency) {
    // this 't' is id
    QByteArray startPacketSend(TRANSMIT_START_PACKET_SIZE, 0);
    QByteArray startPacketReceive(TRANSMIT_START_PACKET_SIZE, 0);
    if(frequency == 8) {
        startPacketSend[0] = (char)0xf0;
        startPacketSend[1] = (char)0x5a;
        startPacketSend[2] = (char)0x5a;
        startPacketSend[3] = (char)0x01;
        startPacketSend[4] = (char)0x02;
        startPacketSend[5] = (char)0xa7;

        startPacketReceive[0] = (char)0xf0;
        startPacketReceive[1] = (char)0x5a;
        startPacketReceive[2] = (char)0x5a;
        startPacketReceive[3] = (char)0x02;
        startPacketReceive[4] = (char)0x02;
        startPacketReceive[5] = (char)0xa8;
    }
    else if(frequency == 6) {
        startPacketSend[0] = (char)0xf0;
        startPacketSend[1] = (char)0x18;
        startPacketSend[2] = (char)0x5a;
        startPacketSend[3] = (char)0x01;
        startPacketSend[4] = (char)0x02;
        startPacketSend[5] = (char)0x65;

        startPacketReceive[0] = (char)0xf0;
        startPacketReceive[1] = (char)0x18;
        startPacketReceive[2] = (char)0x18;
        startPacketReceive[3] = (char)0x02;
        startPacketReceive[4] = (char)0x02;
        startPacketReceive[5] = (char)0x24;
    }
    else{
        qDebug() << "Frequency ERROR !!!";
    }
//    sendCommandSocket.setMulticastInterface(ZNetworkInterfaces::instance()->getFromIndex(0));
//    receiveDataSocket.setMulticastInterface(ZNetworkInterfaces::instance()->getFromIndex(0));
    sendCommandSocket.writeDatagram(startPacketSend, TRANSMIT_START_PACKET_SIZE, QHostAddress(radioSendAddress[t]), PORT_SEND);
    receiveDataSocket.writeDatagram(startPacketReceive, TRANSMIT_START_PACKET_SIZE, QHostAddress(radioReceiveAddress[t]), PORT_SEND);
    std::this_thread::sleep_for(std::chrono::milliseconds(4));
    qDebug() << "Frequency:" << frequency << " Send IP:" << radioSendAddress[t] << " Receive IP:" << radioReceiveAddress[t];
}

void CAction::sendLegacy(int team, const ZSS::Protocol::Robots_Command &commands) {
    auto& socket = sendCommandSocket;
    int size = commands.command_size(); //一包四车
    std::cout<<"command size:"<<size<<"tx size: "<<tx.size()<<std::endl;
    tx.resize(TRANSMIT_PACKET_SIZE);
    tx.fill(0x00);
    tx[0] = 0x40;
    qDebug()<<tx.toHex()<<"tx size: "<<tx.size();
    int i = 1;
    for(i=1;i<=size;i++) {

        if(i%4 == 0) {
            std::cout<<"send command1"<<std::endl;
            socket.writeDatagram(tx.data(), TRANSMIT_PACKET_SIZE, QHostAddress(radioSendAddress[team]), PORT_SEND);
            tx.fill(0x00);
            tx[0] = 0x40;
        }
        auto& command = commands.command(i-1);
        encodeLegacy(command, this->tx, i-1);
        std::cout<<"send command2: "<<i<<std::endl;
    }
    if(i%4!=0){
        sendCommandSocket.writeDatagram(tx.data(), TRANSMIT_PACKET_SIZE, QHostAddress(radioSendAddress[team]), PORT_SEND);
    }

}

void CAction::encodeLegacy(const ZSS::Protocol::Robot_Command &command, QByteArray &tx, int num) {
    std::cout<<"encode!!"<<num<<std::endl;
    quint8 id = quint8(command.robot_id());
    double origin_vx = command.velocity_x() / 10.0;
    double origin_vy = command.velocity_y() / 10.0;
    double origin_vr = command.velocity_r() / 10.0;
    qint16 vx = qint16(origin_vx);
    qint16 vy = qint16(origin_vy);
    qint16 vr = qint16(origin_vr * 160);
    qint16 abs_vx = std::abs(vx);
    qint16 abs_vy = std::abs(vy);
    qint16 abs_vr = std::abs(vr);
    bool kick = command.kick();
    quint16 kickspeed = command.power() / 10.0;
    quint8 power = 0;
    qint8 dribble = command.dribbler_spin() > 0.5 ? 3 : 0;
    tx[0] = (tx[0]) | (1 << (3 - num));
    //RobotID
    tx[1] = (id) & 0x0f;
    tx[0] = tx[0] | 0x08;
    //Robot1 Config
    //shoot or chip
    tx[1] = tx[1] | ((quint8)kick << 6 );
    //power level
    tx[1] = tx[1] | dribble;
    //low bit of vel
    tx[2] = ((vx >= 0)?0:0x80) | (abs_vx & 0x7f);
    tx[3] = ((vy >= 0)?0:0x80) | (abs_vy & 0x7f);
    tx[4] = ((vr >= 0)?0:0x80) | (abs_vr & 0x7f);
    //Don't understand !
    if(tx[2] == char(0xff)) tx[4] = 0xfe;
    if(tx[3] == char(0xff)) tx[5] = 0xfe;
    if(tx[4] == char(0xff)) tx[6] = 0xfe;
    //clear Byte[17-24]
    tx[17] = tx[18] = tx[19] = tx[20] = tx[21] = tx[22] = tx[23] = tx[24] = 0;
    //high bit of vel
    tx[17] = ((abs_vx & 0x180) >> 1) | ((abs_vy & 0x180) >> 3) | ((abs_vr & 0x780) >> 7);
    //shoot power
    tx[21] = 0x7f & power;
}

void CAction::crazy(int team, int id, CVector speed, double rot_vel, int flat, int kickspeed, double dribble) {
    qDebug()<<"crazy";
    ZSS::Protocol::Robots_Command crazy_command;
    auto robot_command = crazy_command.add_command();
    robot_command->set_robot_id(id);
    robot_command->set_velocity_x(speed.x());
    robot_command->set_velocity_y(speed.y());
    robot_command->set_velocity_r(rot_vel);
    robot_command->set_dribbler_spin(dribble);
    robot_command->set_kick(flat);
    robot_command->set_power(kickspeed);
    sendLegacy(team, crazy_command);
    crazy_command.Clear();
}

