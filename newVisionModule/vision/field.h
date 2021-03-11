#ifndef FIELD_H
#define FIELD_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <mutex>
#include <vector>
#include "messageformat.h"
#include "staticparams.h"
#include "singleton.hpp"
#include "zsplugin.hpp"
#include "cvision.h"

class Field : public QQuickPaintedItem, public ZSPlugin{
    Q_OBJECT
public:
    Q_INVOKABLE void resetsize(int, int, bool updated = true);
    Field(QQuickItem *parent = nullptr);
    void paint(QPainter* painter) override;
    void run() override;
    void drawvision();
    void drawcar(const QColor& color, quint8 num, qreal x, qreal y, qreal radian, const QColor& textColor);
    void drawball(const QColor& color, quint8 num, qreal x, qreal y);
    CGeoPoint pointtranform(CGeoPoint origin);
    double directiontransform(double origindir);
    Q_INVOKABLE void repaint();
private:
    QPixmap* map;
    QPainter _painter;
    QRect area;
    std::mutex pixmap_mutex;

};

#endif // FIELD_H
