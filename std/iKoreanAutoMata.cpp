#include "iKoreanAutoMata.h"
#include "iStd.h"

iKoreanWord firstWord[] =
{
	{"ㄱ", {"ㄱ"}, 1}, 
	{"ㄲ", {"ㄲ"}, 1}, 
	{"ㄴ", {"ㄴ"}, 1}, 
	{"ㄷ", {"ㄷ"}, 1}, 
	{"ㄸ", {"ㄸ"}, 1}, 
	{"ㄹ", {"ㄹ"}, 1}, 
	{"ㅁ", {"ㅁ"}, 1}, 
	{"ㅂ", {"ㅂ"}, 1},
	{"ㅃ", {"ㅃ"}, 1}, 
	{"ㅅ", {"ㅅ"}, 1}, 
	{"ㅆ", {"ㅆ"}, 1}, 
	{"ㅇ", {"ㅇ"}, 1}, 
	{"ㅈ", {"ㅈ"}, 1}, 
	{"ㅉ", {"ㅉ"}, 1}, 
	{"ㅊ", {"ㅊ"}, 1}, 
	{"ㅋ", {"ㅋ"}, 1},
	{"ㅌ", {"ㅌ"}, 1}, 
	{"ㅍ", {"ㅍ"}, 1}, 
	{"ㅎ", {"ㅎ"}, 1}
};

iKoreanWord midWord[] =
{
	{"ㅏ", {"ㅏ"}, 1},
	{"ㅐ", {"ㅐ"}, 1},
	{"ㅑ", {"ㅑ"}, 1},
	{"ㅒ", {"ㅒ"}, 1},
	{"ㅓ", {"ㅓ"}, 1},
	{"ㅔ", {"ㅔ"}, 1},
	{"ㅕ", {"ㅕ"}, 1},
	{"ㅖ", {"ㅖ"}, 1},
	{"ㅗ", {"ㅗ"}, 1},
	{"ㅘ", {"ㅗ", "ㅏ"}, 2},
	{"ㅙ", {"ㅗ", "ㅐ"}, 2},
	{"ㅚ", {"ㅗ", "ㅣ"}, 2},
	{"ㅛ", {"ㅛ"}, 1},
	{"ㅜ", {"ㅜ"}, 1},
	{"ㅝ", {"ㅜ", "ㅓ"}, 2},
	{"ㅞ", {"ㅜ", "ㅔ"}, 2},
	{"ㅟ", {"ㅜ", "ㅣ"}, 2},
	{"ㅠ", {"ㅠ"}, 1},
	{"ㅡ", {"ㅡ"}, 1},
	{"ㅢ", {"ㅡ", "ㅣ"}, 2},
	{"ㅣ", {"ㅣ"}, 1}
};

iKoreanWord lastWord[] =
{
	{"N", {"N", "N"}, 0},
	{"ㄱ", {"ㄱ"}, 1},
	{"ㄲ", {"ㄲ", "ㄲ"}, 2},
	{"ㄳ", {"ㄱ", "ㅅ"}, 2},
	{"ㄴ", {"ㄴ"}, 1},
	{"ㄵ", {"ㄴ", "ㅈ"}, 2},
	{"ㄶ", {"ㄴ", "ㅎ"}, 2},
	{"ㄷ", {"ㄷ"}, 1},
	{"ㄹ", {"ㄹ"}, 1},
	{"ㄺ", {"ㄹ", "ㄱ"}, 2},
	{"ㄻ", {"ㄹ", "ㅁ"}, 2},
	{"ㄼ", {"ㄹ", "ㅂ"}, 2},
	{"ㄽ", {"ㄹ", "ㅅ"}, 2},
	{"ㄾ", {"ㄹ", "ㅌ"}, 2},
	{"ㄿ", {"ㄹ", "ㅍ"}, 2},
	{"ㅀ", {"ㄹ", "ㅎ"}, 2},
	{"ㅁ", {"ㅁ"}, 1},
	{"ㅂ", {"ㅂ"}, 1},
	{"ㅄ", {"ㅂ", "ㅅ"}, 1},
	{"ㅅ", {"ㅅ"}, 1},
	{"ㅆ", {"ㅆ", "ㅆ"}, 2},
	{"ㅇ", {"ㅇ"}, 1},
	{"ㅈ", {"ㅈ"}, 1},
	{"ㅊ", {"ㅊ"}, 1},
	{"ㅋ", {"ㅋ"}, 1},
	{"ㅌ", {"ㅌ"}, 1},
	{"ㅍ", {"ㅍ"}, 1},
	{"ㅎ", {"ㅎ"}, 1}
};

iKoreanWord* korConsonant[] =
{
	&firstWord[0], &firstWord[1], &lastWord[3], &firstWord[2],
	&lastWord[5], &lastWord[6], &firstWord[3], &firstWord[4],
	&firstWord[5], &lastWord[9], &lastWord[10], &lastWord[11],
	&lastWord[12], &lastWord[13], &lastWord[14], &lastWord[15],
	&firstWord[6], &firstWord[7], &firstWord[8], &lastWord[18],
	&firstWord[9], &firstWord[10], &firstWord[11], &firstWord[12],
	&firstWord[13], &firstWord[14], &firstWord[15], &firstWord[16],
	&firstWord[17], &firstWord[18]
};

