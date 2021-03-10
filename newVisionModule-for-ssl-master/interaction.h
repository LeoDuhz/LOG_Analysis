#ifndef INTERACTION_H
#define INTERACTION_H

#include <QObject>
#include <QString>
class Interaction : public QObject
{
    Q_OBJECT
public:
    explicit Interaction(QObject *parent = nullptr);
    // set
    Q_INVOKABLE void restartApp();
    // field
    Q_INVOKABLE int getFieldWidth();
    Q_INVOKABLE int getFieldHeight();
    Q_INVOKABLE void setFieldWidth(int);
    Q_INVOKABLE void setFieldHeight(int);
    Q_INVOKABLE void sendCommand();
signals:

public slots:
};

#endif // INTERACTION_H
