#include "FractalData.hpp"

namespace {
	qreal randomReal() {
		std::uniform_real_distribution dist(-0.3, 0.3);
		return dist(*QRandomGenerator::global());
	}

	QColor randomColor() {
		return QColor(QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255));
	}
}// namespace

[[maybe_unused]] FractalData::FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type) : a(a), b(b), c(c), n(n), type(type) {}

FractalData::FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type, const QColor &fractalColor, const QColor &ambienceColor, const QVector3D &camera) : a(a), b(b), c(c), n(n), type(type), fractalColor(fractalColor), ambienceColor(ambienceColor), camera(camera) {}

QJsonObject FractalData::serialize() const {
	QJsonObject serialized;
	serialized.insert("a", a);
	serialized.insert("b", b);
	serialized.insert("c", c);
	serialized.insert("n", n);
	serialized.insert("type", type);
	serialized.insert("Fractal color", fractalColor.name());
	serialized.insert("Ambience color", ambienceColor.name());
	QJsonArray cameraPosition;
	cameraPosition.insert(0, camera.x());
	cameraPosition.insert(1, camera.y());
	cameraPosition.insert(2, camera.z());
	serialized.insert("camera", cameraPosition);
	return serialized;
}

void FractalData::readFrom(QJsonDocument &in) {
	QJsonObject fractalData = in.object().value("Fractal").toObject();
	a = fractalData.value("a").toDouble();
	b = fractalData.value("b").toDouble();
	c = fractalData.value("c").toDouble();
	n = fractalData.value("n").toInt();
	type = static_cast<FractalType>(fractalData.value("type").toInt());
	fractalColor = QColor(fractalData.value("Fractal color").toString());
	ambienceColor = QColor(fractalData.value("Ambience color").toString());
	//	QJsonArray cameraPosition = fractalData.value("camera").toArray();
	//	camera = QVector3D(cameraPosition[0].toDouble(), cameraPosition[1].toDouble(), cameraPosition[2].toDouble());
	camera = QVector3D(0.0, 0.0, 1.5);
}

FractalData::FractalData() {
	a = randomReal();
	b = randomReal();
	c = randomReal();
	n = 2 * (QRandomGenerator::global()->bounded(15) + 1);
	type = FractalType(QRandomGenerator::global()->bounded(3));
	fractalColor = randomColor();
	ambienceColor = randomColor();
}
