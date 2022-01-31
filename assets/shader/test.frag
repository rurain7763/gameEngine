#version 150

#ifdef GL_ES
precision mediump float;
#endif

struct iDirectionLight
{
	vec3 color;
	float intensity;
	vec3 position;
};

uniform sampler2D tex;
uniform iDirectionLight dirLight;

in vec4 colorV;
in vec2 uvV;

out vec4 throwColor;

void main()
{ 
	vec4 color = texture2D(tex, uvV).rgba * colorV;

	throwColor = color;
}

