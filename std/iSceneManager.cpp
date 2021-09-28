#include "iSceneManager.h"
#include "iStd.h"

iSceneManager* iSceneManager::S = NULL;

iSceneManager::iSceneManager()
{
	currScene = NULL;
	sceneStep = -1;
}

iSceneManager::~iSceneManager()
{
}

iSceneManager* iSceneManager::share()
{
	return nullptr;
}

void iSceneManager::addScene(iScene* scene)
{
	scenes.push_back(scene);
}

void iSceneManager::changeScene(iScene* scene, iArray* sendInfo)
{
}

void iSceneManager::changeScene(uint32 sceneIdx, iArray* sendInfo)
{
}

void iSceneManager::updateScene(float dt)
{
	
}



