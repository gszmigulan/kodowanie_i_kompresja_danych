#include "../common/bits.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


short get_val(unsigned short x) {
    if (x % 2 == 1){
      return (x+1) / 2;
    }
    return -(x / 2);
}

unsigned char division(int x) {
    if (x < 0) {
       return 0;
    }
    if (x >= 255){
      return 255;
    }
    return (unsigned char)x;
}

struct PIXEL read_low(struct myFile file){
        struct PIXEL result= (struct PIXEL){0,0,0};
        result.r = (unsigned char)read_bits(&file, 8);
        result.g = (unsigned char)read_bits(&file, 8);
        result.b = (unsigned char)read_bits(&file, 8);
        return result;
}


int main(int argc, char** argv) {
    if (argc < 3) {
        printf("za mało argumetów\n");
        exit(1);
    }
    FILE* infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        printf("nie można otworzyć pliku wejściowego\n");
        exit(1);
    }
    FILE* outfile = fopen(argv[2], "wb");
    if (outfile == NULL) {
        printf("nie można otworzyć pliku wyjściowego\n");
        exit(1);
    }

    struct myFile file =(struct myFile) {
        .file = infile,
        .byte = 0,
        .bits = 0,       
    }; 

    unsigned short width = read_bits(&file, 16);
    unsigned short height = read_bits(&file, 16);
    unsigned char bits = (unsigned char)read_bits(&file, 8);

    size_t size = width * height;
    size_t half = size / 2;

    size_t entries = 1 << bits;

    struct PIXEL* result_low = (struct PIXEL*) malloc(half * sizeof(struct PIXEL));
    struct PIXEL2* result_high = (struct PIXEL2*) malloc(half * sizeof(struct PIXEL2));
    for (size_t i = 0; i < entries; ++i) {
         result_low[i] = read_low(file);
    }

    for (size_t i = 0; i < entries; ++i) {
        unsigned short red = read_bits(&file,9);
        unsigned short green = read_bits(&file,9);
        unsigned short blue = read_bits(&file,9);
        result_high[i].r = get_val(red);
        result_high[i].g = get_val(green);
        result_high[i].b = get_val(blue);
    }

    unsigned char* buff_low = malloc(half);    
    unsigned char* buff_high = malloc(half);

    for (size_t i = 0; i < half; ++i) {
        buff_low[i] = (unsigned char)read_bits(&file, bits);
        buff_high[i] = (unsigned char)read_bits(&file, bits);
    }

    struct PIXEL* image = (struct PIXEL*) malloc(size * sizeof(struct PIXEL*));
    struct PIXEL low;
    struct PIXEL2 high;
    struct PIXEL p1= (struct PIXEL) {0,0,0};
    struct PIXEL p2= (struct PIXEL) {0,0,0};
    for (size_t i = 1; i < half; i++) {
        low = result_low[buff_low[i]];
        high = result_high[buff_high[i]];
        
        p1.r = division((int)low.r + (int)high.r);
        p1.g = division((int)low.g + (int)high.g);
        p1.b = division((int)low.b + (int)high.b);

        image[2*i] = p1;
         p2.r = division((int)low.r - (int)high.r);
         p2.g = division((int)low.g - (int)high.g);
         p2.b = division((int)low.b - (int)high.b);
        image[2*i-1] = p2;
    }

   
    struct my_HEADER h= init_header(width,height);

    char fill_footer[26] =  
    "\0\0\0\0"
    "\0\0\0\0"
    "TRUEVISION-XFILE.";

    struct TGA_FOOTER footer = *((struct TGA_FOOTER*)fill_footer);   
    fseek(outfile, 0, SEEK_SET);
    fwrite(&h, 18, 1, outfile);
    fwrite(image, sizeof(struct PIXEL), size, outfile);
    fwrite(&footer, 26, 1, outfile);
    return 0;
}
