#ifndef CHIPSOLVER_H
#define CHIPSOLVER_H

#include "ballrecords.h"

class VisionModule;
class Chipsolver {
public:
    Chipsolver(VisionModule *vm);
    ~Chipsolver();
    double chipsolveOffset(double timeOff);
    void getbestresult();
    CGeoPoint dealresult();
    void reset();
    void setrecord();
    void setrecord_test();

private:
    CVector3 kickVel;
    CVector3 kickPos;
    CVector3 posNow;
    CVector3 posEnd;
    double l1Error;
    double timeOffset;
    double t;
    double tFly;
    BallRecord ballrecord;

    VisionModule* vm;

};
#endif // CHIPSOLVER_H
