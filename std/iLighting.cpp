#include "iLighting.h"
#include "iStd.h"

iLighting::iLighting()
{
	pointLights = new iPointLight[MAX_POINT_LIGHT_NUM];
	for (int i = 0; i < MAX_POINT_LIGHT_NUM; i++)
	{
		iPointLight* light = &pointLights[i];

		light->flag = POINTLIGHT;
		light->color = {0.f, 0.f, 0.f};
		light->ambientIntensity = 0.f;
		light->diffuseIntensity = 0.f;
		light->specularIntensity = 0.f;

		light->position = {0.f, 0.f, 0.f};
		light->attenuation.constant = 0.f;
		light->attenuation.linear = 0.f;
		light->attenuation.exponential = 0.f;

		activeLights.push_back(light);
	}

	spotLights = new iSpotLight[MAX_SPOT_LIGHT_NUM];
	for (int i = 0; i < MAX_SPOT_LIGHT_NUM; i++)
	{
		iSpotLight* light = &spotLights[i];

		light->flag = SPOTLIGHT;
		light->color = { 0.f, 0.f, 0.f };
		light->ambientIntensity = 0.f;
		light->diffuseIntensity = 0.f;
		light->specularIntensity = 0.f;

		light->position = { 0.f, 0.f, 0.f };
		light->dir = { 0.f, 0.f, 0.f };
		light->degree = 0.f;
		light->attenuation.constant = 0.f;
		light->attenuation.linear = 0.f;
		light->attenuation.exponential = 0.f;

		activeLights.push_back(light);
	}

	dirLight = new iDirectionLight();
	memset(dirLight, 0, sizeof(iDirectionLight));
	dirLight->flag = DIRECTIONLIGHT;

	activeLights.push_back(dirLight);

	activeLights.resize(1 + MAX_POINT_LIGHT_NUM + MAX_SPOT_LIGHT_NUM);
}

iLighting::~iLighting()
{
	delete[] pointLights;
	delete[] spotLights;
	delete dirLight;
}

void iLighting::setDirectionalLight(iVector3f color, iVector3f dir,
									float ambientIntensity, float diffuseIntensity, float specularIntensity)
{
	dirLight->color = color;
	dirLight->dir = dir;
	dirLight->ambientIntensity = ambientIntensity;
	dirLight->diffuseIntensity = diffuseIntensity;
	dirLight->specularIntensity = specularIntensity;
}

void iLighting::setPointLight(int idx, iVector3f color, iVector3f pos, float constant, float linear, float exponential,
							  float ambientIntensity, float diffuseIntensity, float specularIntensity)
{
	if (idx >= MAX_POINT_LIGHT_NUM) return;

	iPointLight* light = &pointLights[idx];

	light->color = color;
	light->ambientIntensity = ambientIntensity;
	light->diffuseIntensity = diffuseIntensity;
	light->specularIntensity = specularIntensity;

	light->position = pos;
	light->attenuation.constant = constant;
	light->attenuation.linear = linear;
	light->attenuation.exponential = exponential;
}

void iLighting::setSpotLight(int idx, iVector3f color, iVector3f pos, iVector3f dir, float degree, 
							 float constant, float linear, float exponential, 
							 float ambientIntensity, float diffuseIntensity, float specularIntensity)
{
	if (idx >= MAX_SPOT_LIGHT_NUM ||
		degree <= 0.f ) return;

	iSpotLight* light = &spotLights[idx];

	light->color = color;
	light->ambientIntensity = ambientIntensity;
	light->diffuseIntensity = diffuseIntensity;
	light->specularIntensity = specularIntensity;

	light->position = pos;
	light->dir = dir;
	light->degree = degree;
	light->attenuation.constant = constant;
	light->attenuation.linear = linear;
	light->attenuation.exponential = exponential;
}


