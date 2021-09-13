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
	float ambientIntensity;
	iVector3f dir;
	float diffuseIntensity;
	iVector3f position;
};

struct iDirectionLight : public iLight
{
	//...?
};

struct iSpotLight : public iLight
{
};
