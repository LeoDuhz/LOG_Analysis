#ifndef MESSAGEFORMAT_H
#define MESSAGEFORMAT_H
#include "staticparams.h"
#include "geometry.h"
#include "messages_robocup_ssl_wrapper.pb.h"
#include "Model/ballstate.h"

//球的信息
class Ball {
public:
    Ball(CGeoPoint position) : isobserved(false), is_new(false), missingframe(0), rawnowpos(position), rawvelocity(CVector(0,0)), filtervelocity(CVector(0,0)), filterdpos(position) {
        evaluator = new BallState();
    }
    Ball(Ball* copied) : isobserved(copied->isobserved), is_new(copied->is_new), state(copied->state), missingframe(copied->missingframe){
        this->rawnowpos = copied->rawnowpos;
        this->filterdpos = copied->filterdpos;
        this->rawpreviouspos = copied->rawpreviouspos;
        this->rawvelocity = copied->rawvelocity;
    }
    void changestate(int state_new) {state = state_new;}
    void update(double w, CGeoPoint pos_new, double DeltaT) {
        this->DeltaT = DeltaT;
        if(is_new) rawnowpos = (rawnowpos * sigmaw + pos_new * w)*(1 / (sigmaw + w));
        else {
            rawnowpos = pos_new;
            is_new = true;
        }
        sigmaw += w;
    }
    void update() {
        rawvelocity = CVector((rawnowpos - rawpreviouspos)) / DeltaT;
        rawpreviouspos = rawnowpos;
        sigmaw = 0;
        is_new = false;
    }
    BallState *evaluator;
    CVector getrawvelocity() {return rawvelocity;}
    CGeoPoint getrawposition() {return rawnowpos;}
    bool isobserved;
    bool is_new;
    int state; // 1 for in air , 2 for on the ground
    int missingframe;
    CGeoPoint rawpreviouspos;
    CGeoPoint rawnowpos;
    CVector rawvelocity;
    CVector filtervelocity;
    CGeoPoint filterdpos;
    double sigmaw; //权值和
    double DeltaT;

};

//机器人信息
class Robot {
public:
    Robot(CGeoPoint position, double dir) : isobserved(0), missingframe(0), rawnowpos(position), rawvelocity(CVector(0,0)), filteredpos(position), filtereddir(dir) {
        this->filterVelocty = CVector(0, 0);
    }
    Robot(Robot* copied) {
        this->rawdir = copied->rawdir;
        this->rawnowpos = copied->rawnowpos;
        this->filtereddir = copied->filtereddir;
        this->filteredpos = copied->filteredpos;
        this->rawpreviouspos = copied->rawpreviouspos;
        this->is_new = copied->is_new;
        this->isobserved = copied->isobserved;
        this->filterVelocty = copied->filterVelocty;
    }
    void update(double w, const CGeoPoint pos_new, const double dir, double DeltaT) {
        rawdir = dir;
        if(is_new) rawnowpos = (rawnowpos * sigmaw + pos_new * w)*(1 / (sigmaw + w));
        else {
            rawnowpos = pos_new;
            is_new = true;
        }
        sigmaw += w;
        this->DeltaT = DeltaT;
    }
    void update() {
        rawvelocity = CVector(rawnowpos - rawpreviouspos) / DeltaT;
        sigmaw = 0;
        is_new = false;
    }
    CVector getrawvelocity() {return rawvelocity;}
    CGeoPoint getrawnowpos() {return rawnowpos;}
    double getrawdir() {return rawdir;}
    bool is_new;
    int isobserved; //用来记录机器人的状态，0为未检测，1为看见一帧或丢失小于特定帧数，2为连续两帧及以上检测到
    CGeoPoint filteredpos;
    CGeoPoint filteredpreviouspos;
    CVector filterVelocty;
    double filteredomega;
    double filteredpreviousdir;
    double filtereddir;
    int missingframe;
    double DeltaT;

private:
    double sigmaw; //权值和
    CGeoPoint rawnowpos;
    CGeoPoint rawpreviouspos;
    double rawdir;
    CVector rawvelocity;
};

//下发的命令信息
struct RobotCommand {
    float vx;
    float vy;
    float vr;
    bool chip;
    bool flat;
    int kickspeed;
    double dribble;
    RobotCommand(): vx(0), vy(0), vr(0) {}
    RobotCommand(float _x, float _y, float _r):
        vx(_x), vy(_y), vr(_r), chip(false), flat(false), kickspeed(0), dribble(0) {}

};

struct RobotInformation {
    bool infrared;
    bool flat;
    bool chip;
    double battery;
    double capacitance;
    double imuDir;
    double imuRotateDir;
    RobotInformation(): infrared(false), flat(false), chip(false), battery(0), capacitance(0), imuDir(0), imuRotateDir(0) {}

};

#endif // MESSAGEFORMAT_H
