#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex;
in vec2 uvV;

out vec4 throwColor;

void main()
{ 
	float depth = texture(tex, uvV).x;
	throwColor = vec4(depth);
}

