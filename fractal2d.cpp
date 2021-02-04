#include "fractal2d.h"

Fractal2D::Fractal2D(QVector<int> const &new_size) {
	this->size = new_size;
}

QVector<int> const &Fractal2D::get_size() {
	return this->size;
}

void Fractal2D::set_size(QVector<int> const &new_size) {
	this->size = new_size;
}

QVector<QVector<QColor>> Fractal2D::get_color_field() const {
	QVector<QVector<QColor>> result;
}
