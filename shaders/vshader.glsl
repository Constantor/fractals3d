#version 140

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform vec2 verts[9] = vec2[](
vec2(0.0, 0.0),
vec2(0.0, -1.0),
vec2(0.0, 1.0),
vec2(-1.0, 0.0),
vec2(-1.0, 1.0),
vec2(-1.0, -1.0),
vec2(1.0, 1.0),
vec2(1.0, -1.0),
vec2(0.0, 0.0)
);

void main() {
    gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);
}