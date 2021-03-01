﻿#include "chipsolver.h"
#include "ballrecords.h"
#include "visionmodule.h"
#include "Eigen/Dense"
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;
using namespace Eigen ;
namespace {
int testmode = 0; //0 时间戳  1测试
static int flyend = 0;
static int num = -1;
}


Chipsolver::Chipsolver(VisionModule* vm)
    : timeOffset(0.05)
    , tFly(9999)
    , vm(vm){

}
Chipsolver::~Chipsolver() {

}
double Chipsolver::chipsolveOffset(double timeOff) {
    int recordsNum = vm->ballrecords.validSize();
    double l1Norm = 0;
    double tOffset = timeOff;
    MatrixXf matA(recordsNum * 2, 5);
    VectorXf b(recordsNum * 2);
    high_resolution_clock::time_point t0;
    t0 = vm->ballrecords.getStart().getcapture();
    const double g = 9810;
    int j = 0;
    for(int i = vm->ballrecords.getstartIndex(); i <= vm->ballrecords.getstartIndex() + recordsNum - 1; i++) {
        double t;
        if (testmode == 0) t = vm->ballrecords[i].timeinterval(t0) * 1e-3 + tOffset;
        else t = vm->ballrecords[i].getidealTime() + tOffset;
        CVector3 campos = vm->ballrecords[i].getcampos();
        CVector3 posProject = vm->ballrecords[i].getpos();
        matA.row(j) << campos.z(), 0, campos.z()*t, 0, (posProject.x() - campos.x())*t;
        b.row(j) << 0.5 * g*t*t*(posProject.x() - campos.x()) + posProject.x()*campos.z();
        j++;
        matA.row(j) << 0, campos.z(), 0, campos.z()*t, (posProject.y() - campos.y())*t;
        b.row(j) << 0.5 * g*t*t*(posProject.y() - campos.y()) + posProject.y()*campos.z();
        j++;
    }
    VectorXf v = matA.colPivHouseholderQr().solve(b);
    VectorXf result = matA * v;
    result -= b;
    kickVel.formXYZ(v(2), v(3), v(4));
    kickPos.formXYZ(v(0), v(1));
    for(int i = 0; i < recordsNum * 2; i++) {
        l1Norm += abs(result(i));
    }
    return l1Norm;
}
void Chipsolver::getbestresult() {
    double toff = 0.05;
    double inc = toff / 2;
    if (vm->ballrecords.validSize() < 5)
        return ;
    while(inc > 1e-4) {
        double solveResultNeg = chipsolveOffset(toff - 1e-5);
        double solveResultPos = chipsolveOffset(toff + 1e-5);
        if(solveResultNeg > solveResultPos) {
            toff += inc;
        } else {
            toff -= inc;
        }
        inc /= 2;
    }
    //cout<< toff <<endl;
    timeOffset = toff;
    l1Error = chipsolveOffset(toff);

}
CGeoPoint Chipsolver::dealresult() {
    if (testmode == 0) setrecord();
    if (t > tFly) {
        flyend++;
        //test
        if (flyend == 2) {
            reset();
            flyend = 0;
        }
    }
    //处理二次弹跳

    if(vm->ballrecords.validSize() <= 5) {
        return CGeoPoint(vm->maintain[0].ball[0].pos.x(), vm->maintain[0].ball[0].pos.y());
    }

    getbestresult();
    high_resolution_clock::time_point kickTime;
    kickTime = vm->ballrecords.getStart().getcapture();
    if (testmode == 0) t = vm->ballrecords[0].timeinterval(kickTime) * 1e-3 + timeOffset;
    else t = vm->ballrecords[0].getidealTime() + timeOffset;
    tFly = kickVel.z() * 2 / 9810;
    posNow = kickPos.addNew(kickVel.multiplyNew(t)).addz(-9810 * 0.5 * t * t);
    posEnd = kickPos.addNew(kickVel.multiplyNew(tFly)).addz(-9810 * 0.5 * tFly * tFly);
    return CGeoPoint(posEnd.x(), posEnd.y( ));

    //cout<<vm->ballrecords[0].getcampos().x()<<","<<vm->ballrecords[0].getcampos().y()<<","<<vm->ballrecords[0].getcampos().z()<<endl;
    /*
    cout << "vm->maintainpos:" << "(" << vm->maintain[0].ball[0].pos.x() << "," << vm->maintain[0].ball[0].pos.y() << ")" << endl;
    cout<<"posNow"<<"("<< posNow.x() <<","<<posNow.y()<<","<<posNow.z()<<")"<<endl;
    cout << "posEnd:" << "(" << posEnd.x() << "," << posEnd.y() << "," << posEnd.z() << ")" << endl;
    cout << "tFly:" << tFly << endl;
    cout << "t:" << t << endl;
    cout << "kickvel:" << kickVel.x()<<","<<kickVel.y()<<","<<kickVel.z()<< endl;
    cout <<"____________________________"<<endl;
    //cout<<"posNow"<<"("<< posNow.x() <<","<<posNow.y()<<","<<posNow.z()<<")"<<endl;
    //cout<<"posEnd"<<"("<< posEnd.x() <<","<<posEnd.y()<<","<<posEnd.z()<<")"<<endl;
    */
    //test

}
void Chipsolver::reset() {

    timeOffset = 0.05;
    tFly = 9999;
    t = 0;
    posEnd.formXYZ(0, 0, 0);
    kickPos.formXYZ(0, 0, 0);
    vm->ballrecords.clear();
}

void Chipsolver::setrecord() {
    if (vm->ballrecords.size() != 50)
        vm->ballrecords = DataQueue<BallRecord>(50);
    //PARAM::CAMERA
    for (int i = 0; i < PARAM::CAMERA; i++) {
        if(vm->camera[i][0].ballSize == 1) {
            ballrecord.setpos(vm->camera[i][0].ball[0].pos);
            //cout << ballrecord.getpos().x() << "," << ballrecord.getpos().y() << endl;
            //cout << camera[i][0].ball[0].pos.x() << "," << camera[i][0].ball[0].pos.y() << endl;
            ballrecord.setcampos(vm->cameraMatrix[i].campos, vm->cameraMatrix[i].height);
            //cout << cameraMatrix[i].campos << "," << cameraMatrix[i].height << endl;
            ballrecord.settime();
            ballrecord.setcamID(i);
            if(vm->ballrecords.getcurrentIndex() == (vm->ballrecords.size() - 1) && vm->ballrecords.validSize() == vm->ballrecords.size()) {
                vm->ballrecords.renew();
            }
            vm->ballrecords.push(ballrecord);
        }
    }

}

