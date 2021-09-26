#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "iType.h"
#include "iVector.h"
#include "iMatrix.h"
#include "iQuaternion.h"
#include "iTree.h"
#include "iArray.h"
#include "iSharedPtr.h"
#include "iHashTable.h"
#include "iQueue.h"
#include "iStack.h"
#include "iString.h"

#include "iImageReader.h"
#include "iAssetReader.h"
#include "iInputManager.h"
#include "iTime.h"
#include "iTransform.h"
#include "iCamera.h"

#include "iOpenGL.h"
#include "iWindows.h"

#define ToRadian(degree) degree * M_PI / 180
#define ToDegree(radian) radian * 180 / M_PI
#define formattedText(buffer, formmat) va_list ap; \
									   va_start(ap, formmat); \
									   ivsprintf(buffer, formmat, ap); \
									   va_end(ap)

float isin(float degree);
float icos(float degree);
float itan(float degree);
int random();
unsigned int nextPow2(unsigned int v);
unsigned int nextPrime(unsigned int v);
bool isPrime(unsigned int v);

char* readFile(const char* path);

void bubbleSort(CompareMethod method, void* data, int elementSize, int num);

void swap(void* left, void* right, int argSize);

void printBit(unsigned int v);
char* toString(int v);
char* toString(float v);
void ivsprintf(char* buffer, const char* str, va_list ap);

bool isSystemLittleEndian();
void IEEE754(float v, int& exponent, float& fraction);
