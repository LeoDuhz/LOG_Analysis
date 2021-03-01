#ifndef LOG_SLIDER_H
#define LOG_SLIDER_H

#include "logreader_global.h"
#include <QString>
#include <QObject>
#include <iostream>
#include <string>
#include "player.h"

class LOGREADERSHARED_EXPORT LogSlider : QObject
{
    Q_OBJECT
  public:
    LogSlider();
    bool loadFile(QString filename);
    Player m_player;
};

#endif // LOG_SLIDER_H
