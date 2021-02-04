#include <QPainter>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fractal2d.h"

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	initial_draw();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::initial_draw() {
	pixmap = QPixmap(this->width(), this->height());
	QPainter painter(&pixmap);

	Fractal2D fractal = Fractal2D({this->width(), this->height()});
	QVector<QVector<QColor>> color_field = fractal.get_color_field();
	for(int i = 0; i < color_field.size(); i++)
		for(int j = 0; j < color_field[i].size(); j++) {
			painter.setPen(color_field[i][j]);
			painter.drawPoint(j, i);
		}

	scene.addPixmap(pixmap);
	view.setScene(&scene);
	this->setCentralWidget(&view);
	view.show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	initial_draw();
}
