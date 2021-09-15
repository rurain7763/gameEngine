#pragma once

#include "iType.h"
#include "iVector.h"

#define DIRECTIONLIGHT	0
#define SPOTLIGHT		1	

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

	struct 
	{ 
		float constant; 
		float linear; 
		float exponential; 
	} attenuation;
};

struct iSpotLight : public iLight
{
};
