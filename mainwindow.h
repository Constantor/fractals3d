#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	Ui::MainWindow *ui;

	QGraphicsView view{};
	QGraphicsScene scene;
	QPixmap pixmap;

	void initialDraw();

	void resizeEvent(QResizeEvent *event) override;
};

