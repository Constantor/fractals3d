#pragma once

#include "fractal2d.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	Ui::MainWindow *ui;

	QGraphicsView view{};
	QGraphicsScene scene;
	QPixmap pixmap;

	Fractal2D fractal;

	void initialDraw();

	void resizeEvent(QResizeEvent *event) override;
};
