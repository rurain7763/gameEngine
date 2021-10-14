#pragma once

#include "gl/glew.h"

#include "iType.h"
#include "iGLModel.h"
#include "iGLFbo.h"
#include "iArray.h"

class iLighting;
struct iLight;

class iGLShadow
{
public:
	iGLShadow(iSize devSize);
	virtual ~iGLShadow();

	void updateDepthBuffer(iMatrix* proj, iCamera* camera, iTransform* trans, 
						   iGLMesh* mesh, iLight* light);

private:
	iGLFbo shadowFbo;

	iArray shadowTexture;
};