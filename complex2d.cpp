#include "complex2d.h"
#include "cmath"

Complex2D::Complex2D(qreal new_real, qreal new_im) {
	real = new_real;
	im = new_im;
}

Complex2D operator+(Complex2D a, Complex2D b) {
	return Complex2D(a.real + b.real, a.im + b.im);
}

Complex2D operator*(Complex2D a, Complex2D b) {
	qreal new_real = a.real * b.real - a.im * b.im;
	qreal new_im = a.real * b.im + a.im * b.real;
	return Complex2D(new_real, new_im);
}

qreal Complex2D::abs() const {
	return sqrt(real * real + im * im);
}

void operator^(Complex2D &z, int const &n) {
	int help = 1;
	const int m = n - n % 2;
	Complex2D old_z = z;

	while(help < m) {
		z = z * z;
		help *= 2;
	}

	if(n % 2 == 1) {
		z = old_z * z;
	}
}
