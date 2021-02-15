#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	//connect(ui->firstCoordBox, SIGNAL(valueChanged(double)), ui->firstCoordBar, SLOT(setValue(int)));
    //connect(ui->firstCoordBar, SIGNAL(valueChanged(int)), ui->firstCoordBox, SLOT(setValue(double)));
}

MainWindow::~MainWindow() {
	delete ui;
}
