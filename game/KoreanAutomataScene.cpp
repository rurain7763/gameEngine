#include "KoreanAutomataScene.h"
#include "Game.h"
#include "iStd.h"

void KoreanAutomataScene::load(iArray* recvInfo)
{
	char* r = iKoreanAutoMata::share()->join("ㅂㅜㅔㄹㄱㅂ");
	delete[] r;

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
	g->drawString("123456789", { 0, 0 });

	g->pushStat();

	g->font = "assets/test/BMEULJIROTTF.ttf";
	g->color = { 0, 1, 0, 1 };
	g->drawString("123456789", { 300, 0 });

	g->pushStat();

	g->font = "assets/test/BMJUA_ttf.ttf";
	g->color = { 0, 0, 1, 1 };
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

#if 1
	ib = new iInputBox();
#endif
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


	g->pushStat();
	g->color = { 1, 1, 1, 1 };
	g->clear();
	
	g->pushStat();
	g->color = { 0, 0, 0, 1 };
	g->font = "arial";//"assets/test/BMEULJIROTTF.ttf";

	ib->draw(dt);
	g->drawString(ib->result, {0, 0});
	delete[] ib->result;

	g->popStat();
	g->popStat();

	iImage* img = g->getiImage();

	if (tex) delete tex;
	tex = new iGLTexture();
	tex->load(GL_TEXTURE_2D, GL_RGBA, img->pixelData, img->width, img->height);

	delete img;

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
#if 1
	delete ib;
#endif

	glDisable(GL_BLEND);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	if(tex) delete tex;
	delete g;
}

iInputBox::iInputBox()
{
	im = iInputManager::share();
	kam = iKoreanAutoMata::share();
	cursor = 0;
}

iInputBox::~iInputBox()
{
}

void iInputBox::updateBuff()
{
	if (im->keyOnce & KEY_w)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅈ";
		else buff += 'w';
	}
	if (im->keyOnce & KEY_a)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅁ";
		else buff += 'a';
	}
	if (im->keyOnce & KEY_s)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㄴ";
		else buff += 's';
	}
	if (im->keyOnce & KEY_d)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅇ";
		else buff += 'd';
	}
	if (im->keyOnce & KEY_q)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅂ";
		else buff += 'q';
	}
	if (im->keyOnce & KEY_e)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㄷ";
		else buff += 'e';
	}
	if (im->keyOnce & KEY_r)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㄱ";
		else buff += 'r';
	}
	if (im->keyOnce & KEY_t)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅅ";
		else buff += 't';
	}
	if (im->keyOnce & KEY_y)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅛ";
		else buff += 'y';
	}
	if (im->keyOnce & KEY_u)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅕ";
		else buff += 'u';
	}
	if (im->keyOnce & KEY_i)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅑ";
		else buff += 'i';
	}
	if (im->keyOnce & KEY_o)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅐ";
		else buff += 'o';
	}
	if (im->keyOnce & KEY_p)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅔ";
		else buff += 'p';
	}
	if (im->keyOnce & KEY_f)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㄹ";
		else buff += 'f';
	}
	if (im->keyOnce & KEY_g)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅎ";
		else buff += 'g';
	}
	if (im->keyOnce & KEY_h)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅗ";
		else buff += 'h';
	}
	if (im->keyOnce & KEY_j)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅓ";
		else buff += 'j';
	}
	if (im->keyOnce & KEY_k)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅏ";
		else buff += 'k';
	}
	if (im->keyOnce & KEY_l)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅣ";
		else buff += 'l';
	}
	if (im->keyOnce & KEY_z)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅋ";
		else buff += 'z';
	}
	if (im->keyOnce & KEY_x)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅌ";
		else buff += 'x';
	}
	if (im->keyOnce & KEY_c)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅊ";
		else buff += 'c';
	}
	if (im->keyOnce & KEY_v)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅍ";
		else buff += 'v';
	}
	if (im->keyOnce & KEY_b)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅠ";
		else buff += 'b';
	}
	if (im->keyOnce & KEY_n)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅜ";
		else buff += 'n';
	}
	if (im->keyOnce & KEY_m)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅡ";
		else buff += 'm';
	}
	if (im->keyOnce & KEY_Q)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅃ";
		else buff += 'Q';
	}
	if (im->keyOnce & KEY_W)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅉ";
		else buff += 'W';
	}
	if (im->keyOnce & KEY_E)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㄸ";
		else buff += 'E';
	}
	if (im->keyOnce & KEY_R)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㄲ";
		else buff += 'R';
	}
	if (im->keyOnce & KEY_T)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅆ";
		else buff += 'T';
	}
	if (im->keyOnce & KEY_O)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅒ";
		else buff += 'O';
	}
	if (im->keyOnce & KEY_P)
	{
		if (im->langMode == LANG_MODE_LOCALE) buff += "ㅖ";
		else buff += 'P';
	}

	if (im->keyOnce & KEY_SPACE)
	{
		buff += ' ';
	}
	if (im->keyOnce & KEY_BACK)
	{

	}
}

void iInputBox::draw(float dt)
{
	updateBuff();

	result = kam->join(buff.str);

	//delete[] result;
}
