#pragma once

#define DEV_WIDTH		1920.f
#define DEV_HEIGHT		1080.f

#define DEFAULT_CAMERA_POSITION	{ 0.f, 0.f, -5.f }

#include "iType.h"

struct iSize;
class iInputManager;
class iTime;
class iCamera;
class iAssetReader;
class iGLShader;
class iThreadPool;
class iSceneManager;

extern iSize* devSize;
extern iInputManager* inputMg;
extern iTime* timeMg;
extern iCamera* camera;
extern iAssetReader* assetReader;
extern iGLShader* shader;
extern iThreadPool* threadPool;
extern iSceneManager* sceneMg;

void loadGame();
void drawGame();
void endGame();



