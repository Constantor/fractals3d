#pragma once

#include "Complex3D.hpp"
#include "FractalPoint.hpp"

#include <QColor>
#include <QPoint>
#include <QVector>

class Fractal3D {
private:
    Complex3D c;
    int n{};
    qreal r_conv{};
    int max_iter{};
    qreal stepx{}, stepy{};

public:
    qreal minX{}, maxX{};
    qreal minY{}, maxY{};
    qreal minZ{}, maxZ{};

private:
    QVector<FractalPoint> colorField;

public:
    Fractal3D() = default;

    Fractal3D(const Complex3D &c, int n, qreal r_conv, int max_iter, qreal stepx,
              qreal stepy, qreal minX, qreal maxX, qreal minY, qreal maxY, qreal minZ, qreal maxZ);

    [[nodiscard]] int transformX(qreal x, int width) const;

    [[nodiscard]] int transformY(qreal y, int height) const;

    [[nodiscard]] QVector<FractalPoint> getColorField() const;

    [[nodiscard]] QVector<FractalPoint> const &getColorFieldR() const;

    void updateColorField();
};
