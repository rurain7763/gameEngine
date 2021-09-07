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

	char* dir = getDirectoryInPath(path);
	iGLModel* r = new iGLModel();

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

			if (src->mMaterialIndex >= 0)
			{
				aiMaterial* mat = scene->mMaterials[src->mMaterialIndex];
				getGLMaterial(dir, r, mat, dst);				
			}

			dst->sendToBuffer();
			r->addMesh(dst);
		}

		for (int i = 0; i < curr->mNumChildren; i++)
			q.push(curr->mChildren[i]);
	}

	delete[] dir;
	imp->FreeScene();

	return r;
}

char* iAssetReader::getDirectoryInPath(const char* path)
{
	int len = strlen(path);
	int endPos = len;
	while (endPos > 0)
	{
		endPos--;
		if (path[endPos] == '/') break;
	}

	char* r = new char[endPos + 1];
	memcpy(r, path, sizeof(char) * endPos);
	r[endPos] = 0;

	return r;
}

char* iAssetReader::getFileNameInPath(const char* path)
{
	int len = strlen(path);
	int startPos = len;
	while(startPos > 0)
	{
		startPos--;
		if (path[startPos] == '\\') break;
	}
	
	int rLen = len - startPos;
	char* r = new char[rLen + 1];
	memcpy(r, &path[startPos], sizeof(char) * rLen);
	r[rLen] = 0;

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
		int n = src->mFaces[i].mNumIndices;

		for (int j = 0; j < n; j++)
			r[off + j] = src->mFaces[i].mIndices[j];
		
		off += n;
	}

	dst->indices = r;
	dst->numIndices = num * 3;
}

void iAssetReader::getGLMaterial(const char* dir, iGLModel* model, 
								 aiMaterial* src, iGLMesh* dst)
{
	for (int i = 1; i < ASSIMP_TEXTURETYPE_NUM; i++)
	{
		aiTextureType type = (aiTextureType)i;
		int num = src->GetTextureCount(type);

		for (int j = 0; j < num; j++)
		{
			aiString path;
			src->GetTexture(type, j, &path);
			char* fileName = getFileNameInPath(path.C_Str());

			char myPath[50];
			sprintf(myPath, "%s/%s", dir, fileName);

			iGLTexture* tex = (iGLTexture*)(*model->textures)[myPath];

			if (!tex)
			{
				tex = new iGLTexture();

				printf("%s loading...\n", myPath);
				tex->load(GL_TEXTURE_2D, myPath, (iGLTexMapType)i);
				if (tex->texID != 0)
				{
					printf("%s load success!\n", myPath);
					model->textures->insert(myPath, tex);
				}
				else printf("%s load failed!\n", myPath);
			}

			dst->textures->push_back(tex);

			delete[] fileName;
		}
	}

	dst->textures->resize(dst->textures->dataNum);
}
