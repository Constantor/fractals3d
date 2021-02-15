#pragma once

#include <QPoint>

class Complex2D {
private:
	qreal real = 0;
	qreal im = 0;

public:
	Complex2D() = default;

	Complex2D(qreal newReal, qreal newIm);

	[[nodiscard]] qreal abs() const;

	friend void operator^(Complex2D &z, int const &n);

	friend Complex2D operator+(Complex2D a, Complex2D b);

	friend Complex2D operator*(Complex2D a, Complex2D b);
};
