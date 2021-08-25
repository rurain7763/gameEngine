#pragma once

struct iStackData;

class iStack
{
public:
	iStack(int size);
	virtual ~iStack();

	void push(void* v);
	void* pop();
	bool empty();

public:
	iStackData* data;
	int top;

public:
	int size;
};

struct iStackData
{
	void* data;
};