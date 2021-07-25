#include"iVector.h"
#include"iStd.h"

//========================================
//				iVector2f
//========================================
iVector2f& iVector2f::normalized()
{
	float len = sqrt(x * x + y * y);
	x /= len;
	y /= len;

	return *this;
}

float iVector2f::magnitudeSqrt()
{
	return x *x + y*y;
}

iVector2f operator-(const iVector2f& v1, const iVector2f& v2)
{
	return iVector2f{ v1.x - v2.x, v1.y - v2.y };
}
//========================================
//				/iVector2f
//========================================

//========================================
//				iVector3f
//========================================
iVector3f iVector3f::operator+(const iVector3f& v) const
{
	iVector3f r;

	r.x = x + v.x;
	r.y = y + v.y;
	r.z = z + v.z;

	return r;
}

iVector3f iVector3f::operator*(const float v) const
{
	iVector3f r;

	r.x = x * v;
	r.y = y * v;
	r.z = z * v;

	return r;
}

iVector3f& iVector3f::operator+=(const iVector3f& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}
iVector3f& iVector3f::operator-=(const iVector3f& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

iVector3f iVector3f::operator-() const
{
	return { -x, -y, -z };
}

float iVector3f::dot(const iVector3f& v)
{
	return x * v.x + y * v.y + z * v.z;
}

iVector3f iVector3f::cross(const iVector3f& v)
{
	float _x = y * v.z - z * v.y;
	float _y = z * v.x - x * v.z;
	float _z = x * v.y - y * v.x;

	return iVector3f{ _x,_y,_z };
}

void iVector3f::rotate(iVector3f axis, float angle)
{
	iQuaternion q(axis, angle);
	iQuaternion inverse = q.inverse();

	iQuaternion r = q * (*this) * inverse;

	x = r.axis.x;
	y = r.axis.y;
	z = r.axis.z;
}

float iVector3f::dot(const iVector3f& v1, const iVector3f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

iVector3f iVector3f::cross(const iVector3f& v1, const iVector3f& v2)
{
	float _x = v1.y * v2.z - v1.z * v2.y;
	float _y = v1.z * v2.x - v1.x * v2.z;
	float _z = v1.x * v2.y - v1.y * v2.x;

	return iVector3f{ _x,_y,_z };
}

iVector3f& iVector3f::normalized()
{
	float len = sqrt(x * x + y * y + z * z);
	x /= len;
	y /= len;
	z /= len;

	return *this;
}
//========================================
//				/iVector3f
//========================================

