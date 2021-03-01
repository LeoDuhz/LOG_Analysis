#include "dealrobot.h"
#include "staticparams.h"
#include "parammanager.h"
#include "matrix2d.h"
#include "visionmodule.h"
#include <iostream>
#include <qdebug.h>
namespace {
int DIFF_VECHILE_MAX_DIFF = 600;//1000
float MAX_SPEED = 5000;
}

CDealRobot::CDealRobot(VisionModule* vm)
    : vm(vm) {
    fieldWidth = 13200;
    fieldHeight = 9900;
    minBelieveFrame = 3;
    maxLostFrame = 15;
    upPossible = 0.1;
    decidePossible = minBelieveFrame * upPossible;
    downPossible = (1.0 - decidePossible) / maxLostFrame;
}

double CDealRobot::posDist(CGeoPoint pos1, CGeoPoint pos2) {
    return std::sqrt((pos1.x() - pos2.x()) * (pos1.x() - pos2.x()) + (pos1.y() - pos2.y()) * ((pos1.y() - pos2.y())));
}

bool CDealRobot::isOnField(CGeoPoint p) {
    if (p.x() < fieldWidth / 2 && p.x() > -fieldWidth / 2 && p.y() < fieldHeight / 2 && p.y() > -fieldHeight / 2)
        return true;
    else
        return false;
}
bool CDealRobot::updateCommand(ZSS::Protocol::Robot_Command command) {
    _command[command.robot_id()] = command;
    return true;
}

double CDealRobot::calculateWeight(int camID, CGeoPoint roboPos) {
    SingleCamera camera = vm->cameraMatrix[camID];
    if (roboPos.x() > camera.leftedge.max && roboPos.x() < camera.rightedge.max &&
            roboPos.y() > camera.downedge.max && roboPos.y() < camera.upedge.max)
        return 1;
    else if (roboPos.x() < camera.leftedge.max && roboPos.x() > camera.leftedge.min)
        return abs(roboPos.x() - camera.leftedge.min) / abs(camera.leftedge.max - camera.leftedge.min);
    else if (roboPos.x() > camera.rightedge.max && roboPos.x() < camera.rightedge.min)
        return abs(roboPos.x() - camera.rightedge.min) / abs(camera.rightedge.max - camera.rightedge.min);
    else if (roboPos.y() < camera.downedge.max && roboPos.y() > camera.downedge.min)
        return abs(roboPos.y() - camera.downedge.min) / abs(camera.downedge.max - camera.downedge.min);
    else if (roboPos.y() > camera.upedge.max && roboPos.y() < camera.upedge.min)
        return abs(roboPos.y() - camera.upedge.min) / abs(camera.upedge.max - camera.upedge.min);
    else return 0.01;//to deal with can see out of border situation
}

void CDealRobot::init() {
//    zpm->loadParam(filteDir, "Vision/FilteDirection", false);
    result.init();
    for (int roboId = 0; roboId < PARAM::ROBOTMAXID; roboId++)
        for (int camId = 0; camId < PARAM::CAMERA; camId++) {
            robotSeqence[PARAM::BLUE][roboId][camId].fill(-1, -32767, -32767, 0);
            robotSeqence[PARAM::YELLOW][roboId][camId].fill(-1, -32767, -32767, 0);
        }

    for (int i = 0; i < PARAM::CAMERA; i++) {
        for (int j = 0; j < vm->camera[i][0].robotSize[PARAM::BLUE]; j++) {
            Robot robot = vm->camera[i][0].robot[PARAM::BLUE][j];
            if (robotPossible[PARAM::BLUE][robot.id] < decidePossible)
                robotSeqence[PARAM::BLUE][robot.id][i] = robot;
            else if  (lastRobot[PARAM::BLUE][robot.id].pos.dist(robot.pos) < DIFF_VECHILE_MAX_DIFF)
                robotSeqence[PARAM::BLUE][robot.id][i] = robot;
        }
        for (int j = 0; j < vm->camera[i][0].robotSize[PARAM::YELLOW]; j++) {
            Robot robot = vm->camera[i][0].robot[PARAM::YELLOW][j];
            if ( robotPossible[PARAM::YELLOW][robot.id] < decidePossible)
                robotSeqence[PARAM::YELLOW][robot.id][i] = robot;
            else if  (lastRobot[PARAM::YELLOW][robot.id].pos.dist(robot.pos) < DIFF_VECHILE_MAX_DIFF)
                robotSeqence[PARAM::YELLOW][robot.id][i] = robot;
        }
    }
    for (int i = 0; i < PARAM::ROBOTMAXID - 1; i++) {
        Robot temp(-32767, -32767, 0, -1);
        sortTemp[PARAM::BLUE][i] = temp;
        sortTemp[PARAM::YELLOW][i] = temp;
    }
}

