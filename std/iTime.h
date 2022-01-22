#pragma once

#include <chrono>
using namespace std::chrono;

#include "iType.h"

class iTime
{
private:
	static iTime* S;
	iTime();

public:
	static iTime* share();

	uint64 getMilliSec();
	long long getPlayTime();
	void update();

private:
	long long programStartTime;
	int count;
	float _fps;
	uint64 prev;

public:
	int fps;
	float deltaTime;
};