#include "MainWindow.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication fractals3DApp(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);

	fractals3DApp.setApplicationName("Fractals 3D");
	fractals3DApp.setApplicationVersion("v1.4");
	fractals3DApp.setOrganizationName("HSE University Saint Petersburg");
	fractals3DApp.setOrganizationDomain("spb.hse.ru");

#ifndef QT_NO_OPENGL
	MainWindow mainWindow{};
	mainWindow.show();
#else
	QLabel note("OpenGL Support required");
	note.show();
#endif

	return fractals3DApp.exec();
}
