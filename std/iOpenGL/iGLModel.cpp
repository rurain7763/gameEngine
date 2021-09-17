#include "iGLModel.h"
#include "iStd.h"

iGLModel::iGLModel()
{
	meshs.resize(10);

	textures = new iHashTable
	(10,
		[](void* tex)
		{
			iGLTexture* t = (iGLTexture*)tex;
			delete t;
		}
	);
}

iGLModel::~iGLModel()
{
	for (int i = 0; i < meshs.dataNum; i++)
		delete (iGLMesh*)meshs[i];

	delete textures;
}

void iGLModel::addMesh(iGLMesh* mesh)
{
	meshs.push_back(mesh);
}

void iGLModel::draw(iMatrix* proj, iCamera* camera, iTransform* trans, iLight* light)
{
	for (int i = 0; i < meshs.dataNum; i++)
	{
		iGLMesh* mesh = (iGLMesh*)meshs[i];
		mesh->draw(proj, camera, trans, light);
	}
}

iGLMesh::iGLMesh()
{
	vertices = NULL;
	numVertices = 0;

	indices = NULL;
	numIndices = 0;

	textures = new iArray(10);
	material = { 0.f, 0.f, 0.f };

	vao = 0;
	vbo = 0;
	ebo = 0;
}

iGLMesh::~iGLMesh()
{
	if (vertices) delete[] vertices;
	if (indices) delete[] indices;
	delete textures;

	if (vao != 0) glDeleteVertexArrays(1, &vao);
	if (vbo != 0) glDeleteBuffers(1, &vbo);
	if (ebo != 0) glDeleteBuffers(1, &ebo);
}

void iGLMesh::sendToBuffer()
{
	if (!vertices || !indices) return;

	if(vao == 0) glGenVertexArrays(1, &vao);
	if(vbo == 0) glGenBuffers(1, &vbo);
	if(ebo == 0) glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(iVertexPNU) * numVertices, vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * numIndices, indices, GL_STATIC_DRAW);

	delete[] vertices;
	delete[] indices;
	vertices = NULL;
	indices = NULL;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void iGLMesh::draw(iMatrix* proj, iCamera* camera, iTransform* trans, iLight* light)
{
	if (vao == 0) return;

	iGLShader* shader = iGLShader::share();
	GLuint programID = shader->useProgram("asset", "asset");

	if (!programID)
	{
		shader->addProgram("asset", "asset");
		programID = shader->useProgram("asset", "asset");
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	iMatrix tvpMat = (*proj) * camera->getMatrix() * trans->getMatrix();
	GLuint mat = glGetUniformLocation(programID, "tvpMat");
	glUniformMatrix4fv(mat, 1, GL_TRUE, tvpMat.getData());

	if (light)
	{
		switch (light->flag)
		{
		case DIRECTIONLIGHT:
		{
			iDirectionLight* dirLight = (iDirectionLight*)light;

			GLuint transMat = glGetUniformLocation(programID, "transMat");
			glUniformMatrix4fv(transMat, 1, GL_TRUE, trans->getTransMatrix().getData());

			GLuint dirLightColor = glGetUniformLocation(programID, "dirLight.color");
			glUniform3fv(dirLightColor, 1, (GLfloat*)&dirLight->color);

			GLuint dirLightDir = glGetUniformLocation(programID, "dirLight.dir");
			glUniform3fv(dirLightDir, 1, (GLfloat*)&dirLight->dir);

			GLuint dirLightAmbientIntens = glGetUniformLocation(programID, "dirLight.ambientIntensity");
			glUniform1f(dirLightAmbientIntens, dirLight->ambientIntensity);

			GLuint dirLightDiffIntens = glGetUniformLocation(programID, "dirLight.diffuseIntensity");
			glUniform1f(dirLightDiffIntens, dirLight->diffuseIntensity);

			GLuint dirLightSpecIntens = glGetUniformLocation(programID, "dirLight.specularIntensity");
			glUniform1f(dirLightSpecIntens, dirLight->specularIntensity);

			GLuint cameraPosLoc = glGetUniformLocation(programID, "cameraPos");
			glUniform3fv(cameraPosLoc, 1, (GLfloat*)&camera->position);

			break;
		}
		case SPOTLIGHT:
			break;
		}
	}

	GLuint pos = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(iVertexPNU), (const void*)offsetof(iVertexPNU, position));

	GLuint normal = glGetAttribLocation(programID, "normal");
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(iVertexPNU), (const void*)offsetof(iVertexPNU, normal));

	GLuint uv = glGetAttribLocation(programID, "uv");
	glEnableVertexAttribArray(uv);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(iVertexPNU), (const void*)offsetof(iVertexPNU, uv));

	for (int i = 0; i < textures->dataNum; i++)
	{
		iGLTexture* tex = (iGLTexture*)(*textures)[i];

		if (tex->mapType == iGLTexMapTypeNone) continue;

		GLenum unit = GL_TEXTURE0 + i;
		tex->bind(unit);
		GLuint loc;

		switch (tex->mapType)
		{
		case iGLTexMapTypeDiffuse:
		{
			loc = glGetUniformLocation(programID, "diffuse");
			glUniform1i(loc, i);
			break;
		}
		case iGLTexMapTypeSpecular:
		{
			loc = glGetUniformLocation(programID, "specular");
			glUniform1i(loc, i);
			break;
		}
		case iGLTexMapTypeAmbient:
			break;
		case iGLTexMapTypeEmissive:
			break;
		case iGLTexMapTypeHeight:
			break;
		case iGLTexMapTypeNormals:
			loc = glGetUniformLocation(programID, "normal");
			glUniform1i(loc, i);
			break;
		case iGLTexMapTypeShininess:
			break;
		case iGLTexMapTypeOpacity:
			break;
		case iGLTexMapTypeLightMap:
			break;
		case iGLTexMapTypeReflection:
			break;
		}
	}

	GLuint materialAmbientLoc = glGetUniformLocation(programID, "material.ambient");
	glUniform3fv(materialAmbientLoc, 1, (GLfloat*)&material.ambient);

	GLuint materialSpecularLoc = glGetUniformLocation(programID, "material.specular");
	glUniform3fv(materialSpecularLoc, 1, (GLfloat*)&material.specular);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(pos);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(uv);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}