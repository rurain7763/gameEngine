#pragma once

#define BLANK_SYMBOL	0

struct iHuffmanNode
{
	int idx;
	short symbol;
	short weight;
	iHuffmanNode* left;
	iHuffmanNode* right;
};

class iHuffmanTree
{
public:
	iHuffmanTree();
	virtual ~iHuffmanTree();

	char searchSymbol(char* code);
	char* searchCode();

public:
	iHuffmanNode* seed;
};

