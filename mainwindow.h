#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QTimer>
#include "scene.h"
#include "fractal3d.h"

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

    QGraphicsView view{};

    Scene scene{};

    int resolutionWidth = -1;
    int resolutionHeight = -1;

    void initialDraw();

    static qreal calculate(int const &size);

    void resizeEvent(QResizeEvent *event) override;
};
