#include "fractal_point.h"

FractalPoint::FractalPoint(qreal new_x, qreal new_y, QColor new_color) {
	x = new_x;
	y = new_y;
	color = new_color;
}

QColor FractalPoint::getColor() const {
	return color;
}

void FractalPoint::setColor(QColor new_color) {
	color = new_color;
}

qreal FractalPoint::getX() const {
	return x;
}

qreal FractalPoint::getY() const {
	return y;
}

void FractalPoint::setX(qreal new_x) {
	x = new_x;
}

void FractalPoint::setY(qreal new_y) {
	y = new_y;
}
