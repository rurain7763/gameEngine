#include"Game.h"
#include"iStd.h"

iInputManager* inputMgt;
iTime* timeMgt;
iCamera* camera;
iPngReader* pngReader;
bool cameraMode;
iSize* devSize;

GLuint vao;
GLuint vbo;
GLuint ebo;
iGLTexturePTR tex;

GLuint vert;
GLuint frag;
GLuint program;

void loadGame()
{
	srand(time(NULL));
	inputMgt = iInputManager::share();
	timeMgt = iTime::share();

	devSize = new iSize;
	devSize->width = DEV_WIDTH;
	devSize->height = DEV_HEIGHT;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(iVertex) * 8, NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vert = createShader("assets/shader/test.vert", VERTEX_SHADER);
	frag = createShader("assets/shader/test.frag", FRAGMENT_SHADER);
	program = createProgram(vert, frag);

	camera = new iCamera(*devSize, { 0.f, 0.f, -5.f });
	cameraMode = false;

#if 1
	pngReader = iPngReader::share();
	pngReader->readPng("assets/test/sample2.png");

	tex = new iGLTexture();
	tex.get()->load(GL_TEXTURE_2D, "assets/test/sample3.png");

	iGLTexturePTR test = new iGLTexturePTR[5];
#endif
}

void drawGame()
{
	static float degree = 0.f;
	degree += timeMgt->deltaTime * 50.f;

	glUseProgram(program);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	iVertex vertices[] =
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

	GLubyte indices[] = { 0,1,2 ,1,3,4, 5,6,3, 7,3,6, 2,4,7, 0,7,6, 0,5,1, 1,5,3,
						  5,0,6, 7,4,3, 2,1,4, 0,2,7 };

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

	iMatrix projMat;
	projMat.loadIdentity();
	projMat.frustrum(60.f, devSize->width, devSize->height, 0.f, 10.f);

	iMatrix viewMat = camera->getMatrix();

	iTransform transMat;
	//transMat.scale(isin(degree), 1, 1);
	transMat.rotate(0, degree, 0);
	transMat.translate(isin(degree), 0, 5.f);

	iMatrix tvpMat = projMat * viewMat * transMat.getMatrix();

	GLuint loc = glGetUniformLocation(program, "tvpMat");
	glUniformMatrix4fv(loc, 1, GL_TRUE, tvpMat.getData());

	GLuint posAttr = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(posAttr);
	glVertexAttribPointer(posAttr, 4, GL_FLOAT, GL_FALSE, sizeof(iVertex), (const void*)offsetof(iVertex, position));

	GLuint colAttr = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(colAttr);
	glVertexAttribPointer(colAttr, 4, GL_FLOAT, GL_FALSE, sizeof(iVertex), (const void*)offsetof(iVertex, color));

	GLuint uvAttr = glGetAttribLocation(program, "uv");
	glEnableVertexAttribArray(uvAttr);
	glVertexAttribPointer(uvAttr, 2, GL_FLOAT, GL_FALSE, sizeof(iVertex), (const void*)offsetof(iVertex, uv));

	GLuint sampler0 = glGetUniformLocation(program, "tex");

	tex.get()->bind(GL_TEXTURE0);
	glUniform1i(sampler0, 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);

	glDisableVertexAttribArray(posAttr);
	glDisableVertexAttribArray(colAttr);
	glDisableVertexAttribArray(uvAttr);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (inputMgt->keyOnce & KEY_P)
	{
		showCursor(false);
		wrapCursor(true);
		cameraMode = true;
	}

	if (inputMgt->keyOnce & KEY_ESCAPE)
	{
		showCursor(true);
		wrapCursor(false);
		cameraMode = false;
	}

	if (cameraMode)
	{
		camera->onKey(inputMgt->keyDown, timeMgt->deltaTime);
		camera->onMouse(inputMgt->mousePos, timeMgt->deltaTime);
	}

	timeMgt->update();
	inputMgt->update();
}

void endGame()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	deleteShader(vert);
	deleteShader(frag);
	deleteProgram(program);

	delete inputMgt;
	delete timeMgt;
	delete devSize;
	delete camera;
	delete pngReader;
}

