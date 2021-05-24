#include "FractalData.hpp"
#include <functional>
#include <vector>
#include <utility>
#include <map>

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

void FractalData::genRandom(bool similarityProtection) {
	a = randomReal();
	b = randomReal();
	c = randomReal();
	n = 2 * (QRandomGenerator::global()->bounded(15) + 1);
	type = FractalType(QRandomGenerator::global()->bounded(3));
	auto genColors = [this]() {
		fractalColor = randomColor();
		ambienceColor = randomColor();
	};
	genColors();
	if(similarityProtection) {
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
								0.2 // threshold
						}
				}
		};
		auto isSimilar = [](QColor const &u, QColor const &v, std::pair<std::function<qreal(std::vector<int>, std::vector<int>)>, qreal> const &metric) -> bool {
			static const auto toVector = [](QColor const &color) -> std::vector<int> {
				return {color.red(), color.green(), color.blue()};
			};
			return metric.first(toVector(u), toVector(v)) < metric.second;
		};
		while(isSimilar(fractalColor, ambienceColor, metrics["minkowski-normalized"]))
			genColors();
	}
}

FractalData::FractalData() {
	genRandom(true);
}
