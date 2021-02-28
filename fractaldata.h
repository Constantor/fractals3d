#pragma once

#include <QDataStream>

struct FractalData {
	qreal a{}, b{}, c{};
	quint8 n{};

	FractalData() = default;
	FractalData(qreal a, qreal b, qreal c, quint8 n);

	QDataStream &operator<<(QDataStream &out) const;
};