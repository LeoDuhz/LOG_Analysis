#ifndef MAINTAIN_H
#define MAINTAIN_H
#include "singleton.hpp"
#include "vision_detection.pb.h"
#include "kalmanfilter.h"
#include "chipsolver.h"
#include "collisiondetect.h"
#include "messageformat.h"
#include <QUdpSocket>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

class VisionModule;
class CMaintain {
  public:
    CMaintain(VisionModule* vm);
    ~CMaintain();
    void run();
    void init();
    void ChipPrediction();
    void StateMachine();

  private:
    ReceiveVisionMessage result;
    KalmanFilter ballKalmanFilter;
    void updateVel(const Matrix2d tempMatrix, ReceiveVisionMessage &result);

//    DataQueue<ReceiveVisionMessage> processBall;
//    DataQueue<ReceiveVisionMessage> processRobot;
//    DataQueue<ReceiveVisionMessage> maintain;
//    DataQueue<BallRecord> ballrecords;
//    ballState ballStateMachine;

    CCollisionDetect collisionDetect;
    Chipsolver chipsolver;

    VisionModule* vm;
};

#endif // MAINTAIN_H
