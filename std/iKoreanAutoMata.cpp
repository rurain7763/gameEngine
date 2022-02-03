#include "iKoreanAutoMata.h"
#include "iStd.h"

iKoreanAutoMata* iKoreanAutoMata::S = NULL;

iKoreanAutoMata::iKoreanAutoMata()
{
	_jamo = new iKoreanJamo[]
	{
		{"ㄱ", { "ㄱ", 0 }, 1}, {"ㄲ", { 0, "ㄲ" }, 2}, {"ㄳ", { "ㄱ", "ㅅ" }, 2},																																			 // 0, 1, 2
		{"ㄴ", { "ㄴ", 0 }, 1}, {"ㄵ", { "ㄴ", "ㅈ" }, 2}, {"ㄶ", { "ㄴ", "ㅎ" }, 2},																																			 // 3, 4, 5
		{"ㄷ", { "ㄷ", 0 }, 1}, {"ㄸ", { 0, "ㄸ" }, 2},																																									 // 6, 7
		{"ㄹ", { "ㄹ", 0 }, 1}, {"ㄺ", { "ㄹ", "ㄱ" }, 2}, {"ㄻ", { "ㄹ", "ㅁ" }, 2}, {"ㄼ", { "ㄹ", "ㅂ" }, 2}, {"ㄽ", { "ㄹ", "ㅅ" }, 2}, {"ㄾ", { "ㄹ", "ㅌ" }, 2}, {"ㄿ", { "ㄹ", "ㅍ" }, 2}, {"ㅀ", { "ㄹ", "ㅎ" }, 2},		 // 8, 9, 10, 11, 12, 13, 14, 15
		{"ㅁ", { "ㅁ", 0 }, 1},																																																 // 16
		{"ㅂ", { "ㅂ", 0 }, 1}, {"ㅃ", { 0, "ㅃ" }, 2}, {"ㅄ", {"ㅂ", "ㅅ"}, 2},																																			 // 17, 18, 19
		{"ㅅ", { "ㅅ", 0 }, 1}, {"ㅆ", { 0, "ㅆ" }, 2},  																																									 // 20, 21
		{"ㅇ", { "ㅇ", 0 }, 1}, 																																																 // 22
		{"ㅈ", { "ㅈ", 0 }, 1}, {"ㅉ", { 0, "ㅉ"}, 2}, 																																									 // 23, 24
		{"ㅊ", { "ㅊ", 0 }, 1},  																																															 // 25
		{"ㅋ", { "ㅋ", 0 }, 1},  																																															 // 26
		{"ㅌ", { "ㅌ", 0 }, 1},  																																															 // 27
		{"ㅍ", { "ㅍ", 0 }, 1},  																																															 // 28
		{"ㅎ", { "ㅎ", 0 }, 1},  																																															 // 29
//==================================================================================================================================================================================================================================================================
		{"ㅏ", { "ㅏ", 0 }, 1},																																																 // 30
		{"ㅐ", { "ㅐ", 0 }, 1},																																																 // 31
		{"ㅑ", { "ㅑ", 0 }, 1},																																																 // 32
		{"ㅒ", { "ㅒ", 0 }, 1},																																																 // 33
		{"ㅓ", { "ㅓ", 0 }, 1},																																																 // 34
		{"ㅔ", { "ㅔ", 0 }, 1},																																																 // 35
		{"ㅕ", { "ㅕ", 0 }, 1},																																																 // 36
		{"ㅖ", { "ㅖ", 0 }, 1},																																																 // 37
		{"ㅗ", { "ㅗ", 0 }, 1}, {"ㅘ", { "ㅗ", "ㅏ" }, 2}, {"ㅙ", { "ㅗ", "ㅐ" }, 2}, {"ㅚ", { "ㅗ", "ㅣ" }, 2},																												 // 38, 39, 40, 41
		{"ㅛ", { "ㅛ", 0 }, 1},																																																 // 42
		{"ㅜ", { "ㅜ", 0 }, 1}, {"ㅝ", { "ㅜ", "ㅓ" }, 2}, {"ㅞ", { "ㅜ", "ㅔ" }, 2}, {"ㅟ", { "ㅜ", "ㅣ" }, 2},																												 // 43, 44, 45, 46
		{"ㅠ", { "ㅠ", 0 }, 1},																																																 // 47 
		{"ㅡ", { "ㅡ", 0 }, 1}, {"ㅢ", { "ㅡ", "ㅣ" }, 2},																																									 // 48, 49
		{"ㅣ", { "ㅣ", 0 }, 1}																																																 // 50
	};

	initConsonant = new iKoreanJamo * []
	{
		&_jamo[0],  // ㄱ
		&_jamo[1],  // ㄲ
		&_jamo[3],  // ㄴ
		&_jamo[6],  // ㄷ
		&_jamo[7],  // ㄸ
		&_jamo[8],  // ㄹ
		&_jamo[16], // ㅁ
		&_jamo[17], // ㅂ
		&_jamo[18], // ㅃ
		&_jamo[20], // ㅅ
		&_jamo[21], // ㅆ
		&_jamo[22], // ㅇ
		&_jamo[23], // ㅈ
		&_jamo[24], // ㅉ
		&_jamo[25], // ㅊ
		&_jamo[26], // ㅋ
		&_jamo[27], // ㅌ
		&_jamo[28], // ㅍ
		&_jamo[29], // ㅎ
	};

	for (uint8 i = 0; i < 19; i++)
	{
		initConsonant[i]->initCost = i;
	}

	mediVowel = new iKoreanJamo * []
	{
		&_jamo[30 + 0],  // ㅏ
		&_jamo[30 + 1],  // ㅐ
		&_jamo[30 + 2],  // ㅑ
		&_jamo[30 + 3],  // ㅒ
		&_jamo[30 + 4],  // ㅓ
		&_jamo[30 + 5],  // ㅔ
		&_jamo[30 + 6],  // ㅕ
		&_jamo[30 + 7],  // ㅖ
		&_jamo[30 + 8],  // ㅗ
		&_jamo[30 + 9],  // ㅘ
		&_jamo[30 + 10], // ㅙ
		&_jamo[30 + 11], // ㅚ
		&_jamo[30 + 12], // ㅛ
		&_jamo[30 + 13], // ㅜ
		&_jamo[30 + 14], // ㅝ
		&_jamo[30 + 15], // ㅞ
		&_jamo[30 + 16], // ㅟ
		&_jamo[30 + 17], // ㅠ
		&_jamo[30 + 18], // ㅡ
		&_jamo[30 + 19], // ㅢ
		&_jamo[30 + 20], // ㅣ
	};

	for (uint8 i = 0; i < 21; i++)
	{
		mediVowel[i]->mediCost = i;
	}

	finConsonant = new iKoreanJamo * []
	{
		NULL,	    // None
		&_jamo[0],  // ㄱ
		&_jamo[1],  // ㄲ
		&_jamo[2],  // ㄳ
		&_jamo[3],  // ㄴ
		&_jamo[4],  // ㄵ
		&_jamo[5],  // ㄶ
		&_jamo[6],  // ㄷ
		&_jamo[8],  // ㄹ
		&_jamo[9],  // ㄺ
		&_jamo[10], // ㄻ
		&_jamo[11], // ㄼ
		&_jamo[12], // ㄽ
		&_jamo[13], // ㄾ
		&_jamo[14], // ㄿ
		&_jamo[15], // ㅀ
		&_jamo[16], // ㅁ
		&_jamo[17], // ㅂ
		&_jamo[19], // ㅄ
		&_jamo[20], // ㅅ
		&_jamo[21], // ㅆ
		&_jamo[22], // ㅇ
		&_jamo[23], // ㅈ
		&_jamo[25], // ㅊ
		&_jamo[26], // ㅋ
		&_jamo[27], // ㅌ
		&_jamo[28], // ㅍ
		&_jamo[29], // ㅎ
	};

	for (int i = 1; i < 28; i++)
	{
		finConsonant[i]->finCost = i;
	}

	for (int i = 0; i < 51; i++)
	{
		_jamo[i].order = i;
		jamo.insert(_jamo[i].jamo, &_jamo[i]);
	}
}

