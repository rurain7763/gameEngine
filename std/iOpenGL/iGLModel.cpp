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

void iGLModel::draw(iMatrix* proj, iCamera* camera, iTransform* trans, iLighting* lighting)
{
	for (int i = 0; i < meshs.dataNum; i++)
	{
		iGLMesh* mesh = (iGLMesh*)meshs[i];
		mesh->draw(proj, camera, trans, lighting);
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

void iGLMesh::draw(iMatrix* proj, iCamera* camera, iTransform* trans, iLighting* lighting)
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

	if (lighting)
	{
		GLuint transMat = glGetUniformLocation(programID, "transMat");
		glUniformMatrix4fv(transMat, 1, GL_TRUE, trans->getTransMatrix().getData());

		GLuint cameraPosLoc = glGetUniformLocation(programID, "cameraPos");
		glUniform3fv(cameraPosLoc, 1, (GLfloat*)&camera->position);

		int pointLightIdx = 0;
		int spotLightIdx = 0;
		for (int i = 0; i < lighting->activeLights.dataNum; i++)
		{
			iLight* light = (iLight*)lighting->activeLights[i];

			switch (light->flag)
			{
			case DIRECTIONLIGHT:
			{
				iDirectionLight* dirLight = (iDirectionLight*)light;

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
				break;
			}
			case POINTLIGHT:
			{
				iString pointLightStr = "pointLights[?].?";

				iPointLight* pointLight = (iPointLight*)light;
				char idx[2];
				sprintf(idx, "%d", pointLightIdx);
				pointLightIdx++;
				pointLightStr.replace(12, 1, idx);

				pointLightStr.replace(15, 1, "color");
				GLuint pointLightColor = glGetUniformLocation(programID, pointLightStr.str);
				glUniform3fv(pointLightColor, 1, (GLfloat*)&pointLight->color);

				pointLightStr.replace(15, 5, "position");
				GLuint pointLightPos = glGetUniformLocation(programID, pointLightStr.str);
				glUniform3fv(pointLightPos, 1, (GLfloat*)&pointLight->position);

				pointLightStr.replace(15, 8, "ambientIntensity");
				GLuint pointLightAmbientIntens = glGetUniformLocation(programID, pointLightStr.str);
				glUniform1f(pointLightAmbientIntens, pointLight->ambientIntensity);

				pointLightStr.replace(15, 16, "diffuseIntensity");
				GLuint pointLightDiffIntens = glGetUniformLocation(programID, pointLightStr.str);
				glUniform1f(pointLightDiffIntens, pointLight->diffuseIntensity);

				pointLightStr.replace(15, 16, "specularIntensity");
				GLuint pointLightSpecIntens = glGetUniformLocation(programID, pointLightStr.str);
				glUniform1f(pointLightSpecIntens, pointLight->specularIntensity);

				pointLightStr.replace(15, 17, "constant");
				GLuint pointLightConstant = glGetUniformLocation(programID, pointLightStr.str);
				glUniform1f(pointLightConstant, pointLight->attenuation.constant);
				
				pointLightStr.replace(15, 8, "linear");
				GLuint pointLightLinear = glGetUniformLocation(programID, pointLightStr.str);
				glUniform1f(pointLightLinear, pointLight->attenuation.linear);

				pointLightStr.replace(15, 6, "exponential");
				GLuint pointLightExponential = glGetUniformLocation(programID, pointLightStr.str);
				glUniform1f(pointLightExponential, pointLight->attenuation.exponential);
				break;
			}
			case SPOTLIGHT:
			{
				iString spotLightStr = "spotLights[?].?";

				iSpotLight* spotLight = (iSpotLight*)light;
				char idx[2];
				sprintf(idx, "%d", spotLightIdx);
				spotLightIdx++;
				spotLightStr.replace(11, 1, idx);

				spotLightStr.replace(14, 1, "color");
				GLuint spotLightColor = glGetUniformLocation(programID, spotLightStr.str);
				glUniform3fv(spotLightColor, 1, (GLfloat*)&spotLight->color);

				spotLightStr.replace(14, 5, "position");
				GLuint spotLightPos = glGetUniformLocation(programID, spotLightStr.str);
				glUniform3fv(spotLightPos, 1, (GLfloat*)&spotLight->position);

				spotLightStr.replace(14, 8, "dir");
				GLuint spotLightDir = glGetUniformLocation(programID, spotLightStr.str);
				glUniform3fv(spotLightDir, 1, (GLfloat*)&spotLight->dir);

				spotLightStr.replace(14, 3, "maximum");
				GLuint spotLightMax = glGetUniformLocation(programID, spotLightStr.str);
				glUniform1f(spotLightMax, icos(spotLight->degree));

				spotLightStr.replace(14, 7, "ambientIntensity");
				GLuint spotLightAmbientIntens = glGetUniformLocation(programID, spotLightStr.str);
				glUniform1f(spotLightAmbientIntens, spotLight->ambientIntensity);

				spotLightStr.replace(14, 16, "diffuseIntensity");
				GLuint spotLightDiffIntens = glGetUniformLocation(programID, spotLightStr.str);
				glUniform1f(spotLightDiffIntens, spotLight->diffuseIntensity);

				spotLightStr.replace(14, 16, "specularIntensity");
				GLuint spotLightSpecIntens = glGetUniformLocation(programID, spotLightStr.str);
				glUniform1f(spotLightSpecIntens, spotLight->specularIntensity);

				spotLightStr.replace(14, 17, "constant");
				GLuint spotLightConstant = glGetUniformLocation(programID, spotLightStr.str);
				glUniform1f(spotLightConstant, spotLight->attenuation.constant);

				spotLightStr.replace(14, 8, "linear");
				GLuint spotLightLinear = glGetUniformLocation(programID, spotLightStr.str);
				glUniform1f(spotLightLinear, spotLight->attenuation.linear);

				spotLightStr.replace(14, 6, "exponential");
				GLuint spotLightExponential = glGetUniformLocation(programID, spotLightStr.str);
				glUniform1f(spotLightExponential, spotLight->attenuation.exponential);
				break;
			}
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
}
