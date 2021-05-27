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
	static const qreal degreesCoefficient = 0.1 / 360;
	static const qreal EPS = 0.0065;
	static const qreal minZoom = 0.9;
	static const qreal maxZoom = 4.48;
	qreal delta = numDegrees.y() * degreesCoefficient;
	qreal newValue = fractalData->zoomCoefficient + delta;
	if(EPS < abs(delta) && minZoom <= newValue && newValue <= maxZoom) {
		fractalData->zoomCoefficient = newValue;
		// qDebug() << fractalData->zoomCoefficient;
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

QVector3D rotate(QVector3D point, qreal alpha, QVector3D axis) {
    qreal t11 = cos(alpha) + (1 - cos(alpha)) * axis.x() * axis.x();
    qreal t12 = (1 - cos(alpha)) * axis.x() * axis.y() - sin(alpha) * axis.z();
	qreal t13 = (1 - cos(alpha)) * axis.x() * axis.z() + sin(alpha) * axis.y();
	qreal t21 = (1 - cos(alpha)) * axis.x() * axis.y() + sin(alpha) * axis.z();
	qreal t22 = cos(alpha) + (1 - cos(alpha)) * axis.y() * axis.y();
	qreal t23 = (1 - cos(alpha)) * axis.y() * axis.z() - sin(alpha) * axis.x();
	qreal t31 = (1 - cos(alpha)) * axis.x() * axis.z() - sin(alpha) * axis.y();
	qreal t32 = (1 - cos(alpha)) * axis.y() * axis.z() + sin(alpha) * axis.x();
	qreal t33 = cos(alpha) + (1 - cos(alpha)) * axis.z() * axis.z();
    return QVector3D(point.x() * t11 + point.y() * t21 + point.z() * t31,
                     point.x() * t12 + point.y() * t22 + point.z() * t32,
                     point.x() * t13 + point.y() * t23 + point.z() * t33);
}

void FractalWidget::mouseMoveEvent(QMouseEvent *e) {
    if(!mousePressed)
        return;
    QVector2D diff = QVector2D(e->position()) - mousePressPosition;
	if(diff.x() == 0 && diff.y() == 0)
		return;
	diff = -diff;
	QVector2D alpha = diff * (M_PI / 720.);

    QVector3D vecAxisY = (pointAxisY - fractalData->camera).normalized();

    pointAxisX = rotate(pointAxisX, alpha.x(), vecAxisY);
    pointAxisY = rotate(pointAxisY, alpha.x(), vecAxisY);
    fractalData->camera = rotate(fractalData->camera, alpha.x(), vecAxisY);

    /*if(diff.x() != 0) {
		rotation = QQuaternion::fromAxisAndAngle(vecAxisY, diff.x()) * rotation;
	}*/

    QVector3D vecAxisX = (pointAxisX - fractalData->camera).normalized();

	fractalData->camera = rotate(fractalData->camera, alpha.y(), vecAxisX);
    pointAxisX = rotate(pointAxisX, alpha.y(), vecAxisX);
    pointAxisY = rotate(pointAxisY, alpha.y(), vecAxisX);

    /*if(diff.y() != 0) {
        rotation = QQuaternion::fromAxisAndAngle(vecAxisX, diff.y()) * rotation;
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
	matrix.translate(0.0, 0.0, -3);
	//matrix.rotate(rotation);
	matrix.rotate({rotation.x(), 1, 0, 0});
	matrix.rotate({rotation.y(), 0, 1, 0});
	matrix.translate(0, 0, 0);
	matrix.lookAt(fractalData->zoomedCamera(), {0, 0, 0}, {0, 1, 0});

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
	program.setUniformValue("CameraPosition", fractalData->zoomedCamera());
	program.setUniformValue("ZoomCoefficient", static_cast<GLfloat>(fractalData->zoomCoefficient));

	// Draw cube geometry
	geometries->drawGeometry(&program);
}

void FractalWidget::setFractalData(FractalData *data) {
	fractalData = data;
}