iKoreanAutoMata::~iKoreanAutoMata()
{
	delete[] initConsonant;
	delete[] mediVowel;
	delete[] finConsonant;
	delete[] _jamo;
}

iKoreanAutoMata* iKoreanAutoMata::share()
{
	if (!S) S = new iKoreanAutoMata();
	return S;
}

char* iKoreanAutoMata::join(const char* s)
{
	int len = strlen(s);

	iStack stk(len);
	uint32 rLen = 0;

	for (int i = 0; i < len; i++)
	{
		uint8 byte = (uint8)s[i];

		if ((byte & KOREAN_BIT_MASK) == KOREAN_BIT_MASK)
		{
			iKoreanLetter* curr = new iKoreanLetter(&s[i]);

			if (stk.empty())
			{
				stk.push(curr);
				rLen+=3;
				i += 2;
				continue;
			}

			iLetter* lett = (iLetter*)stk.pop();

			if (lett->kind == iLETTER_ASCII)
			{
				stk.push(lett);
				stk.push(curr);
				rLen += 3;
				i += 2;
				continue;
			}

			iKoreanLetter* prev = (iKoreanLetter*)lett;

			switch (prev->stat)
			{
			case iKoreanLetterStatConsonantOnly:
			{
				if (curr->stat == iKoreanLetterStatVowelOnly)
				{
					if (prev->initial->elementsNum == 1)
					{
						uint8 ic = getInitCost(prev->letter);
						uint8 mc = getMediCost(curr->letter);

						prev->change(getUnicode(ic, mc, 0));
						stk.push(prev);

						delete curr;
					}
					else if (prev->initial->elementsNum == 2)
					{
						if (prev->initial->elements[0])
						{
							iKoreanJamo* firConsonant = (iKoreanJamo*)jamo[prev->initial->elements[0]];
							iKoreanJamo* secConsonant = (iKoreanJamo*)jamo[prev->initial->elements[1]];

							if (secConsonant->finCost != IKOREANJAMO_INVALID_COST)
							{
								uint16 prevUc = getUnicode(firConsonant->jamo);
								uint16 currUc = getUnicode(secConsonant->initCost, getMediCost(curr->letter), 0);

								prev->change(prevUc);
								curr->change(currUc);
							}

							stk.push(prev);
							stk.push(curr);
							rLen += 3;
						}
						else
						{
							uint8 ic = getInitCost(prev->letter);
							uint8 mc = getMediCost(curr->letter);

							prev->change(getUnicode(ic, mc, 0));
							stk.push(prev);

							delete curr;
						}
					}
				}
				else if (curr->stat == iKoreanLetterStatConsonantOnly)
				{
					uint16 newUnicode = 0;

					if (canConbine(prev->letter, curr->letter, newUnicode))
					{
						prev->change(newUnicode);
						stk.push(prev);
						delete curr;
					}
					else
					{
						stk.push(prev);
						stk.push(curr);
						rLen+=3;
					}
				}

				break;
			}
			case iKoreanLetterStatVowelOnly:
			{
				if (curr->stat == iKoreanLetterStatVowelOnly)
				{
					uint16 newUnicode = 0;

					if (canConbine(prev->letter, curr->letter, newUnicode))
					{
						prev->change(newUnicode);
						stk.push(prev);
						delete curr;
					}
					else
					{
						stk.push(prev);
						stk.push(curr);
						rLen+=3;
					}
				}
				else if (curr->stat == iKoreanLetterStatConsonantOnly)
				{
					stk.push(prev);
					stk.push(curr);
					rLen+=3;
				}

				break;
			}
			case iKoreanLetterStatNoFinal:
			{
				if (curr->stat == iKoreanLetterStatConsonantOnly)
				{
					uint8 finCost = getFinCost(curr->letter);

					if (finCost != IKOREANJAMO_INVALID_COST)
					{
						prev->change(prev->uniCode + finCost);
						stk.push(prev);
						delete curr;
					}
					else
					{
						stk.push(prev);
						stk.push(curr);
						rLen+=3;
					}
				}
				else if (curr->stat == iKoreanLetterStatVowelOnly)
				{
					iKoreanJamo* newJamo = NULL;

					if (canConbine(prev->medial->jamo, curr->letter, newJamo))
					{
						uint16 prevUc = getUnicode(prev->initial->initCost, newJamo->mediCost, 0);
						prev->change(prevUc);

						stk.push(prev);
						delete curr;
					}
					else
					{
						stk.push(prev);
						stk.push(curr);
						rLen += 3;
					}
				}

				break;
			}
			case iKoreanLetterStatIncludeFinal:
			{
				if (curr->stat == iKoreanLetterStatVowelOnly)
				{
					if (prev->fin->elementsNum == 1)
					{
						if (getInitCost(prev->fin->jamo) != IKOREANJAMO_INVALID_COST)
						{
							uint16 prevUc = prev->uniCode - prev->fin->finCost;
							uint16 currUc = getUnicode(prev->fin->initCost, curr->medial->mediCost, 0);

							prev->change(prevUc);
							curr->change(currUc);
						}

						stk.push(prev);
						stk.push(curr);
						rLen+=3;
					}
					else if (prev->fin->elementsNum == 2)
					{
						if (prev->fin->elements[0])
						{
							iKoreanJamo* firConsonant = (iKoreanJamo*)jamo[prev->fin->elements[0]];
							iKoreanJamo* secConsonant = (iKoreanJamo*)jamo[prev->fin->elements[1]];

							if (secConsonant->finCost != IKOREANJAMO_INVALID_COST)
							{
								uint16 prevUc = prev->uniCode - prev->fin->finCost + firConsonant->finCost;
								uint16 currUc = getUnicode(secConsonant->initCost, getMediCost(curr->letter), 0);

								prev->change(prevUc);
								curr->change(currUc);
							}

							stk.push(prev);
							stk.push(curr);
							rLen += 3;
						}
						else
						{
							iKoreanJamo* secConsonant = (iKoreanJamo*)jamo[prev->fin->elements[1]];

							uint16 prevUc = prev->uniCode - prev->fin->finCost;
							uint16 currUc = getUnicode(secConsonant->initCost, getMediCost(curr->letter), 0);

							prev->change(prevUc);
							curr->change(currUc);

							stk.push(prev);
							stk.push(curr);
							rLen += 3;
						}
					}
				}
				else if (curr->stat == iKoreanLetterStatConsonantOnly)
				{
					iKoreanJamo* newJamo = NULL;

					if (canConbine(prev->fin->jamo, curr->letter, newJamo))
					{
						uint16 prevUc = prev->uniCode - prev->fin->finCost + newJamo->finCost;

						prev->change(prevUc);

						stk.push(prev);
						delete curr;
					}
					else
					{
						stk.push(prev);
						stk.push(curr);
						rLen+=3;
					}
				}

				break;
			}
			}

			i += 2;
		}
		else if ((byte & ASCII_BIT_MASK) == 0)
		{
			iASCIILetter* curr = new iASCIILetter(&s[i]);

			stk.push(curr);
			rLen++;
		}
		else
		{
			while (!stk.empty())
			{
				iLetter* letter = (iLetter*)stk.pop();
				delete letter;
			}

			return NULL;
		}
	}

	char* r = new char[rLen + 1];
	int i = rLen;

	while (!stk.empty())
	{
		iLetter* lett = (iLetter*)stk.pop();

		if (lett->kind == iLETTER_ASCII)
		{
			iASCIILetter* al = (iASCIILetter*)lett;
			i -= 1;
			memcpy(&r[i], al->letter, sizeof(char) * al->letterSize);

		}
		else if(lett->kind == iLETTER_KOREAN)
		{
			iKoreanLetter* kl = (iKoreanLetter*)lett;
			i -= 3;
			memcpy(&r[i], kl->letter, sizeof(char) * kl->letterSize);
		}
		
		delete lett;
	}
	r[rLen] = 0;

	return r;
}

