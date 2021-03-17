#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSlider>

namespace {
	int getValFromBox(QDoubleSpinBox *box, QSlider *bar) {
		return (bar->maximum() - bar->minimum()) * (box->value() - box->minimum()) / (box->maximum() - box->minimum());
	}
	double getValFromBar(QDoubleSpinBox *box, QSlider *bar) {
		return box->minimum() + (box->maximum() - box->minimum()) * (bar->value() - bar->minimum()) / (bar->maximum() - bar->minimum());
	}
}// namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	connectBoxBar();
	connect(ui->drawButton, &QPushButton::clicked, [&]() { readAndDraw(); });
	makeMenu();
}

MainWindow::~MainWindow() {
	delete ui;
}
void MainWindow::makeMenu() {
	auto fileMenu = menuBar()->addMenu("File");

	fileMenu->addAction("Load", [&]() { loadFromFile(); });
	fileMenu->addAction("Save", [&]() { saveToFile(); });
	fileMenu->addAction("Save as PNG", [&]() { saveToImage(); });
	fileMenu->addAction("Save as MPEG", [&]() {});

	menuBar()->addAction("Exit", [&]() { QApplication::quit(); });
	menuBar()->addMenu("About");
}

void MainWindow::connectBoxBar() {
	connect(ui->firstCoordBox, &QDoubleSpinBox::valueChanged, ui->firstCoordBar, [&]() { ui->firstCoordBar->setValue(getValFromBox(ui->firstCoordBox, ui->firstCoordBar)); });
	connect(ui->firstCoordBar, &QSlider::valueChanged, ui->firstCoordBox, [&]() { ui->firstCoordBox->setValue(getValFromBar(ui->firstCoordBox, ui->firstCoordBar)); });
	connect(ui->secondCoordBox, &QDoubleSpinBox::valueChanged, ui->secondCoordBar, [&]() { ui->secondCoordBar->setValue(getValFromBox(ui->secondCoordBox, ui->secondCoordBar)); });
	connect(ui->secondCoordBar, &QSlider::valueChanged, ui->secondCoordBox, [&]() { ui->secondCoordBox->setValue(getValFromBar(ui->secondCoordBox, ui->secondCoordBar)); });
	connect(ui->thirdCoordBox, &QDoubleSpinBox::valueChanged, ui->thirdCoordBar, [&]() { ui->thirdCoordBar->setValue(getValFromBox(ui->thirdCoordBox, ui->thirdCoordBar)); });
	connect(ui->thirdCoordBar, &QSlider::valueChanged, ui->thirdCoordBox, [&]() { ui->thirdCoordBox->setValue(getValFromBar(ui->thirdCoordBox, ui->thirdCoordBar)); });
	connect(ui->powerBox, &QSpinBox::valueChanged, ui->powerBar, [&]() { ui->powerBar->setValue(ui->powerBox->value() / 2); });
	connect(ui->powerBar, &QSlider::valueChanged, ui->powerBox, [&]() { ui->powerBox->setValue(2 * ui->powerBar->value()); });
}

void MainWindow::readAndDraw() {
	data = FractalData(ui->firstCoordBox->value(), ui->secondCoordBox->value(), ui->thirdCoordBox->value(), ui->powerBox->value());
}

void MainWindow::loadFromFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Fractal "), "", tr("3D Fractal Data (*.f3d);;All Files (*)"));
	if(fileName.isEmpty())
		return;
	else {
		QFile file(fileName);

		if(!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
									 file.errorString());
			return;
		}

		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		data.readFrom(doc);
		file.close();

		setValues();
	}
}

void MainWindow::saveToFile() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Fractal Input"), "", tr("3D Fractal Data (*.f3d);;All Files (*)"));
	if(!fileName.isEmpty()) {
		QFile file(fileName);

		if(!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
									 file.errorString());
			return;
		}

		QJsonObject output;
		output.insert("Fractal", data.serialize());

		QJsonDocument doc(output);
		file.write(doc.toJson());
		file.close();
	}
}


void MainWindow::setValues() {
	ui->firstCoordBox->setValue(data.a);
	ui->secondCoordBox->setValue(data.b);
	ui->thirdCoordBox->setValue(data.c);
	ui->powerBox->setValue(data.n);
}
void MainWindow::saveToImage() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Fractal Image"), "", tr("Portable Network Graphics (*.png);;All Files (*)"));
	if(!fileName.isEmpty()) {
		QFile file(fileName);

		if(!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
									 file.errorString());
			return;
		}

		file.close();
	}
}
