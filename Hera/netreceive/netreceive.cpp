#include "../Hera/share/staticparams.h"
#include "netreceive.h"
//#include "Windows.h"

NetReceive::NetReceive(QObject *parent) : QObject(parent){
    udpSocketConnect();
}

NetReceive::~NetReceive() {
    udpSocketDisconnect();
}

void NetReceive::udpSocketConnect() {
    connect(&udpReceiveSocket, SIGNAL(readyRead()), this, SLOT(storeData()), Qt::DirectConnection);
    udpReceiveSocket.bind(QHostAddress::AnyIPv4, vision_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    udpReceiveSocket.joinMulticastGroup(QHostAddress(ZSS::SSL_ADDRESS));
//    connect(&udpReceiveSocket, SIGNAL(readyRead()), this, SLOT(storeData()), Qt::DirectConnection);
}

void NetReceive::udpSocketDisconnect() {
    disconnect(&udpReceiveSocket, 0, this, 0);
    udpReceiveSocket.abort();
}

void NetReceive::storeData() {
//    qDebug() << "I RUN";
    while (udpReceiveSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpReceiveSocket.pendingDatagramSize());
        udpReceiveSocket.readDatagram(datagram.data(), datagram.size());
        datagrams.enqueue(datagram);
//        qDebug() << "get visionMsg";
//        qDebug() << datagrams;
//        parse((void*)datagram.data(), datagram.size());
//        这里通过sharememory将数据给convert
//        vm.parse((void*)datagram.data(), datagram.size());

    }
}
