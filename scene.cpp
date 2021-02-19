#include "scene.h"
#include <QDebug>
#include <QPainter>

void Scene::drawField() {
	QPainter painter(&pixmap);
	QVector<FractalPoint> const &colorField = fractal.getColorFieldR();
	for(auto const &point : colorField) {
		painter.setPen(point.getColor());
		painter.drawPoint(fractal.transformX(point.getX(), width()),
						  fractal.transformY(point.getY(), height()));
	}
}

void Scene::drawFieldOnNew() {
	pixmap = QPixmap(width(), height());
	drawField();
	addPixmap(pixmap);
}

void Scene::pressed(QPoint p) {
	inMove = true;
	moveBegin = p;
	moveEnd = {};
	// doing something with a point p
	qDebug() << "Pressed: " << p.rx() << ' ' << p.ry();
}

void Scene::moved(QPoint) {
	if(inMove) {
		// qDebug() << "Pressed and moving at " << p.rx() << ' ' << p.ry();
	}
}

void Scene::released(QPoint p) {
	inMove = false;
	moveEnd = p;
	double dx = -(fractal.maxX - fractal.minX) * (moveEnd.x() - moveBegin.x()) /
				width();
	double dy = -(fractal.maxY - fractal.minY) * (moveEnd.y() - moveBegin.y()) / height();
	fractal.minX += dx;
	fractal.maxX += dx;
	fractal.minY += dy;
	fractal.maxY += dy;
	fractal.updateColorField();
	drawFieldOnNew();
	// doing something with a point p
	qDebug() << "Released: " << p.rx() << ' ' << p.ry();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	pressed(event->screenPos());
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	moved(event->screenPos());
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	released(event->screenPos());
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event) {
	if(event->orientation() != Qt::Vertical || 45 < event->delta()) {
		event->accept();
		return;
	}

	qDebug() << "Zooming " << event->delta();
	qreal k = -zoomSensitivity * event->delta() / 45.;
	qreal bound = 0.98;
	if(k < -bound)
		k = -bound;
	if(bound < k)
		k = bound;

	QPointF pos = event->scenePos();
	qreal left = pos.rx() / width();
	qreal right = 1. - left;
	qreal top = pos.ry() / height();
	qreal bottom = 1. - top;

	qreal dx = fractal.maxX - fractal.minX;
	qreal dy = fractal.maxY - fractal.minY;
	fractal.minX += dx * k * left;
	fractal.maxX += dx * k * right;
	fractal.minY += dy * k * top;
	fractal.maxY += dy * k * bottom;

	fractal.updateColorField();
	drawFieldOnNew();
	event->accept();

	qDebug() << "End zooming";
}