uint8 iKoreanAutoMata::getInitCost(const char* letter)
{
	iKoreanJamo* kj = (iKoreanJamo*)jamo[letter];

	if (kj)
	{
		return kj->initCost;
	}
	else
	{
		return IKOREANJAMO_INVALID_COST;
	}
}

uint8 iKoreanAutoMata::getMediCost(const char* letter)
{
	iKoreanJamo* kj = (iKoreanJamo*)jamo[letter];

	if (kj)
	{
		return kj->mediCost;
	}
	else
	{
		return IKOREANJAMO_INVALID_COST;
	}
}

uint8 iKoreanAutoMata::getFinCost(const char* letter)
{
	iKoreanJamo* kj = (iKoreanJamo*)jamo[letter];

	if (kj)
	{
		return kj->finCost;
	}
	else
	{
		return IKOREANJAMO_INVALID_COST;
	}
}

bool iKoreanAutoMata::canConbine(const char* l, const char* r, uint16& newUnicode)
{
	iKoreanJamo* left = (iKoreanJamo*)jamo[l];
	iKoreanJamo* right = (iKoreanJamo*)jamo[r];

	if (left->elementsNum == 2 || right->elementsNum == 2) return false;

	if (left)
	{
		uint8 idx = left->order + 1;

		while (_jamo[idx].elementsNum == 2)
		{
			if (!strcmp(_jamo[idx].elements[1], r))
			{
				newUnicode = getUnicode(_jamo[idx].jamo);
				return true;
			}

			idx++;
		}
	}

	return false;
}

