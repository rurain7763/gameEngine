#include"iTree.h"
#include"iStd.h"

iHeap::iHeap(Flag f)
{
	flag = f;
	data = new int[DEFAULT_HEAP_SIZE];
	size = DEFAULT_HEAP_SIZE;
	memset(data, NONE_DATA, sizeof(int) * DEFAULT_HEAP_SIZE);
	dataNum = 0;
	height = 0;
}

iHeap::~iHeap()
{
	delete[] data;
}

void iHeap::insert(unsigned int d)
{
	if (dataNum >= size)
	{
		int* copy = new int[size += DEFAULT_HEAP_SIZE];
		memset(copy, NONE_DATA, sizeof(int) * size);
		memcpy(copy, data, sizeof(int) * dataNum);
		delete[] data;
		data = copy;
	}

	data[dataNum] = d;

	int idx = dataNum;

	while (idx != 0)
	{
		if (idx % 2)
		{
			// left
			int parentIdx = idx / 2;

			if (flag)
			{
				//max
				if (data[parentIdx] < data[idx])
				{
					swap(data[parentIdx], data[idx]);
					idx = parentIdx;
				}
				else
				{
					break;
				}
			}
			else
			{
				//min
				if (data[parentIdx] > data[idx])
				{
					swap(data[parentIdx], data[idx]);
					idx = parentIdx;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			// right
			int parentIdx = idx / 2 - 1;

			if (flag)
			{
				//max
				if (data[parentIdx] < data[idx])
				{
					swap(data[parentIdx], data[idx]);
					idx = parentIdx;
				}
				else
				{
					break;
				}
			}
			else
			{
				//min
				if (data[parentIdx] > data[idx])
				{
					swap(data[parentIdx], data[idx]);
					idx = parentIdx;
				}
				else
				{
					break;
				}
			}
		}
	}

	dataNum++;
	height = floor(log2(dataNum)) + 1;
}

int iHeap::pop()
{
	if (dataNum < 1) return NONE_DATA;

	int r = data[0];

	dataNum--;
	data[0] = data[dataNum];
	data[dataNum] = NONE_DATA;
	height = floor(log2(dataNum)) + 1;

	int idx = 0;
	int curHeight = 1;
	
	while (curHeight < height)
	{
		int left = idx * 2 + 1;
		int right = idx * 2 + 2;

		if (flag)
		{
			//max
			if (data[left] == NONE_DATA)
			{
				break;
			}
			else if (data[right] == NONE_DATA)
			{
				if (data[left] > data[idx])
				{
					swap(data[left], data[idx]);
					idx = left;
				}
			}
			else
			{
				if (data[left] > data[right] &&
					data[left] > data[idx])
				{
					swap(data[left], data[idx]);
					idx = left;
				}
				else if (data[left] < data[right] &&
						 data[right] > data[idx])
				{
					swap(data[right], data[idx]);
					idx = right;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			//min
			if (data[left] == NONE_DATA)
			{
				break;
			}
			else if (data[right] == NONE_DATA)
			{
				if (data[left] < data[idx])
				{
					swap(data[left], data[idx]);
					idx = left;
				}
			}
			else
			{
				if (data[left] < data[right] &&
					data[left] < data[idx])
				{
					swap(data[left], data[idx]);
					idx = left;
				}
				else if (data[left] > data[right] &&
						 data[right] < data[idx])
				{			
					swap(data[right], data[idx]);
					idx = right;
				}
				else
				{
					break;
				}
			}
		}

		curHeight++;
	}

	return r;
}
