#include "log_slider.h"
#include <QDebug>
#include <QDir>

LogSlider::LogSlider() {

}

bool LogSlider::loadFile(QString filename) {

    int maxFrame;
    double duration;

    if (m_player.load(filename, maxFrame, duration)) {

        return true;
    }
    return false;
}
