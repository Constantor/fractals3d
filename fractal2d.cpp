#include "fractal2d.h"
#include "complex2d.h"

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

    const double r_conv = 2; // радиус сходимости
    const int n = 2, max_iter = 100; // степень, верхнее ограничение на итерации
    Complex2D c(0.36, 0.36); // константа
    double step = 0.002;

    for (double x = minX; x < maxX; x += step)
        for (double y = minY; y < maxY; y += step) {
            Complex2D z(x, y);
            int iter = 0;

            while(iter < max_iter && z.Complex2D::abs() < r_conv) {
                z = z.Complex2D::pow(n) + c;
                ++iter;
            }

            result.push_back(FractalPoint(x * (maxX - minX) + minX, y * (maxY - minY) + minY,
                                          QColor(0 % 256, 0 % 256, iter % 256)));
        }

    return result;
}