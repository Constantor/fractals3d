#include "complex3d.h"
#include "cmath"

Complex3D::Complex3D(qreal newx, qreal newy, qreal newz) {
    x = newx;
    y = newy;
    z = newz;
}

Complex3D operator+(Complex3D a, Complex3D b) {
    return Complex3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

qreal Complex3D::abs() const {
    return sqrt(x * x + y * y + z * z);
}

qreal Complex3D::theta() const {
    return atan2(sqrt(x * x + y * y), z);
}

qreal Complex3D::phi() const {
    return atan2(y, x);
}

void operator^(Complex3D &z, int const &n) {
    int help = 1;
    const int m = n - n % 2;
    qreal r_n = z.abs();

    while(help < m) {
        r_n = r_n * r_n;
        help *= 2;
    }

    if(n % 2 == 1) {
        r_n = z.abs() * r_n;
    }

    qreal theta_n = z.theta() * n;
    qreal phi_n = z.phi() * n;
    z.x = r_n * sin(theta_n) * cos(phi_n);
    z.y = r_n * sin(theta_n) * sin(phi_n);
    z.z = r_n * cos(theta_n);
}
