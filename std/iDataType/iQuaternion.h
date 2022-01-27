#pragma once

#include"iVector.h"

class iQuaternion
{
public:
	iQuaternion();
	iQuaternion(iVector3f v, float angle);
	~iQuaternion();

	void normalize();
	iQuaternion inverse() const;

public:
	float w;
	iVector3f axis;
};

iQuaternion operator*(const iQuaternion& q1, const iQuaternion& q2);
iQuaternion operator*(const iQuaternion& q, const iVector3f& v);


