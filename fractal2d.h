#pragma once

#include <QColor>
#include <QVector>
#include <QPoint>
#include "fractal_point.h"

class Fractal2D {
private:
	qreal minX, maxX;
	qreal minY, maxY;
	// Later: camera_position

public:
	Fractal2D(qreal newMinX, qreal newMaxX, qreal newMinY, qreal newMaxY);

	qreal getMinX() const;

	qreal getMaxX() const;

	qreal getMinY() const;

	qreal getMaxY() const;

	void setMinX(qreal value);

	void setMaxX(qreal value);

	void setMinY(qreal value);

	void setMaxY(qreal value);

	[[nodiscard]] QVector<FractalPoint> getColorField() const;
};
