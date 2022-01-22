#include"iTime.h"
#include"iStd.h"

iTime* iTime::S = NULL;

iTime::iTime()
{
	time(&programStartTime);
	count = 0;
	_fps = 0.f;
	prev =
	duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	fps = 0;
	deltaTime = 0.f;
}

iTime* iTime::share()
{
	if (!S) S = new iTime();
	return S;
}

uint64 iTime::getMilliSec()
{
	return 
	duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

long long iTime::getPlayTime()
{
	long long now;
	time(&now);

	return now - programStartTime;
}

void iTime::update()
{
	uint64 now = 
	duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	float dt = (float)(now - prev) / 1000.f;

	if (dt - deltaTime > 1.f) dt = deltaTime;

	deltaTime = dt;

	_fps += deltaTime;
	count++;
	if (_fps >= 1.f)
	{
		fps = count;
		_fps = 0.f;
		count = 0;
	}

	prev = now;
}
