#include "complex2d.h"
#include "cmath"

Complex2D::Complex2D(qreal newReal, qreal newIm) {
	real = newReal;
	im = newIm;
}

Complex2D operator+(Complex2D a, Complex2D b) {
	return Complex2D(a.real + b.real, a.im + b.im);
}

Complex2D operator*(Complex2D a, Complex2D b) {
	qreal newReal = a.real * b.real - a.im * b.im;
	qreal newIm = a.real * b.im + a.im * b.real;
	return Complex2D(newReal, newIm);
}

qreal Complex2D::abs() const {
	return sqrt(real * real + im * im);
}

void operator^(Complex2D &z, int const &n) {
	int help = 1;
	const int m = n - n % 2;
	Complex2D oldZ = z;

	while(help < m) {
		z = z * z;
		help *= 2;
	}

	if(n % 2 == 1) {
		z = oldZ * z;
	}
}