bool iKoreanAutoMata::canConbine(const char* l, const char* r, iKoreanJamo*& letter)
{
	iKoreanJamo* left = (iKoreanJamo*)jamo[l];
	iKoreanJamo* right = (iKoreanJamo*)jamo[r];

	if (left->elementsNum == 2 || right->elementsNum == 2) return false;
	
	if (left)
	{
		uint8 idx = left->order + 1;

		while (_jamo[idx].elementsNum == 2)
		{
			if (!strcmp(_jamo[idx].elements[1], r))
			{
				letter = &_jamo[idx];
				return true;
			}

			idx++;
		}
	}

	return false;
}

uint16 iKoreanAutoMata::getUnicode(const char* ks)
{
	return ((uint8)ks[2] & 0x3f) +
		   (((uint8)ks[1] & 0x3f) << 6) +
		   (((uint8)ks[0] & 0xf) << 12);
}

uint16 iKoreanAutoMata::getUnicode(uint8 ic, uint8 mc, uint8 fc)
{
	return ic * KOREAN_FIRST_ORDER + 
		   mc * KOREAN_MIDDLE_ORDER + 
		   fc + KOREAN_CODE_MIN;
}

iLetter::iLetter(uint8 kind)
{
	this->kind = kind;
}

iLetter::~iLetter()
{
}

