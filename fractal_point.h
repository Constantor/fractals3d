#pragma once

#include <QPoint>
#include <QColor>

class FractalPoint {
private:
	qreal x, y;
	QColor color;

public:
	FractalPoint(qreal new_x, qreal new_y, QColor new_color);

	QColor getColor() const;

	void setColor(QColor new_color);

	qreal getX() const;

	qreal getY() const;

	void setX(qreal new_x);

	void setY(qreal new_y);
};
