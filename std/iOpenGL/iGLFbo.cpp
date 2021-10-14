#include"iGLFbo.h"
#include"iStd.h"

iStack iGLFbo::bindingFbo(BINDINGFBO_MAX_STACK_SIZE);

iGLFbo::iGLFbo(iSize size, uint32 fF, GLenum type,
			   GLint minFilter, GLint magFilter,
			   GLuint wrapS, GLuint wrapT)
	:tempTexs(TEMPTEXS_MAX_STACK_SIZE)
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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

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
	:tempTexs(TEMPTEXS_MAX_STACK_SIZE)
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

void iGLFbo::bind()
{
	bindingFbo.push(&fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void iGLFbo::unbind()
{
	bindingFbo.pop();
	GLuint* currFbo = (GLuint*)bindingFbo.topData();

	if (currFbo) glBindFramebuffer(GL_FRAMEBUFFER, *currFbo);
	else glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void iGLFbo::pushTempTexture(iGLTexture* tex)
{
	tempTexs.push(tex);

	bind();

	if (fboFormat == COLOR_FBO)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);
	else if (fboFormat == DEPTH_FBO)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->texID, 0);

	unbind();
}

void iGLFbo::popTempTexture()
{
	tempTexs.pop();
	iGLTexture* tex = (iGLTexture*)tempTexs.topData();

	bind();

	if (tex)
	{
		if (fboFormat == COLOR_FBO)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);
		else if (fboFormat == DEPTH_FBO)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->texID, 0);
	}
	else
	{
		if (fboFormat == COLOR_FBO)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		else if (fboFormat == DEPTH_FBO)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	}

	unbind();
}

void iGLFbo::bindTexture(GLenum texUnit)
{
	iGLTexture* tex = (iGLTexture*)tempTexs.topData();

	glActiveTexture(texUnit);
	if (tex) glBindTexture(GL_TEXTURE_2D, tex->texID);
	else glBindTexture(GL_TEXTURE_2D, texture);
}
