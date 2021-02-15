#include <QPainter>
#include "mainwindow.h"
#include "custom_scene.h"
#include "fractal2d.h"
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

void MainWindow::updateDraw() {
	return;
}

void MainWindow::initialDraw() {
	qreal minY = -1.0;
	qreal maxY = 1.0;
	qreal maxX = static_cast<qreal>(width()) / height();
	qreal minX = -maxX;
	fractal = Fractal2D(Complex2D(0.36, 0.36), 2, 2, 100,
                     calculate(width()), calculate(height()), minX, maxX, minY, maxY);

	pixmap = QPixmap(width(), height());
	scene.fractal = &fractal;
	scene.pixmap = &pixmap;

	/*QPainter painter(&pixmap);
	QVector<FractalPoint> const &colorField = fractal.getColorFieldR();
	for(auto &point : colorField) {
		painter.setPen(point.getColor());
		painter.drawPoint(fractal.transformX(point.getX(), width()),
						  fractal.transformY(point.getY(), height()));
	}*/
	scene.drawField();
	scene.addPixmap(pixmap);
	view.setScene(&scene);
	setCentralWidget(&view);
	view.show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	initialDraw();
}
