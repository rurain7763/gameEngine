#include "KoreanAutomataScene.h"
#include "Game.h"
#include "iStd.h"

void KoreanAutomataScene::load(iArray* recvInfo)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ortho.loadIdentity();
	ortho.ortho(0, devSize->width, devSize->height, 0, 0, 1);

	g = new iGraphics(*devSize);

	g->pushStat();
	g->color = { 1, 1, 1, 1 };
	g->clear();

	g->pushStat();

	g->font = "assets/test/ASMAN.TTF";
	g->color = { 1, 0, 0, 1 };
	g->size = 32;
	g->drawString("123456789", { 0, 0 });

	g->pushStat();

	g->font = "assets/test/BMEULJIROTTF.ttf";
	g->color = { 0, 1, 0, 1 };
	g->size = 44;
	g->drawString("123456789", { 300, 0 });

	g->pushStat();

	g->font = "assets/test/BMJUA_ttf.ttf";
	g->color = { 0, 0, 1, 1 };
	g->size = 56;
	g->drawString("안녕하세요 저는 ... 입니다", { 800, 800 });

	g->popStat();
	g->popStat();
	g->popStat();
	g->popStat();

	iImage* img = g->getiImage();

	tex = new iGLTexture();
	tex->load(GL_TEXTURE_2D, GL_RGBA, img->pixelData, img->width, img->height);

	delete img;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(iVertexPCU) * 4, NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void KoreanAutomataScene::update(float dt)
{
	static float rot = 0.f;
	//rot += dt;

	float w = devSize->width;
	float h = devSize->height;

	float halfW = w / 2.f;
	float halfH = h / 2.f;

	iMatrix viewMat;
	viewMat.loadIdentity();

	iTransform trans;
	trans.translate(halfW, halfH, 0);
	trans.rotate(0, 0, rot * 5.f);

	GLuint program = shader->useProgram("test", "test");

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	iVertexPCU vertices[] =
	{
#if 0
		{{0.f,0.f,0.f,1.f},{1.f,1.f,1.f,1.f},{0.f,0.f}},
		{{0,h,0.f,1.f},{1.f,1.f,1.f,1.f},{0.f,1.f}},
		{{w,0.f,0.f,1.f},{1.f,1.f,1.f,1.f},{1.f,0.f}},
		{{w,h,0.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f}}
#else
		{{-halfW, -halfH, 0.f, 1.f},{1.f,1.f,1.f,1.f},{0.f,0.f}},
		{{-halfW, halfH, 0.f, 1.f},{1.f,1.f,1.f,1.f},{0.f,1.f}},
		{{halfW, -halfH, 0.f, 1.f},{1.f,1.f,1.f,1.f},{1.f,0.f}},
		{{halfW, halfH, 0.f, 1.f},{1.f,1.f,1.f,1.f},{1.f,1.f}}
#endif
	};

	uint8 indices[] = {0, 2, 1 , 1, 2, 3};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

	iMatrix tvpMat = ortho * viewMat * trans.getMatrix();

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

	tex->bind(GL_TEXTURE0);
	glUniform1i(sampler0, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	glDisableVertexAttribArray(posAttr);
	glDisableVertexAttribArray(colAttr);
	glDisableVertexAttribArray(uvAttr);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (inputMg->keyOnce & KEY_1)
	{
		sceneMg->changeScene((uint32)0);
	}
	else if (inputMg->keyOnce & KEY_2)
	{
		sceneMg->changeScene((uint32)1);
	}
	else if (inputMg->keyOnce & KEY_3)
	{
		sceneMg->changeScene((uint32)2);
	}
}

void KoreanAutomataScene::free()
{
	glDisable(GL_BLEND);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	delete tex;
	delete g;
}

