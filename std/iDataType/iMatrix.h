#pragma once

#include"iVector.h"
#include"iQuaternion.h"

class iMatrix
{
public:
	iMatrix();
	iMatrix(const iMatrix& mat);
	iMatrix& operator=(const iMatrix& mat);
	iMatrix operator*(const iMatrix& mat);
	iVector4f operator*(iVector4f& vec);

	void frustrum(float fieldOfView, float width, float height, float n, float f);
	void ortho(float left, float right, float bottom, float top, float n, float f);
	void translate(float x, float y, float z);
	void translate(iVector4f vec);
	void rotate(int x, int y, int z);
	void rotate(const iQuaternion& q);
	void scale(float x, float y, float z);
	void loadIdentity();

	float* getData();

public:
	float data[4][4];
};




