#pragma once

#include"iMatrix.h"
#include"iQuaternion.h"

#define ELUER						  0
#define QUARTERNION					  1
#define ROTATION_METHOD		QUARTERNION

class iTransform
{
public:
	iTransform();

	iMatrix getTransMatrix();
	//TODO
	//iMatrix getRotateMatrix();
	//iMatrix getScaleMatrix();

	void translate(float x, float y, float z);
	void rotate(float xAxis, float yAxis, float zAxis);
	void scale(float x, float y, float z);
	iMatrix getMatrix();

public:
	iVector3f positionV;
	iVector3f rotationV;
	iVector3f scaleV;
};

