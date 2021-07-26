#include"iTime.h"
#include"iStd.h"

iTime* iTime::S = NULL;

iTime::iTime()
{
	count = 0;
	_fps = 0.f;

	now = GetTickCount64();
	playTime = 0;
	fps = 0;
	deltaTime = 0.f;
}

iTime* iTime::share()
{
	if (!S) S = new iTime();
	return S;
}

void iTime::update()
{
	float dt = (GetTickCount64() - now) / 1000.f;

	if (dt - deltaTime > 1.f) dt = deltaTime;

	deltaTime = dt;

	playTime += deltaTime;

	_fps += deltaTime;
	count++;
	if (_fps >= 1.f)
	{
		fps = count;
		_fps = 0.f;
		count = 0;
	}

	now = GetTickCount64();
}
