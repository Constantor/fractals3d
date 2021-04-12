#include "mainwindow.hpp"
#include "ui_mainwindow.h"

namespace {
	int getValFromBox(QDoubleSpinBox *box, QSlider *bar) {
		return (bar->maximum() - bar->minimum()) * (box->value() - box->minimum()) / (box->maximum() - box->minimum());
	}

	double getValFromBar(QDoubleSpinBox *box, QSlider *bar) {
		return box->minimum() + (box->maximum() - box->minimum()) * (bar->value() - bar->minimum()) / (bar->maximum() - bar->minimum());
	}

	QString timeFormat(qint64 time) {
		return QString::number(time / 1000) + " s " + QString::number(time % 1000) + " ms";
	}
}// namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
    ui->recordWidget->close();
	connectBoxBar();
	connect(ui->drawButton, &QPushButton::clicked, [&]() { readAndDraw(); });
	connect(ui->recordButton, &QPushButton::clicked, [&]() { clickAction(); });
	ui->fractalWidget->setFractalData(&data);
	readAndDraw();
	makeMenu();
}

MainWindow::~MainWindow() {
	delete ui;
	delete timer;
	delete elapsedTimer;
	delete temporaryDir;
}

void MainWindow::makeMenu() {
	auto fileMenu = menuBar()->addMenu("File");

	fileMenu->addAction("Load", [&]() { loadFromFile(); });
	fileMenu->addAction("Save", [&]() { saveToFile(); });
	fileMenu->addAction("Save as Image", [&]() { saveToImage(); });
	fileMenu->addAction("Save as Video", [&]() { recordVideo(); });
	fileMenu->addAction("Exit", [&]() { QApplication::quit(); });

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
	ui->fractalWidget->repaint();
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
			QMessageBox::information(this, tr("Unable to save the file"),
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

void MainWindow::saveToImage() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Fractal Image"), "", tr("Image Files(*.png *.jpg *.jpeg *.bmp);;All Files (*)"));
	if(!fileName.isEmpty()) {
		QFileInfo fileInfo(fileName);
		if(fileInfo.exists() && !fileInfo.isWritable()) {
			QMessageBox::information(this, tr("Unable to open file"),
									 "Can't save to " + fileInfo.fileName());
			return;
		}
		ui->fractalWidget->grabFramebuffer().save(fileName);
	}
}

void MainWindow::setValues() {
	ui->firstCoordBox->setValue(data.a);
	ui->secondCoordBox->setValue(data.b);
	ui->thirdCoordBox->setValue(data.c);
	ui->powerBox->setValue(data.n);
}

void MainWindow::recordVideo() {
    ui->recordLabel->setText("Recording is not started.");
    ui->recordProgressBar->setValue(0);
	ui->recordWidget->show();
}

void MainWindow::startRecord() {
	isOnRecord = true;
	temporaryDir = new QTemporaryDir;
	timer = new QTimer(this);
	elapsedTimer = new QElapsedTimer;
	connect(timer, &QTimer::timeout, [&]() { shot(); });
	elapsedTimer->start();
	timer->start(INTERVAL);
}

void MainWindow::shot() {
	qint64 time = elapsedTimer->elapsed();
	//TODO: take a screenshot and write it to QFile(temporaryDir->filePath(QString::number(time)+".png"));
	ui->recordLabel->setText("Recording: " + timeFormat(time));
	ui->recordProgressBar->setValue(100 * time / LIMIT);
	if(time >= LIMIT) {
		stopRecord();
	}
}

void MainWindow::stopRecord() {
	timer->stop();
	isOnRecord = false;
	saveVideo();
	ui->recordWidget->close();
}

void MainWindow::saveVideo() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Fractal Video"), "", tr("Video Files(*.mp4 *.avi);;All Files (*)"));
	if(!fileName.isEmpty()) {
		QFileInfo fileInfo(fileName);
		if(fileInfo.exists() && !fileInfo.isWritable()) {
			QMessageBox::information(this, tr("Unable to open file"),
									 "Can't save to " + fileInfo.fileName());
			return;
		}
		//TODO: start ffmpeg passing fileName and temporaryDir->path() to exec(), it will perform writing
	}
}

void MainWindow::clickAction() {
	if(isOnRecord) {
		stopRecord();
	} else {
		startRecord();
	}
}