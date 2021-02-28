#pragma once

#include <QDataStream>

struct FractalData {
	qreal a{}, b{}, c{};
	quint8 n{};

	FractalData() = default;
	FractalData(qreal a, qreal b, qreal c, quint8 n);

    QDataStream& printTo(QDataStream &out) const;
    QDataStream&  readFrom(QDataStream &in);
};