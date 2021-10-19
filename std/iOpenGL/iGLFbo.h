#pragma once

#include "gl/glew.h"

#include "iType.h"
#include "iStack.h"
#include "iList.h"

#define BINDINGFBO_MAX_STACK_SIZE 30

#define FBOBINDSTATUS_DRAW		 0
#define FBOBINDSTATUS_WRITE		 1
#define FBOBINDSTATUS_BOTH		 2
#define FBOBINDSTATUS_UNBIND	-1

class iGLTexture;

enum iGLFboBufferType
{
	iGLFboBufferTypeColor = 0,
	iGLFboBufferTypeDepth,
	iGLFboBufferTypeStencil,
	iGLFboBufferTypeDepthStencil,
};

class iGLFbo
{
public:
	iGLFbo();
	virtual ~iGLFbo();

	void addColorBuffer(int w, int h, GLenum type = GL_FLOAT,
						GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR,
						GLuint wrapS = GL_CLAMP_TO_EDGE, GLuint wrapT = GL_CLAMP_TO_EDGE);

	void addDepthBuffer(int w, int h, GLenum type = GL_FLOAT,
						GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR,
						GLuint wrapS = GL_CLAMP_TO_EDGE, GLuint wrapT = GL_CLAMP_TO_EDGE);

	void addStencilBuffer();
	void addDepthStencilBuffer();

	void bind();
	void bindForDraw();
	void unbind();

	void bindTexture(GLenum texUnit, iGLFboBufferType type, uint8 idx = 0);

private:
	static iStack bindingFbo;

	iList colorBuffer;
	GLuint depthBuffer;
	GLuint stencilBuffer;
	GLuint depthStencilBuffer;

public:
	uint8 bindStatus;
	GLuint fbo;
};


