#pragma once

#include"iMatirx.h"

#define DEFAULT_CAMERA_SPEED			10.f
#define DEFAULT_CAMERA_SENSITIVE		2.f

struct iSize;

class iCamera
{
public:
	iCamera(iSize& devSize, iVector3f pos);
	virtual ~iCamera();

	bool onKey(int key, float dt);
	void onMouse(iVector2f mousePos, float dt);
	iMatrix getMatrix();

private:
	iSize& devSize;
	iVector2f prevMp;
	float hAngle;
	float vAngle;

public:
	iVector3f position;
	iVector3f lookAt;
	iVector3f up;

	float speed;
	float sensitive;
};