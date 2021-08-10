#pragma once

#define MIN_HEAP			  0	
#define MAX_HEAP			  1
#define DEFAULT_HEAP_SIZE	 20
#define NONE_DATA			 -1

typedef bool Flag;

class iHeap
{
public:
	iHeap(Flag flag);
	virtual ~iHeap();

	void insert(unsigned int data);
	int pop();

private:
	Flag flag;
	int size;
	int* data;

public:
	int dataNum;
	int height;
};
