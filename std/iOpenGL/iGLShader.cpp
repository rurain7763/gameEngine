#include "iGLShader.h"
#include "iStd.h"

iGLShader* iGLShader::S = NULL;

iGLShader::iGLShader()
{
	program = new iHashTable();
	shader = new iHashTable();
}

iGLShader::~iGLShader()
{
	for (iHashTable::iIterator itr = program->begin();
		itr != program->end(); itr++)
	{
		GLuint* id = (GLuint*)itr->data;
		deleteProgram(*id);
		delete id;
	}

	for (iHashTable::iIterator itr = shader->begin();
		itr != shader->end(); itr++)
	{
		GLuint* id = (GLuint*)itr->data;
		deleteShader(*id);
		delete id;
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

GLuint iGLShader::useProgram(const char* vs, const char* fs)
{
	char str[50];
	sprintf(str, "%s/%s", vs, fs);

	GLuint* programID = (GLuint*)program->at(str);

	if (!programID)
	{
		addProgram(vs, fs);
		programID = (GLuint*)program->at(str);
	}

	glUseProgram(*programID);
	currUseProgram = *programID;

	return *programID;
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
