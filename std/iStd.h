#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>

#include "iVector.h"
#include "iSize.h"
#include "iRect.h"
#include "iMatirx.h"
#include "iQuaternion.h"
#include "iTransform.h"
#include "iCamera.h"
#include "iInputManager.h"
#include "iTime.h"
#include "iPngReader.h"

#include "iOpenGL.h"
#include "iWindows.h"

#define ToRadian(degree) degree * M_PI / 180
#define ToDegree(radian) radian * 180 / M_PI

char* readFile(const char* path);
float isin(float degree);
float icos(float degree);
float itan(float degree);
