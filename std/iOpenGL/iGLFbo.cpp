#include"iGLFbo.h"
#include"iStd.h"

iStack iGLFbo::bindingFbo(BINDINGFBO_MAX_STACK_SIZE);

iGLFbo::iGLFbo()
{
	depthBuffer = 0;
	stencilBuffer = 0;
	depthStencilBuffer = 0;

	bindStatus = FBOBINDSTATUS_UNBIND;
	glGenFramebuffers(1, &fbo);
}

iGLFbo::~iGLFbo()
{
	for (int i = 0; i < colorBuffer.num; i++)
	{
		GLuint* tex = (GLuint*)colorBuffer[i];
		glDeleteTextures(1, tex);
		delete tex;
	}

	if (depthBuffer) glDeleteTextures(1, &depthBuffer);
	if (stencilBuffer) glDeleteTextures(1, &stencilBuffer);
	if (depthStencilBuffer) glDeleteTextures(1, &depthStencilBuffer);

	glDeleteFramebuffers(1, &fbo);
}

void iGLFbo::addColorBuffer(int w, int h, GLenum type,
							GLint minFilter, GLint magFilter,
							GLuint wrapS, GLuint wrapT)
{
	uint32 attach = GL_COLOR_ATTACHMENT0 + colorBuffer.num;

	if (attach == GL_MAX_COLOR_ATTACHMENTS) return;

	GLuint* tex = new GLuint;

	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	bind();

	glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, *tex, 0);

	colorBuffer.push_back(tex);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("bind colorbuffer to fbo failed!\n");
	}

	unbind();
}

void iGLFbo::addDepthBuffer(int w, int h, GLenum type,
							GLint minFilter, GLint magFilter,
							GLuint wrapS, GLuint wrapT)
{
	if (depthBuffer != 0) return;

	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	bind();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("bind depthbuffer to fbo failed!\n");
	}

	unbind();
}

void iGLFbo::addStencilBuffer()
{
	//TODO
}

void iGLFbo::addDepthStencilBuffer()
{
	//TODO
}

void iGLFbo::bind()
{
	bindingFbo.push(this);

	bindStatus = FBOBINDSTATUS_BOTH;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void iGLFbo::bindForDraw()
{
	bindingFbo.push(this);

	bindStatus = FBOBINDSTATUS_DRAW;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void iGLFbo::unbind()
{
	bindingFbo.pop();
	bindStatus = FBOBINDSTATUS_UNBIND;

	iGLFbo* currFbo = (iGLFbo*)bindingFbo.topData();

	if (currFbo)
	{
		if (currFbo->bindStatus == FBOBINDSTATUS_DRAW)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currFbo->fbo);
		}
		else if (currFbo->bindStatus == FBOBINDSTATUS_WRITE)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, currFbo->fbo);
		}
		else if (currFbo->bindStatus == FBOBINDSTATUS_BOTH)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, currFbo->fbo);
		}		
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void iGLFbo::bindTexture(GLenum texUnit, iGLFboBufferType type, uint8 idx)
{
	glActiveTexture(texUnit);

	switch (type)
	{
	case iGLFboBufferTypeColor:
	{
		GLuint* tex = (GLuint*)colorBuffer[idx];
		if(tex) glBindTexture(GL_TEXTURE_2D, *tex);
		break;
	}
	case iGLFboBufferTypeDepth:
	{
		if(depthBuffer) glBindTexture(GL_TEXTURE_2D, depthBuffer);
		break;
	}
	case iGLFboBufferTypeStencil:
	{
		if(stencilBuffer) glBindTexture(GL_TEXTURE_2D, stencilBuffer);
		break;
	}
	case iGLFboBufferTypeDepthStencil:
	{
		if (depthStencilBuffer) glBindTexture(GL_TEXTURE_2D, depthStencilBuffer);
		break;
	}
	}
}



