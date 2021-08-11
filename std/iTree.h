#pragma once

#define DEFAULT_HEAP_SIZE	 20

typedef bool Flag;
typedef bool (*iHeapCompareMethod)(void* left, void* right);

struct iHeapData
{
	void* data;
};

class iHeap
{
public:
	iHeap(iHeapCompareMethod compare);
	virtual ~iHeap();

	void insert(void* data);
	void* pop();

private:
	iHeapCompareMethod compare;
	int size;
	iHeapData* data;

public:
	int dataNum;
	int height;
};
