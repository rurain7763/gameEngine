#include"iString.h"
#include"iStd.h"

iString::iString()
{
	str = new char[DEFAULT_STR_SIZE];
	str[0] = 0;
	len = 0;
	size = DEFAULT_STR_SIZE;
}

iString::iString(uint32 s)
{
	if (s == 0) s = DEFAULT_STR_SIZE;

	str = new char[s];
	str[0] = 0;
	len = 0;
	size = s;
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
	if (!s)
	{
		len = 0;
		size = DEFAULT_STR_SIZE;
	}
	else
	{
		len = strlen(s);
		size = len * 2 + 1;
	}

	str = new char[size];
	memcpy(str, s, sizeof(char) * len);
	str[len] = 0;
}

iString::iString(char c)
{
	len = 1;
	size = 2;
	str = new char[size];
	str[0] = c;
	str[1] = 0;
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

iString& iString::operator=(char c)
{
	if (size < 2)
	{
		size = 2 * 2 + 1;
		delete[] str;
		str = new char[size];
	}

	str[0] = c;
	str[1] = 0;
	len = 1;
	
	return *this;
}

iString& iString::operator+=(const iString& istr)
{
	int newLen = len + istr.len;

	if (newLen >= size)
	{
		size = newLen * 2 + 1;
		char* copy = new char[size];
		memcpy(copy, str, sizeof(char) * len);
		delete[] str;
		str = copy;
	}

	memcpy(&str[len], istr.str, sizeof(char) * istr.len);
	str[newLen] = 0;
	len = newLen;

	return *this;
}

iString& iString::operator+=(const char* s)
{
	int sLen = strlen(s);
	int newLen = len + sLen;

	if (newLen >= size)
	{
		size = newLen * 2 + 1;
		char* copy = new char[size];
		memcpy(copy, str, sizeof(char) * len);
		delete[] str;
		str = copy;
	}

	memcpy(&str[len], s, sizeof(char) * sLen);
	str[newLen] = 0;
	len = newLen;

	return *this;
}

iString& iString::operator+=(char c)
{
	int newLen = len + 1;

	if (newLen >= size)
	{
		size = newLen * 2 + 1;
		char* copy = new char[size];
		memcpy(copy, str, sizeof(char) * len);
		delete[] str;
		str = copy;
	}

	str[len] = c;
	str[newLen] = 0;
	len = newLen;

	return *this;
}

char& iString::operator[](int idx)
{
	if (idx >= len)
	{
		char* dummy = NULL;
		return *dummy;
	}

	return str[idx];
}

void iString::resize(uint32 s)
{
	if (s)
	{
		if (len >= s) len = s - 1;

		char* copy = new char[s];
		memcpy(copy, str, sizeof(char) * len);
		copy[len] = 0;
		delete[] str;
		str = copy;

		size = s;
	}
}

void iString::clear()
{
	memset(str, 0, sizeof(char) * len);
	len = 0;
}

bool iString::empty()
{
	return !len;
}

void iString::shrink_to_fit()
{
	size = len + 1;
	char* copy = new char[size];
	memcpy(copy, str, sizeof(char) * len);
	copy[len] = 0;
	delete[] str;
	str = copy;
}

char& iString::at(int idx)
{
	if (idx >= len)
	{
		char* dummy = NULL;
		return *dummy;
	}
	
	return str[idx];
}

iString& iString::append(const iString& istr, uint32 subPos, uint32 subLen)
{
	int appendLen = subLen ? subLen : istr.len;
	int newLen = len + appendLen;

	if (newLen >= size)
	{
		size = newLen * 2 + 1;
		char* copy = new char[size];
		memcpy(copy, str, sizeof(char) * len);
		delete[] str;
		str = copy;
	}

	memcpy(&str[len], &istr.str[subPos], sizeof(char) * appendLen);
	str[newLen] = 0;
	len = newLen;

	return *this;
}

iString& iString::append(const char* s, uint32 num)
{
	int sLen = strlen(s);
	int appendLen = sLen * num;
	int newLen = len + appendLen;

	if (newLen >= size)
	{
		size = newLen * 2 + 1;
		char* copy = new char[size];
		memcpy(copy, str, sizeof(char) * len);
		delete[] str;
		str = copy;
	}

	for(int i = 0 ; i < num; i++) 
		memcpy(&str[len + sLen * i], s, sizeof(char) * sLen);
	str[newLen] = 0;
	len = newLen;

	return *this;
}

iString& iString::append(char c, uint32 num)
{
	int newLen = len + num;

	if (newLen >= size)
	{
		size = newLen * 2 + 1;
		char* copy = new char[size];
		memcpy(copy, str, sizeof(char) * len);
		delete[] str;
		str = copy;
	}

	for (int i = 0; i < num; i++) str[len + i] = c;
	str[newLen] = 0;
	len = newLen;

	return *this;
}

