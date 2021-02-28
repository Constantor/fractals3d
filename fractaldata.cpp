#include "fractaldata.h"

FractalData::FractalData(qreal a, qreal b, qreal c, quint8 n) : a(a), b(b), c(c), n(n) {}


QJsonObject FractalData::serialize() const {
	QJsonObject serialized;
	serialized.insert("a", a);
	serialized.insert("b", b);
	serialized.insert("c", c);
	serialized.insert("n", n);
	return serialized;
}

void FractalData::readFrom(QJsonDocument &in) {
	QJsonObject fractalData = in.object().value("Fractal").toObject();
	a = fractalData.value("a").toDouble();
	b = fractalData.value("b").toDouble();
	c = fractalData.value("c").toDouble();
	n = fractalData.value("n").toInt();
}
