#include"iGLFbo.h"
#include"iStd.h"

iGLFbo::iGLFbo(iSize size, uint32 fF, GLenum type,
			   GLint minFilter, GLint magFilter,
			   GLuint wrapS, GLuint wrapT)
{
	GLenum format[2] = { GL_RGBA, GL_DEPTH_COMPONENT };
	fboFormat = fF;

	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format[fF], size.width, size.height, 0, format[fF], type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (fboFormat == COLOR_FBO)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	else if (fboFormat == DEPTH_FBO)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, texture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("create fbo failed! satus : 0x%x\n", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

iGLFbo::iGLFbo(uint32 width, uint32 height, uint32 fF, GLenum type,
			   GLint minFilter, GLint magFilter,
			   GLuint wrapS, GLuint wrapT)
{
	GLenum format[2] = { GL_RGBA, GL_DEPTH_COMPONENT };
	fboFormat = fF;

	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format[fF], width, height, 0, format[fF], type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (fboFormat == COLOR_FBO)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	else if (fboFormat == DEPTH_FBO)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("create fbo failed! satus : 0x%x\n", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

iGLFbo::~iGLFbo()
{
	glDeleteTextures(1, &texture);
	glDeleteFramebuffers(1, &fbo);
}
