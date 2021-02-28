#include "mainwindow.hpp"
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DRender/qsceneloader.h>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>

int main(int argc, char *argv[]) {
	QApplication fractals3DApp(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);

	fractals3DApp.setApplicationName("Fractals 3D");
	fractals3DApp.setApplicationVersion("0.1");
	fractals3DApp.setOrganizationName("HSE University");
	fractals3DApp.setOrganizationDomain("hse.ru");

#ifndef QT_NO_OPENGL
	//FractalWidget widget;
	//widget.show();
	MainWindow mainWindow{};
	mainWindow.show();
#else
	QLabel note("OpenGL Support required");
	note.show();
#endif

	return fractals3DApp.exec();
}
