#pragma once

#include <QGraphicsScene>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>

class CustomScene : public QGraphicsScene {
Q_OBJECT
protected:
	void pressed(QPoint p);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
};
