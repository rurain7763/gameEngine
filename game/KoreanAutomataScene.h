#pragma once

#include "iType.h"

#include "iSceneManager.h"
#include "iMatrix.h"

class iGraphics;
class iGLTexture;

class KoreanAutomataScene : public iScene
{
public:
	virtual void load(iArray* recvInfo);
	virtual void update(float dt);
	virtual void free();

public:
	iGraphics* g;

	iMatrix ortho;
	iGLTexture* tex;

	uint32 vao;
	uint32 vbo;
	uint32 ebo;
};


