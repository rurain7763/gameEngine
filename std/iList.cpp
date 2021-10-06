#include"iList.h"
#include"iStd.h"

iList::iList()
{
	head = NULL;
	num = 0;
}

iList::~iList()
{
	iListNode* node = head;

	while (node)
	{
		iListNode* next = node->next;
		delete node;
		node = next;
	}
}

void iList::clear()
{
	iListNode* node = head;

	while (node)
	{
		iListNode* next = node->next;
		delete node;
		node = next;
	}

	head = NULL;
	num = 0;
}

void iList::insert(int idx, void* data)
{
	if (idx > num - 1)
	{
		if (head)
		{
			iListNode* node = head;

			while (node->next)
			{
				node = node->next;
			}

			iListNode* newNode = new iListNode;
			newNode->data = data;
			newNode->next = NULL;
			node->next = newNode;
		}
		else
		{
			iListNode* newNode = new iListNode;
			newNode->data = data;
			newNode->next = NULL;
			head = newNode;
		}
	}
	else
	{
		iListNode* prev = head;
		int prevIdx = idx - 1;

		for (int i = 0; i < prevIdx; i++)
		{
			prev = prev->next;
		}

		if (prevIdx == -1)
		{
			iListNode* newNode = new iListNode;
			newNode->data = data;
			newNode->next = head;
			head = newNode;
		}
		else
		{
			iListNode* newNode = new iListNode;
			newNode->data = data;
			newNode->next = prev->next;
			prev->next = newNode;
		}	
	}

	num++;
}

void iList::erase(int idx)
{
	if (idx > num - 1) idx = num - 1;

	iListNode* prev = head;
	int prevIdx = idx - 1;

	for (int i = 0; i < prevIdx; i++)
	{
		prev = prev->next;
	}

	if (prevIdx == -1)
	{
		iListNode* newHead = head->next;
		delete head;
		head = newHead;
	}
	else
	{
		iListNode* targetNode = prev->next;
		prev->next = targetNode->next;
		delete targetNode;
	}

	num--;
}

void iList::push_back(void* data)
{
	if (head)
	{
		iListNode* node = head;

		while (node->next)
		{
			node = node->next;
		}

		iListNode* newNode = new iListNode;
		newNode->data = data;
		newNode->next = NULL;
		node->next = newNode;
	}
	else
	{
		iListNode* newNode = new iListNode;
		newNode->data = data;
		newNode->next = NULL;
		head = newNode;
	}

	num++;
}

void iList::push_front(void* data)
{
	iListNode* newNode = new iListNode;
	newNode->data = data;
	newNode->next = head;
	head = newNode;

	num++;
}

void*& iList::operator[](int idx)
{
	void* dummy = NULL;

	if (idx > num - 1) return dummy;

	iListNode* node = head;

	for (int i = 0; i < idx; i++)
	{
		node = node->next;
	}

	return node->data;
}
