#pragma once

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")

#include "iType.h"
#include "iGLModel.h"
#include "iGLShader.h"
#include "iGLTexture.h"

void loadGL(HWND& hwnd);
void endGL();
void clear();
void swapBuffer(HDC& hdc);

iRect caculateViewPort(iSize devSize, iSize rederingSize);
void setViewPort(int left, int top, int width, int height);
iRect getViewPort();
void coordMousePosToViewPort(iSize devSize, float& x, float& y);

iVector3f* calcNormals(iVector3f* vertices, uint32 numVertices, uint32* indices, uint32 indicesNum);



