#pragma once

#include "iType.h"

#include <png.h>
#include <jpeglib.h>
#include <setjmp.h>

enum iImageType
{
	iImageTypePng = 0,
	iImageTypeJpg,
	iImageTypeJfif,
	iImageTypeJpeg,
	iImageTypeUnknown
};

class iImage
{
public:
	virtual ~iImage();

public:
	iImageType type;

	uint8* pixelData;
	uint32 width, height;
	uint32 stride;
};

class iPng : public iImage
{
public:
	iPng();
	virtual ~iPng();

public:
	uint8 colorType;
	uint8 channels;
	uint8 bitDepth;
};

iPng* readPng(const char* path);

class iJpg : public iImage
{
public:
	iJpg();
	virtual ~iJpg();

public:
	uint8 pixelSize;
};

iJpg* readJpg(const char* path);

char* getImageType(const char* path);

#if 0
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
#endif


