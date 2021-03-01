#ifndef PLAYER_H
#define PLAYER_H

#include "log_file.h"

struct Frame {
    qint64 time;
    MessageType type;
    QByteArray data;
};

class Player
{
public:
    Player();
    ~Player();

public:
    bool load(const QString& filename, int& maxFrame, double& duration);

    QList<Frame*> packets;
};

#endif // PLAYER_H
