#pragma once

#include "GeometryEngine.hpp"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GeometryEngine;

class FractalWidget : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT

public:
	using QOpenGLWidget::QOpenGLWidget;

	~FractalWidget();

protected:
	void mousePressEvent(QMouseEvent *e) override;

	void mouseReleaseEvent(QMouseEvent *e) override;

	void timerEvent(QTimerEvent *e) override;

	void initializeGL() override;

	void resizeGL(int w, int h) override;

	void paintGL() override;

	void initShaders();

	void initTextures();

private:
	const constexpr static qreal EPS = 1e-6;

	QBasicTimer timer;
	QOpenGLShaderProgram program;
	GeometryEngine *geometries = nullptr;

	QOpenGLTexture *texture = nullptr;

	QMatrix4x4 projection;

	QVector2D mousePressPosition;
	QVector3D rotationAxis;
	qreal angularSpeed = 0;
	qreal rotationDelta = 0;
	QQuaternion rotation;
};
