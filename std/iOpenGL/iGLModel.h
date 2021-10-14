#pragma once

#include "GL/glew.h"

#include "iType.h"
#include "iTransform.h"
#include "iMatrix.h"
#include "iCamera.h"
#include "iArray.h"
#include "iSharedPtr.h"

class iGLMesh;
class iHashTable;
class iGLModel;
class iGLShadow;
class iGLLighting;

typedef iSharedPtr<iGLModel>   iGLModelPTR;
typedef iSharedPtr<iGLModel[]> iGLModelARR;

class iGLModel
{
public:
	iGLModel();
	virtual ~iGLModel();

	void addMesh(iGLMesh* mesh);
	void removeMesh(iGLMesh* mesh);
	void removeMesh(int idx);

	void draw(iMatrix* proj, iCamera* camera, iTransform* trans, iGLLighting* lighting = NULL, iGLShadow* shadow = NULL);

public:
	iHashTable* textures;
	iArray meshs;
};

struct iGLMaterial
{
	iVector3f ambient;
	iVector3f specular;
	//...
};

class iGLMesh
{
public:
	iGLMesh();
	virtual ~iGLMesh();

	void sendToBuffer();
	void draw(iMatrix* proj, iCamera* camera, iTransform* trans, iGLLighting* lighting = NULL, iGLShadow* shadow = NULL);

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



