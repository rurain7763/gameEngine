#include"iCamera.h"
#include"iStd.h"

iCamera::iCamera(iSize& ds, iVector3f pos)
{
	position = pos;
	lookAt = { 0, 0, 1 };
	up = { 0, 1, 0 };
	speed = DEFAULT_CAMERA_SPEED;
	sensitive = DEFAULT_CAMERA_SENSITIVE;

	prevMp = { ds.width * .5f, ds.height * .5f };
	hAngle = ToDegree(asin(lookAt.z));
	vAngle = -ToDegree(asin(lookAt.y));

	if (lookAt.z >= 0.f)
	{
		if (lookAt.x >= 0.f)
		{
			hAngle = 360.f - hAngle;
		}
		else
		{
			hAngle = 180.f + hAngle;
		}
	}
	else
	{
		if (lookAt.x >= 0.f)
		{
			hAngle *= -1;
		}
		else
		{
			hAngle = 180.f + hAngle;
		}
	}
}

iCamera::~iCamera()
{
}

bool iCamera::onKey(int key, float dt)
{
	bool in = false;
	iVector3f dir = {0,0,0};

	if (key & KEY_w)
	{
		dir += lookAt;
		in = true;
	}
	else if (key & KEY_s)
	{
		dir -= lookAt;
		in = true;
	}

	if (key & KEY_a)
	{
		iVector3f right = up.cross(lookAt);
		dir -= right;
		in = true;
	}
	else if (key & KEY_d)
	{
		iVector3f right = up.cross(lookAt);
		dir += right;
		in = true;
	}

	if (in)
	{
		dir.normalized();
		position += dir * speed * dt;
	}

	return in;
}

void iCamera::onMouse(iVector2f mousePos, float dt)
{
	iVector2f delta = mousePos - prevMp;
	
	if (delta.magnitudeSqrt() < MOUSE_DELTA_MAX_DISTANSE)
	{
		hAngle += delta.x * sensitive;
		if (hAngle < 0.f) hAngle = 360.f;
		else if (hAngle > 360.f) hAngle = 0.f;
		
		vAngle += delta.y * sensitive;
		vAngle = max(min(vAngle, 90.f), -90.f);
	}

	iVector3f yAxis = { 0.f, 1.f, 0.f };
	iVector3f look = { 1.f, 0.f, 0.f };
	look.rotate(yAxis, hAngle);

	iVector3f right = yAxis.cross(look);
	look.rotate(right, vAngle);

	lookAt = look;
	up = look.cross(right);

	prevMp = mousePos;
}

void iCamera::setLookAt(iVector3f lookPos)
{
	iVector3f up = { 0.f, 1.f, 0.f };
	iVector3f look = { 1.f, 0.f, 0.f };

	iVector3f dir = lookPos - position;
	dir = dir.normalized();

	hAngle = ToDegree(asin(dir.z));
	vAngle = -ToDegree(asin(dir.y));

	if (dir.z >= 0.f)
	{
		if (dir.x >= 0.f)
		{
			hAngle = 360.f - hAngle;
		}
		else
		{
			hAngle = 180.f + hAngle;
		}
	}
	else
	{
		if (dir.x >= 0.f)
		{
			hAngle *= -1;
		}
		else
		{
			hAngle = 180.f + hAngle;
		}
	}

	look.rotate(up, hAngle);

	iVector3f right = up.cross(look);
	look.rotate(right, vAngle);

	lookAt = look;
	this->up = look.cross(right);
}

iMatrix iCamera::getMatrix()
{
	iMatrix rot;
	rot.loadIdentity();

	iVector3f N = lookAt.normalized();
	iVector3f V = up.normalized();
	iVector3f U = up.cross(N);

	rot.data[0][0] = U.x;
	rot.data[0][1] = U.y;
	rot.data[0][2] = U.z;

	rot.data[1][0] = V.x;
	rot.data[1][1] = V.y;
	rot.data[1][2] = V.z;

	rot.data[2][0] = N.x;
	rot.data[2][1] = N.y;
	rot.data[2][2] = N.z;

	iMatrix trans;
	trans.loadIdentity();
	trans.translate(-position.x, -position.y, -position.z);

	return rot * trans;
}
