#pragma once

#include <QColor>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QVector3D>
#include <random>

enum FractalType {
	MANDELBROT,
	PSYCHOFRACTAL,
	ANOTHERFRACTAL,
	FLOWERFRACTAL,
	NEWFRACTAL,
	NEWNEWFRACTAL
};

enum ColorType {
	FRACTAL,
	AMBIENCE
};

class FractalData {
public:
	static const QVector3D baseCamera;
	static constexpr const qreal defaultZoom = -3.0;
	static constexpr const qreal defaultSpeed = 0.25;

	qreal a{}, b{}, c{};
	quint8 n = 2;
	FractalType type{};
	QColor fractalColor = QColor(55, 255, 55);
	QColor ambienceColor = QColor(255, 55, 55);
	QVector3D camera = baseCamera;
	qreal zoomCoefficient = defaultZoom;
	qreal rotateSpeed = defaultSpeed;
	bool isRotating = false;

	void genRandom();

	FractalData();
	[[maybe_unused]] FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type);
	FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type, const QColor &fractalColor, const QColor &ambienceColor, const QVector3D &camera, qreal zoomCoefficient, bool isRotating);


	[[nodiscard]] QJsonObject serialize() const;
	void readFrom(QJsonDocument &in);
};