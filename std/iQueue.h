#pragma once

struct iQueueData;

class iQueue
{
public:
	iQueue(int size);
	virtual ~iQueue();

	void push(void* v);
	void* pop();
	bool empty();

public:
	iQueueData* data;
	int num;
	int front;
	int back;

public:
	int size;
};

struct iQueueData
{
	void* data;
};