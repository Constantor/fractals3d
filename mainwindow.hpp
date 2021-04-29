#pragma once

#include "fractaldata.hpp"
#include "fractalwidget.hpp"
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSlider>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QScrollBar>
#include <QTemporaryDir>
#include <QTimer>
#include <QProgressBar>

#include <ui_mainwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(QOpenGLWidget)

class MainWindow : public QMainWindow {
	Q_OBJECT

	static const qint64 INTERVAL = 40;
	static const qint64 LIMIT = 10000;

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	Ui::MainWindow *ui;
	FractalData data;
	QElapsedTimer *elapsedTimer;
	QTimer *timer{};
	QTemporaryDir *temporaryDir{};
	bool isOnRecord = false;

	void connectBoxBar();
	void makeMenu();

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