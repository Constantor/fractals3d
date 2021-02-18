#pragma once

#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QScrollBar>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	Ui::MainWindow *ui;
	static int getValFromBox(QDoubleSpinBox *box, QScrollBar *bar);
    static double getValFromBar(QDoubleSpinBox *box, QScrollBar *bar);
};
