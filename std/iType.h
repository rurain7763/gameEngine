#pragma once

typedef bool Flag;
typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

typedef bool (*CompareMethod)(void* left, void* right);

struct iSize
{
	float width, height;
};

struct iRect
{
	float x, y;
	float width, height;
};