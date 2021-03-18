#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : protected QOpenGLFunctions {
public:
	GeometryEngine();

	virtual ~GeometryEngine();

	void drawGeometry(QOpenGLShaderProgram *program);

private:
	void initGeometry();

	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
};
