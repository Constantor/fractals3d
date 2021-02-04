#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	QApplication fractals3DApp(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();
	return fractals3DApp.exec();
}
