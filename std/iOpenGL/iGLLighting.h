#pragma once

#include "gl/glew.h"

#include "iType.h"
#include "iVector.h"
#include "iMatrix.h"

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

class iGLLighting
{
public:
	iGLLighting();
	virtual ~iGLLighting();

	void setDirectionalLight(iVector3f color, iVector3f dir,
							 float ambientIntensity = 1.f, float diffuseIntensity = 1.f,
							 float specularIntensity = 1.f);

	void setPointLight(int idx, iVector3f color, iVector3f pos, float constant, float linear, float exponential,
					   float ambientIntensity = 1.f, float diffuseIntensity = 1.f, float specularIntensity = 1.f);

	void setSpotLight(int idx, iVector3f color, iVector3f pos, iVector3f dir, float degree, 
					  float constant, float linear, float exponential,
					  float ambientIntensity = 1.f, float diffuseIntensity = 1.f, float specularIntensity = 1.f);

	void sendToShader(const char* vertexShader, const char* fragmentShader, iMatrix* objTransform, iVector3f lookPos);

private:
	GLuint programID;
	GLuint transMatLoc;
	GLuint cameraPosLoc;

	iPointLight* pointLights;
	struct PointLightLocInfo
	{
		GLuint color;
		GLuint pos;
		GLuint ambientIntens;
		GLuint diffuseIntens;
		GLuint specularIntens;
		GLuint constant;
		GLuint linear;
		GLuint exponential;
	};
	PointLightLocInfo* pointLightLocInfo;

	iSpotLight* spotLights;
	struct SpotLightLocInfo
	{
		GLuint color;
		GLuint pos;
		GLuint dir;
		GLuint maximum;
		GLuint ambientIntens;
		GLuint diffuseIntens;
		GLuint specularIntens;
		GLuint constant;
		GLuint linear;
		GLuint exponential;
	};
	SpotLightLocInfo* spotLightLocInfo;

	iDirectionLight* dirLight;
	struct DirectionLightLocInfo
	{
		GLuint color;
		GLuint dir;
		GLuint ambientIntens;
		GLuint diffuseIntens;
		GLuint specularIntens;
	};
	DirectionLightLocInfo* dirLightLocInfo;
};

