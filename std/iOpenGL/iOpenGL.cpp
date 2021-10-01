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
	glEnable(GL_DEPTH_TEST);

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

iVector3f* calcNormals(iVector3f* vertices, uint32 numVertices, uint32* indices, uint32 indicesNum)
{
	iVector3f* r;
	r = new iVector3f[numVertices];

	for (int i = 0; i < indicesNum; i += 3)
	{
		uint32 i0 = indices[i + 0];
		uint32 i1 = indices[i + 1];
		uint32 i2 = indices[i + 2];

		iVector3f v0 = vertices[i1] - vertices[i0];
		iVector3f v1 = vertices[i2] - vertices[i0];

		iVector3f normal = v0.cross(v1);
		normal.normalized();

		r[i0] += normal;
		r[i1] += normal;
		r[i2] += normal;
	}

	for (int i = 0; i < numVertices; i++) r[i].normalized();

	return r;
}
