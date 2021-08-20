#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(sz) new char[sz]
#define STBI_REALLOC(p,newsz) realloc(p,newsz)
#define STBI_FREE(p) delete[] p
#include "stb_image.h"