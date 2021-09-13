#include "iGLTexture.h"
#include "iStd.h"

iGLTexture::iGLTexture()
{
	texType = 0;
	texID = 0;
	width = 0;
	height = 0;
	pow2Width = 0;
	pow2Height = 0;
}

iGLTexture::~iGLTexture()
{
	if (texID != 0) glDeleteTextures(1, &texID);
}

void iGLTexture::load(GLenum tt, const char* path, iGLTexMapType mt)
{
	char* t = getImageType(path);

	if (!strcmp(t, "png"))
	{
		iPng* png = readPng(path);
		if (!png)
		{
			delete[] t;
			return;
		}
		width = png->width;
		height = png->height;
		pow2Width = nextPow2(png->width);
		pow2Height = nextPow2(png->height);

		texType = tt;
		mapType = mt;
		glGenTextures(1, &texID);
		glBindTexture(texType, texID);

		/*
		Color    Allowed    Interpretation
		Type    Bit Depths

		0       1,2,4,8,16  Each pixel is a grayscale sample.

		2       8,16        Each pixel is an R,G,B triple.

		3       1,2,4,8     Each pixel is a palette index;
							a PLTE chunk must appear.

		4       8,16        Each pixel is a grayscale sample,
							followed by an alpha sample.

		6       8,16        Each pixel is an R,G,B triple,
							followed by an alpha sample.
		*/

		if (png->colorType == 0)
			glTexImage2D(texType, 0, GL_RED, png->width, png->height, 0, GL_RED,
						 GL_UNSIGNED_BYTE, png->rgba);
		else if (png->colorType == 2)
			glTexImage2D(texType, 0, GL_RGB, png->width, png->height, 0, GL_RGB,
						 GL_UNSIGNED_BYTE, png->rgba);
		else if (png->colorType == 6)
			glTexImage2D(texType, 0, GL_RGBA, png->width, png->height, 0, GL_RGBA,
						 GL_UNSIGNED_BYTE, png->rgba);
		else
		{
			bool exception_occured = 0;
		}

		glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(texType, 0);

		delete png;
	}
	else if (!strcmp(t, "jpg") ||
		!strcmp(t, "jfif") ||
		!strcmp(t, "jpeg"))
	{
		iJpg* jpg = readJpg(path);
		if (!jpg)
		{
			delete[] t;
			return;
		}
		width = jpg->width;
		height = jpg->height;
		pow2Width = nextPow2(jpg->width);
		pow2Height = nextPow2(jpg->height);

		texType = tt;
		mapType = mt;
		glGenTextures(1, &texID);
		glBindTexture(texType, texID);

		glTexImage2D(texType, 0, GL_RGB, jpg->width, jpg->height, 0, GL_RGB,
					 GL_UNSIGNED_BYTE, jpg->rgb);

		glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(texType, 0);

		delete jpg;
	}
	else
	{
		int exception = 10;
	}

	delete[] t;
}

void iGLTexture::load(GLenum tt, GLint format,
	uint8* pixels, int w, int h, iGLTexMapType mt)
{
	if (!pixels) return;

	texType = tt;
	mapType = mt;

	glGenTextures(1, &texID);
	glBindTexture(texType, texID);

	width = w;
	height = h;
	pow2Width = nextPow2(width);
	pow2Height = nextPow2(height);

	glTexImage2D(texType, 0, format, pow2Width, pow2Height, 0, format,
				 GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(texType, 0);
}

void iGLTexture::bind(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(texType, texID);
}

void iGLTexture::setTexParmi(GLenum name, GLint parm)
{
	glBindTexture(texType, texID);

	glTexParameteri(texType, name, parm);
	glTexParameteri(texType, name, parm);

	glBindTexture(texType, 0);
}
