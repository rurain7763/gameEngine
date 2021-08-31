#pragma once

#include "iType.h"
#include "iArray.h"

#define ASSIMP_LOAD_FLAGS aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices

struct iGLMesh;

iGLMesh** loadAsset(const char* path);

