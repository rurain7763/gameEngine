#pragma once

#include "iType.h"

#include "iHashTable.h"

#define KOREAN_FIRST_ORDER		(21 * 28)
#define KOREAN_MIDDLE_ORDER			  28
#define KOREAN_LAST_ORDER			   1

#define KOREAN_BIT_MASK			    0xe0
#define ASCII_BIT_MASK			    0x80

#define KOREAN_CODE_MIN		0xac00
#define KOREAN_CODE_MAX		0xd7a3

#define THREE_BYTES_UNICODE_BASE	0xe08080

#define IKOREANJAMO_INVALID_COST		0xff

struct iKoreanJamo
{
	const char* jamo;
	const char* elements[2];
	uint8 elementsNum;
	uint8 initCost = IKOREANJAMO_INVALID_COST;
	uint8 mediCost = IKOREANJAMO_INVALID_COST;
	uint8 finCost = IKOREANJAMO_INVALID_COST;
	uint16 order;
};

class iKoreanAutoMata
{
private:
	static iKoreanAutoMata* S;
	iKoreanAutoMata();

public:
	virtual ~iKoreanAutoMata();
	static iKoreanAutoMata* share();

	char* join(const char* str);

private:
	uint8 getInitCost(const char* letter);
	uint8 getMediCost(const char* letter);
	uint8 getFinCost(const char* letter);

	bool canConbine(const char* left, const char* right, uint16& newUnicode);
	bool canConbine(const char* left, const char* right, iKoreanJamo*& letter);

	uint16 getUnicode(const char* korStr);
	uint16 getUnicode(uint8 initCost, uint8 mediCost, uint8 finCost);

private:
	iKoreanJamo* _jamo;

public:
	iHashTable jamo;

	iKoreanJamo** initConsonant;
	iKoreanJamo** mediVowel;
	iKoreanJamo** finConsonant;
};

#define iLETTER_ASCII	0
#define iLETTER_KOREAN	1

class iLetter
{
public:
	iLetter(uint8 kind);

	virtual ~iLetter();

public:
	uint8 kind;

	char* letter;
	int letterSize;
};

class iASCIILetter : public iLetter
{
public:
	iASCIILetter(const char* str);

	virtual ~iASCIILetter();
};

enum iKoreanLetterStat
{
	iKoreanLetterStatConsonantOnly,
	iKoreanLetterStatVowelOnly,
	iKoreanLetterStatNoFinal,
	iKoreanLetterStatIncludeFinal,
};

class iKoreanLetter : public iLetter
{
public:
	iKoreanLetter(const char* str);
	virtual ~iKoreanLetter();

	void change(uint16 uniCode);

private:
	void update();	

public:
	iKoreanLetterStat stat;

	iKoreanJamo* initial;
	iKoreanJamo* medial;
	iKoreanJamo* fin;

	uint16 uniCode;
};

bool isKorean(uint16 uniCode);
bool isKorConsonant(uint16 uniCode);
bool isKorVowel(uint16 uniCode);


