#pragma once

#include "iType.h"
#include "iArray.h"
#include "iVector.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace Assimp;

#define ASSIMP_LOAD_FLAGS aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices

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
	void getGLVertices(aiMesh* src, iGLMesh* dst);
	void getGLIndices(aiMesh* src, iGLMesh* dst);

private:
	Importer* imp;
};

