#pragma once

#include <QDataStream>
#include <QJsonObject>

struct FractalData {
	qreal a{}, b{}, c{};
	quint8 n{};

	FractalData() = default;
	FractalData(qreal a, qreal b, qreal c, quint8 n);

    [[nodiscard]] QJsonObject serialize() const;
    QDataStream&  readFrom(QDataStream &in);
};