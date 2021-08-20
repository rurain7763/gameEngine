#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex;

in vec4 colorV;
in vec2 uvV;

out vec4 throwColor;

void main()
{ 
	throwColor = texture2D(tex, uvV).rgba * colorV;
}

