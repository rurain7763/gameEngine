#pragma once

#include "iType.h"
#include "iArray.h"

#define ISCENESTEP_LOAD		0
#define ISCENESTEP_UPDATE	1
#define ISCENESTEP_FREE		2

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

	void updateScene(float dt);

private:
	iArray scenes;
	int sceneStep;

public:
	iScene* currScene;
};

class iScene
{
public:
	virtual void load(iArray* recvInfo = NULL) = 0;
	virtual void update(float dt) = 0;
	virtual void free() = 0;
};