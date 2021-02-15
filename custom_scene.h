#pragma once

#include <QGraphicsScene>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <memory>
#include "fractal2d.h"

class CustomScene : public QGraphicsScene {
Q_OBJECT
public:
	QPixmap *pixmap;

	Fractal2D *fractal;

	void draw();

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
