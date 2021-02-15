#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>

class CustomScene : public QGraphicsScene {
	Q_OBJECT
protected:
	bool in_move = false;

	void pressed(QPoint p);

	void moved(QPoint p);

	void released(QPoint p);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
};
