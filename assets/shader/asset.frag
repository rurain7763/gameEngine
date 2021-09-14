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
	vec3 normal = normalize(-normalV);
	vec3 dir = normalize(dirLight.dir);
	float halfDist = dot(normal, dir);
	vec3 reflectLight = dir + (-normal * 2 * halfDist);

	//vec3 reflectLight = reflect(dirLight.dir, normalV);
	vec3 toCamera = cameraPos - worldPos;
	float specular = dot(normalize(toCamera), normalize(reflectLight));
	specular = pow(specular, 32);

	return vec4(dirLight.color * material.specular * specular , 1.0);
}

void main()
{ 
	vec4 color = texture(diffuse, uvV);
	
	vec4 ambient = calcAmbientColor();
	vec4 diffuse = calcDiffuseColor();
	vec4 specular = calcSpecularColor() * diffuse.a;

	throwColor = color * (ambient + diffuse + specular);
}

