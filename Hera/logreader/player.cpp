#include "player.h"
#include <iostream>
#include <QFileInfo>
#include <QtDebug>

Player::Player() {

}

Player::~Player() {
    qDeleteAll(packets);
    packets.clear();
}

bool Player::load(const QString& filename, int& maxFrame, double& duration) {
    QFileInfo fileInfo(filename);

    bool compressed = false;

    if (fileInfo.suffix() == "gz") {
        compressed = true;
    }

    LogFile file(filename, compressed);

    if (!file.openRead()) {
        return false;
    }

    qDeleteAll(packets);
    packets.clear();

    for (;;) {

        Frame* packet = new Frame;

        if (!file.readMessage(packet->data, packet->time, packet->type) ||
                packet->time == 0 || packet->type == 0) {
            delete packet;
            break;
        }

        packets.append(packet);
    }

    maxFrame = packets.size() - 1;
    duration = packets.last()->time - packets.first()->time;
    return true;
}
