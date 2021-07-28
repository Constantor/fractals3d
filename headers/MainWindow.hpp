#pragma once

#include "FractalData.hpp"
#include "FractalWidget.hpp"
#include "Recorder.hpp"

#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QKeyEvent>
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
	static const qint64 LIMIT = 180000;

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	Ui::MainWindow *ui;
	FractalData data{};
	QElapsedTimer *elapsedTimer{};
	QTimer *timer{};
	qint64 time{}, frames{};
	QTemporaryDir *temporaryDir{};
	Recorder *recorder{};
	QSize prevSize{};
	bool isOnRecord = false;
	bool isFullScreen = false;
	bool isSetting = true;

	void connectBoxBar();

	void makeMenu();

	void chooseColor(QColor const &color, ColorType type);

	void askColor(ColorType type);

	void updateButtons();

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

	void generateRandom();

	void hideAndShow();

	void hideBorders();

protected:
	void keyPressEvent(QKeyEvent *event) override;
};
