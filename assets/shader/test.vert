#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 tvpMat;

out vec4 colorV;
out vec2 uvV;

in vec4 position;
in vec4 color;
in vec2 uv;

void main()
{
	gl_Position = tvpMat * position;
	colorV = color;
	uvV = uv;
}

