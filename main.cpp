#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication fractals3DApp(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();
	return fractals3DApp.exec();
}
