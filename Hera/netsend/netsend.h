#ifndef NETSEND_H
#define NETSEND_H

#include <QUdpSocket>
#include "netsend_global.h"

class NETSENDSHARED_EXPORT NetSend
{

public:
    NetSend();
    void udpSend(QByteArray buffer);

private:
    QUdpSocket udpSendSocket;
};

#endif // NETSEND_H