iASCIILetter::iASCIILetter(const char* str)
	:iLetter(iLETTER_ASCII)
{
	letterSize = 1;
	letter = new char[letterSize + 1];
	memcpy(letter, str, sizeof(char) * letterSize);
	letter[1] = 0;
}

iASCIILetter::~iASCIILetter()
{
	delete[] letter;
}

iKoreanLetter::iKoreanLetter(const char* str)
	:iLetter(iLETTER_KOREAN)
{
	letterSize = 3;
	letter = new char[letterSize + 1];

	memcpy(letter, str, sizeof(char) * letterSize);
	letter[3] = 0;

	uniCode = 0;
	uniCode = ((uint8)letter[2] & 0x3f) +
			  (((uint8)letter[1] & 0x3f) << 6) +
			  (((uint8)letter[0] & 0xf) << 12);

	update();
}

iKoreanLetter::~iKoreanLetter()
{
	delete[] letter;
}

void iKoreanLetter::change(uint16 uc)
{
	uint32 bytes = THREE_BYTES_UNICODE_BASE +
				   (uc & 0x3f) +
				   ((uc & 0xfc0) << 2) +
				   ((uc & 0xf000) << 4);

	for (int i = 0; i < 3; i++)
	{
		letter[2 - i] = ((uint8*)&bytes)[i];
	}
	letter[3] = 0;

	uniCode = uc;

	update();
}

void iKoreanLetter::update()
{
	iKoreanAutoMata* kam = iKoreanAutoMata::share();

	initial = NULL;
	medial = NULL;
	fin = NULL;

	if (isKorConsonant(uniCode))
	{
		stat = iKoreanLetterStatConsonantOnly;
		initial = (iKoreanJamo*)kam->jamo[letter];
	}
	else if (isKorVowel(uniCode))
	{
		stat = iKoreanLetterStatVowelOnly;
		medial = (iKoreanJamo*)kam->jamo[letter];
	}
	else
	{
		uint16 uc = uniCode - KOREAN_CODE_MIN;

		uint32 cost = uc / KOREAN_FIRST_ORDER;
		initial = kam->initConsonant[cost];
		uc %= KOREAN_FIRST_ORDER;

		cost = uc / KOREAN_MIDDLE_ORDER;
		medial = kam->mediVowel[cost];
		uc %= KOREAN_MIDDLE_ORDER;

		cost = uc;

		if (cost) stat = iKoreanLetterStatIncludeFinal;
		else stat = iKoreanLetterStatNoFinal;

		fin = kam->finConsonant[cost];
	}
}

bool isKorean(uint16 uc)
{
	return uc >= KOREAN_CODE_MIN && uc <= KOREAN_CODE_MAX;
}

bool isKorConsonant(uint16 uc)
{
	return uc >= 0x3131 && uc <= 0x314e;
}

bool isKorVowel(uint16 uc)
{
	return uc >= 0x314f && uc <= 0x3163;
}
