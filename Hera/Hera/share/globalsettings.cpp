#include "globalsettings.h"
//#include "field.h"
#include "parammanager.h"
CGlobalSettings::CGlobalSettings(QObject *parent):QObject(parent),minimumX(-999999),minimumY(-999999),maximumX(999999),maximumY(999999){

}
bool CGlobalSettings::inChosenArea(float x, float y){
    return (x >= minimumX && x <= maximumX && y >= minimumY && y <= maximumY);
}
bool CGlobalSettings::inChosenArea(CGeoPoint point){
    return (point.x() >= minimumX && point.x() <= maximumX && point.y() >= minimumY && point.y() <= maximumY);
}
