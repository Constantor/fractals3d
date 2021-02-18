#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDoubleSpinBox>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	connect(ui->firstCoordBox, &QDoubleSpinBox::valueChanged, ui->firstCoordBar, [&]() { ui->firstCoordBar->setValue(getValFromBox(ui->firstCoordBox, ui->firstCoordBar)); });
	connect(ui->firstCoordBar, &QScrollBar::valueChanged, ui->firstCoordBox, [&]() { ui->firstCoordBox->setValue(getValFromBar(ui->firstCoordBox, ui->firstCoordBar)); });
	connect(ui->secondCoordBox, &QDoubleSpinBox::valueChanged, ui->secondCoordBar, [&]() { ui->secondCoordBar->setValue(getValFromBox(ui->secondCoordBox, ui->secondCoordBar)); });
	connect(ui->secondCoordBar, &QScrollBar::valueChanged, ui->secondCoordBox, [&]() { ui->secondCoordBox->setValue(getValFromBar(ui->secondCoordBox, ui->secondCoordBar)); });
	connect(ui->thirdCoordBox, &QDoubleSpinBox::valueChanged, ui->thirdCoordBar, [&]() { ui->thirdCoordBar->setValue(getValFromBox(ui->thirdCoordBox, ui->thirdCoordBar)); });
	connect(ui->thirdCoordBar, &QScrollBar::valueChanged, ui->thirdCoordBox, [&]() { ui->thirdCoordBox->setValue(getValFromBar(ui->thirdCoordBox, ui->thirdCoordBar)); });
	connect(ui->powerBox, &QSpinBox::valueChanged, ui->powerBar, [&]() { ui->powerBar->setValue(ui->powerBox->value() / 2); });
	connect(ui->powerBar, &QScrollBar::valueChanged, ui->powerBox, [&]() { ui->powerBox->setValue(2 * ui->powerBar->value()); });
	connect(ui->drawButton, &QPushButton::clicked, [&]() { qDebug() << ui->firstCoordBox->value() << " " << ui->secondCoordBox->value() << " " << ui->thirdCoordBox->value() << "\n"; });
}

MainWindow::~MainWindow() {
	delete ui;
}

int MainWindow::getValFromBox(QDoubleSpinBox *box, QScrollBar *bar) {
	return (bar->maximum() - bar->minimum()) * (box->value() - box->minimum()) / (box->maximum() - box->minimum());
}
double MainWindow::getValFromBar(QDoubleSpinBox *box, QScrollBar *bar) {
	return box->minimum() + (box->maximum() - box->minimum()) * (bar->value() - bar->minimum()) / (bar->maximum() - bar->minimum());
}
