#pragma once

#include "GeometryEngine.hpp"
#include "fractaldata.hpp"

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

class GeometryEngine;

class FractalWidget : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
	using QOpenGLWidget::QOpenGLWidget;

	~FractalWidget();

	void setFractalData(FractalData *fractalData);

protected:
	void mousePressEvent(QMouseEvent *e) override;

	void mouseMoveEvent(QMouseEvent *e) override;

	void mouseReleaseEvent(QMouseEvent *e) override;

	void timerEvent(QTimerEvent *e) override;

	void initializeGL() override;

	void resizeGL(int w, int h) override;

	void paintGL() override;

	void initShaders();

	void initTextures();

private:
	// const constexpr static qreal EPS = 1e-6;

	bool mousePressed = false;
	// QBasicTimer timer;
	QOpenGLShaderProgram program;
	FractalData *fd;
	GeometryEngine *geometries = nullptr;

	QOpenGLTexture *texture = nullptr;

	QMatrix4x4 projection;

	QVector2D mousePressPosition;
	QVector3D rotationAxis;
	qreal rotationDelta = 0;
	QQuaternion rotation;
};