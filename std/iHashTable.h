#pragma once

#define DEFAULT_HASHTABLE_SIZE	1000

#define LINEAR_PROBING		0
#define QUADRATIC_PROBING	1
#define DOUBLE_PROBING		2
#define HASHTABLE_PROBING	LINEAR_PROBING

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
	uint64 hashFunc1(const char* key);
	uint64 hashFunc2(const char* key);

	iBucket* linearProbing(const char* key);
	iBucket* linearSearch(const char* key);

	iBucket* quadraticProbing(const char* key);
	iBucket* quadraticSearch(const char* key);

	iBucket* doubleHashing(const char* key);
	iBucket* doubleSearch(const char* key);

private:
	iBucket* data;

public:
	int size;
	int num;
};

struct iBucket
{
	const char* key;
	void* data;
};