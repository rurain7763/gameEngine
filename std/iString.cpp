#include"iString.h"
#include"iStd.h"

iString::iString()
{
	str = new char[DEFAULT_STR_SIZE];
	str[0] = 0;
	len = 0;
	size = DEFAULT_STR_SIZE;
}

iString::iString(int s)
{
	if (s == 0)
	{
		str = new char[DEFAULT_STR_SIZE];
		str[0] = 0;
		len = 0;
		size = DEFAULT_STR_SIZE;
	}
	else
	{
		str = new char[s];
		str[0] = 0;
		len = 0;
		size = s;
	}
}

iString::iString(const iString& istr)
{
	len = istr.len;
	size = len * 2 + 1;

	str = new char[size];
	memcpy(str, istr.str, sizeof(char) * len);
	str[len] = 0;
}

iString::iString(const char* s)
{
	len = strlen(s);
	size = len * 2 + 1;

	str = new char[size];
	memcpy(str, s, sizeof(char) * len);
	str[len] = 0;
}

iString::iString(char* s)
{
	len = strlen(s);
	size = len * 2 + 1;

	str = new char[size];
	memcpy(str, s, sizeof(char) * len);
	str[len] = 0;
}

iString::~iString()
{
	delete[] str;
}

iString& iString::operator=(const iString& istr)
{
	len = istr.len;

	if (len >= size)
	{
		delete[] str;
		size = len * 2 + 1;
		str = new char[size];
	}

	memcpy(str, istr.str, sizeof(char) * len);
	str[len] = 0;

	return *this;
}

iString& iString::operator=(const char* s)
{
	len = strlen(s);

	if (len >= size)
	{
		delete[] str;
		size = len * 2 + 1;
		str = new char[size];
	}

	memcpy(str, s, sizeof(char) * len);
	str[len] = 0;

	return *this;
}

iString& iString::operator=(char* s)
{
	len = strlen(s);

	if (len >= size)
	{
		delete[] str;
		size = len * 2 + 1;
		str = new char[size];
	}

	memcpy(str, s, sizeof(char) * len);
	str[len] = 0;

	return *this;
}

void iString::resize(int s)
{
	if (s == 0) return;

	if (len >= s) len = s - 1;

	char* copy = new char[s];
	memcpy(copy, str, sizeof(char) * len);
	copy[len] = 0;
	delete[] str;
	str = copy;

	size = s;
}
