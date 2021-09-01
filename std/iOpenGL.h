#pragma once

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")

#include "iType.h"
#include "iArray.h"
#include "iSharedPtr.h"
#include "iVector.h"

#define VERTEX_SHADER		0
#define FRAGMENT_SHADER		1

class iHashTable;
class iGLTexture;
class iGLModel;
class iGLMesh;
typedef iSharedPtr<iGLTexture>   iGLTexturePTR;
typedef iSharedPtr<iGLTexture[]> iGLTextureARR;

void loadGL(HWND& hwnd);
void endGL();
void clear();
void swapBuffer(HDC& hdc);

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

class iGLShader
{
private:
	static iGLShader* S;
	iGLShader();

public:
	virtual ~iGLShader();

	static iGLShader* share();

	void addProgram(const char* vertexShader, const char* fragmentShader);
	GLuint useProgram(const char* vertexShader, const char* fragmentShader) const;

	GLuint setUniformMatrix4x(const char* uniformName) const;

private:
	GLuint createShader(const char* path, Flag flag);
	void deleteShader(GLuint id);
	GLuint createProgram(GLuint vert, GLuint frag);
	void deleteProgram(GLuint program);

private:
	iArray ids;

public:
	iHashTable* shader;
	iHashTable* program;
};

struct iGLShaderInfo
{
	bool flag; // 0 : shader 1 : program
	GLuint id;
};

class iGLModel
{
public:
	iGLModel(uint32 numMeshs);
	virtual ~iGLModel();

	void addMesh(iGLMesh* mesh);
	void draw();

private:
	int num;

public:
	iGLMesh** meshs;
	int numMeshs;
};

class iGLMesh
{
public:
	iGLMesh();
	virtual ~iGLMesh();

	void draw();

public:
	iVertexPNU* vertices;
	int numVertices;

	uint32* indices;
	int numIndices;

	iArray textures;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

