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
			//essential for color type 3 - color types 2 and 6 (true color , true color with alpha)
			//must not appear for color types 0 and 4 (gray scale, gray scale with alpha)
		}
		else if (!strcmp(chunk->type, "IDAT"))
		{
			uint8 compression = chunk->data[0];
			uint8 flag = chunk->data[1];

			int size = chunk->len - 1 - 1 - 2;
			uint8* compressed = new uint8[size];
			memcpy(compressed, &chunk->data[2], sizeof(uint8) * size);

			//data
			{
				iZlibBlock zb;
				zb.data = compressed;
				zb.remain = 0;
				zb.buffer = 0;

				uint8 fin = zb.readBit(1);
				uint8 method = zb.readBit(2);
				uint32 hLit = zb.readBit(5) + 257;
				uint32 hDist = zb.readBit(5) + 1;
				uint32 hClen = zb.readBit(4) + 4;

				uint8 codeLen = zb.readBit(3);

				int x = 10;
			}

			uint16 adler32 = (uint16)chunk->data[2 + size];
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

uint8* iPngReader::lz77Decode(iLZ77Tuple* tuple, int num)
{
	int bufferSize = 0;
	for (int i = 0; i < num; i++)
		bufferSize += tuple[i].length + 1;
	
	uint8* r = new uint8[bufferSize];
	memset(r, 0, sizeof(uint8) * bufferSize);

	int left = 0;
	int right = 0;

	for (int i = 0; i < num; i++)
	{
		iLZ77Tuple* t = &tuple[i];
		
		left = right - t->dist;
		for (int j = 0; j < t->length; j++)
		{
			r[right] = r[left];
			right++, left++;
		}

		r[right] = t->lit;
		right++;
	}

	return r;
}

uint32 iZlibBlock::readBit(int readBit)
{
	buffer = 0;

	int curCount = (8 - remain) % 8;
	int curByte = 0;
	int shift = 0;
	uint8 start = data[curByte];
	start >>= curCount;

	for (int i = 0; i < readBit; i++)
	{
		if (curCount == 8)
		{
			curCount = 0;
			curByte++;
			start = data[curByte];
			shift = 0;
		}

		buffer <<= 1;
		buffer |= (start & (1 << shift)) >> shift;
		curCount++;
		shift++;
	}

	data = &data[curByte];
	remain = 8 - curCount;

	return buffer;
}
