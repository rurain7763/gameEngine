#version 150
#define SPECULAR_POW_VALUE   32
#define MAX_POINT_LIGHT_NUM   3
#define MAX_SPOT_LIGHT_NUM	  3

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

struct iPointLight
{
	vec3 color;
	vec3 position;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;

	float constant;
	float linear;
	float exponential;
};

struct iSpotLight
{
	vec3 color;
	vec3 position;
	vec3 dir;
	float maximum;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;

	float constant;
	float linear;
	float exponential;
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
uniform iPointLight pointLights[MAX_POINT_LIGHT_NUM];
uniform iSpotLight spotLights[MAX_SPOT_LIGHT_NUM];

uniform iMaterial material;

uniform vec3 cameraPos;
in vec3 worldPos;
in vec3 normalV;
in vec2 uvV;

out vec4 throwColor;

vec4 calcDirectionalLight();
vec4 calcPointLights();
vec4 calcSpotLights();

void main()
{ 
	vec4 color = texture(diffuse, uvV);
	vec3 normalV = normalize(normalV);
	
	vec4 dirLight = calcDirectionalLight();
	vec4 totalPointLights = calcPointLights();
	vec4 totalSpotLights = calcSpotLights();

	throwColor = color * (dirLight + totalPointLights + totalSpotLights);
}

vec4 calcAmbientColor(vec3 color, float intensity)
{
	return vec4(color, 1.0) * intensity;
}

vec4 calcDiffuseColor(vec3 color, vec3 lightDir, float intensity)
{
	float diff = dot(normalV, normalize(-lightDir));
	diff = max(diff, 0.0);

	return vec4(color * intensity * diff, floor(diff));
}

vec4 calcSpecularColor(vec3 color, vec3 lightDir, float intensity)
{
	vec3 reflectLight = reflect(lightDir, normalV);
	vec3 toCamera = cameraPos - worldPos;

	float specular = dot(normalize(toCamera), normalize(reflectLight));
	specular = pow(max(specular, 0), SPECULAR_POW_VALUE);

	return vec4(color * intensity * specular, floor(specular));
}

vec4 calcDirectionalLight()
{
	vec4 ambient = calcAmbientColor(dirLight.color, dirLight.ambientIntensity);
	vec4 diffuse = calcDiffuseColor(dirLight.color, dirLight.dir, dirLight.diffuseIntensity);
	vec4 specular = calcSpecularColor(dirLight.color, dirLight.dir, dirLight.specularIntensity);

	return ambient + diffuse + specular;
}

vec4 calcPointLight(iPointLight light)
{ 
	vec3 lightDir = worldPos - light.position;
	float dist = length(lightDir);
	vec3 dir = normalize(lightDir);

	vec4 ambient = calcAmbientColor(light.color, light.ambientIntensity);
	vec4 diffuse = calcDiffuseColor(light.color, dir, light.diffuseIntensity);
	vec4 specular = calcSpecularColor(light.color, dir, light.specularIntensity);

	float attenuation = light.constant + 
						light.linear * dist +
						light.exponential * pow(dist, 2);

	return (ambient + diffuse + specular) / max(attenuation, 1);
}

vec4 calcPointLights()
{
	vec4 r = vec4(0.0);

	#if MAX_POINT_LIGHT_NUM > 0
	r += calcPointLight(pointLights[0]);
	#endif				
	
	#if MAX_POINT_LIGHT_NUM > 1
	r += calcPointLight(pointLights[1]);
	#endif					

	#if MAX_POINT_LIGHT_NUM > 2
	r += calcPointLight(pointLights[2]);
	#endif					

	#if MAX_POINT_LIGHT_NUM > 3
	r += calcPointLight(pointLights[3]);
	#endif			
	
	#if MAX_POINT_LIGHT_NUM > 4
	r += calcPointLight(pointLights[4]);
	#endif			
	
	#if MAX_POINT_LIGHT_NUM > 5 
	r += calcPointLight(pointLights[5]);
	#endif					

	#if MAX_POINT_LIGHT_NUM > 6
	r += calcPointLight(pointLights[6]);
	#endif				
	
	#if MAX_POINT_LIGHT_NUM > 7 
	r += calcPointLight(pointLights[7]);
	#endif					

	#if MAX_POINT_LIGHT_NUM > 8 
	r += calcPointLight(pointLights[8]);
	#endif					

	#if MAX_POINT_LIGHT_NUM > 9
	r += calcPointLight(pointLights[9]);
	#endif

	return r;
}

vec4 calcSpotLight(iSpotLight light)
{ 
	vec3 lightToPixel = worldPos - light.position;
	float spot = dot(normalize(lightToPixel), normalize(light.dir));
	
	spot = 1.0 - ((1.0 - spot) * (1.0 / (1.0 - light.maximum)));

	float dist = length(lightToPixel);
	vec3 dir = normalize(lightToPixel);

	vec4 ambient = calcAmbientColor(light.color, light.ambientIntensity);
	vec4 diffuse = calcDiffuseColor(light.color, dir, light.diffuseIntensity);
	vec4 specular = calcSpecularColor(light.color, dir, light.specularIntensity);

	float attenuation = light.constant + 
						light.linear * dist +
						light.exponential * pow(dist, 2);

	vec4 color =  (ambient + diffuse + specular) / max(attenuation, 1);

	return color * max(spot,0);
}

vec4 calcSpotLights()
{
	vec4 r = vec4(0.0);

	#if MAX_SPOT_LIGHT_NUM > 0
	r += calcSpotLight(spotLights[0]);
	#endif				
	
	#if MAX_SPOT_LIGHT_NUM > 1
	r += calcSpotLight(spotLights[1]);
	#endif					

	#if MAX_SPOT_LIGHT_NUM > 2
	r += calcSpotLight(spotLights[2]);
	#endif					

	#if MAX_SPOT_LIGHT_NUM > 3
	r += calcSpotLight(spotLights[3]);
	#endif			
	
	#if MAX_SPOT_LIGHT_NUM > 4
	r += calcSpotLight(spotLights[4]);
	#endif			
	
	#if MAX_SPOT_LIGHT_NUM > 5 
	r += calcSpotLight(spotLights[5]);
	#endif					

	#if MAX_SPOT_LIGHT_NUM > 6
	r += calcSpotLight(spotLights[6]);
	#endif				
	
	#if MAX_SPOT_LIGHT_NUM > 7 
	r += calcSpotLight(spotLights[7]);
	#endif					

	#if MAX_SPOT_LIGHT_NUM > 8 
	r += calcSpotLight(spotLights[8]);
	#endif					

	#if MAX_SPOT_LIGHT_NUM > 9
	r += calcSpotLight(spotLights[9]);
	#endif

	return r;
}

