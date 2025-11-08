#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class GeometryEngine : protected QOpenGLFunctions {
public:
	GeometryEngine();

	virtual ~GeometryEngine();

	void drawGeometry(QOpenGLShaderProgram *program);
};
