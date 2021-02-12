#include <QMessageBox>
#include <QDebug>
#include "custom_scene.h"

void CustomScene::pressed(QPoint p) {
	// doing something with a point p
	qDebug() << p.x() << ' ' << p.y(); // 0, 0 this is bad
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	emit pressed(event->pos().toPoint());
}