void CDealRobot::MergeRobot() {
    for (int roboId = 0; roboId < PARAM::ROBOTMAXID; roboId++) {
        bool foundBlue = false, foundYellow = false;
        double blueWeight = 0, yellowWeight = 0;
        CGeoPoint blueAverage(0, 0), yellowAverage(0, 0);
        double blueAngle = 0, yellowAngle = 0;
        for (int camId = 0; camId < PARAM::CAMERA; camId++) {
            SingleCamera camera = vm->cameraMatrix[camId];
            double _weight = 0;
            if(robotSeqence[PARAM::BLUE][roboId][camId].pos.x() > -30000 && robotSeqence[PARAM::BLUE][roboId][camId].pos.y() > -30000) {
                foundBlue = true;
                _weight = calculateWeight(camId, robotSeqence[PARAM::BLUE][roboId][camId].pos);
                blueWeight += _weight;
                blueAverage.setX(blueAverage.x() + robotSeqence[PARAM::BLUE][roboId][camId].pos.x() * _weight);
                blueAverage.setY(blueAverage.y() + robotSeqence[PARAM::BLUE][roboId][camId].pos.y() * _weight);
                blueAngle = robotSeqence[PARAM::BLUE][roboId][camId].angle;
            }
            _weight = 0;
            if(robotSeqence[PARAM::YELLOW][roboId][camId].pos.x() > -30000 && robotSeqence[PARAM::YELLOW][roboId][camId].pos.y() > -30000) {
                foundYellow = true;
                _weight = calculateWeight(camId, robotSeqence[PARAM::YELLOW][roboId][camId].pos);
                yellowWeight += _weight;
                yellowAverage.setX(yellowAverage.x() + robotSeqence[PARAM::YELLOW][roboId][camId].pos.x() * _weight);
                yellowAverage.setY(yellowAverage.y() + robotSeqence[PARAM::YELLOW][roboId][camId].pos.y() * _weight);
                yellowAngle = robotSeqence[PARAM::YELLOW][roboId][camId].angle;

            }
        }
        if (foundBlue) {
            Robot ave(blueAverage.x() / blueWeight, blueAverage.y() / blueWeight, blueAngle, roboId);
            result.addRobot(PARAM::BLUE, ave);
        }
        if (foundYellow) {
            Robot ave(yellowAverage.x() / yellowWeight, yellowAverage.y() / yellowWeight, yellowAngle, roboId);
            result.addRobot(PARAM::YELLOW, ave);
        }
    }
    if (PARAM::DEBUG) std::cout << "have found " << result.robotSize[PARAM::BLUE] << "blue car.\t" << result.robotSize[PARAM::YELLOW] << std::endl;
}



