#pragma once

#include <QPoint>
#include <QColor>

class FractalPoint {
private:
	qreal x, y;
	QColor color;

public:
	FractalPoint(qreal newX, qreal newY, QColor newColor);

	QColor getColor() const;

	void setColor(QColor newColor);

	qreal getX() const;

	qreal getY() const;

	void setX(qreal newX);

	void setY(qreal newY);
};
