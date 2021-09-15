#version 150
#define SPECULAR_POW_VALUE 32

#ifdef GL_ES
precision mediump float;
#endif

struct iDirectionLight
{
	vec3 color;
	vec3 dir;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
};

struct iMaterial
{
	vec3 ambient;
	vec3 specular;
};

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;

uniform iDirectionLight dirLight;
uniform iMaterial material;
uniform vec3 cameraPos;

in vec3 worldPos;
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

vec4 calcSpecularColor()
{
	vec3 reflectLight = reflect(normalize(dirLight.dir), normalize(normalV));
	vec3 toCamera = cameraPos - worldPos;

	float specular = dot(normalize(toCamera), normalize(reflectLight));
	specular = pow(max(specular, 0), SPECULAR_POW_VALUE);

	return vec4(dirLight.color * dirLight.specularIntensity *
				material.specular * specular, floor(specular));
}

void main()
{ 
	vec4 color = texture(diffuse, uvV);
	
	vec4 ambient = calcAmbientColor();
	vec4 diffuse = calcDiffuseColor();
	vec4 specular = calcSpecularColor();

	throwColor = color * (ambient + diffuse + specular);
}

