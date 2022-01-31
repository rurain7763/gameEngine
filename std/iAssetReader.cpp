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

void iAssetReader::asyncLoadGLAsset(const char* path, iGLModel** recieve)
{
	iThreadPool::share()->addJob(_asyncLoadGLAsset, (void*)path, (void*)recieve,
								 _asyncLoadGLAssetCallBack);
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
			// 1.f - uv.y if aiProcess_FlipUVs flag is on because openGL uvs are flip vertically
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

	aiColor3D color;
	if (src->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
	{
		dst->material.ambient = { color.r, color.g, color.b };
	}

	if (src->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
	{
		dst->material.specular = { color.r, color.g, color.b };
	}

	dst->textures->resize(dst->textures->dataNum);
}

void _asyncLoadGLAssetCallBack(void* model)
{
	iGLModel* m = (iGLModel*)model;

	for (iHashTable::iIterator itr = m->textures->begin();
		 itr != m->textures->end(); itr++)
	{
		iAsyncLoadGLAssetInfo* info = (iAsyncLoadGLAssetInfo*)itr->data;
		iGLTexture* tex = info->tex;
		iImage* img = info->img;

		if (img->type == iImageTypePng)
		{
			iPng* png = (iPng*)img;

			if (png->colorType == 0)
				tex->load(GL_TEXTURE_2D, GL_RED, png->pixelData, png->width, png->height, info->type);
			else if (png->colorType == 2)
				tex->load(GL_TEXTURE_2D, GL_RGB, png->pixelData, png->width, png->height, info->type);
			else if (png->colorType == 6)
				tex->load(GL_TEXTURE_2D, GL_RGBA, png->pixelData, png->width, png->height, info->type);
		}
		else if (img->type == iImageTypeJpg ||
				 img->type == iImageTypeJfif ||
				 img->type == iImageTypeJpeg)
		{
			iJpg* jpg = (iJpg*)img;
			tex->load(GL_TEXTURE_2D, GL_RGB, jpg->pixelData, jpg->width, jpg->height, info->type);
		}

		tex->path = info->path;
		delete img;
		delete info;

		itr->data = tex;
	}

	for (int i = 0; i < m->meshs.dataNum; i++)
	{
		((iGLMesh*)m->meshs[i])->sendToBuffer();
	}
}

void* _asyncLoadGLAsset(void* p, void* recieve)
{
	const char* path = (const char*)p;
	Importer imp;
	const aiScene* scene = imp.ReadFile(path, ASSIMP_LOAD_FLAGS);

	if (!scene ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode)
	{
		printf("loadAsset() Error : %s \n", imp.GetErrorString());
		return NULL;
	}

	char* dir = getDirectoryInPath(path);
	iGLModel* r = new iGLModel();

	iQueue q(scene->mNumMeshes);
	q.push(scene->mRootNode);

	while (!q.empty())
	{
		aiNode* curr = (aiNode*)q.pop();

		for (int i = 0; i < curr->mNumMeshes; i++)
		{
			aiMesh* src = scene->mMeshes[curr->mMeshes[i]];
			iGLMesh* dst = new iGLMesh();

			//getGLVertices
			int num = src->mNumVertices;
			iVertexPNU* vertices = new iVertexPNU[num];

			for (int i = 0; i < num; i++)
			{
				iVertexPNU* vertex = &vertices[i];

				aiVector3D* vec = &src->mVertices[i];
				aiVector3D* normal = &src->mNormals[i];

				vertex->position = { vec->x, vec->y, vec->z };
				vertex->normal = { normal->x, normal->y, normal->z };

				if (src->mTextureCoords[0])
				{
					aiVector3D* uv = &src->mTextureCoords[0][i];
					// 1.f - uv.y if aiProcess_FlipUVs flag is on because openGL uvs are flip vertically
					vertex->uv = { uv->x, uv->y };
				}
				else vertex->uv = { 0.f, 0.f };
			}

			dst->vertices = vertices;
			dst->numVertices = num;

			//getGLIndices
			num = src->mNumFaces;
			uint32* indices = new uint32[num * 3];
			int off = 0;

			for (int i = 0; i < num; i++)
			{
				int n = src->mFaces[i].mNumIndices;

				for (int j = 0; j < n; j++)
					indices[off + j] = src->mFaces[i].mIndices[j];

				off += n;
			}

			dst->indices = indices;
			dst->numIndices = num * 3;

			if (src->mMaterialIndex >= 0)
			{
				aiMaterial* mat = scene->mMaterials[src->mMaterialIndex];

				//getGLMaterial
				for (int i = 1; i < ASSIMP_TEXTURETYPE_NUM; i++)
				{
					aiTextureType type = (aiTextureType)i;
					int num = mat->GetTextureCount(type);

					for (int j = 0; j < num; j++)
					{
						aiString p;
						mat->GetTexture(type, j, &p);
						char* fileName = getFileNameInPath(p.C_Str());

						char myPath[50];
						sprintf(myPath, "%s/%s", dir, fileName);

						iAsyncLoadGLAssetInfo* info = (iAsyncLoadGLAssetInfo*)(*r->textures)[myPath];
						iGLTexture* tex = NULL;

						if (!info)
						{
							tex = new iGLTexture();
							iImage* img = NULL;

							printf("%s loading...\n", myPath);

							char* t = getImageType(myPath);

							if (!strcmp(t, "png"))
							{
								img = readPng(myPath);
							}
							else if (!strcmp(t, "jpg") ||
									 !strcmp(t, "jfif") ||
									 !strcmp(t, "jpeg"))
							{
								img = readJpg(myPath);
							}
							else
							{
								int exception = 10;
							}
							
							delete[] t;

							if (img)
							{
								printf("%s load success!\n", myPath);

								iAsyncLoadGLAssetInfo* loadedInfo = new iAsyncLoadGLAssetInfo;
								loadedInfo->path = myPath;
								loadedInfo->type = (iGLTexMapType)i;
								loadedInfo->tex = tex;
								loadedInfo->img = img;

								r->textures->insert(myPath, loadedInfo);
							}
							else printf("%s load failed!\n", myPath);
						}
						else
						{
							tex = info->tex;
						}

						dst->textures->push_back(tex);

						delete[] fileName;
					}
				}

				aiColor3D color;
				if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
				{
					dst->material.ambient = { color.r, color.g, color.b };
				}

				if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
				{
					dst->material.specular = { color.r, color.g, color.b };
				}

				dst->textures->resize(dst->textures->dataNum);
			}

			r->addMesh(dst);
		}

		for (int i = 0; i < curr->mNumChildren; i++)
			q.push(curr->mChildren[i]);
	}

	*(iGLModel**)recieve = r;

	delete[] dir;
	imp.FreeScene();

	return r;
}

