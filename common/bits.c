#include "bits.h"

#include <stdio.h>
#include <stdlib.h>


struct my_HEADER init_header(unsigned short w, unsigned short h){
   
   return (struct my_HEADER){
     .idlenght =0,
     .colormaptype= 0,
     .datatypecode=2,
     .colourmaporigin=0,
     .colourmaplength=0,
     .colourmapdepth=0,
     .x_origin=0,
     .y_origin=0,
     .width= w,
     .height= h,
     .bitsperpixel=24,
     .imgagedescriptor=0

    };
}

static unsigned char read_bit(struct myFile* file) {
    if (file->bits == 0) {
        int c = fgetc(file->file);
        if (c == EOF) {
            printf("błąd! niespodziewany koniec pliku");
            exit(1);
        }
        file->bits = 8;
        file->byte = (unsigned char) c;
    }
    unsigned char bit = (file->byte & 0x80) >> 7;
    file->byte <<= 1;
    --file->bits;
    return bit;
}


static void to_file(struct myFile* file){
        fputc(file->byte, file->file);
        file->bits = 0;
        file->byte = 0;
} 

static void write_bit(struct myFile* file, unsigned char bit) {
    file->byte <<= 1;
    if (bit) ++file->byte;
    ++file->bits;
    if (file->bits >= 8) {
        to_file(file);  
    }
}

void flush(struct myFile* file) {
    while (file->bits > 0) {
        write_bit(file, 0);
    }
}

unsigned short read_bits(struct myFile* file, int bits) {
    unsigned short value = 0;
    for (int i = 0; i < bits; i++) {
        unsigned char bit = read_bit(file);
        value <<= 1;
        if (bit) value++;
    }
    return value;
}

void write_it(struct myFile* file, unsigned short v, int bits) {
    for (int i = (bits-1); i >= 0; --i) {
        unsigned char bit = (unsigned char)((v & 1 << i) >> i);
        write_bit(file, bit);
    }
}
