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
				iZlibBlock zBlock(compressed);

				uint8 fin = zBlock.readBit(1);
				// 0 : compressed x / 1 : fixed huffman 
				// 2 : dynamic huffman / 3 : error
				uint8 compressed = zBlock.readBit(2);

				uint32 lit = zBlock.readBit(5) + 257;
				uint32 dist = zBlock.readBit(5) + 1;
				uint32 len = zBlock.readBit(4) + 4;

				uint8 order[] = { 16, 17, 18, 0, 8, 7, 9, 6, 
								  10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
				uint8 firstHuffTreeLen[19];
				memset(firstHuffTreeLen, 0, sizeof(uint8) * 19);

				for (int i = 0; i < len; i++)				
					firstHuffTreeLen[order[i]] = zBlock.readBit(3);
				
				iHuffCode* firstCode = makeHuffCode(firstHuffTreeLen, len);

				int twoHuffTreeLenSize = lit + dist;
				uint8* twoHuffTreeLen = new uint8[twoHuffTreeLenSize];

				int curReadLen = 0;
				while (curReadLen < twoHuffTreeLenSize)
				{
					int v = decodeHuffman(firstCode, len, &zBlock);

					int repeatValue = 0;
					int repeat = 0;

					if (v < 16)
					{
						twoHuffTreeLen[curReadLen] = v;
						curReadLen++;
						continue;
					}
					else if (v == 16)
					{
						repeat = zBlock.readBit(2) + 3;
						repeatValue = twoHuffTreeLen[curReadLen - 1];
					}
					else if (v == 17)
					{
						repeat = zBlock.readBit(3) + 3;
					}
					else if (v == 18)
					{
						repeat = zBlock.readBit(7) + 11;
					}

					for (int i = 0; i < repeat; i++)
					{
						twoHuffTreeLen[curReadLen] = repeatValue;
						curReadLen++;
					}
				}

				int x = 10;
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
	iHuffCode* r = new iHuffCode[num];
	for (int i = 0; i < num; i++)
	{
		iHuffCode* hc = &r[i];
		hc->c = 'a' + i;
		hc->code = -1;
		hc->bitCount = length[i];
	}

	for (int i = num - 1; i > -1; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (r[j].bitCount > r[j + 1].bitCount)
			{
				iHuffCode copy = r[j];
				r[j] = r[j + 1];
				r[j + 1] = copy;
			}
			else if (r[j].bitCount == r[j + 1].bitCount)
			{
				if (r[j].c > r[j + 1].c)
				{
					iHuffCode copy = r[j];
					r[j] = r[j + 1];
					r[j + 1] = copy;
				}
			}
		}
	}

#if 1
	iHuffCode test[19];
	memcpy(test, r, sizeof(iHuffCode) * 19);
#endif

	uint32 prevCode = -1;
	int prevBitNum = -1;

	for (int i = 0; i < num; i++)
	{
		iHuffCode* c = &r[i];

		if (prevBitNum != c->bitCount)		
			c->code = (prevCode + 1) << 1;		
		else		
			c->code = prevCode + 1;
		
		prevCode = c->code;
		prevBitNum = c->bitCount;
	}

	return r;
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
			uint32 reUse = v >> code[j].bitCount;
			stream->buffer <<= code[j].bitCount;
			stream->buffer |= reUse;
			stream->remain += code[j].bitCount;
			return code[j].code;
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
		result |= buffer & 1 << i;
	}

	remain -= readBit;
	buffer >>= readBit;

	return result;
}

