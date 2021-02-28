#include "fractal3d.h"
#include "complex3d.h"

int Fractal3D::transformX(qreal x, int width) const {
    return width * (x - minX) / (maxX - minX);
}

int Fractal3D::transformY(qreal y, int height) const {
    return height * (y - minY) / (maxY - minY);
}

QVector<FractalPoint> Fractal3D::getColorField() const {
    return colorField;
}

QVector<FractalPoint> const &Fractal3D::getColorFieldR() const {
    return colorField;
}

void Fractal3D::updateColorField() {
    colorField.clear();
    const int num_pointx = 600;
	const int num_pointy = 600;
    const int num_layer = 50;
    std::vector<std::vector<std::vector<int> > > iterations(num_pointx, std::vector<std::vector<int> > (num_pointy, std::vector<int> (num_layer, 0)));
    for (int Z = 0; Z < num_layer; ++Z) {
        for(int X = 0; X < num_pointx; ++X) {
            for(int Y = 0; Y < num_pointy; ++Y) {
                qreal z = (Z * 1.0) * (maxZ - minZ) / (num_layer * 1.0) + minZ;
                qreal y = (Y * 1.0) * (maxY - minY) / (num_pointy * 1.0) + minY;
                qreal x = (X * 1.0) * (maxX - minX) / (num_pointx * 1.0) + minX;
                Complex3D z_(x, y, z);

                while(iterations[X][Y][Z] < max_iter && z_.Complex3D::abs() < r_conv) {
                    z_ ^ n;
                    z_ = z_ + c;
                    ++iterations[X][Y][Z];
                }
            }
        }
    }
    for(int X = 0; X < (num_pointx * 1.0); ++X) {
        for(int Y = 0; Y < (num_pointy * 1.0); ++Y) {
            int iter = iterations[X][Y][0];
            for (int Z = 1; Z < num_layer; ++Z) {
                if (iterations[X][Y][Z] < iter) {
                    //iter /= int((Z + 1) * (1.0 / num_layer) + 1);
					iter = Z;
                    break;
                }
                iter = iterations[X][Y][Z];
            }
            qreal y = Y * (maxY - minY) / (num_pointy * 1.0) + minY;
            qreal x = X * (maxX - minX) / (num_pointx * 1.0) + minX;

            colorField.push_back(FractalPoint(x, y,
                                              QColor((iter * 5) % 256, (iter * 5) % 256, (iter * 5) % 256))); //TODO leave only iter, to set brightness
        }
    }
}

Fractal3D::Fractal3D(const Complex3D &c, int n, qreal r_conv, int max_iter, qreal stepx,
                     qreal stepy, qreal minX, qreal maxX, qreal minY, qreal maxY, qreal minZ, qreal maxZ)
        : c(c), n(n), r_conv(r_conv), max_iter(max_iter), stepx(stepx),
          stepy(stepy), minX(minX), maxX(maxX), minY(minY), maxY(maxY), minZ(minZ), maxZ(maxZ) {
    updateColorField();
}
