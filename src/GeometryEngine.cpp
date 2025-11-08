#include "GeometryEngine.hpp"

GeometryEngine::GeometryEngine() {
	initializeOpenGLFunctions();
}

GeometryEngine::~GeometryEngine() = default;

void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program) {
	Q_UNUSED(program);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
