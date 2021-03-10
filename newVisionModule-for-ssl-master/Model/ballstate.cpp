#include "ballstate.h"

BallState::BallState() : state(0){
}

int BallState::GetBallState() {
    return state;
}

void BallState::update(CVector ball_v, double distance_to_car) {
    Ball_speed.push(ball_v);
    /*czkdebug*///std::cout<<"ball state: "<<state<<std::endl;
    if(Ball_speed.validSize()<3) return;
    switch (state) {
    case 0:
        if(distance_to_car < 200) state = 1;
        break;
    case 1:
        if(distance_to_car > 300 && GetFromV()==1){
            state = 2;
//            /*czkdebug*/std::cout<<"begin fly"<<std::endl;
        }
        else if(distance_to_car > 300 && GetFromV()==0) state = 0;
        break;
    case 2:
        if(GetFromV()==0) state = 0;
//        /*czkdebug*/std::cout<<"hit the ground"<<std::endl;
        break;
    default:
        state = 0;
    }
}

int BallState::GetFromV() {
    if(Ball_speed[0].mod()<2) return 0;
    int framesize = Ball_speed.validSize();
    for(int i=0;i<framesize-1&&i<3;i++) {
//        /*czkdebug*/std::cout<<"theta: "<<abs(Ball_speed[i].dir()-Ball_speed[i+3].dir())<<std::endl;
        if(Ball_speed[i+3].mod()>2 && abs(Ball_speed[i].dir()-Ball_speed[i+3].dir())>PARAM::Math::PI/12) return 1;
    }
}
