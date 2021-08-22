#include"iQuaternion.h"
#include"iStd.h"

iQuaternion::iQuaternion()
{
	axis = { 0,0,0 };
	w = 0;
}

iQuaternion::iQuaternion(iVector3f v, float angle)
{
	float _a = angle / 2.f;

	w = icos(_a);
	axis.x = v.x * isin(_a);
	axis.y = v.y * isin(_a);
	axis.z = v.z * isin(_a);
}

iQuaternion::~iQuaternion()
{
}

void iQuaternion::normalize()
{
	float len = sqrt(w * w + axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

	w /= len;
	axis.x /= len;
	axis.y /= len;
	axis.z /= len;
}

iQuaternion iQuaternion::inverse() const
{
	iQuaternion r;

	r.w = w;
	r.axis = -axis;

	return r;
}

iQuaternion operator*(const iQuaternion& q1, const iQuaternion& q2)
{
	iQuaternion r;

	r.w = q1.w * q2.w - iVector3f::dot(q1.axis, q2.axis);
	r.axis = q1.axis * q2.w + q2.axis * q1.w + iVector3f::cross(q1.axis, q2.axis);

	return r;
}

iQuaternion operator*(const iQuaternion& q, const iVector3f& v)
{
	const float w = -(q.axis.x * v.x) - (q.axis.y * v.y) - (q.axis.z * v.z);
	const float x = (q.w * v.x) + (q.axis.y * v.z) - (q.axis.z * v.y);
	const float y = (q.w * v.y) + (q.axis.z * v.x) - (q.axis.x * v.z);
	const float z = (q.w * v.z) + (q.axis.x * v.y) - (q.axis.y * v.x);

	iQuaternion r;
	r.w = w;
	r.axis = { x,y,z };

	return r;
}