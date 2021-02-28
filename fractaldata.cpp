#include "fractaldata.h"

FractalData::FractalData(qreal a, qreal b, qreal c, quint8 n) : a(a), b(b), c(c), n(n) {}

QDataStream &FractalData::operator<<(QDataStream &out) const {
	out << a << " " << b << " "
		<< " " << c << " " << n;
	return out;
}
