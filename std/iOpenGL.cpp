#include"iOpenGL.h"
#include"iStd.h"

HGLRC hGlrc;
iRect* viewPort;

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

void initWnd()
{
	viewPort = new iRect;
	viewPort->position.x = 0;
	viewPort->position.y = 0;
	viewPort->size.width = 1024.f;
	viewPort->size.height = 768.f;
}

void setViewPort(int x, int y, int width, int height)
{
	viewPort->position.x = x;
	viewPort->position.y = y;
	viewPort->size.width = width;
	viewPort->size.height = height;

	glViewport(x, y, width, height);
}

iRect getViewPort()
{
	return *viewPort;
}

GLuint createShader(const char* path, bool flag)
{
	char* str = readFile(path);
	if (!str) return -1;

	GLenum f[] = { GL_VERTEX_SHADER,GL_FRAGMENT_SHADER };
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

		char* error = new char[errLen+1];
		glGetShaderInfoLog(id, errLen, NULL, error);
		printf("Error compiling shader type: '%s'\n", error);
		delete[] error;

		delete[] str;
		return -1;
	}

	delete[] str;
	return id;
}

void deleteShader(GLuint id)
{
	glDeleteShader(id);
}

GLuint createProgram(GLuint vert, GLuint frag)
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

void deleteProgram(GLuint program)
{
	glDeleteProgram(program);
}