iKoreanAutoMata::iKoreanAutoMata()
{
	cost = new int[28];
	for (int i = 0; i < 28; i++) cost[i] = i;

	//first
	for (int i = 0; i < 19; i++)
	{
		first.insert(firstWord[i].word, &cost[i]);
	}

	//mid
	for (int i = 0; i < 21; i++)
	{
		mid.insert(midWord[i].word, &cost[i]);
	}

	//last
	for (int i = 0; i < 28; i++)
	{
		last.insert(lastWord[i].word, &cost[i]);
	}
}

iKoreanAutoMata::~iKoreanAutoMata()
{
	delete[] cost;
}

char* iKoreanAutoMata::join(const char* s)
{
	int len = strlen(s);

	uint8* str = (uint8*)s;
	iStack kor(len);
	int rLen = 0;

	for (int i = 0; i < len; i++)
	{
		uint8 byte = str[i];

		if ((byte & KOREAN_BIT_MASK) == KOREAN_BIT_MASK)
		{
			iKoreanByte* curr = new iKoreanByte(&str[i]);

			if (kor.empty())
			{
				kor.push(curr);
				rLen++;
				i += 2;
				continue;
			}

			iKoreanByte* prev = (iKoreanByte*)kor.pop();

			switch (prev->stat)
			{
			case iKoreanWordStatConsonantOnly:
			{
				if (curr->stat == iKoreanWordStatVowelOnly)
				{
					uint8 fc = *(int*)first[prev->character];
					uint8 mc = *(int*)mid[curr->character];

					uint16 newUnicode = getUnicode(fc, mc, 0);

					prev->changeCharacter(newUnicode);
					kor.push(prev);

					delete curr;
				}
				else if (curr->stat == iKoreanWordStatConsonantOnly)
				{
					uint16 newUnicode = 0;

					int* lw = (int*)last[prev->character];

					if (lw)
					{
						int cost = *lw;

						iKoreanWord* kw = &lastWord[++cost];

						while (kw->elementsNum == 2)
						{
							if (!strcmp(kw->elements[1], curr->character))
							{
								newUnicode = getUnicode(kw->word);
								break;
							}

							kw = &lastWord[++cost];
						}
					}

					if (newUnicode)
					{
						prev->changeCharacter(newUnicode);
						kor.push(prev);
						delete curr;
					}
					else
					{
						kor.push(prev);
						kor.push(curr);
						rLen++;
					}
				}

				break;
			}

			case iKoreanWordStatVowelOnly:
			{
				if (curr->stat == iKoreanWordStatVowelOnly)
				{
					uint16 newUnicode = 0;

					int* mw = (int*)mid[prev->character];

					if (mw)
					{
						int cost = *mw;

						iKoreanWord* kw = &midWord[++cost];

						while (kw->elementsNum == 2)
						{
							if (!strcmp(kw->elements[1], curr->character))
							{
								newUnicode = getUnicode(kw->word);
								break;
							}

							kw = &midWord[++cost];
						}
					}

					if (newUnicode)
					{
						prev->changeCharacter(newUnicode);
						kor.push(prev);
						delete curr;
					}
					else
					{
						kor.push(prev);
						kor.push(curr);
						rLen++;
					}
				}
				else if (curr->stat == iKoreanWordStatConsonantOnly)
				{
					kor.push(prev);
					kor.push(curr);
					rLen++;
				}

				break;
			}
			case iKoreanWordStatWordNoLast:
			{
				if (curr->stat == iKoreanWordStatConsonantOnly)
				{
					int* lw = (int*)last[curr->character];

					if (lw)
					{
						int cost = *lw;

						uint16 newUnicode = prev->uniCode + cost;
						prev->changeCharacter(newUnicode);
						kor.push(prev);
						delete curr;
					}
					else
					{
						kor.push(prev);
						kor.push(curr);
						rLen++;
					}
				}
				else if (curr->stat == iKoreanWordStatVowelOnly)
				{
					kor.push(prev);
					kor.push(curr);
					rLen++;
				}

				break;
			}
			case iKoreanWordStatWordWithLast:
			{
				if (curr->stat == iKoreanWordStatVowelOnly)
				{
					const char* lastWord = 0;

					if (prev->last->elementsNum == 1)
					{
						lastWord = prev->last->elements[0];

						int* fw = (int*)first[lastWord];

						if (fw)
						{
							int cost = *fw;

							uint16 prevUc = prev->uniCode - *(int*)last[lastWord];
							uint16 currUc = getUnicode(cost,
								*(int*)mid[curr->character],
								0);

							prev->changeCharacter(prevUc);
							curr->changeCharacter(currUc);
						}

						kor.push(prev);
						kor.push(curr);
						rLen++;
					}
					else if (prev->last->elementsNum == 2)
					{
						lastWord = prev->last->elements[1];

						int* fw = (int*)first[lastWord];

						if (fw)
						{
							int cost = *fw;

							uint16 prevUc = prev->uniCode -
								*(int*)last[prev->last->word] +
								*(int*)last[prev->last->elements[0]];

							uint16 currUc = getUnicode(cost,
								*(int*)mid[curr->character],
								0);

							prev->changeCharacter(prevUc);
							curr->changeCharacter(currUc);
						}

						kor.push(prev);
						kor.push(curr);
						rLen++;
					}
				}
				else if (curr->stat == iKoreanWordStatConsonantOnly)
				{
					int newCost = -1;

					int* fw = (int*)last[prev->last->word];

					if (fw)
					{
						int cost = *fw;

						iKoreanWord* kw = &lastWord[++cost];

						while (kw->elementsNum == 2)
						{
							if (!strcmp(kw->elements[1], curr->character))
							{
								newCost = cost;
								break;
							}

							kw = &lastWord[++cost];
						}
					}

					if (newCost != -1)
					{
						uint16 prevUc = prev->uniCode -
							*(int*)last[prev->last->word] +
							newCost;

						prev->changeCharacter(prevUc);

						kor.push(prev);
						delete curr;
					}
					else
					{
						kor.push(prev);
						kor.push(curr);
						rLen++;
					}
				}

				break;
			}
			default:
			{
				//...
				break;
			}
			}

			i += 2;
		}
		else if((byte & ASCII_BIT_MASK) == 0)
		{
		}
		else
		{
			//?
		}
	}

	char* r = new char[rLen * 3 + 1];
	int i = rLen - 1;

	while (!kor.empty())
	{
		iKoreanByte* kb = (iKoreanByte*)kor.pop();

		memcpy(&r[3 * i], kb->character, sizeof(char) * 3);
		i--;

		delete kb;
	}
	r[rLen * 3] = 0;

	return r;
}

