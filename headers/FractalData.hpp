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
	FLOWERFRACTAL
};

enum ColorType {
	FRACTAL,
	AMBIENCE
};

class FractalData {
public:
	static const QVector3D baseCamera;// set in FractalData.cpp

	qreal a{}, b{}, c{};
	quint8 n = 2;
	FractalType type{};
	QColor fractalColor = QColor(55, 255, 55);
	QColor ambienceColor = QColor(255, 55, 55);
	QVector3D camera = baseCamera;// I tried (-1.3, -0.6, 1.5), it was more centered, but not so beautiful
	bool isRotating = true;
	qreal zoomCoefficient = 1.0;

	void genRandom();

	FractalData();
	FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type, const QColor &fractalColor, const QColor &ambienceColor, const QVector3D &camera, bool isRotating);

	[[nodiscard]] QJsonObject serialize() const;
	void readFrom(QJsonDocument &in);
};