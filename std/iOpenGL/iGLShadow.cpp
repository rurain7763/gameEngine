#include"iGLShadow.h"
#include"iStd.h"

iGLShadow::iGLShadow(iSize devSize)
{
	shadowFbo.addDepthBuffer(devSize.width, devSize.height);
	
	shadowFbo.bind();

	glDrawBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);

	shadowFbo.unbind();

	shadowTexture.resize(1 + MAX_POINT_LIGHT_NUM + MAX_SPOT_LIGHT_NUM);
}

iGLShadow::~iGLShadow()
{

}

void iGLShadow::updateDepthBuffer(iMatrix* proj, iCamera* camera, iTransform* trans, iGLMesh* mesh, iLight* light)
{
	iGLShader* shader = iGLShader::share();

	GLuint backUpProgramID = shader->currUseProgram;
	iVector3f backUpLookAt = camera->lookAt;
	iVector3f backUpCameraPos = camera->position;

	shadowFbo.bindForDraw();
	glClear(GL_DEPTH_BUFFER_BIT);

	GLuint programID = shader->useProgram("shadow", "shadow");

	if (light->flag == DIRECTIONLIGHT)
	{
		iDirectionLight* dirLight = (iDirectionLight*)light;
		camera->setLookAt(dirLight->dir);
	}
	else if (light->flag == SPOTLIGHT)
	{
		iSpotLight* spotLight = (iSpotLight*)light;
		camera->setLookAt(spotLight->dir);
	}
	else if(light->flag == POINTLIGHT)
	{
		camera->setLookAt(trans->positionV);
	}

	camera->position = light->position;
	iMatrix tvp = (*proj) * camera->getMatrix() * trans->getMatrix();

	GLuint tvpMat = glGetUniformLocation(programID, "tvpMat");
	glUniformMatrix4fv(tvpMat, 1, GL_TRUE, tvp.getData());

	GLuint pos = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(iVertexPNU), (const void*)offsetof(iVertexPNU, position));

	GLuint uv = glGetAttribLocation(programID, "uv");
	glEnableVertexAttribArray(uv);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(iVertexPNU), (const void*)offsetof(iVertexPNU, uv));

	glDisableVertexAttribArray(pos);
	glDisableVertexAttribArray(uv);

	GLuint tex = glGetUniformLocation(programID, "tex");
	shadowFbo.bindTexture(GL_TEXTURE0, iGLFboBufferTypeDepth);
	glUniform1i(tex, 0);

	shadowFbo.unbind();

	camera->position = backUpCameraPos;
	camera->setLookAt(backUpLookAt);
	glUseProgram(backUpProgramID);
}

