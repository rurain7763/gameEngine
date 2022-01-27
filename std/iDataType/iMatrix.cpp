#include"iMatrix.h"
#include"iStd.h"

iMatrix::iMatrix()
{
	memset(data, 0, sizeof(float) * 4 * 4);
}

iMatrix::iMatrix(const iMatrix& mat)
{
	memcpy(data, mat.data, sizeof(float) * 4 * 4);
}

iMatrix& iMatrix::operator=(const iMatrix& mat)
{
	memcpy(data, mat.data, sizeof(float) * 4 * 4);
	return *this;
}

iMatrix iMatrix::operator*(const iMatrix& mat)
{
	iMatrix r;
	memset(r.data, 0, sizeof(float) * 4 * 4);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			r.data[i][j] = data[i][0] * mat.data[0][j] +
						   data[i][1] * mat.data[1][j] +
						   data[i][2] * mat.data[2][j] +
						   data[i][3] * mat.data[3][j];
		}
	}

	return r;
}

iVector4f iMatrix::operator*(iVector4f& vec)
{
	iVector4f r = {0.f, 0.f, 0.f, 0.f};

	float* rs = (float*)&r;
	float* vs = (float*)&vec;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			rs[i] += data[i][j] * vs[j];
		}
	}

	return r;
}

void iMatrix::frustrum(float fov, float w, float h, float n, float f)
{
	if (n < 0 || f < 0) return;

	data[0][0] = (1.f / itan(fov/2.f)) / (w/h);

	data[1][1] = 1.f / itan(fov/2.f);

	data[2][2] = (- n - f) / (n - f);
	data[2][3] = 2.f * (f * n) / (n - f);

	data[3][2] = 1.f;
	data[3][3] = 0.f;
}

void iMatrix::ortho(float l, float r, float b, float t, float n, float f)
{
	data[0][0] = 2.f / (r - l);
	data[0][3] = -(r + l) / (r - l);

	data[1][1] = 2.f / (t - b);
	data[1][3] = -(t + b) / (t - b);

	data[2][2] = 2.f / (f - n);
	data[2][3] = -(f + n) / (f - n);

	data[3][3] = 1.f;
}

void iMatrix::translate(float x, float y, float z)
{
	data[0][3] = x;
	data[1][3] = y;
	data[2][3] = z;
	data[3][3] = 1.f;
}

void iMatrix::translate(iVector4f vec)
{
	data[0][3] = vec.x;
	data[1][3] = vec.y;
	data[2][3] = vec.z;
	data[3][3] = 1.f;
}

void iMatrix::rotate(int x, int y, int z)
{
	data[0][0] = icos(z) * icos(y);
	data[0][1] = icos(z) * isin(y) * isin(x) - isin(z) * icos(x);
	data[0][2] = icos(z) * isin(y) * icos(x) + isin(z) * isin(x);

	data[1][0] = isin(z) * icos(y);
	data[1][1] = isin(z) * isin(y) * isin(x) + icos(z) * icos(x);
	data[1][2] = isin(z) * isin(y) * icos(x) - icos(z) * isin(x);

	data[2][0] = -isin(y);
	data[2][1] = icos(y) * isin(x);
	data[2][2] = icos(y) * icos(x);
}

void iMatrix::rotate(const iQuaternion& q)
{
	float yy2 = 2.f * q.axis.y * q.axis.y;
	float xy2 = 2.f * q.axis.x * q.axis.y;
	float xz2 = 2.f * q.axis.x * q.axis.z;
	float yz2 = 2.f * q.axis.y * q.axis.z;
	float zz2 = 2.f * q.axis.z * q.axis.z;
	float wz2 = 2.f * q.w * q.axis.z;
	float wy2 = 2.f * q.w * q.axis.y;
	float wx2 = 2.f * q.w * q.axis.x;
	float xx2 = 2.f * q.axis.x * q.axis.x;

	data[0][0] = -yy2 - zz2 + 1.f;
	data[0][1] = xy2 + wz2;
	data[0][2] = xz2 - wy2;

	data[1][0] = xy2 - wz2;
	data[1][1] = -xx2 - zz2 + 1.f;
	data[1][2] = yz2 + wx2;

	data[2][0] = xz2 + wy2;
	data[2][1] = yz2 - wx2;
	data[2][2] = -xx2 - yy2 + 1.f;

	data[3][0] = data[3][1] = data[3][2] = 0;
}

void iMatrix::scale(float x, float y, float z)
{
	data[0][0] = x;
	data[1][1] = y;
	data[2][2] = z;
	data[3][3] = 1.f;
}

void iMatrix::loadIdentity()
{
	memset(data, 0, sizeof(float) *4 *4);
	data[0][0] = 1.f;
	data[1][1] = 1.f;
	data[2][2] = 1.f;
	data[3][3] = 1.f;
}

float* iMatrix::getData()
{
	return &data[0][0];
}


