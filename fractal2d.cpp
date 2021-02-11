#include "fractal2d.h"
#include "complex2d.h"

int Fractal2D::transformX(qreal x, int width) const {
	return width * (x - minX) / (maxX - minX);
}

int Fractal2D::transformY(qreal y, int height) const {
	return height * (y - minY) / (maxY - minY);
}

QVector<FractalPoint> Fractal2D::getColorField() const {
	return colorField;
}

Fractal2D::Fractal2D(const Complex2D &c, int n, qreal minX, qreal maxX, qreal minY, qreal maxY) : c(c), n(n), minX(minX), maxX(maxX), minY(minY), maxY(maxY) {
	const qreal r_conv = 2;
	const int max_iter = 100;
	qreal stepx = 0.0008;
	qreal stepy = 0.001;
	for(qreal x = minX; x < maxX; x += stepx)
		for(qreal y = minY; y < maxY; y += stepy) {
			Complex2D z(x, y);
			int iter = 0;

			while(iter < max_iter && z.Complex2D::abs() < r_conv) {
				z = z.Complex2D::pow(n) + c;
				++iter;
			}
			colorField.push_back(FractalPoint(x * (maxX - minX) , y * (maxY - minY) ,
											  QColor(iter % 64, (iter * iter) % 64, iter % 256))); //TODO leave only iter, to set brightness
		}
}
