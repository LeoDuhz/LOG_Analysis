#ifndef ACTIONMODULE_H
#define ACTIONMODULE_H
#include <QObject>
#include <QUdpSocket>
#include <QMutex>
#include "geometry.h"
#include "zsplugin.hpp"
#include "singleton.hpp"
#include "zss_cmd.pb.h"
#include "staticparams.h"

class CAction : public QObject {
    Q_OBJECT
public:
    CAction(QObject *parent = nullptr);
    ~CAction();
    void sendLegacy(int team, const ZSS::Protocol::Robots_Command&);
    bool connectRadio(int, int);
    void encodeLegacy(const ZSS::Protocol::Robot_Command&, QByteArray& tx, int num);
    void crazy(int team, int id, CVector speed, double rot_vel, int flat, int kickspeed, double dribble);

private:
    void sendStartPacket(int, int);
    void getsensor();
private:
    int team[PARAM::TEAMS];
    QByteArray tx;
    QByteArray rx;
    QUdpSocket sendCommandSocket;
    QUdpSocket receiveDataSocket;
    QMutex robotInfoMutex;
};
typedef Singleton<CAction> CactionModule;
#endif // ACTIONMODULE_H
