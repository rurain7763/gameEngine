#pragma once

#include "GL/glew.h"

#include "iType.h"

class iHashTable;

#define VERTEX_SHADER		0
#define FRAGMENT_SHADER		1

class iGLShader
{
private:
	static iGLShader* S;
	iGLShader();

public:
	virtual ~iGLShader();

	static iGLShader* share();

	void addProgram(const char* vertexShader, const char* fragmentShader);
	GLuint useProgram(const char* vertexShader, const char* fragmentShader);

	//TODO
	//GLuint setUniformMatrix4x(const char* uniformName) const;

	static GLuint createShader(const char* path, Flag flag);
	static void deleteShader(GLuint id);
	static GLuint createProgram(GLuint vert, GLuint frag);
	static void deleteProgram(GLuint program);

public:
	iHashTable* shader;
	iHashTable* program;

	GLuint currUseProgram;
};

