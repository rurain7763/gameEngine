#pragma once

#include"iMatrix.h"

#define DEFAULT_CAMERA_SPEED			10.f
#define DEFAULT_CAMERA_SENSITIVE		0.05f
#define MOUSE_DELTA_MAX_DISTANSE		800.f * 800.f

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