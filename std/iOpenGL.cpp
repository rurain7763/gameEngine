#include"iOpenGL.h"
#include"iStd.h"

HGLRC hGlrc;
iRect* viewPort;
iGLShader* iGLShader::S = NULL;

void loadGL(HWND& hwnd)
{
	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	HDC hDc = GetDC(hwnd);

	int  pf;
	pf = ChoosePixelFormat(hDc, &pfd);
	SetPixelFormat(hDc, pf, &pfd);

	hGlrc = wglCreateContext(hDc);
	wglMakeCurrent(hDc, hGlrc);

	printf("[OpenGL] Status : OK \nOpenGL : %s\n", glewGetString(GLEW_VERSION));

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("[glewInit] Error : %s\n", glewGetErrorString(err));
	}
	printf("[glewInit] Status : OK \nGLew : %s\n", glewGetString(GLEW_VERSION));

#if 1 // set openGL version
	if (wglewIsSupported("WGL_ARB_create_context"))
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hGlrc);

		GLint attr[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
			WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		hGlrc = wglCreateContextAttribsARB(hDc, 0, attr);
		wglMakeCurrent(hDc, hGlrc);

		printf("[openGL set version] Status : OK\n");
		printf("New version : GL=%s , Shader=%s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
#endif

	int maxTexUnit = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTexUnit);
	printf("Max texture unit num : %d\n", maxTexUnit);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	viewPort = new iRect;
}

void endGL()
{
	wglDeleteContext(hGlrc);

	delete viewPort;
}

void clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void swapBuffer(HDC& hdc)
{
	SwapBuffers(hdc);
}

iRect caculateViewPort(iSize devSize, iSize rederingSize)
{
	float ratio = devSize.width / devSize.height;

	float h = rederingSize.height;
	float w = h * ratio;
	float posX, posY;

	if (w < rederingSize.width)
	{
		posX = (rederingSize.width - w) / 2.f;
		posY = 0;

		return iRect{ posX, posY, w, h };
	}

	w = rederingSize.width;
	h = w / ratio;

	if (h < rederingSize.height)
	{
		posX = 0;
		posY = (rederingSize.height - h) / 2.f;

		return iRect{ posX, posY, w, h };
	}

	return iRect{ 0.f, 0.f, 0.f, 0.f };
}

void setViewPort(int x, int y, int width, int height)
{
	viewPort->x = x;
	viewPort->y = y;
	viewPort->width = width;
	viewPort->height = height;

	glViewport(x, y, width, height);
}

iRect getViewPort()
{
	return *viewPort;
}

void coordMousePosToViewPort(iSize devSize, float& x, float& y)
{
	iVector2f wRange = { viewPort->x, viewPort->x + viewPort->width };
	iVector2f hRange = { viewPort->y, viewPort->y + viewPort->height };

	x = ((x - wRange.x) / (wRange.y - wRange.x)) * devSize.width;
	y = ((y - hRange.x) / (hRange.y - hRange.x)) * devSize.height;
}

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
	if(texID != 0) glDeleteTextures(1, &texID);
}

void iGLTexture::load(GLenum type, const char* path)
{	
	iPng* png = readPng(path);
	pow2Width = nextPow2(png->width);
	pow2Height = nextPow2(png->height);

	texType = type;
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
	else if(png->colorType == 2)
		glTexImage2D(texType, 0, GL_RGB, png->width, png->height, 0, GL_RGB,
					 GL_UNSIGNED_BYTE, png->rgba);
	else if(png->colorType == 6)
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

void iGLTexture::load(GLenum type, GLint format, uint8* pixels, int w, int h)
{
	if (!pixels) return;

	texType = type;

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

iGLShader::iGLShader()
{
	program = new iHashTable();
	shader = new iHashTable();
}

iGLShader::~iGLShader()
{
	for (int i = 0; i < ids.dataNum; i++)
	{
		iGLShaderInfo* info = (iGLShaderInfo*)ids[i];
		
		if (info->flag) deleteProgram(info->id);
		else deleteShader(info->id);

		delete info;
	}

	delete shader;
	delete program;
}

iGLShader* iGLShader::share()
{
	if (!S) S = new iGLShader();
	return S;
}

void iGLShader::addProgram(const char* vs, const char* fs)
{
	GLuint* vertID = (GLuint*)shader->at(vs);
	GLuint* fragID = (GLuint*)shader->at(fs);
	char path[100];
	bool shouldMake = false;

	if (!vertID)
	{
		sprintf(path, "assets/shader/%s.vert", vs);

		iGLShaderInfo* info = new iGLShaderInfo;
		info->flag = 0;
		info->id = createShader(path, VERTEX_SHADER);
		vertID = &info->id;

		shader->insert(vs, &info->id);
		ids.push_back(info);
		shouldMake = true;
	}
	
	if (!fragID)
	{
		sprintf(path, "assets/shader/%s.frag", fs);

		iGLShaderInfo* info = new iGLShaderInfo;
		info->flag = 0;
		info->id = createShader(path, FRAGMENT_SHADER);
		fragID = &info->id;

		shader->insert(fs, &info->id);
		ids.push_back(info);
		shouldMake = true;
	}

	if (shouldMake)
	{
		iGLShaderInfo* info = new iGLShaderInfo;
		info->flag = 1;
		info->id = createProgram(*vertID, *fragID);

		char str[50];
		sprintf(str, "%s/%s", vs, fs);

		program->insert(str, &info->id);
		ids.push_back(info);
	}
}

GLuint iGLShader::useProgram(const char* vs, const char* fs) const
{
	char str[50];
	sprintf(str, "%s/%s", vs, fs);

	GLuint* programID = (GLuint*)program->at(str);

	if (programID)
	{
		glUseProgram(*programID);
		return *programID;
	}

	return 0;
}

GLuint iGLShader::createShader(const char* path, Flag flag)
{
	char* str = readFile(path);
	if (!str) return -1;

	GLenum f[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	GLuint id = glCreateShader(f[flag]);
	int len = strlen(str);
	glShaderSource(id, 1, &str, &len);
	glCompileShader(id);

	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int errLen = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errLen);

		char* error = new char[errLen + 1];
		glGetShaderInfoLog(id, errLen, NULL, error);
		printf("Error compiling shader type: '%s'\n", error);
		delete[] error;

		delete[] str;
		return -1;
	}

	delete[] str;
	return id;
}

void iGLShader::deleteShader(GLuint id)
{
	glDeleteShader(id);
}

GLuint iGLShader::createProgram(GLuint vert, GLuint frag)
{
	GLuint id = glCreateProgram();
	glAttachShader(id, vert);
	glAttachShader(id, frag);
	glLinkProgram(id);
	glDetachShader(id, vert);
	glDetachShader(id, frag);

	GLint success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		int errLen = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &errLen);

		char* error = new char[errLen + 1];
		glGetProgramInfoLog(id, errLen, NULL, error);
		printf("Error linking shader program: '%s'\n", error);
		delete[] error;

		return -1;
	}

	return id;
}

void iGLShader::deleteProgram(GLuint program)
{
	glDeleteProgram(program);
}


