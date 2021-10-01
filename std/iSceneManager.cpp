#include "iSceneManager.h"
#include "iStd.h"

iSceneManager* iSceneManager::S = NULL;

iSceneManager::iSceneManager()
{
	sceneStep = -1;
	currScene = NULL;
	next = NULL;
	sendInfo = NULL;
	delay = 0.f;
}

iSceneManager::~iSceneManager()
{
	if (currScene && (sceneStep != ISCENESTEP_FREE))
		currScene->free();
}

iSceneManager* iSceneManager::share()
{
	if (!S) S = new iSceneManager();
	return S;
}

void iSceneManager::addScene(iScene* scene)
{
	if (!currScene)
	{
		sceneStep = ISCENESTEP_LOAD;
		currScene = scene;
		next = scene;
		sendInfo = NULL;
	}

	scenes.push_back(scene);
}

void iSceneManager::changeScene(iScene* scene, iArray* si)
{
	if (sceneStep == ISCENESTEP_UPDATE)
	{
		sceneStep = ISCENESTEP_FREE;
		next = scene;
		delay = 0.f;
		sendInfo = si;
	}	
}

void iSceneManager::changeScene(uint32 sceneIdx, iArray* si)
{
	if (sceneStep == ISCENESTEP_UPDATE)
	{
		sceneStep = ISCENESTEP_FREE;
		next = (iScene*)scenes[sceneIdx];
		delay = 0.f;
		sendInfo = si;
	}
}

void iSceneManager::update(float dt)
{
	if (!currScene) return;

	switch (sceneStep)
	{
	case ISCENESTEP_LOAD:
	{
		currScene->load(sendInfo);
		sceneStep = ISCENESTEP_UPDATE;
		sendInfo = NULL;
		break;
	}
	case ISCENESTEP_UPDATE:
	{
		currScene->update(dt);
		break;
	}
	case ISCENESTEP_FREE:
	{
		if (delay == 0.f)
		{
			currScene->free();
		}
		else if(delay >= ISCENE_FREE_DELAY)
		{ 
			currScene = next;
			sceneStep = ISCENESTEP_LOAD;
		}
		delay += dt;
		break;
	}
	}
}



