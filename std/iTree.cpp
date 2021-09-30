#include"iTree.h"
#include"iStd.h"

iHeap::iHeap(CompareMethod c)
{
	compare = c;
	size = DEFAULT_HEAP_SIZE;
	data = new iHeapData[DEFAULT_HEAP_SIZE];
	dataNum = 0;
	height = 0;
}

iHeap::~iHeap()
{
	delete[] data;
}

void iHeap::insert(void* d)
{
	if (dataNum >= size)
	{
		iHeapData* copy = new iHeapData[size += DEFAULT_HEAP_SIZE];
		memcpy(copy, data, sizeof(iHeapData) * dataNum);
		delete[] data;
		data = copy;
	}

	data[dataNum].data = d;

	int idx = dataNum;
	int parentIdx;

	while (idx != 0)
	{
		if (idx % 2) parentIdx = idx / 2;
		else parentIdx = idx / 2 - 1;

		if (compare(data[parentIdx].data, data[idx].data))
		{
			swap(&data[parentIdx], &data[idx], sizeof(iHeapData));
			idx = parentIdx;
		}
		else
		{
			break;
		}
	}

	dataNum++;
	height = floor(log2(dataNum)) + 1;
}

void* iHeap::pop()
{
	if (dataNum < 1) return NULL;

	void* r = data[0].data;

	dataNum--;
	data[0] = data[dataNum];
	data[dataNum].data = NULL;
	height = floor(log2(dataNum)) + 1;

	int idx = 0;
	int curHeight = 1;
	
	while (curHeight < height)
	{
		int left = idx * 2 + 1;
		int right = idx * 2 + 2;

		if (left >= dataNum)
		{
			break;
		}
		else if (right >= dataNum)
		{
			if (compare(data[idx].data, data[left].data))
			{
				swap(&data[left], &data[idx], sizeof(iHeapData));
				idx = left;
			}
		}
		else
		{
			if (compare(data[right].data, data[left].data) &&
				compare(data[idx].data, data[left].data))
			{
				swap(&data[left], &data[idx], sizeof(iHeapData));
				idx = left;
			}
			else if (compare(data[left].data, data[right].data) &&
					 compare(data[idx].data, data[right].data))
			{
				swap(&data[right], &data[idx], sizeof(iHeapData));
				idx = right;
			}
			else
			{
				break;
			}
		}

		curHeight++;
	}

	return r;
}

iBinarySearchTree::iBinarySearchTree(CompareMethod e, MinMethod m)
{
	root = NULL;
	equalFunc = e;
	minFunc = m;
	num = 0;
	height = 0;
}

iBinarySearchTree::~iBinarySearchTree()
{
	iQueue q(num == 0 ? num + 1 : num);
	q.push(root);

	while (!q.empty())
	{
		iBinaryTreeNode* n = (iBinaryTreeNode*)q.pop();

		if(n->left) q.push(n->left);
		if(n->right) q.push(n->right);

		delete n;
	}
}

void iBinarySearchTree::insert(void* v)
{
	iBinaryTreeNode* n = root;
	int flag = ROOT_NODE;

	while (n)
	{
		if (equalFunc(n->data, v)) return;

		if (minFunc(n->data, v) == n->data) //right
		{
			if (!n->right)
			{
				flag = RIGHT_NODE;
				break;
			}
			n = n->right;
		}
		else //left
		{
			if (!n->left)
			{
				flag = LEFT_NODE;
				break;
			}

			n = n->left;
		}
	}

	if (flag == LEFT_NODE)
	{
		n->left = new iBinaryTreeNode;
		n->left->data = v;
		n->left->left = NULL;
		n->left->right = NULL;	
	}
	else if (flag == RIGHT_NODE)
	{
		n->right = new iBinaryTreeNode;
		n->right->data = v;
		n->right->left = NULL;
		n->right->right = NULL;
	}
	else
	{
		root = new iBinaryTreeNode;
		root->data = v;
		root->left = NULL;
		root->right = NULL;
	}

	num++;
	height = floor(log2(num)) + 1;
}

void iBinarySearchTree::remove(void* v)
{
	iBinaryTreeNode* parent = root;
	iBinaryTreeNode* curr = root;

	while (curr)
	{
		if (equalFunc(curr->data, v))
		{
			break;
		}

		parent = curr;
		
		if (minFunc(curr->data, v) == curr->data) curr = curr->right;	
		else curr = curr->left;
	}

	if (!curr) return; // no such data

	if (curr->left && curr->right)
	{
		// 2 child
		iBinaryTreeNode* min = curr->right;
		parent = curr;

		while (min->left != NULL)
		{
			parent = min;
			min = min->left;
		}

		if (min == curr->right)
		{
			curr->data = min->data;
			curr->right = min->right;
			delete min;
		}
		else if (min->right == NULL)
		{
			curr->data = min->data;
			parent->left = NULL;
			delete min;
		}
		else if (min->right)
		{
			curr->data = min->data;
			parent->left = min->right;
			delete min;
		}
	}
	else if (curr->left || curr->right)
	{
		// 1 child
		iBinaryTreeNode* child = curr->left ? curr->left : curr->right;

		if (curr == root) root = child;
		else if (parent->left == curr) parent->left = child;
		else parent->right = child;
		delete curr;
	}
	else
	{
		// no child
		if (parent->left == curr) parent->left = NULL;
		else parent->right = NULL;
		delete curr;
	}

	num--;
	height = floor(log2(num)) + 1;
}

bool iBinarySearchTree::find(void* v)
{
	iBinaryTreeNode* curr = root;

	while (curr)
	{
		if (equalFunc(curr->data, v)) return true;
		
		if (minFunc(curr->data, v) == curr->data) curr = curr->right;
		else curr = curr->left;
	}

	return false;
}

iBinaryTreeNode* iBinarySearchTree::inOrder(void* v)
{
	iStack stk(num);
	iBinaryTreeNode* n = root;

	while (1)
	{
		while (n)
		{
			stk.push(n);
			n = n->left;
		}

		if (!stk.empty())
		{
			iBinaryTreeNode* tn = (iBinaryTreeNode*)stk.pop();

			if (equalFunc(tn->data, v))
			{
				n = tn;
				break;
			}

			n = tn->right;
		}
		else break;
	}

	return n;
}
