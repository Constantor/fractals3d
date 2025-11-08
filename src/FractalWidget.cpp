#include "FractalWidget.hpp"

#include <QMessageBox>
#include <QOpenGLContext>
#include <QSurfaceFormat>


namespace {
	QVector3D transformColor(const QColor &color) {
		return QVector3D(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
	}

	void showCritical(QWidget *widget, const QString &title, const QString &message) {
		QMessageBox box(widget ? widget->window() : nullptr);
		box.setIcon(QMessageBox::Critical);
		box.setWindowTitle(title);
		box.setText(message);
		box.exec();
	}

	QString describeContext(QOpenGLContext *ctx) {
		if(ctx == nullptr)
			return QStringLiteral("unknown OpenGL context");
		const auto fmt = ctx->format();
		const QString api = ctx->isOpenGLES() ? QStringLiteral("OpenGL ES") : QStringLiteral("OpenGL");
		return QStringLiteral("%1 %2.%3").arg(api).arg(fmt.majorVersion()).arg(fmt.minorVersion());
	}
}// namespace

FractalWidget::~FractalWidget() {
	// Make sure the context is current when deleting the buffers.
	makeCurrent();
	vao.destroy();
	delete geometries;
	doneCurrent();
	delete timer;
	delete elapsedTimer;
}

void FractalWidget::wheelEvent(QWheelEvent *e) {
	static const qreal degreesCoefficient = 1.0 / 360;
	static const qreal EPS = 0.001;
	QPoint numDegrees = e->angleDelta();

	static const qreal minZoom = -5.0;
	static const qreal maxZoom = 9.0;
	qreal delta = numDegrees.y() * degreesCoefficient;
	qreal newValue = fractalData->zoomCoefficient + delta;
	qreal newSpeed = fractalData->rotateSpeed;

	if(newValue > fractalData->defaultZoom) {
		newSpeed /= (1 + (newValue - fractalData->defaultZoom) * 0.1);
	}

	if(EPS < abs(delta) && minZoom <= newValue && newValue <= maxZoom) {
		fractalData->zoomCoefficient = newValue;
		fractalData->rotateSpeed = newSpeed;
		update();
	}
}

void FractalWidget::mousePressEvent(QMouseEvent *e) {
	// Save mouse press position
	mousePressPosition = QVector2D(e->position());
	mousePressed = true;
}

void FractalWidget::mouseReleaseEvent(QMouseEvent *) {
	mousePressed = false;
}

namespace {
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
};// namespace

void FractalWidget::rotateFractal(QVector2D const &diff) {
	if(diff.x() == 0 && diff.y() == 0)
		return;
	QVector2D alpha = diff * (M_PI / 720.);

	QVector3D vecAxisY = (pointAxisY - fractalData->camera).normalized();

	pointAxisX = rotate(pointAxisX, alpha.x(), vecAxisY);
	pointAxisY = rotate(pointAxisY, alpha.x(), vecAxisY);
	fractalData->camera = rotate(fractalData->camera, alpha.x(), vecAxisY);

	QVector3D vecAxisX = (pointAxisX - fractalData->camera).normalized();

	fractalData->camera = rotate(fractalData->camera, alpha.y(), vecAxisX);
	pointAxisX = rotate(pointAxisX, alpha.y(), vecAxisX);
	pointAxisY = rotate(pointAxisY, alpha.y(), vecAxisX);

	update();
}

void FractalWidget::mouseMoveEvent(QMouseEvent *e) {
	if(!mousePressed)
		return;
	rotateFractal(QVector2D(e->position()) - mousePressPosition);
	mousePressPosition = QVector2D(e->position());
}

void FractalWidget::initializeGL() {
	this->setMouseTracking(true);

	initializeOpenGLFunctions();

	QOpenGLContext *ctx = context();
	if(ctx == nullptr) {
		showCritical(this, tr("OpenGL initialization failed"), tr("Qt could not acquire an OpenGL context. Update GPU drivers or run the application under X11."));
		return;
	}

	const auto fmt = ctx->format();
	const bool isDesktopOpenGL = !ctx->isOpenGLES() && fmt.renderableType() == QSurfaceFormat::OpenGL;
	const bool versionOk = fmt.majorVersion() > 3 || (fmt.majorVersion() == 3 && fmt.minorVersion() >= 1);
	if(!isDesktopOpenGL || !versionOk) {
		const QString details = tr("Fractals 3D needs a desktop OpenGL 3.1+ context, but Qt created %1.\n\nOn Wayland try launching with `QT_QPA_PLATFORM=xcb` or install drivers exposing desktop OpenGL.").arg(describeContext(ctx));
		showCritical(this, tr("Desktop OpenGL required"), details);
		return;
	}

	if(!vao.create()) {
		showCritical(this, tr("OpenGL error"), tr("Failed to create a vertex array object. Ensure that your system supports OpenGL 3.1 or newer."));
		return;
	}
	vao.bind();
	glClearColor(0, 0, 0, 1);

	if(!initShaders())
		return;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if(!geometries)
		geometries = new GeometryEngine;

	if(!timer) {
		timer = new QTimer;
		connect(timer, &QTimer::timeout, [&]() { autoRotate(); });
	}
	if(!elapsedTimer)
		elapsedTimer = new QElapsedTimer();
	elapsedTimer->start();
	timer->start(0);
	shaderErrorShown = false;
	renderingReady = true;
}

bool FractalWidget::initShaders() {
	auto fail = [&](const QString &stage) {
		QString details = stage;
		const QString log = program.log();
		if(!log.isEmpty()) {
			details += "\n\n" + tr("Driver log:") + "\n" + log;
		}
		details += tr("\n\nIf you are on Wayland, try starting the application with `QT_QPA_PLATFORM=xcb` to get a desktop OpenGL context.");
		showCritical(this, tr("Shader compilation failed"), details);
		return false;
	};

	program.removeAllShaders();
	if(!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
		return fail(tr("Could not compile vshader.glsl"));
	if(!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
		return fail(tr("Could not compile fshader.glsl"));
	if(!program.link())
		return fail(tr("Could not link the OpenGL shader program"));
	if(!program.bind())
		return fail(tr("Could not bind the OpenGL shader program"));
	return true;
}

void FractalWidget::resizeGL(int w, int h) {
	// Calculate aspect ratio
	qreal aspect = qreal(w) / qreal(h ? h : 1);

	const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

	projection.setToIdentity();

	// Set perspective projection
	projection.perspective(fov, aspect, zNear, zFar);
}

void FractalWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(!renderingReady || geometries == nullptr)
		return;
	QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
	if(!program.bind()) {
		if(!shaderErrorShown) {
			shaderErrorShown = true;
			QString details = tr("Unable to bind the shader program for rendering.");
			const QString log = program.log();
			if(!log.isEmpty())
				details += "\n\n" + tr("Driver log:") + "\n" + log;
			details += tr("\n\nTry forcing an X11 session with `QT_QPA_PLATFORM=xcb` if you are running under Wayland.");
			showCritical(this, tr("OpenGL error"), details);
		}
		renderingReady = false;
		return;
	}

	QMatrix4x4 matrix;
	matrix.translate(0.0, 0.0, fractalData->zoomCoefficient);
	matrix.lookAt(fractalData->camera, -fractalData->camera, pointAxisY - fractalData->camera);

	program.setUniformValue("mvp_matrix", projection * matrix);

	program.setUniformValue("POWER", (GLint) fractalData->n);
	program.setUniformValue("Resolution", dynamic_cast<QApplication *>(QCoreApplication::instance())->devicePixelRatio() * QVector2D(this->width(), this->height()));
	program.setUniformValue("CriticalPointX", (GLfloat) fractalData->a);
	program.setUniformValue("CriticalPointY", (GLfloat) fractalData->b);
	program.setUniformValue("CriticalPointZ", (GLfloat) fractalData->c);
	program.setUniformValue("TYPE", (GLint) fractalData->type);
	program.setUniformValue("Ambience", transformColor(fractalData->ambienceColor));
	program.setUniformValue("ColorFractal", transformColor(fractalData->fractalColor));
	program.setUniformValue("CameraPosition", fractalData->camera);
	program.setUniformValue("ZoomCoefficient", (GLfloat) fractalData->zoomCoefficient);

	// Draw geometry
	geometries->drawGeometry(&program);
	program.release();
}

void FractalWidget::setFractalData(FractalData *data) {
	fractalData = data;
}

void FractalWidget::autoRotate() {
	if(fractalData->isRotating) {
		auto nextPos = static_cast<qreal>(elapsedTimer->elapsed());
		qreal dx = (nextPos - autoRotationPos) * fractalData->absoluteSpeed;
		autoRotationPos = nextPos;
		rotateFractal({static_cast<float>(dx), 0.0});
	}
}
