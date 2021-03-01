#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H
#include <QObject>
#include "geometry.h"
#include <singleton.hpp>
class CGlobalSettings : public QObject{
    Q_OBJECT
public:
    CGlobalSettings(QObject *parent = 0);
    double minimumX,maximumX,minimumY,maximumY;
    double ballPlacementX,ballPlacementY;
    bool inChosenArea(float x, float y);
    bool inChosenArea(CGeoPoint);
signals:
    void needRepaint();
    void addOutput(const QString&);
    void clearOutput();
};
#endif // GLOBALSETTINGS_H
