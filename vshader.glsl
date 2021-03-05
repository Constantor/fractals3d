#version 150

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform vec2 verts[6] = vec2[](vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(1.0, 1.0),
                                vec2(1.0, 1.0), vec2(-1.0, 1.0), vec2(-1.0, -1.0));

void main()
{
	gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);
}
//uniform mat4 mvp_matrix;

//attribute vec4 a_position;
//attribute vec2 a_texcoord;

//varying vec2 v_texcoord;

////! [0]
//void main()
//{
//	// Calculate vertex position in screen space
//	gl_Position = mvp_matrix * a_position;
//}
////! [0]


//#version 150

//uniform mat4 mvp_matrix;

//in vec4 ciPosition;

//void main()
//{
//	gl_Position = mvp_matrix * ciPosition;
//}

