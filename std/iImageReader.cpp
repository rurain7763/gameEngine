#include"iImageReader.h"
#include"iStd.h"

iImage::~iImage()
{
}

iPng::iPng()
{
	type = iImageTypePng;
	rgba = NULL;
	stride = 0;
	width = 0, height = 0;
	colorType = 0xff;
	channels = 0xff;
	bitDepth = 0xff;
}

iPng::~iPng()
{
	if (rgba) delete[] rgba;
}

iPng* readPng(const char* path)
{
	FILE* file = fopen(path, "rb");

	if (!file)
	{
		printf("readPng() : no such %s\n", path);
		return NULL;
	}

	iPng* r = new iPng();
	uint8** row = NULL;

	uint8 header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) return NULL;
	fseek(file, 0, SEEK_SET);

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);

	if (setjmp(png_jmpbuf(png)))
	{
		if (row)
		{
			for (int i = 0; i < r->height; i++)
				delete[] row[i];
			delete[] row;
		}

		delete r;
		png_destroy_read_struct(&png, &info, NULL);
		printf("readPng() : error occured when png read\n");

		//Make sure you return here. libPNG will jump to here if something
		//goes wrong, and if you continue with your normal code, you might
		//End up with an infinite loop.
		fclose(file);
		return NULL;
	}

	png_init_io(png, file);
	png_read_info(png, info);

	r->width = png_get_image_width(png, info);
	r->height = png_get_image_height(png, info);
	r->colorType = png_get_color_type(png, info);
	r->channels = png_get_channels(png, info);
	r->bitDepth = png_get_bit_depth(png, info);

	if (r->colorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png);
		r->channels = 3;
	}
	else if ((r->colorType == PNG_COLOR_TYPE_GRAY) && (r->bitDepth < 8))
	{
		//PNG_COLOR_TYPE_GRAY is always 8 bit depth
		png_set_expand_gray_1_2_4_to_8(png);
		r->bitDepth = 8;
	}

	if (png_get_valid(png, info, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png);
		r->channels += 1;
	}

	//read any color_type into 8bit depth, rgba format
	if (r->bitDepth == 16)
		png_set_strip_16(png);

	//if don't have alpha channel then fill it with 0
	if (r->colorType == PNG_COLOR_TYPE_RGB ||
		r->colorType == PNG_COLOR_TYPE_GRAY ||
		r->colorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_filter(png, 0, PNG_FILLER_AFTER);
	}

	if (r->colorType == PNG_COLOR_TYPE_GRAY ||
		r->colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png);
	}

	png_read_update_info(png, info);

	r->stride = r->width * r->bitDepth * r->channels / 8;

	row = new uint8 * [r->height];
	for (int i = 0; i < r->height; i++)
		row[i] = new uint8[png_get_rowbytes(png, info)];

	png_read_image(png, row);
	r->rgba = new uint8[r->width * r->height * r->bitDepth * r->channels / 8];

	for (int i = 0; i < r->height; i++)
	{
		memcpy(&r->rgba[r->stride * i], row[i], sizeof(uint8) * r->stride);
		delete[] row[i];
	}

	delete[] row;
	png_destroy_read_struct(&png, &info, NULL);
	fclose(file);

	return r;
}

iJpg::iJpg()
{
	type = iImageTypeUnknown;
	rgb = NULL;
	stride = 0;
	width = 0;
	height = 0;
	pixelSize = 0xff;
}

iJpg::~iJpg()
{
	if (rgb) delete[] rgb;
}

