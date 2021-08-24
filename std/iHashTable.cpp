#include "iHashTable.h"
#include "iStd.h"

iHashTable::iHashTable()
{
	data = new iBucket[DEFAULT_HASHTABLE_SIZE];
	memset(data, 0, sizeof(iBucket) * DEFAULT_HASHTABLE_SIZE);
	size = DEFAULT_HASHTABLE_SIZE;
	num = 0;
}

iHashTable::iHashTable(int s)
{
	data = new iBucket[s];
	memset(data, 0, sizeof(iBucket) * s);
	size = s;
	num = 0;
}

iHashTable::~iHashTable()
{
	delete[] data;
}

void iHashTable::insert(const char* key, void* data)
{
	if (num >= size) return;

	uint64 hash = hashFunc1(key);

#if HASHTABLE_PROBING == LINEAR_PROBING
	iBucket* b = linearProbing(key);
	b->key = key;
	b->data = data;
#elif HASHTABLE_PROBING == QUADRATIC_PROBING
	iBucket* b = quadraticProbing(key);
	b->key = key;
	b->data = data;
#elif HASHTABLE_PROBING == DOUBLE_PROBING
	iBucket* b = doubleHashing(key);
	b->key = key;
	b->data = data;
#endif

	num++;
}

void* iHashTable::at(const char* key)
{
	if (num == 0) return NULL;

	uint64 hash = hashFunc1(key);

#if HASHTABLE_PROBING == LINEAR_PROBING
	iBucket* b = linearSearch(key);
	return b->data;
#elif HASHTABLE_PROBING == QUADRATIC_PROBING
	iBucket* b = quadraticSearch(key);
	return b->data;
#elif HASHTABLE_PROBING == DOUBLE_PROBING
	iBucket* b = doubleSearch(key);
	return b->data;
#endif
}

void iHashTable::remove(const char* key)
{
}

void*& iHashTable::operator[](const char* key)
{
	uint64 hash = hashFunc1(key);

#if HASHTABLE_PROBING == LINEAR_PROBING
	iBucket* b = linearSearch(key);
	return b->data;
#elif HASHTABLE_PROBING == QUADRATIC_PROBING
	iBucket* b = quadraticSearch(key);
	return b->data;
#elif HASHTABLE_PROBING == DOUBLE_PROBING
	iBucket* b = doubleSearch(key);
	return b->data;
#endif
}

uint64 iHashTable::hashFunc1(const char* key)
{
	int len = strlen(key);
	int v = 0;
	if (len == 1) v = key[0];
	else v = key[0] + key[len - 1];

	return v % size;
}

uint64 iHashTable::hashFunc2(const char* key)
{
	int v = 0;

	return v % size;
}

iBucket* iHashTable::linearProbing(const char* key)
{
	uint64 hash = hashFunc1(key);
	int start = hash;

	do
	{
		start %= size;

		iBucket* b = &data[start];
		if (b->data == NULL) return b;

		start++;
	} while (start != hash);

	return NULL;
}

iBucket* iHashTable::linearSearch(const char* key)
{
	uint64 hash = hashFunc1(key);
	int start = hash;

	do
	{
		start %= size;

		iBucket* b = &data[start];
		if (b->key == key) return b;
		
		start++;
	} while (start != hash);

	return NULL;
}

iBucket* iHashTable::quadraticProbing(const char* key)
{
	uint64 hash = hashFunc1(key);
	int start = hash;
	int step = 1;

	do
	{
		start %= size;

		iBucket* b = &data[start];
		if (b->data == NULL) return b;

		start += step * step;
		step++;
	} while (step <= size);

	return NULL;
}

iBucket* iHashTable::quadraticSearch(const char* key)
{
	uint64 hash = hashFunc1(key);
	int start = hash;
	int step = 1;

	do
	{
		start %= size;

		iBucket* b = &data[start];
		if (b->key == key) return b;

		start += step * step;
		step++;
	} while (step <= size);

	return NULL;
}

iBucket* iHashTable::doubleHashing(const char* key)
{
	uint64 hash;

	do
	{
		
	} while (1);

	return NULL;
}

iBucket* iHashTable::doubleSearch(const char* key)
{
	uint64 hash;

	do
	{

	} while (1);

	return NULL;
}
