#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#if 1
#include "stb_image.h"
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
#include "iMatirx.h"
#include "iQuaternion.h"
#include "iTree.h"
#include "iArray.h"
#include "iSharedPtr.h"

#include "iPngReader.h"
#include "iInputManager.h"
#include "iTime.h"
#include "iTransform.h"
#include "iCamera.h"

#include "iOpenGL.h"
#include "iWindows.h"

#define ToRadian(degree) degree * M_PI / 180
#define ToDegree(radian) radian * 180 / M_PI

float isin(float degree);
float icos(float degree);
float itan(float degree);
int random();

unsigned int nextPow2(unsigned int v);
char* readFile(const char* path);
void bubbleSort(CompareMethod method, void* data, int elementSize, int num);
void swap(void* left, void* right, int argSize);
void printBit(unsigned int v);