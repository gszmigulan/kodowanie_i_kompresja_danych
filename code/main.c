#include "../common/pixels.h"
#include "comp.h"
#include "../common/bits.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned short get_val(short x) {
   
    if (x > 0){ 
       return (unsigned short)(2*x-1);
    }
    return (unsigned short)(-2*x);
}


void write_low(struct myFile file, struct PIXEL result){
        write_it(&file, result.r, 8);
        write_it(&file, result.g, 8);
        write_it(&file, result.b, 8);
}


static void write_to_file( unsigned short width,  unsigned short height, unsigned char bits, struct PIXEL* result_low, unsigned char* buff_low,
 struct PIXEL2* result_high, unsigned char* buff_high, FILE* outfile, int half) {
    
   struct myFile file = (struct myFile) {
        .file = outfile,
        .byte = 0,
        .bits = 0,   
    };
    write_it(&file, width, 16);
    write_it(&file, height, 16);
    write_it(&file, bits, 8);

    int nr = 1 << bits;

    for (int i = 0; i < nr; ++i) {
        write_low(file, result_low[i]);
    }
    for (int i = 0; i < nr; ++i) {
        short er = get_val(result_high[i].r);
        short eg = get_val(result_high[i].g);
        short eb = get_val(result_high[i].b);
        write_it(&file, er, 9);
        write_it(&file, eg, 9);
        write_it(&file, eb, 9);
    }
    for (int i = 0; i < half; ++i) {
        write_it(&file, buff_low[i], bits);
        write_it(&file, buff_high[i], bits);
    }

    flush(&file);
}


static struct PIXEL2 high_pixel(struct PIXEL l, struct PIXEL r) {

    struct PIXEL2 p = {0, 0, 0};
    p.r= (short)(((int)l.r - (int)r.r) / 2);
    p.g= (short)(((int)l.g - (int)r.g) / 2);
    p.b= (short)(((int)l.b - (int)r.b) / 2);
    return p;
}

static struct PIXEL low_pixel(struct PIXEL l, struct PIXEL r) {

    struct PIXEL p = {0,0,0};
    p.r= (unsigned char)((((int)l.r + (int)r.r) / 2) % 256);
    p.g= (unsigned char)((((int)l.g + (int)r.g) / 2) % 256);
    p.b= (unsigned char)((((int)l.b + (int)r.b) / 2) % 256);
    return p;

}



int main(int argc, char** argv) {
    if (argc < 4) {
        printf(" za mało argumetów!\n");
        exit(1);
    }

    FILE* image = fopen(argv[1], "rb");
    if (image == NULL) {
        printf("Nie można otworzyć pliku wejściowego \n");
        exit(1);
    }

      FILE* outfile = fopen(argv[2], "wb");
    if (outfile == NULL) {
        printf(" nie można otworzyć pliku wyjściowego \n");
        exit(1);
    }
    
    int bits;
    sscanf(argv[3], "%d", &bits);

    if (bits > 7 || bits < 1 ) {
        printf("zła ilość bitów , podaj wartość od 1 do 7 \n");
        exit(1);
    } 

  
    unsigned char header[18];
    fread(&header,1,18,image);

    int height = header[14]+256*header[15];
    int width = header[12]+256*header[13];
    fseek(image, 0, SEEK_SET);
    fseek(image, -26, SEEK_END);


    size_t size = height * width;
    struct PIXEL* img = (struct PIXEL*) malloc(size * sizeof(struct PIXEL*));
    fseek(image, 18, SEEK_SET); 
    fread(img, 3, size, image);
    
    unsigned char footer[26];     
    fread(&footer,1,26,image);
    size_t half = size/2;
   
    struct PIXEL* temp_low = (struct PIXEL*) malloc(half * sizeof(struct PIXEL*));
    struct PIXEL2* temp_high = (struct PIXEL2*) malloc(size * sizeof(struct PIXEL2));
    for (size_t i = 1; i < half; i++) {
         temp_high[i] = high_pixel(img[2*i], img[2*i-1]);
         temp_low[i] = low_pixel(img[2*i], img[2*i-1]);
    }

    unsigned char* buff_low = malloc(half);
    struct PIXEL* result_low = quant_low(temp_low, buff_low, half, bits);
    free(temp_low);
    unsigned char* buff_high = malloc(half);
    struct PIXEL2* result_high = quant_high(temp_high, buff_high, half, bits);

    free(temp_high);
    write_to_file(width, height, (unsigned char)bits, result_low, buff_low, result_high, buff_high, outfile, (int) half);
}
