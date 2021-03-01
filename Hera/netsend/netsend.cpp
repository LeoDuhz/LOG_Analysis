#include <QDebug>
#include "netsend.h"
#include "../Hera/share/proto/cpp/vision_detection.pb.h"
#include "../Hera/share/staticparams.h"

NetSend::NetSend()
    : udpSendSocket() {

}

void NetSend::udpSend(QByteArray buffer) {
//    qDebug() << buffer.data();
    udpSendSocket.writeDatagram(buffer.data(), buffer.size(), QHostAddress("127.0.0.1"), ZSS::Athena::VISION_SEND[0]);
    udpSendSocket.writeDatagram(buffer.data(), buffer.size(), QHostAddress("127.0.0.1"), ZSS::Athena::VISION_SEND[1]);
    udpSendSocket.writeDatagram(buffer.data(), buffer.size(), QHostAddress("127.0.0.1"), ZSS::Athena::SEND_TO_PYTHON);
}
