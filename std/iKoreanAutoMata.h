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

class iKoreanByte;

class iKoreanAutoMata
{
public:
	iKoreanAutoMata();
	virtual ~iKoreanAutoMata();

	char* join(const char* str);

private:
	iHashTable first;
	iHashTable mid;
	iHashTable last;

#if 1
	iHashTable initConsonantCost;
	iHashTable medivalVowelCost;
	iHashTable finConsonantCost;
#endif

	int* cost;
};

enum iKoreanWordStat
{
	iKoreanWordStatConsonantOnly,
	iKoreanWordStatVowelOnly,
	iKoreanWordStatWordNoLast,
	iKoreanWordStatWordWithLast,
};

struct iKoreanWord
{
	const char* word;
	const char* elements[2];
	int elementsNum;
};

class iKoreanByte
{
public:
	iKoreanByte(const uint8* v);

	void changeCharacter(uint32 uniCode);

private:
	void updateWord(uint16 uniCode);

public:
	char character[4];

	iKoreanWordStat stat;
	iKoreanWord* first;
	iKoreanWord* mid;
	iKoreanWord* last;

	uint32 bytes;
	uint16 uniCode;
};

bool isConsonant(uint16 uniCode);
bool isVowel(uint16 uniCode);
bool isKorean(uint16 uniCode);

uint16 getUnicode(const char* korChar);
uint16 getUnicode(uint8 fisrtCost, uint8 midCost, uint8 lastCost);

//TODO ascii

#if 1
enum iKoreanLetterStat
{
	iKoreanLetterStatConsonantOnly,
	iKoreanLetterStatVowelOnly,
	iKoreanLetterStatNoFinal,
	iKoreanLetterStatIncludeFinal,
};

struct iKoreanJamo
{
	const char* jamo;
	const char* elements[2];
	uint8 elementsNum;
};

class iKoreanLetter
{
public:
	iKoreanLetterStat stat;

	iKoreanJamo* initial;
	iKoreanJamo* medial;
	iKoreanJamo* fin;
};
#endif