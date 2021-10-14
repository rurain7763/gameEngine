#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 tvpMat;

out vec4 colorV;
out vec2 uvV;

in vec3 position;
in vec2 uv;

void main()
{
	gl_Position = tvpMat * vec4(position, 1.0);
	uvV = uv;
}

