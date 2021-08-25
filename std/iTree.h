#pragma once

#define DEFAULT_HEAP_SIZE	 300

#define LEFT_NODE	0
#define RIGHT_NODE	1
#define ROOT_NODE	2

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

struct iBinaryTreeNode
{
	iBinaryTreeNode* left;
	iBinaryTreeNode* right;

	void* data;
};

class iBinarySearchTree
{
public:
	iBinarySearchTree(CompareMethod equal, CompareMethod min);
	virtual ~iBinarySearchTree();

	void insert(void* v);
	void remove(void* v);

private:
	iBinaryTreeNode* inOrder(void* v);

	iBinaryTreeNode* root;
	CompareMethod equalFunc;
	CompareMethod minFunc;
	
public:
	int num;
	int height;
};