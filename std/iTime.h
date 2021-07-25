#pragma once

void updateDt();

class iTime
{
private:
	static iTime* S;
	iTime();

public:
	static iTime* share();
	void update();

private:
	int count;
	float _fps;

public:
	unsigned long long now;
	unsigned long long playTime;
	int fps;
	float deltaTime;
};