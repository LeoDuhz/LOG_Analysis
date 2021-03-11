#ifndef KALMANFILTER_H
#define KALMANFILTER_H
#include "messageformat.h"
#include <Eigen/Dense>

class Kalmanfilter {
public:
    Kalmanfilter(const CGeoPoint origin);
    Kalmanfilter(const CGeoPoint origin, const double dir);
    CGeoPoint update(const CGeoPoint pos, const CVector ballspeed, double DeltaT);
    void update(const CGeoPoint pos, double dir, const RobotCommand, double DeltaT, CGeoPoint& filterpos, double& filterdir);
//    void update(const CGeoPoint pos, double dir, double DeltaT, CGeoPoint& filterpos, double& filterdir);
    void calibration(const CGeoPoint);
    void calibration(const CGeoPoint, const double dir);
    void reset(const CGeoPoint origin);
    void reset(const CGeoPoint origin, const double dir);
    void normalize(double& dir);

private:
    Eigen::MatrixXd A, B, H, Q, R, P_predict, P_head; //模型参数
    Eigen::VectorXd X_head, X_predict, U_k; //状态量与输入
    Eigen::MatrixXd K; //卡尔曼增益
};

#endif // KALMANFILTER_H

