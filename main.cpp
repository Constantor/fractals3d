#include <QApplication>

#include "modeler.h"

int main(int argc, char *argv[]) {
	QApplication fractals3DApp(argc, argv);
	Modeler modeler;
	modeler.show();

	return fractals3DApp.exec();
}
