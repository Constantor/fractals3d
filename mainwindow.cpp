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

void MainWindow::initialDraw() {
	pixmap = QPixmap(width(), height());
	qreal minY = -1.0;
	qreal maxY = 1.0;
	qreal maxX = static_cast<qreal>(width()) / height();
	qreal minX = -maxX;

	fractal = Fractal2D(Complex2D(0.36, 0.36), 2, 2, 100,
                     calculate(width()), calculate(height()), minX, maxX, minY, maxY);

	scene.fractal = &fractal;
	scene.pixmap = &pixmap;
	scene.addPixmap(pixmap);
	//scene.draw();

	QPainter painter = QPainter(&pixmap);
	//painter.setPen(QColor(0, 0, 0));
	//painter.drawRect(0, 0, this->width(), this->height());
	QVector<FractalPoint> colorField = fractal.getColorFieldR();
	for(auto const &point : colorField) {
		painter.setPen(point.getColor());
		painter.drawPoint(fractal.transformX(point.getX(), width()),
						  fractal.transformY(point.getY(), height()));
	}

	view.setScene(&scene);
	setCentralWidget(&view);
	view.show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	fractal.updateColorField();
	scene.draw();
}
