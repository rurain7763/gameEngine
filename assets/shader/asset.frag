#version 150

#ifdef GL_ES
precision mediump float;
#endif

struct iDirectionLight
{
	vec3 color;
	float ambientIntensity;
	vec3 dir;
	float diffuseIntensity;
};

struct iMaterial
{
	vec3 ambient;
};

uniform iDirectionLight dirLight;
uniform iMaterial material;

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;

in vec3 normalV;
in vec2 uvV;

out vec4 throwColor;

vec4 calcAmbientColor()
{
	return vec4(dirLight.color, 1.0) * (material.ambient, 1.0) *dirLight.ambientIntensity;
}

vec4 calcDiffuseColor()
{
	float diff = dot(normalize(normalV), normalize(-dirLight.dir));
	diff = max(diff, 0.0);

	return vec4(dirLight.color * dirLight.diffuseIntensity * diff, floor(diff));
}

void main()
{ 
	vec4 color = texture(diffuse, uvV);
	
	vec4 ambient = calcAmbientColor();
	vec4 diffuse = calcDiffuseColor();

	throwColor = color * (ambient + diffuse);
}

