#pragma once

#include "gl/glew.h"

#include "iType.h"

#define COLOR_FBO  0
#define DEPTH_FBO  1

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

public:
	uint32 fboFormat;

	GLuint fbo;
	GLuint texture;
};


