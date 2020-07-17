
#include "../common/pixels.h"
#include "comp.h"

#include <memory.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include<stdio.h>

unsigned char closest(struct PIXEL pixel, int q, struct PIXEL* data) {
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int sum;
    unsigned int min = UINT_MAX;
    int id = -1;
    
    for (int i = 0; i < q; i++) {
      red = (unsigned int) abs((int)pixel.r - (int)data[i].r);
      green = (unsigned int) abs((int)pixel.g -  (int)data[i].g);
      blue = (unsigned int) abs((int)pixel.b - (int)data[i].b);
      sum = red + green + blue;

        if (sum < min) {
            id = i;
            min = sum;
        }
    }
    return (unsigned char) id;
}


double get_err( struct PIXEL* input, unsigned char* nr, size_t size, struct PIXEL* data ) {
    double all=0.0;
    for (size_t i = 0; i < size; i++) {
        struct PIXEL p = input[i];
        struct PIXEL p1 = data[nr[i]];

        double r = p.r - p1.r;
        double g = p.g - p1.g;
        double b = p.b - p1.b;
        all += r * r + g * g + b * b;
    }
    all /= 3 * size;
    return all;
}
struct PIXEL* quant_low(struct PIXEL* input, unsigned char* nr, size_t size, int bits) {
    
    const double epsilon = 1e-7;
    const int quants = 1 << bits;
    struct PIXEL* data = (struct PIXEL*) calloc((size_t)quants, sizeof(struct PIXEL));
     srand((unsigned int)time(NULL));
    for (int i = 0; i < quants; i++) {
        unsigned int idx = (unsigned int)rand() % size;
        data[i] = input[idx];
    }
      for (size_t i = 0; i < size; i++) {
       nr[i] = closest(input[i], quants, data);
    }
    double error = get_err(input, nr, size, data);
    for (int i = 0; i < 256; i++) {
          unsigned long long* red = calloc((size_t)quants, sizeof(unsigned long long));
          unsigned long long* green = calloc((size_t)quants, sizeof(unsigned long long));
          unsigned long long* blue = calloc((size_t)quants, sizeof(unsigned long long));
          unsigned long long* s = calloc((size_t)quants, sizeof(unsigned long long));
          unsigned char id;

          for (size_t i = 0; i < size; i++) {
            id = nr[i];
            red[id] += input[i].r;
            green[id] += input[i].g;
            blue[id] += input[i].b;
            ++s[id];
           }

           for (int i = 0; i < quants; i++) {
              if (s[i] == 0) {
                 data[i].r = 0;
                 data[i].g = 0;
                 data[i].b = 0;
                 break;
              }
               data[i].r = (unsigned char)(red[i]/ s[i]);
               data[i].g = (unsigned char)(green[i]/ s[i]);
               data[i].b = (unsigned char)(blue[i]/ s[i]);
           }

           free(red);free(green);
           free(blue);free(s);
         for (size_t i = 0; i < size; i++) {
            nr[i] = closest(input[i], quants, data);
         }

        double new_err = get_err(input, nr, size, data);
        double derr = (error - new_err) / error;

        if (fabs(derr) < epsilon) break; 
        error = new_err;
    }
    return data;
}






