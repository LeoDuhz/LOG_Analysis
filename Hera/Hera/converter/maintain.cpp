#include "maintain.h"
#include "staticparams.h"
#include "collisiondetect.h"
#include "visionmodule.h"
#include "dealball.h"

CMaintain::CMaintain(VisionModule* vm)
    : vm(vm)
    , collisionDetect(vm)
    , chipsolver(vm){

}

CMaintain::~CMaintain() {

}


void CMaintain::init() {
    result.init();
    result.addBall(vm->processBall[0].ball[0]);
    for (int i = 0; i < vm->processRobot[0].robotSize[PARAM::BLUE]; i++)
        result.addRobot(PARAM::BLUE, vm->processRobot[0].robot[PARAM::BLUE][i]);
    for (int i = 0; i < vm->processRobot[0].robotSize[PARAM::YELLOW]; i++)
        result.addRobot(PARAM::YELLOW, vm->processRobot[0].robot[PARAM::YELLOW][i]);
}


void CMaintain::run() {  //TODO move to visionmodule
    init();
    //Ball Statemachine
    if (collisionDetect.ballCloseEnough2Analyze(PARAM::BLUE) ||
            collisionDetect.ballCloseEnough2Analyze(PARAM::YELLOW) ||
            collisionDetect.ballIsOnEdge(result.ball[0].pos))
        //离车近
    {
        if(vm->ballrecords.validSize() > 0) {
            chipsolver.reset();
        }

        //判断碰撞
        collisionDetect.analyzeData();
        //ball kalmanfilter FOLLOW
        auto & tempMatrix = ballKalmanFilter.follow(result.ball[0].pos);
        updateVel(tempMatrix, result);
    } else {        //离车远
        //判断挑球
        result.ball[0].predict_pos = chipsolver.dealresult();
        CGeoLine line(result.ball[0].pos, vm->maintain[-7].ball[0].pos);
        CGeoPoint middlePoint(vm->maintain[-4].ball[0].pos);
        if(line.projection(middlePoint).dist(middlePoint) > 1.0)
            //std::cout<<"now its chip dist="<<line.projection(middlePoint).dist(middlePoint)<<std::endl;
            if (line.projection(middlePoint).dist(middlePoint) > CHIP_DIS) {
                vm->ballStateMachine = chip_pass;
            } else
                vm->ballStateMachine = flat_pass;
        //ball kalmanfilter
        auto & tempMatrix = ballKalmanFilter.update(result.ball[0].pos);
        updateVel(tempMatrix, result);
    }

    vm->maintain.push(result);

}

void CMaintain::updateVel(const Matrix2d tempMatrix, ReceiveVisionMessage& result) {
    // 1.进行Kalman滤波，估计球的位置以及球速
    CGeoPoint filtPoint (tempMatrix(0, 0), tempMatrix(1, 0));
    CVector ballVel(tempMatrix(2, 0)* ZSS::Athena::FRAME_RATE, tempMatrix(3, 0)*ZSS::Athena::FRAME_RATE);
    result.ball[0].fill(result.ball[0].pos.x(), result.ball[0].pos.y(), 0, ballVel);
    // 2.延时补偿，根据延时帧率将位置和速度进行修正
    for( int i = 0; i < ZSS::Athena::TOTAL_LATED_FRAME; ++i ) {
        //thisCycle.SetPos(thisCycle.Pos() + thisCycle.Vel() / ZSS::Athena::FRAME_RATE);
        CVector uniVec = ballVel / (ballVel.mod() + 1.0);
        if ( ballVel.mod() > ZSS::Athena::BALL_DELC_CHANGE_POINT )
            ballVel = ( uniVec * ( ballVel.mod() - ZSS::Athena::BALL_FAST_DEC / ZSS::Athena::FRAME_RATE ));
        else if ( ballVel.mod() > 50 )
            ballVel = ( uniVec * ( ballVel.mod() - ZSS::Athena::BALL_SLOW_DEC / ZSS::Athena::FRAME_RATE ));
        else {
            ballVel = (CVector(0, 0));
        }
    }
    // 3.估计球速大小及方向，特别是方向
    if(vm->lastPossible > 0.3) { //MAYBE FIX
        // 计算球速方向:
        double movingDirection = ballVel.dir();
        ballVel.setVector(ballVel.mod()*std::cos(movingDirection), ballVel.mod()*std::sin(movingDirection));
    } else {
        double predictDir = ballVel.dir();
        double velMod = ballVel.mod();
        if (velMod < 0.1) {
            velMod = 0.1;
        }
        ballVel.setVector(velMod * std::cos(predictDir), velMod * std::sin(predictDir));
    }
    result.ball[0].fill(result.ball[0].pos.x(), result.ball[0].pos.y(), 0, ballVel);
}
