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
	uint32 remain;

	uint32 buffer;
};

class iPngReader
{
public:
	iPngReader(const char* path);
	virtual ~iPngReader();

	uint32 bigEndian(uint8* data, int num);
	iChunk* readChunk(uint8* data);

	uint8* lz77Decode(iLZ77Tuple* tuple, int num);

public:
};

