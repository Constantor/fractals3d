#pragma once

#include <QGraphicsScene>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include "fractal2d.h"

class CustomScene : public QGraphicsScene {
Q_OBJECT
public:
	Fractal2D *fractal{};
	QPixmap *pixmap;

	void drawField();

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
