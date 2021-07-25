#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 tvpMat;

out vec4 colorV;

in vec4 position;
in vec4 color;

void main()
{
	gl_Position = tvpMat * position;
	colorV = color;
}

