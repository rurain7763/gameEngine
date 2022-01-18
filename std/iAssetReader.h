#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace Assimp;

#include "iType.h"
#include "iArray.h"
#include "iVector.h"
#include "iString.h"
#include "iGLTexture.h"
#include "iImageReader.h"

//aiProcess_FlipUVs
#define ASSIMP_LOAD_FLAGS aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices
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
	void asyncLoadGLAsset(const char* path, iGLModel** recieve);

private:
	void getGLVertices(aiMesh* src, iGLMesh* dst);
	void getGLIndices(aiMesh* src, iGLMesh* dst);
	void getGLMaterial(const char* directory, iGLModel* model, 
					   aiMaterial* src, iGLMesh* dst);

private:
	Importer* imp;
};

struct iAsyncLoadGLAssetInfo
{
	iString path;
	iGLTexMapType type;

	iGLTexture* tex;
	iImage* img;
};

void _asyncLoadGLAssetCallBack(void* model);
void* _asyncLoadGLAsset(void* path, void* recieve);