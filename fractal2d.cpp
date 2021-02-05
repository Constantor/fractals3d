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

int Fractal2D::transformX(qreal x, int width) const {
	return width * (x - minX) / (maxX - minX);
}

int Fractal2D::transformY(qreal y, int height) const {
	return height * (y - minY) / (maxY - minY);
}

void Fractal2D::setMinX(qreal value) &{
	minX = value;
}

void Fractal2D::setMaxX(qreal value) &{
	maxX = value;
}

void Fractal2D::setMinY(qreal value) &{
	minY = value;
}

void Fractal2D::setMaxY(qreal value) &{
	maxY = value;
}

QVector<FractalPoint> Fractal2D::getColorField() const {
	QVector<FractalPoint> result;

	// temporary code
	int n = 80'000;
	while(n--) {
		double k1 = (static_cast<qreal>(rand()) / RAND_MAX - 0.5) * 2;
		double k2 = (static_cast<qreal>(rand()) / RAND_MAX - 0.5) * 2;
		result.push_back(FractalPoint(k1 * (maxX - minX) + minX, k2 * (maxY - minY) + minY,
									  QColor(rand() % 256, rand() % 256, rand() % 256)));
	}
	// end temporary code

	// TODO: generate 2D fractal


	return result;
}
