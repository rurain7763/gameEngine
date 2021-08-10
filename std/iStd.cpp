#include"iStd.h"

char* readFile(const char* path)
{
	FILE* file = fopen(path, "rb");

	if (!file) return NULL;

	fseek(file, 0, SEEK_END);
	int len = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = new char[len+1];
	fread(buffer, sizeof(char), len, file);
	buffer[len] = 0;

	fclose(file);

	return buffer;
}

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

void swap(int& left, int& right)
{
	int copy = left;
	left = right;
	right = copy;
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