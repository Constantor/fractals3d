#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
	class Modeler;
}
QT_END_NAMESPACE

class Modeler : public QMainWindow {
Q_OBJECT

public:
	explicit Modeler(QWidget *parent = nullptr);

	~Modeler();

private slots:

	void constructFractal();

	//void about();

	void exit();

private:
	Ui::Modeler *ui;
};