iJpg* readJpg(const char* path)
{
	FILE* file = fopen(path, "rb");

	if (!file)
	{
		printf("readJPG() : no such %s\n", path);
		return NULL;
	}

	iJpg* r = new iJpg();

	struct iJPGError
	{
		jpeg_error_mgr error;
		jmp_buf jmpBuf;
	};

	jpeg_decompress_struct jpg;
	iJPGError err;

	jpg.err = jpeg_std_error(&err.error);
	err.error.error_exit = [](j_common_ptr info)
	{
		iJPGError* err = (iJPGError*)info->err;
		info->err->output_message(info);
		longjmp(err->jmpBuf, 1);
	};

	if (setjmp(err.jmpBuf))
	{
		delete r;
		jpeg_destroy_decompress(&jpg);
		fclose(file);
		return NULL;
	}

	jpeg_create_decompress(&jpg);
	jpeg_stdio_src(&jpg, file);

	jpeg_read_header(&jpg, TRUE);

	/*
	JCS_UNKNOWN,		error/unspecified
	JCS_GRAYSCALE,		monochrome
	JCS_RGB,			red/green/blue
	JCS_YCbCr,			Y/Cb/Cr (also known as YUV)
	JCS_CMYK,			C/M/Y/K
	JCS_YCCK			Y/Cb/Cr/K
	*/
	jpg.out_color_space = JCS_RGB;

	jpeg_start_decompress(&jpg);

	r->width = jpg.output_width;
	r->height = jpg.output_height;
	r->pixelSize = jpg.output_components;

	r->stride = r->width * r->pixelSize;
	r->rgb = new uint8[r->stride * r->height];

	int off = 0;
	while (jpg.output_scanline < jpg.output_height)
	{
		uint8* row = &r->rgb[r->stride * off];
		jpeg_read_scanlines(&jpg, &row, 1);
		off++;
	}

	jpeg_finish_decompress(&jpg);
	jpeg_destroy_decompress(&jpg);
	fclose(file);

	char* type = getImageType(path);
	if (!strcmp("jpg", type))
	{
		r->type = iImageTypeJpg;
	}
	else if (!strcmp("jfif", type))
	{
		r->type = iImageTypeJfif;
	}
	else if (!strcmp("jpeg", type))
	{
		r->type = iImageTypeJpeg;
	}
	else
	{
		r->type = iImageTypeUnknown;
	}

	delete[] type;

	return r;
}

char* getImageType(const char* path)
{
	int len = strlen(path);
	int dotPos = len;
	while (dotPos > 0)
	{
		dotPos--;
		if (path[dotPos] == '.') break;
	}

	int typeLen = len - dotPos - 1;
	char* r = new char[typeLen + 1];
	memcpy(r, &path[dotPos + 1], sizeof(char) * typeLen);
	r[typeLen] = 0;

	return r;
}

#if 0 //TODO make to complete
iPngReader* iPngReader::S = NULL;

iPngReader::iPngReader()
{
	litDistBitLengthOrder = new uint8[]
	{
		16, 17, 18, 0, 8, 7, 9,
		6, 10, 5, 11, 4, 12, 3,
		13, 2, 14, 1, 15
	};

	lz77LengthBaseLen = new uint32[]
	{
		3, 4, 5, 6, 7, 8, 9, 10, //257 - 264
		11, 13, 15, 17,          //265 - 268
		19, 23, 27, 31,          //269 - 273
		35, 43, 51, 59,          //274 - 276
		67, 83, 99, 115,         //278 - 280
		131, 163, 195, 227,      //281 - 284
		258                      //285
	};

	lz77LengthExtraBit = new uint8[]
	{
		0, 0, 0, 0, 0, 0, 0, 0, //257 - 264
		1, 1, 1, 1,				//265 - 268
		2, 2, 2, 2,				//269 - 273
		3, 3, 3, 3,				//274 - 276
		4, 4, 4, 4,				//278 - 280
		5, 5, 5, 5,				//281 - 284
		0						//285
	};

	lz77DistBaseLen = new uint32[]
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

	lz77DistExtraBit = new uint32[]
	{
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
				0 , 0		//30-31
	};
}

iPngReader::~iPngReader()
{
	delete[] litDistBitLengthOrder;
	delete[] lz77LengthBaseLen;
	delete[] lz77LengthExtraBit; 
	delete[] lz77DistBaseLen;
	delete[] lz77DistExtraBit;
}

