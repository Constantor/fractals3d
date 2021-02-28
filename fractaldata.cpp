#include "fractaldata.h"

FractalData::FractalData(double a, double b, double c, unsigned int n) : a(a), b(b), c(c), n(n) {}

QDataStream &FractalData::operator<<(QDataStream &out) const {
	out << a << " " << b << " "
		<< " " << c << " " << n;
	return out;
}
