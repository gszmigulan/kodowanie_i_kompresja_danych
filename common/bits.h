#include "pixels.h"
#include "stdio.h"

struct my_HEADER {
    char idlenght;
    char colormaptype;
    char datatypecode;
    short int colourmaporigin;
    short int colourmaplength;
    char colourmapdepth;
    short int x_origin;
    short int y_origin;
    short width;
    short height;
    char bitsperpixel;
    char imgagedescriptor;
};

struct TGA_FOOTER {
    unsigned int ext_offset;
    unsigned int dev_offset;
    char signature[16];
    unsigned char dot;
    unsigned char zero;
};

struct myFile {
    FILE* file;
    unsigned char byte;
    unsigned char bits;
};

struct my_HEADER init_header(unsigned short w, unsigned short h);
void flush(struct myFile* file);
unsigned short read_bits(struct myFile* file, int bits);
void write_it(struct myFile* file, unsigned short value, int bits);