iPngReader* iPngReader::share()
{
	if (!S) S = new iPngReader();
	return S;
}

void* iPngReader::readPng(const char* path)
{
	char* data = readFile(path);

	char png[4] = { 0 };
	memcpy(png, &data[1], sizeof(char) * 3);

	if (strcmp(png, "PNG"))
	{
		delete[] data;
		return NULL;
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
			
			uint8* result = readZLib(&chunk->data[2]);
			delete result;

			uint16 adler32 = bigEndian((uint8*)&chunk->data[chunk->len - 2], 2);
		}
		else // ...
		{

		}

		step += 4 + 4 + chunk->len + 4;
		delete chunk;
	}

	delete[] data;

	return NULL;
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

	r->data = &data[step];
	step += sizeof(uint8) * r->len;

	r->crc32 = bigEndian((uint8*)&data[step], 4);
	step += sizeof(uint32);

	return r;
}

uint8* iPngReader::readZLib(uint8* data)
{
	iZlibBlock zBlock(data);

	uint8 fin = zBlock.readBit(1);
	// 0 : compressed x / 1 : fixed huffman
	// 2 : dynamic huffman / 3 : error
	uint8 compressed = zBlock.readBit(2);

	uint8* r = NULL;

	if (compressed == 0)
	{
		int x = 10;
	}
	else if (compressed == 1)
	{
		int y = 10;
	}
	else if (compressed == 2)
	{
		r = decodeDynamicHuffman(zBlock);
	}
	else if (compressed == 3)
	{

	}

	return r;
}

uint8* iPngReader::decodeDynamicHuffman(iZlibBlock& zBlock)
{
	uint32 litLenLengthNum = zBlock.readBit(5) + 257;
	uint32 distLengthNum = zBlock.readBit(5) + 1;
	uint32 lDLengthNum = zBlock.readBit(4) + 4;

	iHuffCode* ldCode = new iHuffCode[lDLengthNum];

	for (int i = 0; i < lDLengthNum; i++)
	{
		iHuffCode* hc = &ldCode[i];
		hc->c = litDistBitLengthOrder[i];
		hc->bitCount = zBlock.readBit(3);
	}

	makeDynamicHuffCode(ldCode, lDLengthNum);

	iHuffCode* litHuffCode = new iHuffCode[litLenLengthNum];
	iHuffCode* distHuffCode = new iHuffCode[distLengthNum];
	uint32 curReadLen = 0;
	uint32 prev = 0;
	uint8 symbol = 0;
	int repeat = 0;

	uint32 numldHuffTree = litLenLengthNum + distLengthNum;
	while (curReadLen < numldHuffTree)
	{
		iHuffCode* v = decodeHuffman(ldCode, lDLengthNum, &zBlock);

		if (v->c <= 15)
		{
			symbol = v->c;
			repeat = 1;
		}
		else if (v->c == 16)
		{
			symbol = prev;
			repeat = 3 + zBlock.readBit(2);
		}
		else if (v->c == 17)
		{
			symbol = 0;
			repeat = 3 + zBlock.readBit(3);
		}
		else if (v->c == 18)
		{
			symbol = 0;
			repeat = 11 + zBlock.readBit(7);
		}

		for (int i = 0; i < repeat; i++)
		{
			if (curReadLen < litLenLengthNum)
			{
				int idx = curReadLen;
				litHuffCode[idx].bitCount = symbol;
				litHuffCode[idx].c = idx;
			}
			else
			{
				int idx = curReadLen - litLenLengthNum;
				distHuffCode[idx].bitCount = symbol;
				distHuffCode[idx].c = idx;
			}

			curReadLen++;
		}

		prev = symbol;
	}

	makeDynamicHuffCode(litHuffCode, litLenLengthNum);
	makeDynamicHuffCode(distHuffCode, distLengthNum);

	iArray rgba;

	while (1)
	{
		iHuffCode* hc = decodeHuffman(litHuffCode, litLenLengthNum, &zBlock);

		if (hc->c == 256)
		{
			break;
		}
		else if (hc->c < 257)
		{
			rgba.push_back(&hc->c);
		}
		else
		{
			int lenIdx = hc->c - 257;
			int length = lz77LengthBaseLen[lenIdx] + zBlock.readBit(lz77LengthExtraBit[lenIdx]);

			iHuffCode* dc = decodeHuffman(distHuffCode, distLengthNum, &zBlock);
			int dist = lz77DistBaseLen[dc->c] + zBlock.readBit(lz77DistExtraBit[dc->c]);

			int start = rgba.dataNum - dist;
			for (int i = 0; i < length; i++)
			{
				rgba.push_back(rgba[start + i]);
			}
		}
	}

	uint8* r = new uint8[rgba.dataNum];

	for (int i = 0; i < rgba.dataNum; i++)
		r[i] = *(uint8*)rgba[i];
	
	delete[] ldCode;
	delete[] litHuffCode;
	delete[] distHuffCode;

	return r;
}

