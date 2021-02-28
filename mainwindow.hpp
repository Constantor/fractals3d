#pragma once

#include "fractalwidget.hpp"
#include "fractaldata.hpp"
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QScrollBar>
#include <ui_mainwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(QOpenGLWidget)

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	Ui::MainWindow *ui;
	FractalData data;

	void connectBoxBar();
	void makeMenu();

	void readAndDraw();
	void saveToFile();
	void loadFromFile();
	void setValues();
};
