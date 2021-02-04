#pragma once

#include <QColor>
#include <QVector>

class Fractal2D {
private:
	QVector<int> size = {0, 0}; // width, height
	// QVector<int> camera_position;

public:
	Fractal2D(QVector<int> const &new_size);

	QVector<int> const &get_size() const;

	int get_width() const;

	int get_height() const;

	void set_size(QVector<int> const &new_size);

	[[nodiscard]] QVector<QVector<QColor>> get_color_field() const;
};
