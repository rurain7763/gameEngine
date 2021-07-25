#pragma once

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")

struct iRect;

void loadGL(HWND& hwnd);
void endGL();
void clear();
void swapBuffer(HDC& hdc);
void setViewPort(int left, int top, int width, int height);
iRect getViewPort();

#define VERTEX_SHADER		0
#define FRAGMENT_SHADER		1
GLuint createShader(const char* path, bool flag);
void deleteShader(GLuint id);
GLuint createProgram(GLuint vert,GLuint frag);
void deleteProgram(GLuint program);




