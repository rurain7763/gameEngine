#pragma once

#include "iType.h"
#include "iSceneManager.h"

#include "iMatrix.h"
#include "iGLTexture.h"
#include "iGLModel.h"
#include "iGLLighting.h"

class TestScene : public iScene
{
public:
	virtual void load(iArray* recvInfo);
	virtual void update(float dt);
	virtual void free();

public:
	iMatrix projMat;

	iGLTexturePTR tex;
	iGLModel* model;
	iGLLighting* lighting;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};