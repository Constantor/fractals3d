#include <QPainter>
#include <QDebug>
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

	qreal minY = -1.0;
	qreal maxY = 1.0;
	qreal maxX = static_cast<qreal>(this->width()) / this->height();
	qreal minX = -maxX;
	Fractal2D fractal = Fractal2D(minX, maxX, minY, maxY);
	QVector<FractalPoint> colorField = fractal.getColorField();
	for(int i = 0; i < colorField.size(); i++) {
		painter.setPen(colorField[i].getColor());
		painter.drawPoint(fractal.transformX(colorField[i].getX(), this->width()),
						  fractal.transformY(colorField[i].getY(), this->height()));
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
