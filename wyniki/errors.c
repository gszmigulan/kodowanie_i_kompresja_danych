#include "../common/pixels.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {

   if(argc < 3){
      printf("za mało argumentów \n");
      exit(1);
   }

      FILE* infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        printf("Nie można otworzyć pliku wejściowego \n");
        exit(1);
    }
    
    FILE* outfile = fopen(argv[2], "rb");
    if (outfile == NULL) {
        printf("Nie można otworzyć pliku wyjściowego \n");
        exit(1);
    }

    size_t size;
    unsigned char header[18];
    fread(&header,1,18,infile);

    int height = header[14]+256*header[15];
    int width = header[12]+256*header[13];
    fseek(infile, 0, SEEK_SET);
    fseek(infile, -26, SEEK_END);


    size = height * width;
    struct PIXEL* image1 = (struct PIXEL*) malloc(size * sizeof(struct PIXEL*));
    fseek(infile, 18, SEEK_SET); 
    fread(image1, 3, size, infile);
    
    unsigned char footer2[26];     
    fread(&footer2,1,26,infile);



    size_t size_r;
    unsigned char header_r[18];
    fread(&header_r,1,18,outfile);

    int height_r = header_r[14]+256*header_r[15];
    int width_r = header_r[12]+256*header_r[13];
    fseek(outfile, 0, SEEK_SET);
    fseek(outfile, -26, SEEK_END);


    size_r = height_r * width_r;
    struct PIXEL* image2 = (struct PIXEL*) malloc(size_r * sizeof(struct PIXEL*));
    fseek(outfile, 18, SEEK_SET);
    fread(image2, 3, size, outfile);
    
    unsigned char footer_r[26];     
    fread(&footer_r,1,26,outfile);


     double all=0.0;
     double red=0.0;
     double green=0.0;
     double blue=0.0;
     double all2= 0.0;
     double red2= 0.0;
     double green2=0.0;
     double blue2= 0.0;
     struct PIXEL i1;
     struct PIXEL i2;
     double r;
     double g;
     double b;
 
     for (size_t i = 0; i < size; i++) {
        i1 = image1[i];
        i2 = image2[i];
        r = i1.r - i2.r;
        g = i1.g - i2.g;
        b = i1.b - i2.b;
        all += r * r + g * g + b * b;
        red += r * r;
        green += g * g;
        blue += b * b;
        all2 += i1.r * i1.r + i1.g * i1.g + i1.b * i1.b;
        red2 += i1.r * i1.r;
        green2 += i1.g * i1.g;
        blue2 += i1.b * i1.b;
    }

    all2 /= all;
    red2 /= red;
    green2 /= green;
    blue2 /= blue;
    all /= 3 * size;
    red /= size;
    green /= size;
    blue /= size;
    
    //printf("    |  all |  r  |  g  |  b  | \n");
    printf("MSE: all=%.4f, r=%.4f, g=%.4f,  b=%.4f\n", all, red, green, blue);
    printf("SNR: all=%.4f, r=%.4f, g=%.4f, b=%.4f\n", all2, red2, green2, blue2);
    printf("SNR(db): all=%.4f, r=%.4f, g=%.4f, b=%.4f\n", 10*log10(all2),  10*log10(red2), 10*log10(green2), 10*log10(blue2));
}
