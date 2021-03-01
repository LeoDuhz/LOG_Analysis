#ifndef DEALBALL_H
#define DEALBALL_H

#include "messageformat.h"
#include "geometry.h"
#include "kalmanfilter.h"
#include "staticparams.h"

class VisionModule;
class CDealBall {
  public:
    CDealBall(VisionModule* vm);
    void run();
    void choseBall();
    bool getValid() {
        return validBall;
    }
    double getBallSpeed() {
        return result.ball[0].velocity.mod();
    }
    void updateVel(const Matrix2d tempMatrix, ReceiveVisionMessage& result);
  private:
    double posDist(CGeoPoint, CGeoPoint);
    bool ballNearVechile(Ball, double);
    void init();
    void mergeBall();
    double calculateWeight(int camID, CGeoPoint);
    Ball ballSequence[PARAM::BALLNUM][PARAM::CAMERA];
    Ball lastBall, currentBall;
    ReceiveVisionMessage result;
    int actualBallNum = 0;
    int minBelieveFrame;
    bool validBall;
    double upPossible, downPossible;
    long long _cycle, lastFrame, currentFrame;

    VisionModule* vm;
};

#endif // DEALBALL_H
