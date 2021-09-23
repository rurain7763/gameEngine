#pragma once

#include "iType.h"
#include "iVector.h"
#include "iArray.h"

#define DIRECTIONLIGHT	1
#define POINTLIGHT		2
#define SPOTLIGHT		3

#define MAX_POINT_LIGHT_NUM	 3
#define MAX_SPOT_LIGHT_NUM	 3

struct Attenuation
{
	float constant;
	float linear;
	float exponential;
};

struct iLight
{
	uint8 flag;

	iVector3f color;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
};

struct iDirectionLight : public iLight
{
	iVector3f dir;
};

struct iPointLight : public iLight
{
	iVector3f position;

	Attenuation attenuation;
};

struct iSpotLight : public iLight
{
	iVector3f position;
	iVector3f dir;
	float degree;

	Attenuation attenuation;
};

class iLighting
{
public:
	iLighting();
	virtual ~iLighting();

	void setDirectionalLight(iVector3f color, iVector3f dir,
							 float ambientIntensity = 1.f, float diffuseIntensity = 1.f,
							 float specularIntensity = 1.f);
	void setPointLight(int idx, iVector3f color, iVector3f pos, float constant, float linear, float exponential,
					   float ambientIntensity = 1.f, float diffuseIntensity = 1.f, float specularIntensity = 1.f);

	void setSpotLight(int idx, iVector3f color, iVector3f pos, iVector3f dir, float degree, 
					  float constant, float linear, float exponential,
					  float ambientIntensity = 1.f, float diffuseIntensity = 1.f, float specularIntensity = 1.f);

private:
	iPointLight* pointLights;
	iSpotLight* spotLights;
	iDirectionLight* dirLight;

public:
	iArray activeLights;
};

