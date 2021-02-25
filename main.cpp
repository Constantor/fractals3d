#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QScreen>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DExtras/qt3dwindow.h>
#include "mainwidget.h"

int main(int argc, char *argv[]) {
	QApplication fractals3DApp(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);

	fractals3DApp.setApplicationName("Fractals 3D");
	fractals3DApp.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
	MainWidget widget;
	widget.show();
#else
	QLabel note("OpenGL Support required");
    note.show();
#endif
	return fractals3DApp.exec();
}
