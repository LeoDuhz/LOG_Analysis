#ifndef BALLSTATE_H
#define BALLSTATE_H
#include "geometry.h"
#include "staticparams.h"
#include "dataqueue.hpp"

class BallState
{
public:
    BallState();
    int GetBallState();
    void update(CVector ball_v, double distance_to_car);
    int GetFromV();
private:
    DataQueue<CVector>Ball_speed;
    int state; // 0 for normal, 1 for nearkicker, 2 for isinair, 4 for down;
    int BallDetectionFrame;
};

#endif // BALLSTATE_H
