#ifndef DEALROBOT_H
#define DEALROBOT_H
#include "kalmanfilter.h"
#include "kalmanfilterdir.h"
#include "zss_cmd.pb.h"
#include <singleton.hpp>
#include "messageformat.h"

class VisionModule;
class CDealRobot {
  public:
    CDealRobot(VisionModule *vm);
    void run();
    void MergeRobot();
    void selectRobot();
    bool updateCommand(ZSS::Protocol::Robot_Command command);

  private:
    Robot robotSeqence[2][PARAM::ROBOTMAXID][PARAM::CAMERA];
    ReceiveVisionMessage result;
    Robot sortTemp[2][PARAM::ROBOTMAXID];
    Robot lastRobot[2][PARAM::ROBOTMAXID];
    Robot currentRobot[2][PARAM::ROBOTMAXID];
    void init();
    double posDist(CGeoPoint, CGeoPoint);
    double calculateWeight(int camID, CGeoPoint roboPos);
    void sortRobot(int);
    bool isOnField(CGeoPoint);
    KalmanFilter _kalmanFilter[2][PARAM::ROBOTMAXID];
    KalmanFilter _dirFilter[2][PARAM::ROBOTMAXID];
    bool filteDir;
    int validNum[2];
    int minBelieveFrame, maxLostFrame, fieldWidth, fieldHeight;
    double upPossible, decidePossible, downPossible;
    ZSS::Protocol::Robot_Command _command[PARAM::ROBOTMAXID];

    double robotPossible[2][PARAM::ROBOTMAXID];

//    DataQueue<ReceiveVisionMessage> processRobot;
//    DataQueue<ReceiveVisionMessage> camera[PARAM::CAMERA];
//    SingleCamera cameraMatrix[PARAM::CAMERA];

    VisionModule* vm;
};

#endif // DEALROBOT_H
