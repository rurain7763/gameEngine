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
				uint8 length[] = { 4, 2, 1, 4, 3 };

				iZlibBlock zBlock(compressed);

				uint8 fin = zBlock.readBit(1);

				// 0 : compressed x / 1 : fixed huffman 
				// 2 : dynamic huffman / 3 : error
				uint8 compressed = zBlock.readBit(2);
				uint32 lit = zBlock.readBit(5);
				uint32 dist = zBlock.readBit(5);
				uint32 len = zBlock.readBit(4);
			}

			delete[] compressed;

			uint16 adler32 = bigEndian((uint8*)&chunk->data[2 + size], 2);
		}
		else // ...
		{

		}

		step += 4 + 4 + chunk->len + 4;
		delete chunk;
	}

	delete[] data;
}

iPngReader::~iPngReader()
{
}

uint32 iPngReader::bigEndian(uint8* data, int num)
{
	uint32 result = 0;

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

uint8* iPngReader::decodeLz77(iLZ77Tuple* tuple, int num)
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
		
		left = right - t->distance;
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

iHuffCode* iPngReader::makeHuffCode(uint8* length, int num)
{
	int maxBit = -1;

	iHuffCode* r = new iHuffCode[num];
	for (int i = 0; i < num; i++)
	{
		if (r[length[i] - 1].c == -1)
		{
			r[length[i] - 1].code = -1;
			r[length[i] - 1].bitCount = length[i];
			r[length[i] - 1].c = 'a' + i;
		}
		else
		{
			r[length[i]].code = -1;
			r[length[i]].bitCount = length[i];
			r[length[i]].c = 'a' + i;
		}

		maxBit = max(maxBit, length[i]);
	}

	uint32* lastCode = new uint32[maxBit];
	memset(lastCode, -1, sizeof(uint32) * maxBit);
	r[0].code = 0;
	lastCode[0] = 0;

	for (int i = 1; i < num; i++)
	{
		iHuffCode* c = &r[i];
		int idx = c->bitCount - 1;

		if (lastCode[idx] == -1)	
			c->code = lastCode[idx - 1] + 1 << 1;		
		else
			c->code = lastCode[idx] + 1;
		
		lastCode[idx] = c->code;
	}

	delete[] lastCode;

	return r;
}

uint8* iPngReader::decodeHuffman(iHuffCode* code, int codeNum, 
								 iZlibBlock* stream, int bitNum, 
								 int maxBit)
{
	uint32 v = 0;
	int count = maxBit;
	int loop = 0;

	uint8* buffer = new uint8[1024];
	int size = 1024;
	int bufferIdx = 0;

	while (loop < bitNum)
	{
		v |= stream->readBit(count) << maxBit - count;

		for (int i = 0; i < codeNum; i++)
		{
			bool find = true;
			count = code[i].bitCount;
			
			int shift = count - 1;
			int leftMask = 1;
			int rightMask = 1 << shift;

			for (int j = 0; j < count; j++)
			{
				int left = (v & leftMask << j) >> j;
				int right = (code[i].code & rightMask >> j) >> (shift - j);

				if (left != right)
				{
					find = false;
					break;
				}
			}

			if (find)
			{
				if (bufferIdx >= size)
				{
					uint8* copy = new uint8[size += 100];
					memcpy(copy, buffer, sizeof(uint8) * bufferIdx);
					delete[] buffer;
					buffer = copy;
				}

				buffer[bufferIdx] = code[i].c;
				bufferIdx++;

				loop += count;
				v >>= count;
				break;
			}
		}
	}

	uint8* r = new uint8[bufferIdx + 1];
	memcpy(r, buffer, sizeof(uint8) * bufferIdx);
	r[bufferIdx] = 0;
	delete[] buffer;

	return r;
}

iChunk::~iChunk()
{
	delete[] data;
}

iZlibBlock::iZlibBlock(uint8* s)
{
	stream = s;
	remain = 0;
	buffer = 0;
}

uint32 iZlibBlock::readBit(int readBit)
{
	if (remain < readBit)
	{
		float needBit = readBit - remain;
		float needByte = needBit / 8.f;
		needByte = ceil(needByte);

		for (int i = 0; i < needByte; i++)
		{
			uint32 v = stream[i];
			buffer |= v << (i*8 + remain);
		}

		stream = &stream[(int)needByte];
		remain += needByte * 8;
	}

	uint32 result = 0;
	for (int i = 0; i < readBit; i++)
	{
		result |= buffer & 1 << i;
	}

	remain -= readBit;
	buffer >>= readBit;

	return result;
}

