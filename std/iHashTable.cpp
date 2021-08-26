#include "iHashTable.h"
#include "iStd.h"

iHashTable::iHashTable()
{
	uint32 prime = nextPrime(DEFAULT_HASHTABLE_SIZE);

	data = new iBucket[prime];
	strcpy(dummy, "DUMMY");
	dummyNum = 0;
	memset(data, 0, sizeof(iBucket) * prime);
	size = prime;
	num = 0;
}

iHashTable::iHashTable(int s)
{
	uint32 prime = nextPrime(s);

	data = new iBucket[prime];
	strcpy(dummy, "DUMMY");
	dummyNum = 0;
	memset(data, 0, sizeof(iBucket) * prime);
	size = prime;
	num = 0;
}

iHashTable::~iHashTable()
{
	delete[] data;
}

void iHashTable::insert(const char* key, void* d)
{
	uint64 hash = hashFunc1(key);

#if HASHTABLE_PROBING == LINEAR_PROBING
	iBucket* b = linearProbing(hash);
	b->hash = hash;
	b->data = d;
#elif HASHTABLE_PROBING == QUADRATIC_PROBING
	iBucket* b = quadraticProbing(hash);
	b->hash = hash;
	b->data = d;
#elif HASHTABLE_PROBING == DOUBLE_PROBING
	iBucket* b = doubleHashing(hash);
	b->hash = hash;
	b->data = d;
#endif

	num++;
	if (num + dummyNum >= size * .7f) resize();
}

void* iHashTable::at(const char* key)
{
	if (num == 0) return NULL;

	uint64 hash = hashFunc1(key);

#if HASHTABLE_PROBING == LINEAR_PROBING
	iBucket* b = linearProbing(hash);
	return b->data;
#elif HASHTABLE_PROBING == QUADRATIC_PROBING
	iBucket* b = quadraticProbing(hash);
	return b->data;
#elif HASHTABLE_PROBING == DOUBLE_PROBING
	iBucket* b = doubleSearch(hash);
	return b->data;
#endif
}

void iHashTable::remove(const char* key)
{
	if (num == 0) return;

	uint64 hash = hashFunc1(key);

#if HASHTABLE_PROBING == LINEAR_PROBING
	iBucket* b = linearProbing(hash);
#elif HASHTABLE_PROBING == QUADRATIC_PROBING
	iBucket* b = quadraticProbing(hash);
#elif HASHTABLE_PROBING == DOUBLE_PROBING
	iBucket* b = doubleSearch(key);
#endif

	b->data = dummy;
	dummyNum++;
	num--;

	if ( num + dummyNum >= size * .7f ) resize();
}

void*& iHashTable::operator[](const char* key)
{
	uint64 hash = hashFunc1(key);

#if HASHTABLE_PROBING == LINEAR_PROBING
	iBucket* b = linearProbing(hash);
	return b->data;
#elif HASHTABLE_PROBING == QUADRATIC_PROBING
	iBucket* b = quadraticProbing(hash);
	return b->data;
#elif HASHTABLE_PROBING == DOUBLE_PROBING
	iBucket* b = doubleHashing(hash);
	return b->data;
#endif
}

void iHashTable::resize()
{
	uint32 prime = nextPrime(size);
	iBucket* copy = new iBucket[prime];
	memset(copy, 0, sizeof(iBucket) * prime);
	num = 0;

	for (int i = 0; i < size; i++)
	{
		iBucket* from = &data[i];

		if (from->hash == 0 || from->data == dummy) continue;

#if HASHTABLE_PROBING == QUADRATIC_PROBING
		int idx = from->hash % prime;
		
		iBucket* to = &copy[idx];
		int step = 1;

		while (to->data != NULL)
		{
			idx = (idx + step * step) % size;
			step++;
			to = &copy[idx];
		}

		to->hash = from->hash;
		to->data = from->data;
		num++;
#endif
	}

	delete[] data;
	data = copy;
	size = prime;
	dummyNum = 0;
}

uint64 iHashTable::hashFunc1(const char* key)
{
	//horner's method => when x = 31
	uint64 h = 0;
	int len = strlen(key);

	for (int i = 0; i < len; i++)
		h = ((h << 5) - h) + key[i];
	
	return h;
}

uint64 iHashTable::hashFunc2(const char* key)
{
	//TODO : make double hashing
	int v = 0;

	return v;
}

iBucket* iHashTable::linearProbing(uint64 hash)
{
	uint64 idx = hash % size;
	
	iBucket* b = &data[idx];

	while(b->data != NULL)
	{
		if (b->hash == hash && b->data != dummy) break;

		idx = (idx + 1) % size;
		b = &data[idx];
	}

	return b;
}

iBucket* iHashTable::quadraticProbing(uint64 hash)
{
	uint32 idx = hash % size;

	iBucket* b = &data[idx];
	uint32 step = 1;

	while (b->data != NULL && step <= size)
	{
		if (b->hash == hash && b->data != dummy) break;

		idx = (idx + step * step) % size;
		step++;

		b = &data[idx];
	}

	return b;
}

iBucket* iHashTable::doubleHashing(uint64 hash)
{
	//TODO : make double hashing
	return NULL;
}

