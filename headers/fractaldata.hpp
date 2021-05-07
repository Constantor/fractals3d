#pragma once

#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

enum FractalType {
	MANDELBROT,
	PSYCHOFRACTAL,
	ANOTHERFRACTAL,
	FLOWERFRACTAL
};

struct FractalData {
	qreal a{}, b{}, c{};
	quint8 n{};
	FractalType type{};

	FractalData() = default;
	FractalData(qreal a, qreal b, qreal c, quint8 n, FractalType type);

	[[nodiscard]] QJsonObject serialize() const;
	void readFrom(QJsonDocument &in);
};