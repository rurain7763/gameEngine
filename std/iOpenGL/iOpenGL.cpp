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

iGLShader::iGLShader()
{
	program = new iHashTable
	(
		[](void* data)
		{
			GLuint* id = (GLuint*)data;
			deleteProgram(*id);
		}
	);
	shader = new iHashTable
	(
		[](void* data)
		{
			GLuint* id = (GLuint*)data;
			deleteShader(*id);
		}
	);
}

iGLShader::~iGLShader()
{
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

		GLuint* id = new GLuint;
		*id = createShader(path, VERTEX_SHADER);
		vertID = id;

		sprintf(path, "%s.vert", vs);
		shader->insert(path, id);
		shouldMake = true;
	}
	
	if (!fragID)
	{
		sprintf(path, "assets/shader/%s.frag", fs);

		GLuint* id = new GLuint;
		*id = createShader(path, FRAGMENT_SHADER);
		fragID = id;

		sprintf(path, "%s.frag", vs);
		shader->insert(path, id);
		shouldMake = true;
	}

	if (shouldMake)
	{
		GLuint* id = new GLuint;
		*id = createProgram(*vertID, *fragID);

		char str[50];
		sprintf(str, "%s/%s", vs, fs);

		program->insert(str, id);
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

iGLModel::iGLModel()
{
	meshs.resize(10);

	textures = new iHashTable
	(10, 
		[](void* tex)
		{
			iGLTexture* t = (iGLTexture*)tex;
			delete t;
		}
	);
}

iGLModel::~iGLModel()
{
	for (int i = 0; i < meshs.dataNum; i++)
		delete (iGLMesh*)meshs[i];
	
	delete textures;
}

void iGLModel::addMesh(iGLMesh* mesh)
{
	meshs.push_back(mesh);
}

void iGLModel::draw(iMatrix* tvpMat, iLight* light)
{
	for (int i = 0; i < meshs.dataNum; i++)
	{
		iGLMesh* mesh = (iGLMesh*)meshs[i];
		mesh->draw(tvpMat, light);
	}
}

iGLMesh::iGLMesh()
{
	vertices = NULL;
	numVertices = 0;

	indices = NULL;
	numIndices = 0;

	textures = new iArray(10);

	vao = 0;
	vbo = 0;
	ebo = 0;
}

iGLMesh::~iGLMesh()
{
	if(vertices) delete[] vertices;
	if(indices) delete[] indices;
	delete textures;

	if (vao != 0) glDeleteVertexArrays(1, &vao);
	if (vbo != 0) glDeleteBuffers(1, &vbo);
	if (ebo != 0) glDeleteBuffers(1, &ebo);
}

void iGLMesh::sendToBuffer()
{
	if (!vertices || !indices) return;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(iVertexPNU) * numVertices, vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * numIndices, indices, GL_STATIC_DRAW);

	delete[] vertices;
	delete[] indices;
	vertices = NULL;
	indices = NULL;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void iGLMesh::draw(iMatrix* tvpMat, iLight* light)
{
	if (vao == 0) return;

	iGLShader* shader = iGLShader::share();
	GLuint programID = shader->useProgram("asset", "asset");

	if (!programID)
	{
		shader->addProgram("asset", "asset");
		programID = shader->useProgram("asset", "asset");
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	GLuint mat = glGetUniformLocation(programID, "tvpMat");
	glUniformMatrix4fv(mat, 1, GL_TRUE, tvpMat->getData());

	if (light)
	{
		switch (light->flag)
		{
		case DIRECTIONLIGHT:
		{
			GLuint transMat = glGetUniformLocation(programID, "transMat");
			iMatrix trans;
			trans.loadIdentity();
			trans.translate({ light->position.x, light->position.y, light->position.z });
			glUniformMatrix4fv(transMat, 1, GL_TRUE, trans.getData());

			GLuint dirLightColor = glGetUniformLocation(programID, "dirLight.color");
			glUniform3fv(dirLightColor, 1, (GLfloat*)&light->color);

			GLuint dirLightIntensity = glGetUniformLocation(programID, "dirLight.intensity");
			glUniform1f(dirLightIntensity, light->intensity);

			GLuint dirLightDir = glGetUniformLocation(programID, "dirLight.dir");
			glUniform3fv(dirLightDir, 1, (GLfloat*)&light->dir);

			GLuint dirLightDiffIntens = glGetUniformLocation(programID, "dirLight.diffuseIntensity");
			glUniform1f(dirLightDiffIntens, light->diffuseIntensity);
			break;
		}
		case SPOTLIGHT:
			break;
		}
	}
	
	GLuint pos = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(iVertexPNU), (const void*)offsetof(iVertexPNU, position));

	GLuint normal = glGetAttribLocation(programID, "normal");
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(iVertexPNU), (const void*)offsetof(iVertexPNU, normal));

	GLuint uv = glGetAttribLocation(programID, "uv");
	glEnableVertexAttribArray(uv);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(iVertexPNU), (const void*)offsetof(iVertexPNU, uv));

	for (int i = 0; i < textures->dataNum; i++)
	{
		iGLTexture* tex = (iGLTexture*)(*textures)[i];

		if (tex->mapType == iGLTexMapTypeNone) continue;

		GLenum unit = GL_TEXTURE0 + i;
		tex->bind(unit);
		GLuint loc;

		switch (tex->mapType)
		{
		case iGLTexMapTypeDiffuse:
		{
			loc = glGetUniformLocation(programID, "diffuse");
			glUniform1i(loc, i);
			break;
		}
		case iGLTexMapTypeSpecular:
		{
			loc = glGetUniformLocation(programID, "specular");
			glUniform1i(loc, i);
			break;
		}
		case iGLTexMapTypeAmbient:
			break;
		case iGLTexMapTypeEmissive:
			break;
		case iGLTexMapTypeHeight:
			break;
		case iGLTexMapTypeNormals:
			break;
		case iGLTexMapTypeShininess:
			break;
		case iGLTexMapTypeOpacity:
			break;
		case iGLTexMapTypeLightMap:
			break;	
		case iGLTexMapTypeReflection:
			break;
		}
	}

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(pos);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(uv);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
