#pragma once

struct iListNode;

class iList
{
public:
	iList();
	virtual ~iList();

	void clear();
	void insert(int idx, void* data);
	void erase(int idx);
	void push_back(void* data);
	void push_front(void* data);

	void*& operator[](int idx);

private:
	iListNode* head;

public:
	int num;
};

struct iListNode
{
	void* data;
	iListNode* next;
};

