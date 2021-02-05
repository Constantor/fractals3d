#include <QPainter>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fractal2d.h"

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
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

	Fractal2D fractal = Fractal2D(-1.0, 1.0, -1.0, 1.0);
	QVector<FractalPoint> colorField = fractal.getColorField();
	for(int i = 0; i < colorField.size(); i++) {
		painter.setPen(colorField[i].getColor());
		painter.drawPoint(colorField[i].getX() * this->width(), colorField[i].getY() * this->height());
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
