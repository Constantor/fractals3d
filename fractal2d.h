#pragma once

#include "complex2d.h"
#include "fractal_point.h"
#include <QColor>
#include <QPoint>
#include <QVector>
#include <complex>

class Fractal2D {
private:
	Complex2D c;
	int n{};
	qreal minX{}, maxX{};
	qreal minY{}, maxY{};
	QVector<FractalPoint> colorField;

public:
	Fractal2D() = default;
	Fractal2D(const Complex2D &c, int n, qreal minX, qreal maxX, qreal minY, qreal maxY);

	[[nodiscard]] int transformX(qreal x, int width) const;

	[[nodiscard]] int transformY(qreal y, int height) const;

	[[nodiscard]] QVector<FractalPoint> getColorField() const;
};
