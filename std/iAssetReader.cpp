#include "iAssetReader.h"
#include "iStd.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

iVertexPNU** getVertex(aiMesh* mesh);
uint32* getIndices(aiMesh* mesh);

iGLMesh** loadAsset(const char* path)
{
	Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(path, ASSIMP_LOAD_FLAGS);
	
	if (!scene ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode)
	{
		printf("loadAsset() Error : %s \n", imp.GetErrorString());
		return NULL;
	}

	iQueue q(scene->mNumMeshes);
	q.push(scene->mRootNode);

	while(!q.empty())
	{
		aiNode* curr = (aiNode*)q.pop();

		int num = curr->mNumMeshes;
		for (int i = 0; i < num; i++)
		{
			uint32 idx = curr->mMeshes[i];
			aiMesh* mesh = scene->mMeshes[idx];

			iVertexPNU** vertex = getVertex(mesh);
			uint32* indices = getIndices(mesh);			
		}

		num = curr->mNumChildren;

		for (int i = 0; i < num; i++)
			q.push(curr->mChildren[i]);
	}

	return nullptr;
}

iVertexPNU** getVertex(aiMesh* mesh)
{
	int num = mesh->mNumVertices;
	iVertexPNU** r = new iVertexPNU * [num];

	for (int i = 0; i < num; i++)
	{
		iVertexPNU* vertex = new iVertexPNU;

		aiVector3D* vec = &mesh->mVertices[i];
		aiVector3D* normal = &mesh->mNormals[i];

		vertex->position = { vec->x, vec->y, vec->z };
		vertex->normal = { normal->x, normal->y, normal->z };

		if (mesh->mTextureCoords[0])
		{
			aiVector3D* uv = &mesh->mTextureCoords[0][i];
			vertex->uv = { uv->x, uv->y };
		}
		else vertex->uv = { 0.f, 0.f };

		r[i] = vertex;
	}

	return r;
}

uint32* getIndices(aiMesh* mesh)
{
	int num = mesh->mNumFaces;
	uint32* r = new uint32[num * 3];
	int off = 0;

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < 3; j++)
			r[off + j] = mesh->mFaces[i].mIndices[j];
		
		off += 3;
	}

	return r;
}