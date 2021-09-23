#pragma once

#include "GL/glew.h"

#include "iType.h"
#include "iTransform.h"
#include "iMatrix.h"
#include "iCamera.h"
#include "iArray.h"
#include "iLighting.h"

class iGLMesh;
class iHashTable;

struct iGLMaterial
{
	iVector3f ambient;
	iVector3f specular;
	//...
};

class iGLModel
{
public:
	iGLModel();
	virtual ~iGLModel();

	void addMesh(iGLMesh* mesh);
	void draw(iMatrix* proj, iCamera* camera, iTransform* trans, iLighting* lighting = NULL);

public:
	iTransform transform;
	iHashTable* textures;
	iArray meshs;
};

class iGLMesh
{
public:
	iGLMesh();
	virtual ~iGLMesh();

	void sendToBuffer();
	void draw(iMatrix* proj, iCamera* camera, iTransform* trans, iLighting* lighting = NULL);

public:
	iVertexPNU* vertices;
	int numVertices;

	uint32* indices;
	int numIndices;

	iArray* textures;
	iGLMaterial material;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};



