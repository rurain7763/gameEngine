#version 150
#define MAX_DIFFUSE_NUM		3
#define MAX_SPECULAR_NUM	1

#ifdef GL_ES
precision mediump float;
#endif

struct iDirectionLight
{
	vec3 color;
	float intensity;
	vec3 position;
};

uniform iDirectionLight dirLight;

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;

in vec3 normalV;
in vec2 uvV;

out vec4 throwColor;

void main()
{ 
	vec4 color = texture(diffuse, uvV);
	color *= vec4(dirLight.color, 1.0) * dirLight.intensity; //ambient

	//diffuse
	vec3 n = normalize(normalV);
	vec3 dir = normalize(dirLight.position - gl_FragCoord.xyz);

	throwColor = texture(diffuse, uvV);
}

