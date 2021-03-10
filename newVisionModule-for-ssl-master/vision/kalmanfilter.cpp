#include "kalmanfilter.h"
#include "staticparams.h"

Kalmanfilter::Kalmanfilter(const CGeoPoint origin) {
    X_head = Eigen::Vector2d();
    X_head[0] = origin.x();
    X_head[1] = origin.y();
    A = Eigen::Matrix2d::Identity(2, 2);
    B = Eigen::Matrix2d(2, 2);
    H = Eigen::Matrix2d::Identity(2, 2);
    P_head = Eigen::Matrix2d::Identity(2, 2);
    K = Eigen::VectorXd(1);
    Q = Eigen::Matrix2d::Identity(2, 2) * 10;
    R = Eigen::Matrix2d::Identity(2, 2) * 10;
}

Kalmanfilter::Kalmanfilter(const CGeoPoint origin, const double dir){
    X_head = Eigen::Vector3d();
    X_head[0] = origin.x();
    X_head[1] = origin.y();
    X_head[2] = dir;
    A = Eigen::Matrix3d::Identity(3, 3);
    B = Eigen::Matrix3d(3, 3);
    H = Eigen::Matrix3d::Identity(3, 3);
    P_head = Eigen::Matrix3d::Identity(3, 3);
    K = Eigen::VectorXd(1);
    Q = Eigen::Matrix3d::Identity(3, 3) * 1;
    R = Eigen::Matrix3d::Identity(3, 3) * 30;
}

CGeoPoint Kalmanfilter::update(const CGeoPoint pos, const CVector ballspeed, double DeltaT) {
    Eigen::Vector2d Y(pos.x(), pos.y());
    B = Eigen::Matrix2d::Identity(2, 2) * DeltaT;
    U_k = Eigen::Vector2d(ballspeed.x(), ballspeed.y());
    X_predict = A * X_head + B * U_k;
    P_predict = A * P_head * A.transpose() + Q;
    K = P_predict * H.transpose() * (H * P_head * H.transpose() + R).inverse();
    P_head = (Eigen::Matrix2d::Identity(2, 2) - K * H) * P_predict;
    X_head = X_predict + K * (Y - H * X_predict);
    CGeoPoint filterpos(X_head[0], X_head[1]);
    return filterpos;
}

void Kalmanfilter::normalize(double &dir){
    if(dir<=PARAM::Math::PI && dir>-PARAM::Math::PI) return;
    while(dir>PARAM::Math::PI || dir<-PARAM::Math::PI){
        if(dir>PARAM::Math::PI) dir -= 2*PARAM::Math::PI;
        else dir+=2*PARAM::Math::PI;
    }
}

void Kalmanfilter::update(const CGeoPoint pos, double dir,const RobotCommand command, double DeltaT, CGeoPoint& filterpos, double& filterdir) {
    //if(X_head[2]>PARAM::Math::PI*5/6 && dir < -PARAM::Math::PI*5/6) dir = dir + 2*PARAM::Math::PI;
    if (std::fabs(X_head[2]-dir)>std::fabs(X_head[2]-dir-2*PARAM::Math::PI)) dir += 2*PARAM::Math::PI;
    else if(std::fabs(X_head[2]-dir)>std::fabs(X_head[2]-dir+2*PARAM::Math::PI)) dir -= 2*PARAM::Math::PI;
    Eigen::Vector3d Y(pos.x(), pos.y(), dir);
    B = Eigen::Matrix3d::Identity(3, 3) * DeltaT;
    U_k = Eigen::Vector3d(command.vx, command.vy, command.vr);
    X_predict = A * X_head + B * U_k;
    P_predict = A * P_head * A.transpose() + Q;
    K = P_predict * H.transpose() * (H * P_head * H.transpose() + R).inverse();
    P_head = (Eigen::Matrix3d::Identity(3, 3) - K * H) * P_predict;
    X_head = X_predict + K * (Y - H * X_predict);
    filterpos = CGeoPoint(X_head[0], X_head[1]);
    normalize(X_head[2]);
    filterdir = X_head[2];
}

void Kalmanfilter::reset(const CGeoPoint origin) {
    std::cout<<"reset"<<std::endl;
    X_head[0] = origin.x();
    X_head[1] = origin.y();
}

void Kalmanfilter::reset(const CGeoPoint origin, const double dir) {
    std::cout<<"reset"<<std::endl;
    X_head[0] = origin.x();
    X_head[1] = origin.y();
    X_head[2] = dir;
}





