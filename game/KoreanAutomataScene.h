#pragma once

#include "iType.h"

#include "iSceneManager.h"

class KoreanAutomataScene : public iScene
{
public:
	virtual void load(iArray* recvInfo);
	virtual void update(float dt);
	virtual void free();

public:
	
};


