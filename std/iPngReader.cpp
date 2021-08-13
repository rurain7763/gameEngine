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

	uint8 ldOrder[] = 
	{ 
		16, 17, 18, 0, 8, 7, 9,
		6, 10, 5, 11, 4, 12, 3,
		13, 2, 14, 1, 15 
	};

	uint32 lenBase[] = 
	{
		3, 4, 5, 6, 7, 8, 9, 10, //257 - 264
		11, 13, 15, 17,          //265 - 268
		19, 23, 27, 31,          //269 - 273 
		35, 43, 51, 59,          //274 - 276
		67, 83, 99, 115,         //278 - 280
		131, 163, 195, 227,      //281 - 284
		258                      //285
	};

	uint8 lenExtraBit[] = 
	{
		0, 0, 0, 0, 0, 0, 0, 0, //257 - 264
		1, 1, 1, 1,				//265 - 268
		2, 2, 2, 2,				//269 - 273 
		3, 3, 3, 3,				//274 - 276
		4, 4, 4, 4,				//278 - 280
		5, 5, 5, 5,				//281 - 284
		0						//285
	};

	uint32 distBase[] = 
	{
		/*0*/	1, 2, 3, 4,    //0-3
		/*1*/	5, 7,          //4-5
		/*2*/	9, 13,         //6-7
		/*3*/	17, 25,        //8-9
		/*4*/	33, 49,        //10-11
		/*5*/	65, 97,        //12-13
		/*6*/	129, 193,      //14-15
		/*7*/	257, 385,      //16-17
		/*8*/	513, 769,      //18-19
		/*9*/	1025, 1537,    //20-21
		/*10*/	2049, 3073,	   //22-23
		/*11*/	4097, 6145,    //24-25
		/*12*/	8193, 12289,   //26-27
		/*13*/	16385, 24577,  //28-29
				0   , 0        //30-31
	};

	uint32 distExtraBit[] = {
		/*0*/	0, 0, 0, 0, //0-3
		/*1*/	1, 1,       //4-5
		/*2*/	2, 2,       //6-7
		/*3*/	3, 3,       //8-9
		/*4*/	4, 4,       //10-11
		/*5*/	5, 5,       //12-13
		/*6*/	6, 6,       //14-15
		/*7*/	7, 7,       //16-17
		/*8*/	8, 8,       //18-19
		/*9*/	9, 9,       //20-21
		/*10*/	10, 10,     //22-23
		/*11*/	11, 11,     //24-25
		/*12*/	12, 12,     //26-27
		/*13*/	13, 13,     //28-29
				0 , 0	    //30-31 
	};

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

				iHuffCode* litHuffCode = new iHuffCode[lenLit];
				iHuffCode* distHuffCode = new iHuffCode[lenDist];
				uint32 curReadLen = 0;
				uint32 prev = 0;
				uint8 symbol = 0;
				int repeat = 0;

				uint32 numldHuffTree = lenLit + lenDist;
				while (curReadLen < numldHuffTree)
				{
					iHuffCode v = decodeHuffman(ldCode, lenLD, &zBlock);

					if (v.c <= 15)
					{
						symbol = v.c;
						repeat = 1;
					}
					else if (v.c == 16)
					{
						symbol = prev;
						repeat = 3 + zBlock.readBit(2);			
					}
					else if (v.c == 17)
					{
						symbol = 0;
						repeat = 3 + zBlock.readBit(3);
					}
					else if (v.c == 18)
					{
						symbol = 0;
						repeat = 11 + zBlock.readBit(7);						
					}

					for (int i = 0; i < repeat; i++)
					{
						if (curReadLen < lenLit)
						{
							int idx = curReadLen;
							litHuffCode[idx].bitCount = symbol;
							litHuffCode[idx].c = idx;
						}
						else
						{
							int idx = curReadLen - lenLit;
							distHuffCode[idx].bitCount = symbol;
							distHuffCode[idx].c = idx;
						}

						printf("(%d:%d) ", v.c, symbol);

						curReadLen++;
					}

					prev = symbol;
				}

				makeHuffCode(litHuffCode, lenLit);
				makeHuffCode(distHuffCode, lenDist);

