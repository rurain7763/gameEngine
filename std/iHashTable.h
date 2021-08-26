#pragma once

#define DEFAULT_HASHTABLE_SIZE	500

#define LINEAR_PROBING		0
#define QUADRATIC_PROBING	1
#define DOUBLE_PROBING		2
#define HASHTABLE_PROBING	QUADRATIC_PROBING

#include "iType.h"

struct iBucket;

class iHashTable
{
public:
	iHashTable();
	iHashTable(int size);
	virtual ~iHashTable();

	void insert(const char* key, void* data);
	void* at(const char* key);
	void remove(const char* key);

	void*& operator[](const char* key);

private:
	void resize();

	uint64 hashFunc1(const char* key);
	uint64 hashFunc2(const char* key);

	iBucket* linearProbing(uint64 hash);
	iBucket* quadraticProbing(uint64 hash);
	iBucket* doubleHashing(uint64 hash);

private:
	iBucket* data;
	char dummy[6];
	int dummyNum;

public:
	int size;
	int num;
};

struct iBucket
{
	uint64 hash;
	void* data;
};