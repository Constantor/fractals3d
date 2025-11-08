#include "MainWindow.hpp"
#include <QtGui/QSurfaceFormat>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	// QOpenGLWidget instances pick up the default format that is active when
	// QApplication is constructed. On Wayland Qt prefers an OpenGL ES context by
	// default, which cannot compile our GLSL 1.40 shaders. Force a desktop OpenGL
	// context and configure it *before* QApplication is created so the widget gets
	// a compatible context everywhere.
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 1);
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);

	QApplication fractals3DApp(argc, argv);

	fractals3DApp.setApplicationName("Fractals 3D");
	fractals3DApp.setApplicationVersion("v1.5");
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
