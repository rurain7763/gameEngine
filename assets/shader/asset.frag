#version 150
#define MAX_DIFFUSE_NUM		3
#define MAX_SPECULAR_NUM	1

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;

in vec3 normalV;
in vec2 uvV;

out vec4 throwColor;

void main()
{ 
	vec4 diffuse = texture(diffuse0, uvV);
	vec4 specular = texture(specular0, uvV);
	vec4 normal = texture(normal0, uvV);

	throwColor = (diffuse + specular) * normal;
}

