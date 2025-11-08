#version 140

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

// Single screen-sized triangle. Using only three vertices keeps gl_VertexID in
// range on every platform, avoiding undefined behavior that produced blank
// output on stricter Wayland/mesa drivers.
const vec2 positions[3] = vec2[](
	vec2(-1.0, -1.0),
	vec2(3.0, -1.0),
	vec2(-1.0, 3.0)
);

void main() {
	gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
