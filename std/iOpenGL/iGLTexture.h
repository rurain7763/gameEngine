#pragma once

#include "GL/glew.h"

#include "iType.h"
#include "iSharedPtr.h"
#include "iString.h"

class iGLTexture;

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

	iString path;
	iGLTexMapType mapType;
	int width;
	int height;
	int pow2Width;
	int pow2Height;
};

