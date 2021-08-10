#pragma once

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

struct iChunk
{
	~iChunk();
	char type[5];

	uint32 len;
	uint8* data;
	uint32 crc32;
};

struct iLZ77Tuple
{
	uint8 distance;
	uint8 length;
	uint8 lit;
};

struct iZlibBlock
{
	iZlibBlock(uint8* stream);
	uint32 readBit(int readBit);

	uint8* stream;

	uint32 buffer;
	uint32 remain;
};

struct iHuffCode
{
	uint32 code = -1;
	uint8 bitCount = -1;
	char c = -1;
};

class iPngReader
{
public:
	iPngReader(const char* path);
	virtual ~iPngReader();

	uint32 bigEndian(uint8* data, int num);
	iChunk* readChunk(uint8* data);

	uint8* decodeLz77(iLZ77Tuple* tuple, int num);
	int makeHuffCode(iHuffCode*& code, int num);
	uint32 decodeHuffman(iHuffCode* code, int codeNum,
						 iZlibBlock* stream);

public:
};

