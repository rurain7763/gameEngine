#pragma once

#include "iType.h"

#include "iSceneManager.h"
#include "iMatrix.h"
#include "iString.h"

class iGraphics;
class iGLTexture;
class iInputBox;

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
	iInputBox* ib;

	uint32 vao;
	uint32 vbo;
	uint32 ebo;
};

class iKoreanAutoMata;
class iInputManager;

class iInputBox
{
public:
	iInputBox();
	virtual ~iInputBox();

	void draw(float dt);

private:
	void updateBuff();

	iInputManager* im;
	iKoreanAutoMata* kam;
	iString buff;
public:

	uint64 cursor;
	char* result;
};


