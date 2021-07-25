#pragma once

#include"iMatirx.h"
#include"iQuaternion.h"

#define ELUER			0
#define QUARTERNION		1
#define ROTATION_METHOD	QUARTERNION

class iTransform
{
public:
	iTransform();

	void translate(float x, float y, float z);
	void rotate(float xAxis, float yAxis, float zAxis);
	void scale(float x, float y, float z);
	iMatrix getMatrix();

private:
	iMatrix finalMat;
	iVector3f positionV;
	iVector3f rotationV;
	iVector3f scaleV;
};

