#pragma once

struct iVector2f
{
	iVector2f& normalized();
	float magnitudeSqrt();

	float x, y;
};

iVector2f operator-(const iVector2f& v1, const iVector2f& v2);

struct iVector3f
{
	iVector3f operator+(const iVector3f& v) const;
	iVector3f operator*(const float v) const;

	iVector3f& operator+=(const iVector3f& v);
	iVector3f& operator-=(const iVector3f& v);

	iVector3f operator-() const;

	float dot(const iVector3f& v);
	iVector3f cross(const iVector3f& v);
	void rotate(iVector3f axis, float angle);

	static float dot(const iVector3f& v1, const iVector3f& v2);
	static iVector3f cross(const iVector3f& v1, const iVector3f& v2);
	iVector3f& normalized();

	float x, y, z;
};

struct iVector4f
{
	float x, y, z, w;
};

struct iVertexPCU
{
	iVector4f position;
	iVector4f color;
	iVector2f uv;
};

struct iVertexPNU
{
	iVector3f position;
	iVector3f normal;
	iVector2f uv;
};





