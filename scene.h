#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QPoint>
#include <QWheelEvent>
#include "fractal2d.h"

class Scene : public QGraphicsScene {
Q_OBJECT
public:
	Fractal2D fractal;
	QPixmap pixmap;
	qreal zoomSensitivity = 2.;

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

	void wheelEvent(QGraphicsSceneWheelEvent *event);

signals:
};
