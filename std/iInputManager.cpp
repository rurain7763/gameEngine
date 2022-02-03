#include"iInputManager.h"
#include"iStd.h"

iInputManager* iInputManager::S = NULL;

iInputManager::iInputManager()
{
	langMode = LANG_MODE_NATIVE;
	keyDown = 0;
	keyBuffer = 0;
	keyOnce = 0;
	mousePos = { 0,0 };
}

iInputManager* iInputManager::share()
{
	if (!S) S = new iInputManager();

	return S;
}

void iInputManager::changeLanguageMode()
{
	if (langMode == LANG_MODE_NATIVE)
	{
		langMode = LANG_MODE_LOCALE;
	}
	else if (langMode == LANG_MODE_LOCALE)
	{
		langMode = LANG_MODE_NATIVE;
	}
}

void iInputManager::setKeyDown(iKey key)
{
	keyDown |= key;

	if (keyBuffer & key)
	{
		keyOnce &= ~key;
	}
	else
	{
		keyOnce |= key;
		keyBuffer |= key;
	}
}

void iInputManager::setKeyUp(iKey key)
{
	keyDown &= ~key;
	keyBuffer &= ~key;
}

void iInputManager::setMousePos(float x, float y)
{
	mousePos = { x,y };
}

void iInputManager::update()
{
	keyOnce = 0;
}