iKoreanByte::iKoreanByte(const uint8* v)
{
	memcpy(character, v, sizeof(char) * 3);
	character[3] = 0;

	bytes = ((uint32)v[0] << 16) +
		    ((uint32)v[1] << 8) + 
		    (uint32)v[2];

	uniCode = (bytes & 0x3f) +
			  ((bytes & 0x3f00) >> 2) +
			  ((bytes & 0x0f0000) >> 4);

	updateWord(uniCode);
}

void iKoreanByte::changeCharacter(uint32 uc)
{
	bytes = THREE_BYTES_UNICODE_BASE +
		    (uc & 0x3f) + 
			((uc & 0xfc0) << 2)  + 
			((uc & 0xf000) << 4);

	uint8* b = (uint8*)&bytes;

	for (int i = 0; i < 3; i++)
	{
		character[2 - i] = b[i];
	}
	character[3] = 0;

	uniCode = uc;

	updateWord(uniCode);
}

void iKoreanByte::updateWord(uint16 uniCode)
{
	first = NULL;
	mid = NULL;
	last = NULL;

	if (isConsonant(uniCode))
	{
		stat = iKoreanWordStatConsonantOnly;
		int idx = uniCode - 0x3131;

		first = korConsonant[idx];
	}
	else if (isVowel(uniCode))
	{
		stat = iKoreanWordStatVowelOnly;
		int idx = uniCode - 0x314f;

		mid = &midWord[idx];
	}
	else
	{
		uint16 uc = uniCode - KOREAN_CODE_MIN;
		int idx;

		idx = uc / KOREAN_FIRST_ORDER;
		first = &firstWord[idx];
		uc %= KOREAN_FIRST_ORDER;

		idx = uc / KOREAN_MIDDLE_ORDER;
		mid = &midWord[idx];
		uc %= KOREAN_MIDDLE_ORDER;

		idx = uc;

		if (idx != 0)
		{
			stat = iKoreanWordStatWordWithLast;
			last = &lastWord[idx];
		}
		else
		{
			stat = iKoreanWordStatWordNoLast;
			last = &lastWord[0];
		}
	}
}

bool isConsonant(uint16 uc)
{
	return uc >= 0x3131 && uc <= 0x314e;
}

bool isVowel(uint16 uc)
{
	return uc >= 0x314f && uc <= 0x3163;
}

bool isKorean(uint16 uc)
{
	return uc >= KOREAN_CODE_MIN && uc <= KOREAN_CODE_MAX;
}

uint16 getUnicode(const char* kc)
{
	return ((uint8)kc[2] & 0x3f) +
		   (((uint8)kc[1] & 0x3f) << 6) +
		   (((uint8)kc[0] & 0xf) << 12);
}

uint16 getUnicode(uint8 fc, uint8 mc, uint8 lc)
{
	return fc * KOREAN_FIRST_ORDER + 
		   mc * KOREAN_MIDDLE_ORDER + 
		   lc + KOREAN_CODE_MIN;
}