#include "iAssetReader.h"
#include "iStd.h"

iAssetReader* iAssetReader::S = NULL;

iAssetReader::iAssetReader()
{
	imp = new Importer();
}

iAssetReader::~iAssetReader()
{
	delete imp;
}

iAssetReader* iAssetReader::share()
{
	if (!S) S = new iAssetReader();
	return S;
}

iGLModel* iAssetReader::loadGLAsset(const char* path)
{
	const aiScene* scene = imp->ReadFile(path, ASSIMP_LOAD_FLAGS);
	
	if (!scene ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode)
	{
		printf("loadAsset() Error : %s \n", imp->GetErrorString());
		return NULL;
	}

	iGLModel* r = new iGLModel(scene->mNumMeshes);

	iQueue q(scene->mNumMeshes);
	q.push(scene->mRootNode);

	while(!q.empty())
	{
		aiNode* curr = (aiNode*)q.pop();

		for (int i = 0; i < curr->mNumMeshes; i++)
		{
			aiMesh* src = scene->mMeshes[curr->mMeshes[i]];
			iGLMesh* dst = new iGLMesh();

			getGLVertices(src, dst);
			getGLIndices(src, dst);

			r->addMesh(dst);
		}

		for (int i = 0; i < curr->mNumChildren; i++)
			q.push(curr->mChildren[i]);
	}

	delete scene;

	return r;
}

void iAssetReader::getGLVertices(aiMesh* src, iGLMesh* dst)
{
	int num = src->mNumVertices;
	iVertexPNU* r = new iVertexPNU[num];

	for (int i = 0; i < num; i++)
	{
		iVertexPNU* vertex = &r[i];

		aiVector3D* vec = &src->mVertices[i];
		aiVector3D* normal = &src->mNormals[i];

		vertex->position = { vec->x, vec->y, vec->z };
		vertex->normal = { normal->x, normal->y, normal->z };

		if (src->mTextureCoords[0])
		{
			aiVector3D* uv = &src->mTextureCoords[0][i];
			vertex->uv = { uv->x, uv->y };
		}
		else vertex->uv = { 0.f, 0.f };
	}

	dst->vertices = r;
	dst->numVertices = num;
}

void iAssetReader::getGLIndices(aiMesh* src, iGLMesh* dst)
{
	int num = src->mNumFaces;
	uint32* r = new uint32[num * 3];
	int off = 0;

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < 3; j++)
			r[off + j] = src->mFaces[i].mIndices[j];
		
		off += 3;
	}

	dst->indices = r;
	dst->numIndices = num;
}