#include "iLighting.h"
#include "iStd.h"

iLighting::iLighting()
{
	light = NULL;
}

iLighting::~iLighting()
{
	if (light) delete light;
}

void iLighting::setDirectionLight(iVector3f color, float intensity)
{
	iDirectionLight* dl = new iDirectionLight();
	dl->color = color;
	dl->intensity = intensity;
	dl->flag = DIRECTIONLIGHT;

	light = dl;
}

void iLighting::setSpotLight()
{
	
}

void iLighting::setPosition(float x, float y, float z)
{
	transform.translate(x, y, z);
	iVector4f pos = { 0.f, 0.f, 0.f, 1.f };
	transform.getMatrix() * pos;
}

iVector3f iLighting::getPosition()
{
	return { light->position.x, light->position.y, light->position.z };
}
