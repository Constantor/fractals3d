#include "FractalWidget.hpp"

#include <QMouseEvent>
#include <cmath>

namespace {
	QVector3D transformColor(const QColor &color) {
		return QVector3D(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
	}
}// namespace

FractalWidget::~FractalWidget() {
	// Make sure the context is current when deleting the buffers.
	makeCurrent();
	delete geometries;
	doneCurrent();
}

void FractalWidget::wheelEvent(QWheelEvent *e) {
	QPoint numPixels = e->pixelDelta();
	QPoint numDegrees = e->angleDelta();
	e->accept();
}

void FractalWidget::mousePressEvent(QMouseEvent *e) {
	// Save mouse press position
	mousePressPosition = QVector2D(e->position());
	mousePressed = true;
}

void FractalWidget::mouseReleaseEvent(QMouseEvent *e) {
	mousePressed = false;
}

void FractalWidget::mouseMoveEvent(QMouseEvent *e) {
	if(!mousePressed)
		return;
	QVector2D diff = QVector2D(e->position()) - mousePressPosition;

	QVector3D n = QVector3D(0, diff.x(), 0.0).normalized();
	float alpha = 0.5 * (diff.x() * M_PI / 360.0);
	rotationDelta = diff.x() / 4;
	fd->camera = QVector3D(fd->camera.x() * cos(alpha) - fd->camera.z() * sin(alpha), fd->camera.y(), fd->camera.x() * sin(alpha) + fd->camera.z() * cos(alpha));
	rotationAxis = (n * rotationDelta).normalized();

	if(rotationDelta != 0) {
		rotation = QQuaternion::fromAxisAndAngle(rotationAxis, rotationDelta) * rotation;
		update();
		mousePressPosition = QVector2D(e->position());
		rotationDelta = 0;
	}
}

//void FractalWidget::mouseMoveEvent(QMouseEvent *e) {
//    if(!mousePressed) {
//        return;
//    }
//
//    QVector2D diff = QVector2D(e->position()) - mousePressPosition;
//
//    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
//
//    rotationDelta = diff.length() / 4;
//
//    QVector3D vec = QVector3D(diff.length() * sin(acos(fd->camera.x() / sqrt(fd->camera.x() * fd->camera.x() + fd->camera.y() * fd->camera.y() + fd->camera.z() * fd->camera.z()))),
//                              diff.length() * sin(acos(fd->camera.y() / sqrt(fd->camera.x() * fd->camera.x() + fd->camera.y() * fd->camera.y() + fd->camera.z() * fd->camera.z()))),
//                              diff.length() * sin(acos(fd->camera.z() / sqrt(fd->camera.x() * fd->camera.x() + fd->camera.y() * fd->camera.y() + fd->camera.z() * fd->camera.z()))));
//
//    float alphaX = 0.5 * (vec.x() * M_PI / 360.0);
//    float alphaY = 0.5 * (vec.y() * M_PI / 360.0);
//    float alphaZ = 0.5 * (vec.z() * M_PI / 360.0);
////
//    fd->camera = QVector3D(fd->camera.x(), fd->camera.y() * cos(alphaX) - fd->camera.z() * sin(alphaX), fd->camera.y() * sin(alphaX) + fd->camera.z() * cos(alphaX));
//    fd->camera = QVector3D(fd->camera.x() * cos(alphaY) - fd->camera.z() * sin(alphaY), fd->camera.y(), fd->camera.x() * sin(alphaY) + fd->camera.z() * cos(alphaY));
//    //fd->camera = QVector3D(fd->camera.x() * cos(alphaZ) - fd->camera.y() * sin(alphaZ), fd->camera.x() * sin(alphaZ) + fd->camera.y() * cos(alphaZ), fd->camera.z());
//
//    rotationAxis = (n * rotationDelta).normalized();
//
//    if (rotationDelta != 0) {
//        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, rotationDelta) * rotation;
//        update();
//        mousePressPosition = QVector2D(e->position());
//        rotationDelta = 0;
//    }
//}

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
	// timer.start(12, this);
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
	const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

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
	matrix.translate(0.0, 0.0, -5.0);
	matrix.rotate(rotation);

	// Set modelview-projection matrix
	program.setUniformValue("mvp_matrix", projection * matrix);

	program.setUniformValue("POWER", (GLint) fd->n);
	program.setUniformValue("Resolution", QVector2D(this->height(), this->width()));
	program.setUniformValue("CriticalPointX", (GLfloat) fd->a);
	program.setUniformValue("CriticalPointY", (GLfloat) fd->b);
	program.setUniformValue("CriticalPointZ", (GLfloat) fd->c);
	program.setUniformValue("TYPE", (GLint) fd->type);
	program.setUniformValue("Ambience", transformColor(fd->ambienceColor));
	program.setUniformValue("ColorFractal", transformColor(fd->fractalColor));
	program.setUniformValue("CameraPosition", QVector3D(fd->camera));

	// Draw cube geometry
	geometries->drawGeometry(&program);
}

void FractalWidget::setFractalData(FractalData *fractalData) {
	fd = fractalData;
}