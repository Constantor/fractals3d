#include <QPainter>
#include <QSize>
#include "mainwindow.h"
#include "fractal3d.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    initialDraw();
}

MainWindow::~MainWindow() {
    delete ui;
}

qreal MainWindow::calculate(int const &size) {
    return 1.0 / size;
}

void MainWindow::initialDraw() {
    if(resolutionWidth == -1)
        resolutionWidth = width();
    if(resolutionHeight == -1)
        resolutionHeight = height();

    scene.pixmap = QPixmap(resolutionWidth, resolutionHeight);

    QPainter painter(&scene.pixmap);

    qreal minZ = 0.5;
    qreal maxZ = 0.75;
    qreal minY = -0.5;
    qreal maxY = 0.5;
    qreal maxX = 0.5;
    qreal minX = -0.5;

    scene.fractal = Fractal3D(Complex3D(0.6, 0.1, 0.6), 7, 2, 100, calculate(resolutionWidth), calculate(resolutionHeight),
                              minX, maxX, minY, maxY, minZ, maxZ);

    QVector<FractalPoint> colorField = scene.fractal.getColorField();
    for(auto &point : colorField) {
        painter.setPen(point.getColor());
        painter.drawPoint(scene.fractal.transformX(point.getX(), resolutionWidth),
                          scene.fractal.transformY(point.getY(), resolutionHeight));
    }
    painter.end();
    scene.addPixmap(scene.pixmap);
    view.setScene(&scene);
    this->setCentralWidget(&view);
    view.show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    //initialDraw();
    QPixmap scaled = scene.pixmap.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //scene.pixmap = scaled;
    QPainter painter(&scaled);
    painter.drawPixmap(0, 0, scaled);
    painter.end();
    scene.addPixmap(scaled);
    // scene.pixmap = scaled;
}