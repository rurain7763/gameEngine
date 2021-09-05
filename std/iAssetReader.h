#pragma once

#include "iType.h"
#include "iArray.h"
#include "iVector.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace Assimp;

#define ASSIMP_LOAD_FLAGS aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
#define ASSIMP_TEXTURETYPE_NUM aiTextureType::aiTextureType_UNKNOWN

struct iGLMesh;
struct iGLModel;

class iAssetReader
{
private:
	static iAssetReader* S;
	iAssetReader();

public:
	virtual ~iAssetReader();

	static iAssetReader* share();
	iGLModel* loadGLAsset(const char* path);

private:
	char* getDirectoryInPath(const char* path);
	char* getFileNameInPath(const char* path);

	void getGLVertices(aiMesh* src, iGLMesh* dst);
	void getGLIndices(aiMesh* src, iGLMesh* dst);
	void getGLMaterial(const char* directory, iGLModel* model, 
					   aiMaterial* src, iGLMesh* dst);

private:
	Importer* imp;
};

