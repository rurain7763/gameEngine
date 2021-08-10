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

	uint8 ldOrder[] = { 16, 17, 18, 0, 8, 7, 9,
					    6, 10, 5, 11, 4, 12, 3,
					    13, 2, 14, 1, 15 };

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
				iZlibBlock zBlock(compressed);

				uint8 fin = zBlock.readBit(1);
				// 0 : compressed x / 1 : fixed huffman 
				// 2 : dynamic huffman / 3 : error
				uint8 compressed = zBlock.readBit(2);

				//============================== data
				uint32 lenLit = zBlock.readBit(5) + 257;
				uint32 lenDist = zBlock.readBit(5) + 1;
				uint32 lenLD = zBlock.readBit(4) + 4;

				iHuffCode* ldCode = new iHuffCode[lenLD];
				
				for (int i = 0; i < lenLD; i++)
				{
					iHuffCode* hc = &ldCode[i];
					hc->c = ldOrder[i];
					hc->bitCount = zBlock.readBit(3);
				}

				makeHuffCode(ldCode, lenLD);
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

int iPngReader::makeHuffCode(iHuffCode*& code, int num)
{
	int returnIdx = 0;

	for (int i = 0; i < num; i++)
	{
		iHuffCode* hc = &code[i];

		if (hc->bitCount == 0) continue;

		uint32 symbol = hc->c;
		uint32 count = hc->bitCount;

		returnIdx++;
	}

	iHuffCode* r = new iHuffCode[returnIdx];
	delete[] code;
	code = r;

	return returnIdx;
}

uint32 iPngReader::decodeHuffman(iHuffCode* code, int codeNum, 
								 iZlibBlock* stream)
{
	int maxBit = code[codeNum - 1].bitCount;

	uint32 v = stream->readBit(maxBit);
	int i,j;

	for (i = 0; i < codeNum; i++)
	{
		bool find = true;
		int count = code[i].bitCount;
			
		int shift = count - 1;
		int leftMask = 1;
		int rightMask = 1 << shift;

		for (j = 0; j < count; j++)
		{
			int left = (v & leftMask << j) >> j;
			int right = (code[i].code & rightMask >> j) >> (shift - j);

			if (left != right)
			{
				find = false;
				break;
			}
		}

		if (count != 0 && find)
		{
			uint32 reUse = v >> code[i].bitCount;
			stream->buffer <<= code[i].bitCount;
			stream->buffer |= reUse;
			stream->remain += code[i].bitCount;
			return code[i].c;
		}
	}

	return 0;
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
		result |= buffer & (1 << i);
	}

	remain -= readBit;
	buffer >>= readBit;

	return result;
}

