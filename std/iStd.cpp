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

char* toString(int v)
{
	bool negative = v < 0;

	v = abs(v);
	int digit = log10(v) + 1;
	char* r = new char[digit + 1];
	int place = pow(10, digit - 1);

	for (int i = 0; i < digit; i++)
	{
		char c = (v / place) + '0';
		r[i] = c;

		v %= place;
		place /= 10;
	}

	r[digit] = 0;

	if (negative)
	{
		char* copy = new char[digit + 2];
		copy[0] = '-';
		memcpy(&copy[1], r, sizeof(char) * digit);
		copy[digit + 1] = 0;
		delete[] r;
		r = copy;
	}

	return r;
}

char* toString(float v)
{
	return NULL;
}

void ivsprintf(char* buff, const char* s, va_list ap)
{
	int idx = 0;
	int len = strlen(s);

	for (int i = 1; i < len; i++)
	{
		if (s[i - 1] == '%')
		{
			switch (s[i])
			{
			case 'd':
			{
				int v = va_arg(ap, int);
				char* vStr = toString(v);
				int l = strlen(vStr);
				memcpy(&buff[idx], vStr, sizeof(char) * l);
				delete[] vStr;
				idx += l;
				i++;
				break;
			}
			case 's':
			{
				char* v = va_arg(ap, char*);
				int l = strlen(v);
				memcpy(&buff[idx], v, sizeof(char) * l);
				idx += l;
				i++;
				break;
			}
			case 'f':
			{
				break;
			}
			case '%':
			{
				//TODO
				int exception = 0;
				break;
			}
			default:
				//TODO
				int exception = 0;
				break;
			}
		}
		else
		{
			buff[idx] = s[i - 1];
			idx++;
		}
	}

	buff[idx] = 0;
}
