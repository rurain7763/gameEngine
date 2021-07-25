#include"iPngReader.h"
#include"iStd.h"

iPngReader::iPngReader(const char* path)
{
	char* data = readFile(path);

	char png[4] = { 0 };
	memcpy(png, &data[1], sizeof(char) * 3);

	if (strcmp(png, "PNG"))
	{
		delete data;
		return;
	}

	int step = 8;

	while (1)
	{
		iChunk* chunk = readChunk((uint8*)&data[step]);

		if (chunk->len == 0) break; // IEND

		if (!strcmp(chunk->type, "IHDR"))
		{
			uint32 width = bigEndian(&chunk->data[0], 4);
			uint32 height = bigEndian(&chunk->data[4], 4);
		}
		else if (!strcmp(chunk->type, "PLTE"))
		{
		}
		else if (!strcmp(chunk->type, "IDAT"))
		{
		}
		else // ...
		{

		}

		step += 4 + 4 + chunk->len + 4;
		delete chunk;
	}

	delete data;
}

iPngReader::~iPngReader()
{
}

int iPngReader::bigEndian(uint8* data, int num)
{
	int result = 0;

	for (int i = 0; i < num; i++)
	{
		result <<= 8;
		result |= data[i];
	}

	return result;
}

iChunk* iPngReader::readChunk(uint8* data)
{
	int step = 0;
	iChunk* r = new iChunk;

	r->len = bigEndian((uint8*)&data[step], sizeof(uint32));
	step += sizeof(uint32);

	memcpy(r->type, &data[step], sizeof(char) * 4);
	r->type[4] = 0;
	step += sizeof(char) * 4;

	r->data = new uint8[r->len];
	memcpy(r->data, &data[step], sizeof(uint8) * r->len);
	step += sizeof(uint8) * r->len;

	r->crc32 = bigEndian((uint8*)&data[step], 4);
	step += sizeof(uint32);

	return r;
}
