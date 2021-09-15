#pragma once

#define DEFAULT_STR_SIZE 20

#include "iType.h"

class iString
{
public:
	iString();
	iString(uint32 size);
	iString(const iString& istr);
	iString(const char* str, ...);
	iString(char c);
	virtual ~iString();

	iString& operator=(const iString& istr);
	iString& operator=(const char* str);
	iString& operator=(char c);

	iString& operator+=(const iString& istr);
	iString& operator+=(const char* str);
	iString& operator+=(char c);

	char& operator[](int idx);

	void resize(uint32 size);
	void clear();
	bool empty();
	void shrink_to_fit();
	char& at(int idx);

public:
	char* str;
	int size;
	int len;
};

