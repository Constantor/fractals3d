#pragma once

#include <QDataStream>

struct FractalData {
	//TODO USE Qt builtin types
	double a{}, b{}, c{};
	unsigned int n{};

	FractalData() = default;
	FractalData(double a, double b, double c, unsigned int n);

    QDataStream &operator<<(QDataStream &out) const;
};