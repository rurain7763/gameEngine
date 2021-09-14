#pragma once

#include "iType.h"
#include "iVector.h"
#include "iTransform.h"

#define DIRECTIONLIGHT	0
#define SPOTLIGHT		1	

struct iLight
{
	uint8 flag;

	iVector3f color;
	iVector3f dir;
	iVector3f position;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
};

struct iDirectionLight : public iLight
{
	//...?
};

struct iSpotLight : public iLight
{
};
