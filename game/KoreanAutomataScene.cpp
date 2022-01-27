#include "KoreanAutomataScene.h"
#include "Game.h"
#include "iStd.h"

void KoreanAutomataScene::load(iArray* recvInfo)
{
	delete getFont("arial");
	delete getFont("assets/test/ASMAN.TTF");
}

void KoreanAutomataScene::update(float dt)
{
	if (inputMg->keyOnce & KEY_1)
	{
		sceneMg->changeScene((uint32)0);
	}
	else if (inputMg->keyOnce & KEY_2)
	{
		sceneMg->changeScene((uint32)1);
	}
	else if (inputMg->keyOnce & KEY_3)
	{
		sceneMg->changeScene((uint32)2);
	}
}

void KoreanAutomataScene::free()
{
}

