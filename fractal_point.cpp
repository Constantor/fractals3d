#include "fractal_point.h"

FractalPoint::FractalPoint(qreal newX, qreal newY, QColor newColor) {
	x = newX;
	y = newY;
	color = newColor;
}

QColor FractalPoint::getColor() const {
	return color;
}

void FractalPoint::setColor(QColor newColor) {
	color = newColor;
}

qreal FractalPoint::getX() const {
	return x;
}

qreal FractalPoint::getY() const {
	return y;
}

void FractalPoint::setX(qreal newX) & {
	x = newX;
}

void FractalPoint::setY(qreal newY) & {
	y = newY;
}
