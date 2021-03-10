#include <QGuiApplication>
#include <QProcess>
#include <QtDebug>
#include "interaction.h"
#include "parammanager.h"
#include "actionmodule.h"
namespace{
    auto pm = ZSS::LParamManager::instance();
}
Interaction::Interaction(QObject *parent) : QObject(parent){
}

void Interaction::restartApp(){
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
void Interaction::setFieldWidth(int width){
    pm->changeParam("auto/fieldWidth",width);
}
void Interaction::setFieldHeight(int height){
    pm->changeParam("auto/fieldHeight",height);
}
int Interaction::getFieldWidth(){
    int width;
    pm->loadParam(width,"auto/fieldWidth",2400);
    return width;
}
int Interaction::getFieldHeight(){
    int height;
    pm->loadParam(height,"auto/fieldHeight",1800);
    return height;
}
void Interaction::sendCommand() {
    /*czkdebug*/std::cout<<"send command"<<std::endl;

}
