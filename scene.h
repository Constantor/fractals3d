#pragma once

#include "fractal3d.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QPoint>

class Scene : public QGraphicsScene {
Q_OBJECT
public:
    Fractal3D fractal;
    QPixmap pixmap;

    void drawField();

    void drawFieldOnNew();

protected:
    bool inMove = false;

    QPoint moveBegin{}, moveEnd{};

    void pressed(QPoint p);

    void moved(QPoint p);

    void released(QPoint p);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
};
