#pragma once

#include "iType.h"
#include <png.h>

struct iPng;
struct iChunk;
struct iZlibBlock;
struct iHuffCode;

class iPngReader
{
private:
	static iPngReader* S;
	iPngReader();

public:
	virtual ~iPngReader();

	static iPngReader* share();
	void* readPng(const char* path);

private:
	uint32 bigEndian(uint8* data, int num);
	iChunk* readChunk(uint8* data);
	uint8* readZLib(uint8* data);

	uint8* decodeDynamicHuffman(iZlibBlock& zBlock);

	void makeDynamicHuffCode(iHuffCode*& code, uint32& num);
	iHuffCode* decodeHuffman(iHuffCode* code, int codeNum,
							 iZlibBlock* stream);

private:
	uint8* litDistBitLengthOrder;

	uint32* lz77LengthBaseLen;
	uint8* lz77LengthExtraBit;

	uint32* lz77DistBaseLen;
	uint32* lz77DistExtraBit;
};

struct iChunk
{
	char type[5];

	uint32 len;
	uint8* data;
	uint32 crc32;
};

struct iZlibBlock
{
	iZlibBlock(uint8* stream);
	uint32 readBit(int readBit);
	uint32 reverseBit(uint32 v, int bitNum);

	uint8* stream;

	uint32 buffer;
	uint32 remain;
};

struct iHuffCode
{
	uint32 code = -1;
	uint8 bitCount = -1;
	uint32 c = -1;
};

struct iPng
{
	iPng();
	virtual ~iPng();

	uint8* rgba;
	uint32 stride;
	uint32 width, height;
	uint8 colorType;
	uint8 channels;
	uint8 bitDepth;
};

iPng* readPng(const char* path);

