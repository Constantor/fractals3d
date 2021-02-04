#include "fractal2d.h"

Fractal2D::Fractal2D(QVector<int> const &new_size) {
	this->size = new_size;
}

QVector<int> const &Fractal2D::get_size() const {
	return this->size;
}

int Fractal2D::get_width() const {
	return this->size[0];
}

int Fractal2D::get_height() const {
	return this->size[1];
}

void Fractal2D::set_size(QVector<int> const &new_size) {
	this->size = new_size;
}

QVector<QVector<QColor>> Fractal2D::get_color_field() const {
	QVector<QVector<QColor>> result(size[1], QVector<QColor>(size[0]));

	// temporary code
	for(int i = 0; i < size[1]; i++)
		for(int j = 0; j < size[0]; j++)
			result[i][j] = QColor(rand() % 256, rand() % 256, rand() % 256);
	// end temporary code

	// TODO: generate 2D fractal
	return result;
}
