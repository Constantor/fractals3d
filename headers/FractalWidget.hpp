#pragma once

#include "GeometryEngine.hpp"
#include "FractalData.hpp"

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
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

	void mouseReleaseEvent(QMouseEvent *e) override;

	void initializeGL() override;

	void resizeGL(int w, int h) override;

	void paintGL() override;

	void initShaders();

private:
	bool mousePressed = false;
	QOpenGLShaderProgram program;
	FractalData *fractalData;
	GeometryEngine *geometries = nullptr;

	QMatrix4x4 projection;
	QVector2D mousePressPosition;
	QVector3D rotationAxis;
	qreal rotationDelta = 0;
	QQuaternion rotation;
};
