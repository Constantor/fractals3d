#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include "fractal2d.h"
#include "custom_scene.h"

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
	//QGraphicsScene scene;
	CustomScene scene;

	QPixmap pixmap;

	Fractal2D fractal;

	void initialDraw();

	static qreal calculate(int const &size);

	void resizeEvent(QResizeEvent *event) override;
};
