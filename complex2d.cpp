#include "complex2d.h"
#include "cmath"

Complex2D::Complex2D(double new_real, double new_im) {
    this->real = new_real;
    this->im = new_im;
}

Complex2D operator+(Complex2D a, Complex2D b) {
    return Complex2D(a.real + b.real, a.im + b.im);
}

Complex2D operator*(Complex2D a, Complex2D b) {
    double new_real = a.real * b.real - a.im * b.im;
    double new_im = a.real * b.im + a.im * b.real;
    return Complex2D(new_real, new_im);
}

double Complex2D::abs() const {
    return sqrt(this->real * this->real + this->im * this->im);
}

Complex2D Complex2D::pow(int const &n) {
    int help = 1;
    Complex2D new_z(this->real, this->im);

    while (help < n - n % 2) {
        new_z = new_z * new_z;
        help *= 2;
    }

    if (n % 2 == 1) {
        new_z = new_z * (*this);
    }
    return new_z;
}