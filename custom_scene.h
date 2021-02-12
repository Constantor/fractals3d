#pragma once

#include <QGraphicsScene>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>

class CustomScene : public QGraphicsScene {
Q_OBJECT
protected:
	void pressed(QPoint p);

	void released(QPoint p);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
};
