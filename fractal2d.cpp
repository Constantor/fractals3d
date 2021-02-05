#include "fractal2d.h"

Fractal2D::Fractal2D(qreal newMinX, qreal newMaxX, qreal newMinY, qreal newMaxY) {
	min_x = newMinX;
	max_x = newMaxX;
	min_y = newMinY;
	max_y = newMaxY;
}

qreal Fractal2D::getMinX() const {
	return min_x;
}

qreal Fractal2D::getMaxX() const {
	return max_x;
}

qreal Fractal2D::getMinY() const {
	return min_y;
}

qreal Fractal2D::getMaxY() const {
	return max_y;
}

void Fractal2D::setMinX(qreal value) {
	min_x = value;
}

void Fractal2D::setMaxX(qreal value) {
	max_x = value;
}

void Fractal2D::setMinY(qreal value) {
	min_y = value;
}

void Fractal2D::setMaxY(qreal value) {
	max_y = value;
}

QVector<FractalPoint> Fractal2D::getColorField() const {
	QVector<FractalPoint> result;

	// temporary code
	int n = 30000;
	while(n--)
		result.push_back(FractalPoint((static_cast<qreal>(rand()) / RAND_MAX - 0.5) * 2, (static_cast<qreal>(rand()) / RAND_MAX - 0.5) * 2, QColor(rand() % 256, rand() % 256, rand() % 256)));
	// end temporary code

	// TODO: generate 2D fractal

	return result;
}