void CDealRobot::sortRobot(int color) {
    for (int id = 0; id < PARAM::ROBOTMAXID; id++) {
        bool found = false;
        for (int i = 0; i < result.robotSize[color]; i++)
            if (result.robot[color][i].id == id ) {
                if ((isOnField(result.robot[color][i].pos)) &&
                        (robotPossible[color][id] < decidePossible
                         || result.robot[color][i].pos.dist(lastRobot[color][id].pos) < DIFF_VECHILE_MAX_DIFF)) {
                    lastRobot[color][id] = result.robot[color][i];
                    found = true;
                }
            }
        if (found)  robotPossible[color][id] += upPossible;
        else { // 没看到车，猜测一个原始位置
            lastRobot[color][id].setPos(lastRobot[color][id].pos + lastRobot[color][id].velocity / ZSS::Athena::FRAME_RATE);
            if (robotPossible[color][id] >= decidePossible)
                robotPossible[color][id] -= downPossible;
            else robotPossible[color][id] -= decidePossible;
        }

        if (robotPossible[color][id] > 1.0) robotPossible[color][id] = 1.0;
        if(robotPossible[color][id] < 0.0)  robotPossible[color][id] = 0.0;
    }
    validNum[color] = 0;
    for (int id = 0; id < PARAM::ROBOTMAXID; id++)
        if(robotPossible[color][id] >= decidePossible) sortTemp[color][validNum[color]++] = lastRobot[color][id];

    //sort
    for (int i = 0; i < validNum[color] - 1; i++) {
        int maxj = i;
        for (int j = i + 1; j < validNum[color]; j++)
            if (robotPossible[color][sortTemp[color][maxj].id] <
                    robotPossible[color][sortTemp[color][j].id]) maxj = j;
        if (maxj != i) {
            Robot temp;
            temp = sortTemp[color][maxj];
            sortTemp[color][maxj] = sortTemp[color][i];
            sortTemp[color][i] = temp;
        }
    }

}
void CDealRobot::run() {
    init();
    MergeRobot();
    sortRobot(PARAM::BLUE);
    sortRobot(PARAM::YELLOW);
    result.init();
    //重新加入概率排序后的车
    for (int i = 0; i < validNum[PARAM::BLUE]; i++)
        result.addRobot(PARAM::BLUE, sortTemp[PARAM::BLUE][i]);
    for (int i = 0; i < validNum[PARAM::YELLOW]; i++)
        result.addRobot(PARAM::YELLOW, sortTemp[PARAM::YELLOW][i]);
    //滤波
    for (int i = 0; i < validNum[PARAM::BLUE]; i++) {
        Robot & robot = result.robot[PARAM::BLUE][i];
        auto & playerPosVel = _kalmanFilter[PARAM::BLUE][robot.id].update(robot.pos.x(), robot.pos.y());
        CGeoPoint filtPoint (playerPosVel(0, 0), playerPosVel(1, 0));
        CVector PlayVel(playerPosVel(2, 0), playerPosVel(3, 0));
        auto &playerRotVel = _dirFilter[PARAM::BLUE][robot.id].update(std::cos(robot.angle), std::sin(robot.angle));
        double filterDir = std::atan2(playerRotVel(1, 0), playerRotVel(0, 0));
        double rotVel = playerRotVel(0, 1) * std::cos(90 * 3.1416 / 180 + filterDir)
                        + playerRotVel(1, 1) * std::sin(90 * 3.1416 / 180 + filterDir);
        robot.pos = filtPoint;
        robot.angle = filterDir;
        robot.velocity = PlayVel;
        robot.rotateVel = rotVel;
    }
    for (int i = 0; i < validNum[PARAM::YELLOW]; i++) {
        Robot & robot = result.robot[PARAM::YELLOW][i];
        auto & playerPosVel = _kalmanFilter[PARAM::YELLOW][robot.id].update(robot.pos.x(), robot.pos.y());
        CGeoPoint filtPoint (playerPosVel(0, 0), playerPosVel(1, 0));
        CVector PlayVel(playerPosVel(2, 0), playerPosVel(3, 0));
        auto &playerRotVel = _dirFilter[PARAM::YELLOW][robot.id].update(std::cos(robot.angle), std::sin(robot.angle));
        double filterDir = std::atan2(playerRotVel(1, 0), playerRotVel(0, 0));
        double rotVel = playerRotVel(0, 1) * std::cos(90 * 3.1416 / 180 + filterDir)
                        + playerRotVel(1, 1) * std::sin(90 * 3.1416 / 180 + filterDir);
        robot.pos = filtPoint;
        robot.angle = filterDir;
        robot.velocity = PlayVel;
        robot.rotateVel = rotVel;
    }

    vm->processRobot.push(result);
}


