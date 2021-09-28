#include "iGLLighting.h"
#include "iStd.h"

iGLLighting::iGLLighting()
{
	programID = 0;

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
	}

	dirLight = new iDirectionLight();
	memset(dirLight, 0, sizeof(iDirectionLight));
	dirLight->flag = DIRECTIONLIGHT;
}

iGLLighting::~iGLLighting()
{
	delete[] pointLights;
	if (pointLightLocInfo) delete pointLightLocInfo;

	delete[] spotLights;
	if (spotLightLocInfo) delete[] spotLightLocInfo;

	delete dirLight;
	if (dirLightLocInfo) delete[] dirLightLocInfo;
}

void iGLLighting::setDirectionalLight(iVector3f color, iVector3f dir,
									float ambientIntensity, float diffuseIntensity, float specularIntensity)
{
	dirLight->color = color;
	dirLight->dir = dir;
	dirLight->ambientIntensity = ambientIntensity;
	dirLight->diffuseIntensity = diffuseIntensity;
	dirLight->specularIntensity = specularIntensity;
}

void iGLLighting::setPointLight(int idx, iVector3f color, iVector3f pos, float constant, float linear, float exponential,
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

void iGLLighting::setSpotLight(int idx, iVector3f color, iVector3f pos, iVector3f dir, float degree,
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

void iGLLighting::sendToShader(const char* vertex, const char* frag, iMatrix* trans, iVector3f lookPos)
{
	iGLShader* shader = iGLShader::share();
	GLuint newProgramID = shader->useProgram(vertex, frag);

	if (programID != newProgramID)
	{
		programID = newProgramID;

		transMatLoc = glGetUniformLocation(programID, "transMat");
		cameraPosLoc = glGetUniformLocation(programID, "cameraPos");

		pointLightLocInfo = new PointLightLocInfo[MAX_POINT_LIGHT_NUM];
		for (int i = 0; i < MAX_POINT_LIGHT_NUM; i++)
		{
			iString pointLightStr = "pointLights[?].?";
			char idx[2];
			sprintf(idx, "%d", i);
			pointLightStr.replace(12, 1, idx);

			pointLightStr.replace(15, 1, "color");
			pointLightLocInfo[i].color = glGetUniformLocation(programID, pointLightStr.str);

			pointLightStr.replace(15, 5, "position");
			pointLightLocInfo[i].pos = glGetUniformLocation(programID, pointLightStr.str);

			pointLightStr.replace(15, 8, "ambientIntensity");
			pointLightLocInfo[i].ambientIntens = glGetUniformLocation(programID, pointLightStr.str);

			pointLightStr.replace(15, 16, "diffuseIntensity");
			pointLightLocInfo[i].diffuseIntens = glGetUniformLocation(programID, pointLightStr.str);

			pointLightStr.replace(15, 16, "specularIntensity");
			pointLightLocInfo[i].specularIntens = glGetUniformLocation(programID, pointLightStr.str);

			pointLightStr.replace(15, 17, "constant");
			pointLightLocInfo[i].constant = glGetUniformLocation(programID, pointLightStr.str);

			pointLightStr.replace(15, 8, "linear");
			pointLightLocInfo[i].linear = glGetUniformLocation(programID, pointLightStr.str);

			pointLightStr.replace(15, 6, "exponential");
			pointLightLocInfo[i].exponential = glGetUniformLocation(programID, pointLightStr.str);
		}

		spotLightLocInfo = new SpotLightLocInfo[MAX_SPOT_LIGHT_NUM];
		for (int i = 0; i < MAX_SPOT_LIGHT_NUM; i++)
		{
			iString spotLightStr = "spotLights[?].?";
			char idx[2];
			sprintf(idx, "%d", i);
			spotLightStr.replace(11, 1, idx);

			spotLightStr.replace(14, 1, "color");
			spotLightLocInfo[i].color = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 5, "position");
			spotLightLocInfo[i].pos = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 8, "dir");
			spotLightLocInfo[i].dir = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 3, "maximum");
			spotLightLocInfo[i].maximum = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 7, "ambientIntensity");
			spotLightLocInfo[i].ambientIntens = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 16, "diffuseIntensity");
			spotLightLocInfo[i].diffuseIntens = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 16, "specularIntensity");
			spotLightLocInfo[i].specularIntens = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 17, "constant");
			spotLightLocInfo[i].exponential = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 8, "linear");
			spotLightLocInfo[i].linear = glGetUniformLocation(programID, spotLightStr.str);

			spotLightStr.replace(14, 6, "exponential");
			spotLightLocInfo[i].exponential = glGetUniformLocation(programID, spotLightStr.str);
		}

		dirLightLocInfo = new DirectionLightLocInfo;
		dirLightLocInfo->color = glGetUniformLocation(programID, "dirLight.color");
		dirLightLocInfo->dir = glGetUniformLocation(programID, "dirLight.dir");
		dirLightLocInfo->ambientIntens = glGetUniformLocation(programID, "dirLight.ambientIntensity");
		dirLightLocInfo->diffuseIntens = glGetUniformLocation(programID, "dirLight.diffuseIntensity");
		dirLightLocInfo->specularIntens = glGetUniformLocation(programID, "dirLight.specularIntensity");
	}

	glUniformMatrix4fv(transMatLoc, 1, GL_TRUE, trans->getData());
	glUniform3f(cameraPosLoc, lookPos.x, lookPos.y, lookPos.z);

	glUniform3fv(dirLightLocInfo->color, 1, (GLfloat*)&dirLight->color);
	glUniform3fv(dirLightLocInfo->dir, 1, (GLfloat*)&dirLight->dir);
	glUniform1f(dirLightLocInfo->ambientIntens, dirLight->ambientIntensity);
	glUniform1f(dirLightLocInfo->diffuseIntens, dirLight->diffuseIntensity);
	glUniform1f(dirLightLocInfo->specularIntens, dirLight->specularIntensity);

	for (int i = 0; i < MAX_POINT_LIGHT_NUM; i++)
	{
		iPointLight* pointLight = &pointLights[i];
		glUniform3fv(pointLightLocInfo[i].color, 1, (GLfloat*)&pointLight->color);
		glUniform3fv(pointLightLocInfo[i].pos, 1, (GLfloat*)&pointLight->position);
		glUniform1f(pointLightLocInfo[i].ambientIntens, pointLight->ambientIntensity);
		glUniform1f(pointLightLocInfo[i].diffuseIntens, pointLight->diffuseIntensity);
		glUniform1f(pointLightLocInfo[i].specularIntens, pointLight->specularIntensity);
		glUniform1f(pointLightLocInfo[i].constant, pointLight->attenuation.constant);
		glUniform1f(pointLightLocInfo[i].linear, pointLight->attenuation.linear);
		glUniform1f(pointLightLocInfo[i].exponential, pointLight->attenuation.exponential);
	}

	for (int i = 0; i < MAX_SPOT_LIGHT_NUM; i++)
	{
		iSpotLight* spotLight = &spotLights[i];
		glUniform3fv(spotLightLocInfo[i].color, 1, (GLfloat*)&spotLight->color);
		glUniform3fv(spotLightLocInfo[i].pos, 1, (GLfloat*)&spotLight->position);
		glUniform3fv(spotLightLocInfo[i].dir, 1, (GLfloat*)&spotLight->dir);
		glUniform1f(spotLightLocInfo[i].maximum, icos(spotLight->degree));
		glUniform1f(spotLightLocInfo[i].ambientIntens, spotLight->ambientIntensity);
		glUniform1f(spotLightLocInfo[i].diffuseIntens, spotLight->diffuseIntensity);
		glUniform1f(spotLightLocInfo[i].specularIntens, spotLight->specularIntensity);
		glUniform1f(spotLightLocInfo[i].constant, spotLight->attenuation.constant);
		glUniform1f(spotLightLocInfo[i].linear, spotLight->attenuation.linear);
		glUniform1f(spotLightLocInfo[i].exponential, spotLight->attenuation.exponential);
	}
}


