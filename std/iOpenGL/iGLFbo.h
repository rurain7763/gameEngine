#pragma once

#include "gl/glew.h"

#include "iType.h"
#include "iStack.h"

#define BINDINGFBO_MAX_STACK_SIZE 30

#define COLOR_FBO  0
#define DEPTH_FBO  1

#define TEMPTEXS_MAX_STACK_SIZE 30

class iGLTexture;

class iGLFbo
{
public:
	iGLFbo(iSize size, uint32 fboFormat, GLenum type = GL_FLOAT,
		   GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR,
		   GLuint wrapS = GL_CLAMP_TO_EDGE, GLuint wrapT = GL_CLAMP_TO_EDGE);

	iGLFbo(uint32 width, uint32 height, uint32 fboFormat, GLenum type = GL_FLOAT,
		   GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR, 
		   GLuint wrapS = GL_CLAMP_TO_EDGE, GLuint wrapT = GL_CLAMP_TO_EDGE);

	virtual ~iGLFbo();

	void bind();
	void unbind();

	void pushTempTexture(iGLTexture* tex);
	void popTempTexture();

	void bindTexture(GLenum texUnit);

private:
	static iStack bindingFbo;
	iStack tempTexs;

public:
	uint32 fboFormat;

	GLuint fbo;
	GLuint texture;
};


