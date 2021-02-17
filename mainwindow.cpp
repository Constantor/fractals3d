#include <QPainter>
#include "mainwindow.h"
#include "custom_scene.h"
#include "fractal2d.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	initialDraw();
}

MainWindow::~MainWindow() {
	delete ui;
}

qreal MainWindow::calculate(int const &size) {
	return 1.0 / size;
}

void MainWindow::drawWithNewObjects() {
	scene.pixmap = QPixmap(width(), height());
	scene.drawField();
	scene.addPixmap(scene.pixmap);
	// scene.drawFieldOnNew(); // does not work, chto konechno pizdets
}

void MainWindow::drawWithNewAll() {
	qreal minY = -1.0;
	qreal maxY = 1.0;
	qreal maxX = static_cast<qreal>(width()) / height();
	qreal minX = -maxX;

	scene.fractal = Fractal2D(Complex2D(0.36, 0.36), 2, 2, 100,
							  calculate(width()), calculate(height()), minX, maxX, minY, maxY);
	drawWithNewObjects();
}

void MainWindow::initialDraw() {
	/*qreal minY = -1.0;
	qreal maxY = 1.0;
	qreal maxX = static_cast<qreal>(width()) / height();
	qreal minX = -maxX;

	scene.fractal = Fractal2D(Complex2D(0.36, 0.36), 2, 2, 100,
							  calculate(width()), calculate(height()), minX, maxX, minY, maxY);
	drawWithNewObjects();*/
	drawWithNewAll();

	view.setScene(&scene);
	setCentralWidget(&view);
	view.show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	qDebug() << "Resize drawing";
	//resizeTimer.start();
	//drawWithNewAll();
	qDebug() << "Ended resize";

	/*qreal minY = -1.0;
	qreal maxY = 1.0;
	qreal maxX = static_cast<qreal>(width()) / height();
	qreal minX = -maxX;

	scene.fractal = Fractal2D(Complex2D(0.36, 0.36), 2, 2, 100,
							  calculate(width()), calculate(height()), minX, maxX, minY, maxY);
	scene.drawFieldOnNew();*/
	drawWithNewAll();

	/*scene.fractal.minY = -1.0;
	scene.fractal.maxY = 1.0;
	scene.fractal.maxX = static_cast<qreal>(width()) / height();
	scene.fractal.minX = -scene.fractal.maxX;
	scene.fractal.updateColorField();
	drawWithNewObjects();*/
	//drawWithNewAll();
}
