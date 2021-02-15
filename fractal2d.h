#pragma once

#include "complex2d.h"
#include "fractal_point.h"
#include <QColor>
#include <QPoint>
#include <QVector>

class Fractal2D {
private:
	Complex2D c;
	int n{};
	qreal r_conv{};
	int max_iter{};
	qreal stepx{}, stepy{};
public:
	qreal minX{}, maxX{};
	qreal minY{}, maxY{};
private:
	QVector<FractalPoint> colorField;

public:
	Fractal2D() = default;

	Fractal2D(const Complex2D &c, int n, qreal r_conv, int max_iter, qreal stepx,
			  qreal stepy, qreal minX, qreal maxX, qreal minY, qreal maxY);

	[[nodiscard]] int transformX(qreal x, int width) const;

	[[nodiscard]] int transformY(qreal y, int height) const;

	[[nodiscard]] QVector<FractalPoint> getColorField() const;

	[[nodiscard]] QVector<FractalPoint> const &getColorFieldR() const;

	void updateColorField();
};
