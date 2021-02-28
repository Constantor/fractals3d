#include "fractaldata.h"

FractalData::FractalData(qreal a, qreal b, qreal c, quint8 n) : a(a), b(b), c(c), n(n) {}


QDataStream &FractalData::readFrom(QDataStream &in) {
	try {
		in >> a >> b >> c >> n;
	} catch(...) {
		throw;
	}
	return in;
}
QJsonObject FractalData::serialize() const {
	QJsonObject serialized;
	serialized.insert("a", a);
	serialized.insert("b", b);
	serialized.insert("c", c);
	serialized.insert("n", n);
	return serialized;
}
