#include "Game.h"
#include "iStd.h"

iSize* devSize;
iInputManager* inputMg;
iTime* timeMg;
iCamera* camera;
bool cameraMode;
iAssetReader* assetReader;
iGLShader* shader;
iThreadPool* threadPool;
iSceneManager* sceneMg;

void loadGame()
{
	isSystemLittleEndian();
	srand(time(NULL));

	devSize = new iSize;
	devSize->width = DEV_WIDTH;
	devSize->height = DEV_HEIGHT;

	inputMg = iInputManager::share();
	timeMg = iTime::share();

	camera = new iCamera(*devSize, { 0.f, 0.f, -5.f });
	cameraMode = false;

	assetReader = iAssetReader::share();
	threadPool = iThreadPool::share();
	sceneMg = iSceneManager::share();
	shader = iGLShader::share();
}

void drawGame()
{
	sceneMg->update(timeMg->deltaTime);

	if (inputMg->keyOnce & KEY_P)
	{
		if (!cameraMode)
		{
			showCursor(false);
			wrapCursor(true);
			cameraMode = true;
		}
		else
		{
			showCursor(true);
			wrapCursor(false);
			cameraMode = false;
		}
	}

	if (cameraMode)
	{
		camera->onKey(inputMg->keyDown, timeMg->deltaTime);
		camera->onMouse(inputMg->mousePos, timeMg->deltaTime);
	}

	timeMg->update();
	inputMg->update();
	threadPool->update();
}

void endGame()
{
	delete inputMg;
	delete timeMg;
	delete camera;
	delete devSize;
	delete assetReader;
	delete shader;
	delete threadPool;
	delete sceneMg;
}
