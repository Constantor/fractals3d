#pragma once

#include "Recorder.hpp"
#include "fractaldata.hpp"
#include "fractalwidget.hpp"
#include <QDoubleSpinBox>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMainWindow>
#include <QMessageBox>
#include <QProgressBar>
#include <QScrollBar>
#include <QSlider>
#include <QTemporaryDir>
#include <QTimer>

#include <ui_mainwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(QOpenGLWidget)

class MainWindow : public QMainWindow {
	Q_OBJECT

	static const qint64 INTERVAL = 10;
	static const qint64 LIMIT = 10000;

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	Ui::MainWindow *ui;
	FractalData data;
	QElapsedTimer *elapsedTimer{};
	QTimer *timer{};
    qint64 time{}, frames{};
    QTemporaryDir *temporaryDir{};
	Recorder *recorder{};
	bool isOnRecord = false;
	QColor chosenColor{};

	void connectBoxBar();
	void makeMenu();

	void chooseColor(QColor const &color);
	void askColor();

	void readAndDraw();
	void saveToFile();
	void saveToImage();
	void loadFromFile();
	void setValues();
	void recordVideo();

	void startRecord();
	void shot();
	void stopRecord();
	void saveVideo();
	void recordClickAction();
};
