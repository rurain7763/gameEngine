#version 150

#ifdef GL_ES
precision mediump float;
#endif

in vec4 colorV;

out vec4 throwColor;

void main()
{
	throwColor = colorV;
}

