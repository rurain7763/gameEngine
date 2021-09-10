#include "iSceneManager.h"
#include "iStd.h"

iSceneManager* iSceneManager::S = NULL;

iSceneManager::iSceneManager()
{

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
}



