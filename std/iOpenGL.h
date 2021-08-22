#pragma once

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")

#include "iType.h"
#include "iSharedPtr.h"

#define VERTEX_SHADER		0
#define FRAGMENT_SHADER		1

class iGLTexture;
typedef iSharedPtr<iGLTexture>   iGLTexturePTR;
typedef iSharedPtr<iGLTexture[]> iGLTextureARR;

void loadGL(HWND& hwnd);
void endGL();
void clear();
void swapBuffer(HDC& hdc);

GLuint createShader(const char* path, Flag flag);
void deleteShader(GLuint id);
GLuint createProgram(GLuint vert,GLuint frag);
void deleteProgram(GLuint program);

iRect caculateViewPort(iSize devSize, iSize rederingSize);
void setViewPort(int left, int top, int width, int height);
iRect getViewPort();
void coordMousePosToViewPort(iSize devSize, float& x, float& y);

class iGLTexture
{
public:
	iGLTexture();
	virtual ~iGLTexture();

	void load(GLenum texType, const char* path);
	void load(GLenum texType, GLint format, uint8* pixels, int width, int height);
	void bind(GLenum texUnit);

	void setTexParmi(GLenum name, GLint parm);

public:
	GLenum texType;
	GLuint texID;
	int width;
	int height;
	int pow2Width;
	int pow2Height;
};




