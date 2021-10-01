#pragma once

#include "iType.h"
#include "iArray.h"

#define ISCENESTEP_LOAD		0
#define ISCENESTEP_UPDATE	1
#define ISCENESTEP_FREE		2

#define ISCENE_FREE_DELAY	1.f

class iScene;

class iSceneManager
{
private:
	iSceneManager();
	static iSceneManager* S;

public:
	virtual ~iSceneManager();
	static iSceneManager* share();

	void addScene(iScene* scene);
	void changeScene(iScene* scene, iArray* sendInfo = NULL);
	void changeScene(uint32 sceneIdx, iArray* sendInfo = NULL);

	void update(float dt);

private:
	iArray scenes;

	int sceneStep;
	iScene* currScene;
	iScene* next;
	float delay;
	iArray* sendInfo;
};

class iScene
{
public:
	virtual void load(iArray* recvInfo) = 0;
	virtual void update(float dt) = 0;
	virtual void free() = 0;
};