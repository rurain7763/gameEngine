#include"iStd.h"

float isin(float degree)
{
	return sin(ToRadian(degree));
}

float icos(float degree)
{
	if (degree == 90) return 0;
	return cos(ToRadian(degree));
}

float itan(float degree)
{
	return tan(ToRadian(degree));
}

int random()
{
	return rand();
}

unsigned int nextPow2(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 16;
	v++;

	return v;
}

unsigned int nextPrime(unsigned int v)
{
	while (v <= 0xffffffff)
	{
		if (isPrime(++v)) return v;
	}

	return 0;
}

bool isPrime(unsigned int v)
{
	if (v == 1) return false;

	for (int i = 2 ; i * i <= v; i++)
	{
		if (!(v % i)) return false;
	}

	return true;
}

char* readFile(const char* path)
{
	FILE* file = fopen(path, "rb");

	if (!file) return NULL;

	fseek(file, 0, SEEK_END);
	int len = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = new char[len + 1];
	fread(buffer, sizeof(char), len, file);
	buffer[len] = 0;

	fclose(file);

	return buffer;
}

void bubbleSort(CompareMethod m, void* data, int elementSize, int num)
{
	char* d = (char*)data;

	for (int i = num - 1; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			int leftIdx = j * elementSize;
			int rightIdx = (j + 1) * elementSize;

			if (m(&d[leftIdx], &d[rightIdx]))
			{
				swap(&d[leftIdx], &d[rightIdx], elementSize);
			}
		}
	}
}

char* istrcpy(const char* str)
{
	int len = strlen(str);
	char* r = new char[len + 1];
	memcpy(r, str, sizeof(char) * len);
	r[len] = 0;

	return r;
}

void swap(void* left, void* right, int argSize)
{
	char* copy = new char[argSize];
	memcpy(copy, left, argSize);
	memcpy(left, right, argSize);
	memcpy(right, copy, argSize);

	delete[] copy;
}

void printBit(unsigned int v)
{
	unsigned int b = 0x80000000;

	for (int i = 0; i < 32; i++)
	{
		int r = v & b;
		if (r) printf("1");
		else printf("0");
		b >>= 1;

		if (i % 4 == 3) printf(" ");
	}

	printf("\n");
}

