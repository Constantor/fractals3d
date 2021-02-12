#include <QMessageBox>
#include <QDebug>
#include "custom_scene.h"

void CustomScene::pressed(QPoint p) {
	in_move = true;
	// doing something with a point p
	qDebug() << "Pressed: " << p.x() << ' ' << p.y(); // 0, 0 this is bad
}

void CustomScene::moved(QPoint p) {
	if(in_move) {
		qDebug() << "Pressed and moving";
	}
}

void CustomScene::released(QPoint p) {
	in_move = false;
	// doing something with a point p
	qDebug() << "Released: " << p.x() << ' ' << p.y(); // 0, 0 this is bad
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	emit pressed(event->pos().toPoint());
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	emit moved(event->pos().toPoint());
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	emit released(event->pos().toPoint());
}
