#include <QPainter>
#include "mainwindow.h"
#include "ui_mainwindow.h"

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
	//TODO: add class Fractal 2D, get points from it, and display as painter.drawPoint()
	painter.fillRect(this->width() / 3, this->height() / 3, this->width() / 3, this->height() / 3, Qt::red);
	scene.addPixmap(pixmap);
	view.setScene(&scene);
	this->setCentralWidget(&view);
	view.show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	initial_draw();
}