#if 1
				iHuffCode test[171];
				memcpy(test, litHuffCode, sizeof(iHuffCode)* lenLit);

				uint32 tcodenum = 5;
				iHuffCode* tcode = new iHuffCode[tcodenum];

				tcode[0].bitCount = 4;
				tcode[0].c = 'a';

				tcode[1].bitCount = 2;
				tcode[1].c = 'b';

				tcode[2].bitCount = 1;
				tcode[2].c = 'c';

				tcode[3].bitCount = 4;
				tcode[3].c = 'd';

				tcode[4].bitCount = 3;
				tcode[4].c = 'e';

				makeHuffCode(tcode, tcodenum);

				iHuffCode tcodeTest[5];
				memcpy(tcodeTest, tcode, sizeof(iHuffCode) * tcodenum);

				uint8 testStream[] = { 191 , 123 };
				iZlibBlock tblock(testStream);

				for (int i = 0; i < 4; i++)
				{
					iHuffCode v = decodeHuffman(tcode, tcodenum, &tblock);
					printf("%c ", v.c);
				}

				delete[] tcode;
#endif

				int resultIdx = 0;

				while (1)
				{
					if (resultIdx == 6)
					{
						int x = 10;
					}

					iHuffCode hc = decodeHuffman(litHuffCode, lenLit, &zBlock);
					
					if (hc.c == 256)
					{
						break;
					}
					else if (hc.c < 257)
					{
						//result[resultIdx] = hc.c;
						printf("%d ", hc.c);
						resultIdx++;
					}
					else
					{
						iLZ77Tuple tuple;

						int lenIdx = hc.c - 257;
						tuple.length = lenBase[lenIdx] + zBlock.readBit(lenExtraBit[lenIdx]);

						iHuffCode dc = decodeHuffman(distHuffCode, lenDist, &zBlock);
						tuple.distance = distBase[dc.c] + zBlock.readBit(distExtraBit[dc.c]);

						resultIdx++;
					}
				}

				delete[] ldCode;
				delete[] litHuffCode;
				delete[] distHuffCode;
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

void iPngReader::makeHuffCode(iHuffCode*& code, uint32& num)
{
	iHeap sort
	(
		[](void* left, void* right)
		{
			iHuffCode* l = (iHuffCode*)left;
			iHuffCode* r = (iHuffCode*)right;

			if (l->bitCount == r->bitCount) return l->c > r->c;
			else return l->bitCount > r->bitCount;
		}
	);

	for (int i = 0; i < num; i++)
	{
		iHuffCode* hc = &code[i];

		if (hc->bitCount == 0) continue;

		sort.insert(hc);
	}

	num = sort.dataNum;
	iHuffCode* r = new iHuffCode[num];

	uint32 prevCode = -1;
	uint32 prevBit = -1;
	for (int i = 0; i < num; i++)
	{
		iHuffCode* hc = &r[i];

		*hc = *(iHuffCode*)sort.pop();
		if (hc->bitCount != prevBit) hc->code = (prevCode + 1) << 1;
		else hc->code = prevCode + 1;
		
		prevCode = hc->code;
		prevBit = hc->bitCount;
	}

	delete[] code;
	code = r;
}

iHuffCode iPngReader::decodeHuffman(iHuffCode* code, int codeNum, 
									iZlibBlock* stream)
{
	int i, j;
	for (i = 0; i < codeNum; i++)
	{
		iHuffCode* hc = &code[i];
		uint32 v = stream->readBit(hc->bitCount);

		uint32 inv = 0;
		for (j = 0; j < hc->bitCount; j++)
		{
			inv <<= 1;
			inv |= (v & (1 << j)) >> j;
		}
		
		if (hc->code != inv)
		{
			stream->buffer <<= hc->bitCount;
			stream->buffer |= v;
			stream->remain += hc->bitCount;
		}
		else break;	
	}

	return code[i];
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

