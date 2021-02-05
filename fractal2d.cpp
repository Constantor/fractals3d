#include "fractal2d.h"

Fractal2D::Fractal2D(qreal newMinX, qreal newMaxX, qreal newMinY, qreal newMaxY) {
	minX = newMinX;
	maxX = newMaxX;
	minY = newMinY;
	maxY = newMaxY;
}

qreal Fractal2D::getMinX() const {
	return minX;
}

qreal Fractal2D::getMaxX() const {
	return maxX;
}

qreal Fractal2D::getMinY() const {
	return minY;
}

qreal Fractal2D::getMaxY() const {
	return maxY;
}

void Fractal2D::setMinX(qreal value) {
	minX = value;
}

void Fractal2D::setMaxX(qreal value) {
	maxX = value;
}

void Fractal2D::setMinY(qreal value) {
	minY = value;
}

void Fractal2D::setMaxY(qreal value) {
	maxY = value;
}

QVector<FractalPoint> Fractal2D::getColorField() const {
	QVector<FractalPoint> result;

	// temporary code
	int n = 30000;
	while(n--)
		result.push_back(FractalPoint((static_cast<qreal>(rand()) / RAND_MAX - 0.5) * 2,
									  (static_cast<qreal>(rand()) / RAND_MAX - 0.5) * 2,
									  QColor(rand() % 256, rand() % 256, rand() % 256)));
	// end temporary code

	// TODO: generate 2D fractal


	return result;
}
