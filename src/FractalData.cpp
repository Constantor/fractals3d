#include "FractalData.hpp"
#include <functional>
#include <map>
#include <utility>
#include <vector>
#include <QVector>
#include <QPair>

const QVector3D FractalData::baseCamera = QVector3D(0, 0, 1.5);// is more centered, but worse in some way

namespace {
	qreal randomReal() {
		std::uniform_real_distribution dist(-0.3, 0.3);
		return dist(*QRandomGenerator::global());
	}

	QColor randomColor() {
		return QColor(QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255));
	}

	static std::map<const std::string, const std::pair<std::function<qreal(std::vector<int> const &, std::vector<int> const &)>, const qreal>> metrics = {
			{"minkowski-normalized",
			 {
					 [](std::vector<int> const &u, std::vector<int> const &v) -> qreal {
						 static const int p = 5;
						 qreal out = 0;
						 for(size_t i = 0; i < std::min(u.size(), v.size()); i++)
							 out += std::pow(std::abs(u[i] - v[i]) / 255., p);
						 return std::pow(out, 1. / p) / std::pow(3, 1. / p);
					 },
					 0.25// threshold
			 }}};

	bool isSimilar(QColor const &u, QColor const &v, std::pair<std::function<qreal(std::vector<int>, std::vector<int>)>, qreal> const &metric) {
		static const auto toVector = [](QColor const &color) -> std::vector<int> {
			return {color.red(), color.green(), color.blue()};
		};
		return metric.first(toVector(u), toVector(v)) < metric.second;
	};

	bool isBlack(QColor const &color) {
		return color.value() < 150;
	}

}// namespace

[[maybe_unused]] FractalData::FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type) : a(a), b(b), c(c), n(n), type(type) {}

FractalData::FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type, const QColor &fractalColor, const QColor &ambienceColor, const QVector3D &camera, qreal zoomCoefficient) : a(a), b(b), c(c), n(n), type(type), fractalColor(fractalColor), ambienceColor(ambienceColor), camera(camera), zoomCoefficient(zoomCoefficient) {}

QJsonObject FractalData::serialize() const {
	QJsonObject serialized;
	serialized.insert("a", a);
	serialized.insert("b", b);
	serialized.insert("c", c);
	serialized.insert("n", n);
	serialized.insert("type", type);
	serialized.insert("Fractal color", fractalColor.name());
	serialized.insert("Ambience color", ambienceColor.name());
	serialized.insert("zoomCoefficient", zoomCoefficient);
	QJsonArray cameraPosition;
	cameraPosition.insert(0, camera.x());
	cameraPosition.insert(1, camera.y());
	cameraPosition.insert(2, camera.z());
	serialized.insert("camera", cameraPosition);
	return serialized;
}

void FractalData::readFrom(QJsonDocument &in) {
	genRandom();
	QJsonObject fractalData;
	try {
		fractalData = in.object().value("Fractal").toObject();
	} catch(...) {
		return;
	}
	if(fractalData.contains("camera")) {
		QJsonArray cameraPosition = fractalData.value("camera").toArray();
		camera = QVector3D(cameraPosition[0].toDouble(), cameraPosition[1].toDouble(), cameraPosition[2].toDouble());
	} else
		camera = baseCamera;
	for(auto &[name, reference] : QVector<QPair<QString, qreal&>>{{"a", a}, {"b", b}, {"c", c}})
		if(fractalData.contains(name))
			reference = fractalData.value(name).toDouble();
	if(fractalData.contains("n"))
		n = fractalData.value("n").toInt();
	if(fractalData.contains("type"))
		type = static_cast<FractalType>(fractalData.value("type").toInt());
	if(fractalData.contains("Fractal color"))
		fractalColor = QColor(fractalData.value("Fractal color").toString());
	if(fractalData.contains("Ambience color"))
		ambienceColor = QColor(fractalData.value("Ambience color").toString());
	if(fractalData.contains("zoomCoefficient"))
		zoomCoefficient = fractalData.value("zoomCoefficient").toDouble();
}

void FractalData::genRandom() {
	a = randomReal();
	b = randomReal();
	c = randomReal();
	n = 2 * (QRandomGenerator::global()->bounded(4) + 2);
	type = FractalType(QRandomGenerator::global()->bounded(5));
	auto genColors = [this]() {
		fractalColor = randomColor();
		ambienceColor = randomColor();
	};
	do {
		genColors();;
	} while(isSimilar(fractalColor, ambienceColor, metrics["minkowski-normalized"]) || isBlack(fractalColor));
	zoomCoefficient = defaultZoom;
}

FractalData::FractalData() {
	genRandom();
}

QVector3D FractalData::zoomedCamera() const {
	return camera / zoomCoefficient;
}
