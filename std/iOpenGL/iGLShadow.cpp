#include"iGLShadow.h"
#include"iStd.h"

iGLShadow::iGLShadow(iSize devSize)
	:shadowFbo(devSize, DEPTH_FBO, GL_FLOAT)
{
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

	shadowFbo.bind();
	GLuint programID = shader->useProgram("shadow", "shadow");

	camera->setLookAt(trans->positionV);
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

	glDrawBuffer(GL_NONE);

	shadowFbo.unbind();

	camera->position = backUpCameraPos;
	camera->setLookAt(backUpLookAt);
	glUseProgram(backUpProgramID);
}

