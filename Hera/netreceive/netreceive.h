#ifndef NETRECEIVE_H
#define NETRECEIVE_H

#include <QObject>
#include <QQueue>
#include <QUdpSocket>
#include "netreceive_global.h"

class NETRECEIVESHARED_EXPORT NetReceive : public QObject
{
    Q_OBJECT
public:
    NetReceive(QObject *parent = 0);
    ~NetReceive();
    QQueue<QByteArray> datagrams;

public slots:
    void storeData();

private:
    QUdpSocket udpReceiveSocket;
    void udpSocketConnect();
    void udpSocketDisconnect();


    int vision_port = 10020;

//    VisionModule vm;
};

#endif // NETRECEIVE_H
