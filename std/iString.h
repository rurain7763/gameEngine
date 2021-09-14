#pragma once

#define DEFAULT_STR_SIZE 100

class iString
{
public:
	iString();
	iString(int size);
	iString(const iString& istr);
	iString(const char* str);
	iString(char* str);
	virtual ~iString();

	iString& operator=(const iString& istr);
	iString& operator=(const char* str);
	iString& operator=(char* str);

	void resize(int size);

private:
	int size;

public:
	char* str;
	int len;
};

