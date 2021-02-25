#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "fractalwidget.hpp"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), fractalWidget(new FractalWidget(this)) {
	ui->setupUi(this);
	
	fractalWidget->resize(width(), height());
}

MainWindow::~MainWindow() {
	delete ui;
}
