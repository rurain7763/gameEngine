#include "iGraphics.h"
#include "iStd.h"

#include "iKoreanAutoMata.h"

iGraphics::iGraphics(iSize& devSize)
    :stat(IGRAPHICS_STACK_SIZE)
{
    iKoreanAutoMata am;

    const char* test[] = 
    {
        "ㅃㅏㄹㄹㅣㅃㅏㄹㄹㅣㅎㅏㅅㅔㅇㅛ",
        "ㄱㄱㄱㄱㄱㄱㄴㄴㄴㄴㄴㄷㄷㄷ",
        "ㅁㅏㄴㄷㅡㄹㄱㅣ",
        "ㄱㅏㅇㅇㅏㅈㅣ",
        "ㅊㅣㄹㅎㅋㅣㄴㅎㅁㅓㄱㅅㅉㅏㄲ",
        "ㅁㅏㅅㅇㅣㅆㄴㅡㄴㅎㅏㄴㄱㅡㄹㅇㅗㅌㅗㅁㅏㅌㅏㅁㅏㄴㄷㅡㄹㄱㅣ",
        "ㅇㅏㄴㄴㅕㅇㅎㅏㅅㅔㅇㅛㄱㅏㅁㅅㅏㅎㅐㅇㅛㅈㅏㄹㅇㅣㅆㅇㅓㅇㅛㄷㅏㅅㅣㅁㅏㄴㄴㅏㅇㅛ",
    };

    for (int i = 0; i < 7; i++)
    {
        char* r = am.join(test[i]);

        wchar_t* test = multiByteToWideChar(r);
        delete[] test;

        delete[] r;
    }

    collection = new PrivateFontCollection;
    
    bmp = new Bitmap(devSize.width, devSize.height, PixelFormat32bppARGB);
    graphics = Graphics::FromImage(bmp);

    graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
}

iGraphics::~iGraphics()
{
    while (!stat.empty())
    {
        delete stat.pop();
    }

    iHashTable::iIterator begin = fonts.begin();
    iHashTable::iIterator end = fonts.end();

    for (iHashTable::iIterator itr = begin; itr != end; itr++)
    {
        Font* f = (Font*)itr->data;

        delete f;
    }

    for (int i = 0; i < fontNames.dataNum; i++)
    {
        char* fn = (char*)fontNames[i];
        delete[] fn;
    }

    delete collection;
    delete graphics;
    delete bmp;
}

void iGraphics::clear()
{
    int w = (int)bmp->GetWidth();
    int h = (int)bmp->GetHeight();

    Rect rect = { 0, 0, w, h };
    BitmapData bd;

    if (bmp->LockBits(&rect, ImageLockModeWrite, bmp->GetPixelFormat(), &bd) ==
        Status::Ok)
    {
        uint8* rgba = (uint8*)bd.Scan0;

        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                int start = bd.Stride * i + j * 4;

                rgba[start + 0] = 255.f * color.b;
                rgba[start + 1] = 255.f * color.g;
                rgba[start + 2] = 255.f * color.r;
                rgba[start + 3] = 255.f * color.a;
            }
        }

        bmp->UnlockBits(&bd);
    }
}

void iGraphics::pushStat()
{
    iString* str = new iString(font);
    uint8* s = new uint8(size);
    iColor* c = new iColor(color);

    stat.push(str);
    stat.push(s);
    stat.push(c);
}

void iGraphics::popStat()
{
    iColor* c = (iColor*)stat.pop();
    uint8* s = (uint8*)stat.pop();
    iString* str = (iString*)stat.pop();

    font = str->str;
    size = *s;
    color = *c;

    delete str;
    delete s;
    delete c;
}

iImage* iGraphics::getiImage()
{
    iImage* r = NULL;

    int w = (int)bmp->GetWidth();
    int h = (int)bmp->GetHeight();

    Rect rect = { 0, 0, w, h };
    BitmapData bd;

    if (bmp->LockBits(&rect, ImageLockModeRead, bmp->GetPixelFormat(), &bd) ==
        Status::Ok)
    {
        r = new iImage;

        int len = w * h * 4;
        r->pixelData = new uint8[len];
        memset(r->pixelData, 0, len);

        uint32* data = (uint32*)bd.Scan0;
        int stride = bd.Stride / 4;

        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                uint32 c = data[stride * i + j];
                
                int start = bd.Stride * i + (j * 4);

                r->pixelData[start + 0] = (c & 0x00ff0000) >> 16;
                r->pixelData[start + 1] = (c & 0x0000ff00) >> 8;
                r->pixelData[start + 2] = (c & 0x000000ff);
                r->pixelData[start + 3] = (c & 0xff000000) >> 24;
            }
        }

        r->width = w;
        r->height = h;
        r->stride = bd.Stride;
        r->type = iImageTypeUnknown;

        bmp->UnlockBits(&bd);
    }

    return r;
}

void iGraphics::drawString(const char* s, iVector2f pos)
{
    if (font.len == 0) return;

    Font* f = (Font*)fonts[font.str];
    
    if (!f)
    {
        f = getFont(font.str, size);
    }

    if (f)
    {
        wchar_t* str = multiByteToWideChar(s);
        int len = lstrlenW(str);
        
        PointF p = { pos.x, pos.y };
        SolidBrush brush(Color(IGRAPHICS_COLOR));

        graphics->DrawString(str, len, f, p, &brush);

        delete[] str;
    }
}

Font* iGraphics::getFont(const char* fontName, int size)
{
    Font* r = NULL;

    wchar_t* path = multiByteToWideChar(fontName);

    FontFamily fontF(path);

    if (fontF.IsAvailable())
    {
        r = new Font(&fontF, size);
        fonts.insert(fontName, r);
    }
    else
    {
        collection->AddFontFile(path);

        int num = collection->GetFamilyCount();
        FontFamily* ff = new FontFamily[num];
        int found = 0;
        collection->GetFamilies(num, ff, &found);

        for (int i = 0; i < found; i++)
        {      
            wchar_t name[128];
            ff[i].GetFamilyName(name);

            char* trans = wideCharToMultiByte(name);

            int j;

            for (j = 0; j < fontNames.dataNum; j++)
            {
                char* fn = (char*)fontNames[j];

                if (!strcmp(fn, trans)) break;
            }

            if (j >= fontNames.dataNum)
            {
                r = new Font(&ff[i], size);
                fonts.insert(fontName, r);
                fontNames.push_back(trans);
            }
            else
            {
                delete[] trans;
            }
        }

        delete[] ff;
    }

    delete[] path;

    return r;
}