#include "FractalWidget.hpp"

#include <QMouseEvent>
#include <cmath>
#include <QApplication>

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
	QPoint numDegrees = e->angleDelta();
	static const qreal degreesCoefficient = 0.7 / 360;
	static const qreal EPS = 0.0065;
	static const qreal minZoom = 0.3;
	static const qreal maxZoom = 22;
	qreal multiplier = numDegrees.y() * degreesCoefficient;
	qreal newValue = fractalData->zoomCoefficient * (1. + multiplier);
	if(EPS < abs(multiplier) && minZoom <= newValue && newValue <= maxZoom) {
		fractalData->zoomCoefficient = newValue;
		update();
	}
}

void FractalWidget::mousePressEvent(QMouseEvent *e) {
	// Save mouse press position
	mousePressPosition = QVector2D(e->position());
	mousePressed = true;
}

void FractalWidget::mouseReleaseEvent(QMouseEvent *e) {
	mousePressed = false;
}


QVector3D rotate(QVector3D point, float alpha, QVector3D axis) {
    double t11 = cos(alpha) + (1 - cos(alpha)) * axis.x() * axis.x();
    double t12 = (1 - cos(alpha)) * axis.x() * axis.y() - sin(alpha) * axis.z();
    double t13 = (1 - cos(alpha)) * axis.x() * axis.z() + sin(alpha) * axis.y();
    double t21 = (1 - cos(alpha)) * axis.x() * axis.y() + sin(alpha) * axis.z();
    double t22 = cos(alpha) + (1 - cos(alpha)) * axis.y() * axis.y();
    double t23 = (1 - cos(alpha)) * axis.y() * axis.z() - sin(alpha) * axis.x();
    double t31 = (1 - cos(alpha)) * axis.x() * axis.z() - sin(alpha) * axis.y();
    double t32 = (1 - cos(alpha)) * axis.y() * axis.z() + sin(alpha) * axis.x();
    double t33 = cos(alpha) + (1 - cos(alpha)) * axis.z() * axis.z();
    return QVector3D(point.x() * t11 + point.y() * t21 + point.z() * t31,
                     point.x() * t12 + point.y() * t22 + point.z() * t32,
                     point.x() * t13 + point.y() * t23 + point.z() * t33);
}

void FractalWidget::mouseMoveEvent(QMouseEvent *e) {
    if(!mousePressed)
        return;
    QVector2D diff = QVector2D(e->position()) - mousePressPosition;

    float diffX = diff.x();
    float diffY = diff.y();
    QVector3D vecAxisY = (pointAxisY - fractalData->camera).normalized();
    rotationDelta = diffX / 4;

    float alphaX = diffX * M_PI / 720.0;

    pointAxisX = rotate(pointAxisX, alphaX, vecAxisY);
    pointAxisY = rotate(pointAxisY, alphaX, vecAxisY);
    fractalData->camera = rotate(fractalData->camera, alphaX, vecAxisY);

    /*if(rotationDelta != 0) {
        rotation = QQuaternion::fromAxisAndAngle(vecAxisY, rotationDelta) * rotation;
        rotationDelta = 0;
    }*/

    QVector3D vecAxisX = (pointAxisX - fractalData->camera).normalized();

    QVector3D nY = vecAxisX.normalized();
    rotationDelta = diffY / 4;

    float alphaY = diffY * M_PI / 720.0;

	fractalData->camera = rotate(fractalData->camera, alphaY, vecAxisX);
    pointAxisX = rotate(pointAxisX, alphaY, vecAxisX);
    pointAxisY = rotate(pointAxisY, alphaY, vecAxisX);

    /*if(rotationDelta != 0) {
        rotation = QQuaternion::fromAxisAndAngle(vecAxisX, rotationDelta) * rotation;
        rotationDelta = 0;
    }*/

    update();
    mousePressPosition = QVector2D(e->position());
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
	//matrix.rotate(rotation);
	matrix.rotate({rotation.x(), 0, 1, 0});
	matrix.rotate({rotation.y(), 1, 0, 0});
	matrix.translate(0, 0, 0);

	// Set modelview-projection matrix
	program.setUniformValue("mvp_matrix", projection * matrix);

	program.setUniformValue("POWER", (GLint) fractalData->n);
	QApplication* app = dynamic_cast<QApplication*>(QCoreApplication::instance());
	program.setUniformValue("Resolution", app->devicePixelRatio() * QVector2D(this->width(), this->height()));
	program.setUniformValue("CriticalPointX", (GLfloat) fractalData->a);
	program.setUniformValue("CriticalPointY", (GLfloat) fractalData->b);
	program.setUniformValue("CriticalPointZ", (GLfloat) fractalData->c);
	program.setUniformValue("TYPE", (GLint) fractalData->type);
	program.setUniformValue("Ambience", transformColor(fractalData->ambienceColor));
	program.setUniformValue("ColorFractal", transformColor(fractalData->fractalColor));
	program.setUniformValue("CameraPosition", fractalData->camera);
	program.setUniformValue("ZoomCoefficient", static_cast<GLfloat>(1. / fractalData->zoomCoefficient));

	// Draw cube geometry
	geometries->drawGeometry(&program);
}

void FractalWidget::setFractalData(FractalData *data) {
	fractalData = data;
}