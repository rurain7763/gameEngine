#pragma once

typedef unsigned char uint8;
typedef unsigned int  uint32;

struct iChunk
{
	char type[5];

	uint32 len;
	uint8* data;
	uint32 crc32;
};

struct iLZ77Tuple
{
	uint8 dist;
	uint8 length;
	uint8 literal;
};

class iPngReader
{
public:
	iPngReader(const char* path);
	virtual ~iPngReader();

	int bigEndian(uint8* data, int num);
	iChunk* readChunk(uint8* data);

	uint8* lz77Decode(iLZ77Tuple* tuple, int num);

public:
};
