#include "fractalwidget.hpp"

#include <QMouseEvent>

#include <cmath>

FractalWidget::~FractalWidget() {
	// Make sure the context is current when deleting the texture
	// and the buffers.
	makeCurrent();
	delete geometries;
	doneCurrent();
}

void FractalWidget::mousePressEvent(QMouseEvent *e) {
	// Save mouse press position
	mousePressPosition = QVector2D(e->position());
	mousePressed = true;
}

void FractalWidget::mouseReleaseEvent(QMouseEvent *) {
	mousePressed = false;
}

void FractalWidget::mouseMoveEvent(QMouseEvent *e) {
	if(!mousePressed) {
		return;
	}
	QVector2D diff = QVector2D(e->position()) - mousePressPosition;

	// Rotation axis is perpendicular to the mouse position difference
	// vector
	QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

	rotationDelta = diff.length() / 4;

	// Calculate new rotation axis as weighted sum
	rotationAxis = (n * rotationDelta).normalized();

	if(rotationDelta != 0) {
		rotation = QQuaternion::fromAxisAndAngle(rotationAxis, rotationDelta) * rotation;
		if(!actedInPeriod) {
			update();
			actedInPeriod = true;
		}
		mousePressPosition = QVector2D(e->position());
		rotationDelta = 0;
	}
}

void FractalWidget::timerEvent(QTimerEvent *) {
	actedInPeriod = false;
}

void FractalWidget::initializeGL() {
	this->setMouseTracking(true);

	initializeOpenGLFunctions();

	glClearColor(0, 0, 0, 1);

	initShaders();

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);

	geometries = new GeometryEngine;

	// Use QBasicTimer because its faster than QTimer
	timer.start(frequency, this);
}

void FractalWidget::initShaders() {
	// Compile vertex shader
	if(!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
		close();

	// Compile fragment shader
	if(!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
		close();

	// Link shader pipeline
	if(!program.link())
		close();

	// Bind shader pipeline for use
	if(!program.bind())
		close();
}

void FractalWidget::resizeGL(int w, int h) {
	// Calculate aspect ratio
	qreal aspect = qreal(w) / qreal(h ? h : 1);

	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	const qreal zNear = 4.0, zFar = 8.0, fov = 45.0;

	// Reset projection
	projection.setToIdentity();

	// Set perspective projection
	projection.perspective(fov, aspect, zNear, zFar);
}

void FractalWidget::paintGL() {
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate model view transformation
	QMatrix4x4 matrix;
	matrix.translate(2.0, 2.0, 2.0);
	matrix.rotate(rotation);

	// Set modelview-projection matrix
	program.setUniformValue("mvp_matrix", projection * matrix);

	// Draw cube geometry
	geometries->drawCubeGeometry(&program);
}