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


