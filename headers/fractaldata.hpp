#pragma once

#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QColor>

enum FractalType {
	MANDELBROT,
	PSYCHOFRACTAL,
	ANOTHERFRACTAL,
	FLOWERFRACTAL
};

enum ColorType{
	FRACTAL,
	AMBIENCE
};

struct FractalData {
	qreal a{}, b{}, c{};
	quint8 n{};
	FractalType type{};
    QColor fractalColor = QColor(55, 255, 55);
    QColor ambienceColor = QColor(255, 55, 55);

	FractalData() = default;
	[[maybe_unused]] FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type);
	FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type, const QColor &fractalColor, const QColor &ambienceColor);

	[[nodiscard]] QJsonObject serialize() const;
	void readFrom(QJsonDocument &in);
};