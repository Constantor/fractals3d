#pragma once

#include "FractalData.hpp"
#include "GeometryEngine.hpp"

#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QTimer>
#include <QVector2D>

class FractalWidget : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
	using QOpenGLWidget::QOpenGLWidget;

	~FractalWidget();

	void setFractalData(FractalData *data);

protected:
	void wheelEvent(QWheelEvent *e) override;

	void mousePressEvent(QMouseEvent *e) override;

	void mouseMoveEvent(QMouseEvent *e) override;

	void mouseReleaseEvent(QMouseEvent *) override;

	void initializeGL() override;

	void resizeGL(int w, int h) override;

	void paintGL() override;

	void initShaders();

	void rotateFractal(QVector2D const &diff);

	void autoRotate();

private:
	bool mousePressed = false;
	QOpenGLShaderProgram program;
	FractalData *fractalData;
	GeometryEngine *geometries = nullptr;
	QElapsedTimer *elapsedTimer = nullptr;
	QTimer *timer = nullptr;

	QMatrix4x4 projection;
	QVector2D mousePressPosition;
	QVector3D pointAxisX = QVector3D(1.0, 0.0, 1.5);
	QVector3D pointAxisY = QVector3D(0.0, 1.0, 1.5);
    qreal autoRotationPos = 0.0;
};
