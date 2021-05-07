#include "fractaldata.hpp"

FractalData::FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type) : a(a), b(b), c(c), n(n), type(type) {}


QJsonObject FractalData::serialize() const {
	QJsonObject serialized;
	serialized.insert("a", a);
	serialized.insert("b", b);
	serialized.insert("c", c);
	serialized.insert("n", n);
	serialized.insert("type", type);
	return serialized;
}

void FractalData::readFrom(QJsonDocument &in) {
	QJsonObject fractalData = in.object().value("Fractal").toObject();
	a = fractalData.value("a").toDouble();
	b = fractalData.value("b").toDouble();
	c = fractalData.value("c").toDouble();
	n = fractalData.value("n").toInt();
	type = static_cast<FractalType>(fractalData.value("type").toInt());
}