#include "field.h"
#include "actionmodule.h"
#include <QDebug>
#include <QtMath>
namespace  {
    const static QColor CAR_COLOR[2]  = {QColor(25, 30, 150), QColor(241, 201, 50)};
    const static QColor COLOR_ORANGE(255, 0, 255);
    const static QColor COLOR_DARKGREEN(48, 80, 48);
    int carDiameter = 24;
    int ballradius = 5;
    int numberSize = -20;
}
Field::Field(QQuickItem *parent) : QQuickPaintedItem(parent), ZSPlugin("drawmap") {
    map = new QPixmap(1200,900);
    area = QRect(0, 0, 1200, 900);
    _painter.begin(map);
    _painter.setPen(QPen(QBrush(QColor(255,255,255)),20));
    _painter.setRenderHint(QPainter::Antialiasing, true);
    _painter.setRenderHint(QPainter::TextAntialiasing, true);
}

void Field::paint(QPainter *painter) {
    if(pixmap_mutex.try_lock()){
            painter->drawPixmap(area, *map);
            pixmap_mutex.unlock();
      }
}

void Field::repaint() {
//    static int count;
//    packet.ParseFromArray(ptr, size);
    //map->fill(QColor("#8eb993"));
    /*czkdebug*///std::cout<<"repaint"<<std::endl;
    if(pixmap_mutex.try_lock()){
        map->fill(COLOR_DARKGREEN);//COLORTODO
        drawvision();
    }
    pixmap_mutex.unlock();
    _painter.drawPixmap(area, *map);
    this->update(area);
}

CGeoPoint Field::pointtranform(CGeoPoint origin) {
    const CGeoPoint final(origin.x()/10 + 600, (-origin.y()/10 + 450));
    return final;
}

double Field::directiontransform(double origindir) {
    double dir = (origindir/M_PI*180)+22.5;
    /*czkdebug*///std::cout<<"transformed dir"<<dir<<std::endl;
    return dir;
}

void Field::drawvision() {
//    drawcar(CAR_COLOR[0], 1, 1000, 500, PARAM::Math::PI/4,Qt::white);
//    drawcar(CAR_COLOR[1], 1, 200, 40, PARAM::Math::PI/4,Qt::white);
//    drawball(COLOR_ORANGE, 1000, 200);
    /*czkdebug*///std::cout<<"hello"<<CvisionModule::instance()->Frame.validSize()<<std::endl;
    if(CvisionModule::instance()->Frame.validSize()<1) return;
    int robotnum[2] = {0,0};
    FilterFrame frame = CvisionModule::instance()->Frame[0];
    for(int color = PARAM::BLUE;color<=PARAM::YELLOW;color++) {
        for (int i=0;i<PARAM::ROBOTNUM;i++){
            Robot* robot = frame.robots[color][i];
            if(robot->isobserved == 1) {
                const CGeoPoint pos = pointtranform(robot->filteredpos);
                double dir = directiontransform(robot->filtereddir);
                /*czkdebug*///std::cout<<"robot"<<i<<"->filtereddir"<<robot->filtereddir<<std::endl;
                drawcar(CAR_COLOR[color], i, pos.x(), pos.y(), dir, Qt::white);
                robotnum[color]++;
            }
        }
    }
    /*czkdebug*///std::cout<<"ball number: "<<CvisionModule::instance()->ballnum<<std::endl;
    if(CvisionModule::instance()->get_oneball()){
        Ball* ball = frame.balls[0];
        const CGeoPoint ballpos = pointtranform(ball->filterdpos);
        drawball(COLOR_ORANGE, 0, ballpos.x(), ballpos.y());
    }
    else{
        int i;
        for(i=0;i<PARAM::BALLNUM;i++) {
            Ball* ball = frame.balls[i];
            if(!ball->isobserved) continue;
            const CGeoPoint ballpos = pointtranform(ball->filterdpos);
            /*czkdebug*///std::cout<<"ball pos"<<ballpos<<std::endl;
            drawball(COLOR_ORANGE, i, ballpos.x(), ballpos.y());
        }
    }

    /*czkdebug*///std::cout<<"ballnum: "<<i<<std::endl;
}

void Field::drawcar(const QColor& color, quint8 num, qreal x, qreal y, double dir, const QColor& textColor) {
    static qreal radius = carDiameter / 2.0;
    _painter.setBrush(Qt::NoBrush);
    _painter.setPen(QPen(color, 4));
    _painter.drawPie(QRectF(x - radius, y - radius, 2 * radius, 2 * radius), dir*16, 270*16);
    _painter.setBrush(Qt::NoBrush);
    _painter.setPen(QPen(textColor, 30));
    QFont font;
    int fontSize = -numberSize;
    font.setPixelSize(fontSize);
    _painter.setFont(font);
    _painter.drawText(x - numberSize,y + carDiameter * 0.4, QString::number(num, 16).toUpper());
}

void Field::drawball(const QColor &color, quint8 num, qreal x, qreal y) {
    _painter.setBrush(Qt::NoBrush);
    _painter.setPen(QPen(color, 9));
    _painter.drawEllipse(QRectF(x - ballradius, y - ballradius, 2 * ballradius, 2 * ballradius));
    QFont font;
    int fontSize = -numberSize;
    font.setPixelSize(fontSize);
    _painter.setFont(font);
    _painter.drawText(x - numberSize, y, QString::number(num, 16).toUpper());
}

void Field::run(){
}

void Field::resetsize(int, int, bool updated){
    return;
}

