#pragma once

#include "iType.h"

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
	void changeScene(iScene* scene);
	void changeScene(uint32 sceneIdx);

public:
	iScene* currScene;

	iScene** scene;
	int sceneNum;
};

class iScene
{
public:
	virtual void load() = 0;
	virtual void update(float dt) = 0;
	virtual void free() = 0;
};