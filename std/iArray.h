#pragma once

#define DEFAULT_ARRAY_SIZE			  1000
#define DUMY_DATA			0xffffffff / 2

struct iArrayData;

class iArray
{
public:
	iArray();
	iArray(int size);
	virtual ~iArray();

	void insert(int idx, void* data);
	void erase(int idx);
	void push_back(void* data);
	void* pop_back();
	void resize(int size);

	void*& operator[](int idx);

private:
	unsigned int dumy;
	int size;

public:
	iArrayData* data;
	int dataNum;
};

struct iArrayData
{
	void* data;
};