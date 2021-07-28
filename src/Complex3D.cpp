#include "Complex3D.hpp"
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
