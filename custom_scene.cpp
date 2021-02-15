#include <QDebug>
#include <QPainter>
#include "custom_scene.h"

void CustomScene::drawField() {
	assert(fractal != nullptr && pixmap != nullptr);
	QPainter painter(pixmap);
	QVector<FractalPoint> const &colorField = fractal->getColorFieldR();
	for(auto const &point : colorField) {
		painter.setPen(point.getColor());
		painter.drawPoint(fractal->transformX(point.getX(), width()),
						  fractal->transformY(point.getY(), height()));
	}
}

void CustomScene::pressed(QPoint p) {
	inMove = true;
	moveBegin = p;
	moveEnd = {};
	// doing something with a point p
	qDebug() << "Pressed: " << p.rx() << ' ' << p.ry();
}

void CustomScene::moved(QPoint p) {
	if(inMove) {
		// qDebug() << "Pressed and moving at " << p.rx() << ' ' << p.ry();
	}
}

void CustomScene::released(QPoint p) {
	inMove = false;
	moveEnd = p;
	double dx = -(fractal->maxX - fractal->minX) * (moveEnd.x() - moveBegin.x()) /
				width();
	double dy = -(fractal->maxY - fractal->minY) * (moveEnd.y() - moveBegin.y()) / height();
	fractal->minX += dx;
	fractal->maxX += dx;
	fractal->minY += dy;
	fractal->maxY += dy;
	fractal->updateColorField();
	drawField();
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
