#include <QCoreApplication>
#include <QMessageBox>

#include "modeler.h"
#include "ui_modeler.h"

Modeler::Modeler(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::Modeler) {
	ui->setupUi(this);
	this->setCentralWidget(ui->centralWidget);

	// connect(ui->actionExit, &QAction::triggered, this, &Modeler::exit);
	// connect(ui->actionAbout, &QAction::triggered, this, &Modeler::about);
	// connect(ui->actionConstruct, &QAction::triggered, this, &Modeler::contructFractal);
}

Modeler::~Modeler() {
	delete ui;
}

void Modeler::constructFractal() {

}

void Modeler::exit() {
	QCoreApplication::quit();
}

/*void Modeler::about() {
	QMessageBox::about(this, tr("About Fractals 3D"),
					   tr("<b>Fractals 3D</b> is an app for building and exploring three-dimensional fractals. Written in modern C ++ by three authors: Stepan Konstantinov, Daria Ledneva, Sergey Zhuravlev."));
}*/
