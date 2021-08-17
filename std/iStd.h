#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "iVector.h"
#include "iSize.h"
#include "iRect.h"
#include "iMatirx.h"
#include "iQuaternion.h"
#include "iTree.h"
#include "iArray.h"

#include "iPngReader.h"
#include "iInputManager.h"
#include "iTime.h"
#include "iTransform.h"
#include "iCamera.h"

#include "iOpenGL.h"
#include "iWindows.h"

#define ToRadian(degree) degree * M_PI / 180
#define ToDegree(radian) radian * 180 / M_PI

typedef bool (*CompareMethod)(void* left, void* right);

float isin(float degree);
float icos(float degree);
float itan(float degree);
int random();

char* readFile(const char* path);
void bubbleSort(CompareMethod method, void* data, int elementSize, int num);
void swap(void* left, void* right, int argSize);
void printBit(unsigned int v);