void iPngReader::makeDynamicHuffCode(iHuffCode*& code, uint32& num)
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

		if (hc->bitCount != 0) sort.insert(hc);
	}

	num = sort.dataNum;
	iHuffCode* r = new iHuffCode[num];

	uint32 prevCode = -1;
	uint32 prevBit = -1;
	for (int i = 0; i < num; i++)
	{
		iHuffCode* hc = &r[i];

		memcpy(hc, sort.pop(), sizeof(iHuffCode));

		if (hc->bitCount != prevBit)		
			hc->code = (prevCode + 1) << (hc->bitCount - prevBit);		
		else hc->code = prevCode + 1;

		prevCode = hc->code;
		prevBit = hc->bitCount;
	}

	delete[] code;
	code = r;
}

iHuffCode* iPngReader::decodeHuffman(iHuffCode* code, int codeNum,
									 iZlibBlock* stream)
{
	uint32 curBitCount = 0;
	uint32 reverseBit = 0;
	uint32 bits = 0;
	for (int i = 0; i < codeNum; i++)
	{
		iHuffCode* hc = &code[i];

		if (curBitCount != hc->bitCount)
		{
			stream->buffer <<= curBitCount;
			stream->buffer |= bits;
			stream->remain += curBitCount;

			bits = stream->readBit(hc->bitCount);
			reverseBit = stream->reverseBit(bits, hc->bitCount);
			curBitCount = hc->bitCount;
		}

		if (hc->code == reverseBit) return hc;
	}

	return NULL;
}

iZlibBlock::iZlibBlock(uint8* s)
{
	stream = s;
	remain = 0;
	buffer = 0;
}

uint32 iZlibBlock::readBit(int readBit)
{
	if (readBit == 0) return 0;

	if (remain < readBit)
	{
		float needBit = readBit - remain;
		float needByte = needBit / 8.f;
		needByte = ceil(needByte);

		for (int i = 0; i < needByte; i++)
		{
			uint32 v = stream[i];
			buffer |= v << (i * 8 + remain);
		}

		stream = &stream[(int)needByte];
		remain += needByte * 8;
	}

	uint32 mask = 0xffffffff >> (32 - readBit);
	uint32 result = buffer & mask;
	
	remain -= readBit;
	buffer >>= readBit;

	return result;
}

uint32 iZlibBlock::reverseBit(uint32 v, int bitNum)
{
	v = ((v & 0xffff0000) >> 16) | ((v & 0x0000ffff) << 16);
	v = ((v & 0xff00ff00) >> 8) | ((v & 0x00ff00ff) << 8);
	v = ((v & 0xf0f0f0f0) >> 4) | ((v & 0x0f0f0f0f) << 4);
	v = ((v & 0xcccccccc) >> 2) | ((v & 0x33333333) << 2);
	v = ((v & 0xaaaaaaaa) >> 1) | ((v & 0x55555555) << 1);

	return v >> (32 - bitNum);
}
#endif


