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

void MainWindow::initialDraw() {
	scene.pixmap = QPixmap(this->width(), this->height());

	QPainter painter(&scene.pixmap);

	qreal minY = -1.0;
	qreal maxY = 1.0;
	qreal maxX = static_cast<qreal>(this->width()) / this->height();
	qreal minX = -maxX;

	scene.fractal = Fractal2D(Complex2D(0.36, 0.36), 2, 2, 100, calculate(width()), calculate(height()), minX, maxX, minY, maxY);

	QVector<FractalPoint> colorField = scene.fractal.getColorField();
	for(auto &point : colorField) {
		painter.setPen(point.getColor());
		painter.drawPoint(scene.fractal.transformX(point.getX(), this->width()),
						  scene.fractal.transformY(point.getY(), this->height()));
	}
	scene.addPixmap(scene.pixmap);
	view.setScene(&scene);
	this->setCentralWidget(&view);
	view.show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	initialDraw();
}
