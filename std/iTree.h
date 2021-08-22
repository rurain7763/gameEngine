#pragma once

#define DEFAULT_HEAP_SIZE	 300

#include "iType.h"

struct iHeapData
{
	void* data;
};

class iHeap
{
public:
	iHeap(CompareMethod compare);
	virtual ~iHeap();

	void insert(void* data);
	void* pop();

private:
	CompareMethod compare;
	int size;
	iHeapData* data;

public:
	int dataNum;
	int height;
};
