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

Fractal2D::Fractal2D(const Complex2D &c, int n, qreal r_conv, int max_iter, qreal stepx,
					 qreal stepy, qreal minX, qreal maxX, qreal minY, qreal maxY)
	: c(c), n(n), r_conv(r_conv), max_iter(max_iter), stepx(stepx),
	  stepy(stepy), minX(minX), maxX(maxX), minY(minY), maxY(maxY) {
	for(qreal x = minX; x < maxX; x += stepx) {
		for(qreal y = minY; y < maxY; y += stepy) {
			Complex2D z(x, y);
			int iter = 0;

			while(iter < max_iter && z.Complex2D::abs() < r_conv) {
				z ^ n;
				z = z + c;
				++iter;
			}
			colorField.push_back(FractalPoint(x * (maxX - minX), y * (maxY - minY),
											  QColor(iter % 64, (iter * iter) % 64, iter % 256)));//TODO leave only iter, to set brightness
		}
	}
}
