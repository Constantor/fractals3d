#include "fractalwidget.hpp"

#include <QMouseEvent>

#include <cmath>

FractalWidget::~FractalWidget() {
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
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
    if(!mousePressed) {
        return;
    }
    QVector2D diff = QVector2D(e->position()) - mousePressPosition;
    float diffX = diff.x();
	float diffY = diff.y();

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(0, diff.x(), 0.0).normalized();
    float alpha = 0.5 * (diff.x() * M_PI / 360.0);
    rotationDelta = diff.length() / 4;
	//короче есть окружность в x-z кордах, которая x^2+z^2=2.25
    //есть изначально x = 0, z = 1.5
	camera = QVector3D(camera.x() * cos(alpha) - camera.z() * sin(alpha), 0.0, camera.x() * sin(alpha) + camera.z() * cos(alpha));
    // Calculate new rotation axis as weighted sum
    rotationAxis = (n * rotationDelta).normalized();

    if(rotationDelta != 0) {
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, rotationDelta) * rotation;
        update();
        mousePressPosition = QVector2D(e->position());
        rotationDelta = 0;
    }
}

void FractalWidget::timerEvent(QTimerEvent *) {
    // timer
}

void FractalWidget::initializeGL() {
    this->setMouseTracking(true);

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

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

void FractalWidget::initTextures() {
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
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

    texture->bind();

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
    program.setUniformValue("CameraPosition", QVector3D(camera));

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawGeometry(&program);
}

void FractalWidget::setFractalData(FractalData *fractalData) {
    fd = fractalData;
}