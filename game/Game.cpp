#include "Game.h"
#include "iStd.h"
#include "TestScene.h"
#include "ServerScene.h"
#include "KoreanAutomataScene.h"

iSize* devSize;
iInputManager* inputMg;
iTime* timeMg;
iCamera* camera;
bool cameraMode;
iAssetReader* assetReader;
iGLShader* shader;
iThreadPool* threadPool;
iSceneManager* sceneMg;
iConnectionManager* connMg;
iKoreanAutoMata* korAm;

ServerScene scene1;
TestScene scene2;
KoreanAutomataScene scene3;

void loadGame()
{
	isSystemLittleEndian();
	srand(time(NULL));

#ifdef INCLUDE_INETWORK_HEADER
	loadNetwork();
#endif

#ifdef _WIN32
	loadGdi();
#endif

	devSize = new iSize;
	devSize->width = DEV_WIDTH;
	devSize->height = DEV_HEIGHT;

	inputMg = iInputManager::share();
	timeMg = iTime::share();

	camera = new iCamera(*devSize, { 0.f, 0.f, -5.f });
	cameraMode = false;

	assetReader = iAssetReader::share();
	shader = iGLShader::share();
	threadPool = iThreadPool::share();
	sceneMg = iSceneManager::share();
	connMg = iConnectionManager::share();
	korAm = iKoreanAutoMata::share();

	sceneMg->addScene(&scene3);
	sceneMg->addScene(&scene1);
	sceneMg->addScene(&scene2);
}

void drawGame()
{
	sceneMg->update(timeMg->deltaTime);

#if 0
	if (inputMg->keyOnce & KEY_p)
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
#endif

	timeMg->update();
	inputMg->update();
	threadPool->update();
}

void endGame()
{
	delete sceneMg;
	delete inputMg;
	delete timeMg;
	delete camera;
	delete devSize;
	delete assetReader;
	delete shader;
	delete connMg;
	delete korAm;
	delete threadPool;

#ifdef INCLUDE_INETWORK_HEADER
	endNetwork();
#endif

#ifdef _WIN32
	endGdi();
#endif
}
