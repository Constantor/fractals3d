#include <QPainter>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initial_draw();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initial_draw() {
    pixmap = QPixmap(this->width(), this->height());
    QPainter painter(&pixmap);
    //TODO: add class Fractal 2D, get points from it, and display as painter.drawPoint()
    painter.fillRect(0, 0, this->width(), this->height(), Qt::red);
    scene.addPixmap(pixmap);
    view = new QGraphicsView(&scene);
    this->setCentralWidget(view);
    view->show();
}
