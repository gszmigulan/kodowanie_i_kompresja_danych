#ifndef PIXELS
#define PIXELS
#include <stdint.h>

#pragma pack(1)
struct PIXEL {
    unsigned char b;
    unsigned char g;
    unsigned char r;
};

struct PIXEL2 {
    short r;
    short g;
    short b;
};

#endif
