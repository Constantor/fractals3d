#include <QApplication>
#include <QDebug>
#include <QtWidgets/QLabel>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QLabel label("Hello, world!");
	label.show();
	return app.exec();
}
