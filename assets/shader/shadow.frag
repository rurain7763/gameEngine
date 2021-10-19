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
	depth = 1.0 - (1.0 - depth) * 25.0;
	throwColor = vec4(depth);
}

