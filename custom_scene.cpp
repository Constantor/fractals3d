#include <QMessageBox>
#include <QDebug>
#include "custom_scene.h"

void CustomScene::pressed(QPoint p) {
	in_move = true;
	// doing something with a point p
	qDebug() << "Pressed: " << p.rx() << ' ' << p.ry();
}

void CustomScene::moved(QPoint p) {
	if(in_move) {

		qDebug() << "Pressed and moving at " << p.rx() << ' ' << p.ry();
	}
}

void CustomScene::released(QPoint p) {
	in_move = false;
	// doing something with a point p
	qDebug() << "Released: " << p.rx() << ' ' << p.ry();
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	pressed(event->screenPos());
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	moved(event->screenPos());
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	released(event->screenPos());
}
