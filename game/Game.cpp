#include"Game.h"
#include"iStd.h"

iInputManager* inputMgt;
iTime* timeMgt;
iCamera* camera;
bool cameraMode;
iSize* devSize;
iAssetReader* assetReader;
iThreadPool* threadPool;

GLuint vao;
GLuint vbo;
GLuint ebo;

iMatrix projMat;
iGLTexturePTR tex;
iGLShader* shader;
iGLModel* model;
iGLLighting* lighting;

void threadTest1()
{
	printf("threadTest1\n");
}

void threadTest2(void* arg)
{
	printf("threadTest2 %s\n", (const char*)arg);
}

void threadTest3(void* arg1, void* arg2)
{
	printf("threadTest3 %s %s\n", (const char*)arg1, (const char*)arg2);
}

void loadGame()
{
	isSystemLittleEndian();
	srand(time(NULL));

	inputMgt = iInputManager::share();
	timeMgt = iTime::share();

	devSize = new iSize;
	devSize->width = DEV_WIDTH;
	devSize->height = DEV_HEIGHT;

	assetReader = iAssetReader::share();
	threadPool = iThreadPool::share();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(iVertexPCU) * 8, NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader = iGLShader::share();
	shader->addProgram("test", "test");

	projMat.loadIdentity();
	projMat.frustrum(60.f, devSize->width, devSize->height, 1.f, 100.f);

	camera = new iCamera(*devSize, { 0.f, 0.f, -5.f });
	cameraMode = false;

	tex = new iGLTexture();
	tex.get()->load(GL_TEXTURE_2D, "assets/test/2.jfif");

	model = assetReader->loadGLAsset("assets/test/back/backpack.obj");

	lighting = new iGLLighting();
}

void drawGame()
{
	static iVector3f boxPos;
	static iVector3f testModelPos;
	static iVector3f origin = { 0.f, 0.f, 0.f };

	static float degree = 0.f;
	degree += timeMgt->deltaTime * 50.f;

	static float time = 0;
	time += timeMgt->deltaTime;

	if (time >= 3.f)
	{
		time = 0.f;
		threadPool->addJob(threadTest1);
		threadPool->addJob(threadTest2, (void*)"say");
		threadPool->addJob(threadTest3, (void*)"hello", (void*)"world");
	}

	lighting->setDirectionalLight({	.1f, .1f, .1f }, origin - boxPos, 0.1f);
	lighting->setPointLight(0, { 1.f, 1.f, 1.f }, boxPos, 1.f, 1.f, 1.f);
	lighting->setPointLight(1, { 1.f, 1.f, 1.f }, { isin(degree) * 3.f, icos(degree) * 3.f, 0.f}, 1.f, 1.f, 1.f);
	lighting->setSpotLight(0, { 1.f, 1.f, 1.f }, camera->position, camera->lookAt, 30.f, .3f, .5f, .2f);

	iMatrix viewMat = camera->getMatrix();

	iTransform transMat;
	testModelPos = { isin(degree) * 3.f, 0.f, 0.f };
	//transMat.translate(testModelPos.x, testModelPos.y, testModelPos.z);
	//transMat.scale(isin(degree) < 0 ? isin(degree) * - 1 : isin(degree), 0.4f, 1.6f);
	//transMat.rotate(0, degree, 0);

	for (int i = 0; i < 1; i++)
	{
		if(model)
			model->draw(&projMat, camera, &transMat, lighting);
	}

	GLuint program = shader->useProgram("test", "test");

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	iVertexPCU vertices[] =
	{
		{{.5f,.5,.5f,1.f},{1.f,1.f,1.f,1.f},{0.f,0.f}},
		{{-.5f,.5f,-.5f,1.f},{1.f,1.f,1.f,1.f},{0.f,1.f}},
		{{-.5f,.5f,.5f,1.f},{1.f,1.f,1.f,1.f},{1.f,0.f}},
		{{.5f,-.5f,-.5f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f}},
		{{-.5f,-.5f,-.5f,1.f},{1.f,1.f,1.f,1.f},{0.f,0.f}},
		{{.5f,.5f,-.5f,1.f},{1.f,1.f,1.f,1.f},{1.f,0.f}},
		{{.5f,-.5f,.5f,1.f},{1.f,1.f,1.f,1.f},{0.f,1.f}},
		{{-.5f,-.5f,.5f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f}}
	};

	GLubyte indices[] = { 0,1,2, 1,3,4, 5,6,3, 7,3,6, 2,4,7, 0,7,6, 0,5,1, 1,5,3,
						  5,0,6, 7,4,3, 2,1,4, 0,2,7 };

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

	iTransform transMat1;
	//transMat.scale(isin(degree), 1, 1);
	//transMat1.rotate(0, degree, 0);
	//transMat1.translate(5 + isin(degree), 0, 5.f);
	//boxPos = { icos(degree) * 5.f, 0.f, isin(degree) * 5.f };
	boxPos = { 0.f, icos(degree) * 3.f, isin(degree) * 3.f };
	transMat1.translate(boxPos.x, boxPos.y, boxPos.z);

	iMatrix tvpMat = projMat * viewMat * transMat1.getMatrix();

	GLuint loc = glGetUniformLocation(program, "tvpMat");
	glUniformMatrix4fv(loc, 1, GL_TRUE, tvpMat.getData());

	GLuint posAttr = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(posAttr);
	glVertexAttribPointer(posAttr, 4, GL_FLOAT, GL_FALSE, sizeof(iVertexPCU), (const void*)offsetof(iVertexPCU, position));

	GLuint colAttr = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(colAttr);
	glVertexAttribPointer(colAttr, 4, GL_FLOAT, GL_FALSE, sizeof(iVertexPCU), (const void*)offsetof(iVertexPCU, color));

	GLuint uvAttr = glGetAttribLocation(program, "uv");
	glEnableVertexAttribArray(uvAttr);
	glVertexAttribPointer(uvAttr, 2, GL_FLOAT, GL_FALSE, sizeof(iVertexPCU), (const void*)offsetof(iVertexPCU, uv));

	GLuint sampler0 = glGetUniformLocation(program, "tex");

	tex.get()->bind(GL_TEXTURE0);
	glUniform1i(sampler0, 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);

	transMat1.translate(isin(degree) * 3.f, icos(degree) * 3.f, 0.f);
	tvpMat = projMat* viewMat* transMat1.getMatrix();
	glUniformMatrix4fv(loc, 1, GL_TRUE, tvpMat.getData());
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);

	glDisableVertexAttribArray(posAttr);
	glDisableVertexAttribArray(colAttr);
	glDisableVertexAttribArray(uvAttr);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (inputMgt->keyOnce & KEY_P)
	{
		if (!cameraMode)
		{
			showCursor(false);
			wrapCursor(true);
			cameraMode = true;
		}
		else
		{
			showCursor(true);
			wrapCursor(false);
			cameraMode = false;
		}
	}

	if (cameraMode)
	{
		camera->onKey(inputMgt->keyDown, timeMgt->deltaTime);
		camera->onMouse(inputMgt->mousePos, timeMgt->deltaTime);
	}

	timeMgt->update();
	inputMgt->update();
	threadPool->update();
}

void endGame()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	delete inputMgt;
	delete timeMgt;
	delete devSize;
	delete camera;
	delete shader;
	delete assetReader;
	delete model;

	delete lighting;
	delete threadPool;
}
