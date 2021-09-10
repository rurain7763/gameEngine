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
#include "iTransform.h"

#define VERTEX_SHADER		0
#define FRAGMENT_SHADER		1

class iMatrix;
class iHashTable;
class iGLTexture;
class iGLModel;
class iGLMesh;
struct iLight;

typedef iSharedPtr<iGLTexture>   iGLTexturePTR;
typedef iSharedPtr<iGLTexture[]> iGLTextureARR;

enum iGLTexMapType
{
	iGLTexMapTypeNone = 0,
	iGLTexMapTypeDiffuse,
	iGLTexMapTypeSpecular,
	iGLTexMapTypeAmbient,
	iGLTexMapTypeEmissive,
	iGLTexMapTypeHeight,
	iGLTexMapTypeNormals,
	iGLTexMapTypeShininess,
	iGLTexMapTypeOpacity,
	iGLTexMapTypeLightMap,
	iGLTexMapTypeReflection,
	iGLTexMapTypeMax
};

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

	void load(GLenum texType, const char* path, iGLTexMapType mapType = iGLTexMapTypeNone);
	void load(GLenum texType, GLint format, 
			  uint8* pixels, int width, int height, iGLTexMapType mapType = iGLTexMapTypeNone);
	void bind(GLenum texUnit);

	void setTexParmi(GLenum name, GLint parm);

public:
	GLenum texType;
	GLuint texID;

	iGLTexMapType mapType;
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

	//TODO
	//GLuint setUniformMatrix4x(const char* uniformName) const;

	static GLuint createShader(const char* path, Flag flag);
	static void deleteShader(GLuint id);
	static GLuint createProgram(GLuint vert, GLuint frag);
	static void deleteProgram(GLuint program);

public:
	iHashTable* shader;
	iHashTable* program;
};

class iGLModel
{
public:
	iGLModel();
	virtual ~iGLModel();

	void addMesh(iGLMesh* mesh);
	void draw(iMatrix* proj, iCamera* camera, iTransform* trans, iLight* light = NULL);

public:
	iTransform transform;
	iHashTable* textures;
	iArray meshs;
};

class iGLMesh
{
public:
	iGLMesh();
	virtual ~iGLMesh();

	void sendToBuffer();
	void draw(iMatrix* proj, iCamera* camera, iTransform* trans, iLight* light = NULL);

public:
	iVertexPNU* vertices;
	int numVertices;

	uint32* indices;
	int numIndices;

	iArray* textures;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

