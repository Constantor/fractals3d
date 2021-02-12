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

void MainWindow::initialDraw() {
	pixmap = QPixmap(this->width(), this->height());

	QPainter painter(&pixmap);

	qreal minY = -1.0;
	qreal maxY = 1.0;
	qreal maxX = static_cast<qreal>(this->width()) / this->height();
	qreal minX = -maxX;

	fractal = Fractal2D(Complex2D(0.36, 0.36), 2, minX, maxX, minY, maxY);

	QVector<FractalPoint> colorField = fractal.getColorField();
	for(auto &point : colorField) {
		painter.setPen(point.getColor());
		painter.drawPoint(fractal.transformX(point.getX(), this->width()),
						  fractal.transformY(point.getY(), this->height()));
	}
	scene.addPixmap(pixmap);
	view.setScene(&scene);
	this->setCentralWidget(&view);
	view.show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	initialDraw();
}
