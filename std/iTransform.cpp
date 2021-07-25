#include"iTransform.h"
#include"iStd.h"

iTransform::iTransform()
{
	finalMat.loadIdentity();
	memset(&positionV, 0, sizeof(iVector3f));
	memset(&rotationV, 0, sizeof(iVector3f));
	scaleV.x = 1.f;
	scaleV.y = 1.f;
	scaleV.z = 1.f;	
}

void iTransform::translate(float x, float y, float z)
{
	positionV.x = x;
	positionV.y = y;
	positionV.z = z;
}

void iTransform::rotate(float xAxis, float yAxis, float zAxis)
{
	rotationV.x = xAxis;
	rotationV.y = yAxis;
	rotationV.z = zAxis;
}

void iTransform::scale(float x, float y, float z)
{
	scaleV.x = x;
	scaleV.y = y;
	scaleV.z = z;
}

iMatrix iTransform::getMatrix()
{
	iMatrix s;
	s.loadIdentity();
	s.scale(scaleV.x, scaleV.y, scaleV.z);

	iMatrix t;
	t.loadIdentity();
	t.translate(positionV.x, positionV.y, positionV.z);

	iMatrix r;
	r.loadIdentity();

#if ROTATION_METHOD
	iQuaternion qx({ 1,0,0 }, rotationV.x);
	iQuaternion qy({ 0,1,0 }, rotationV.y);
	iQuaternion qz({ 0,0,1 }, rotationV.z);

	iQuaternion q = qx * qy * qz;

	r.rotate(q);	
#else
	r.rotate(rotationV.x, rotationV.y, rotationV.z);
#endif

	return t * r * s;
}
