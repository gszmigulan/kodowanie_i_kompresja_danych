#include "../common/pixels.h"
#include <stdlib.h>

struct PIXEL* quant_low(struct PIXEL* input, unsigned char* quantIds, size_t size, int bits);
unsigned char closest(struct PIXEL pixel, int quants, struct PIXEL* data);
double get_err( struct PIXEL* input, unsigned char* nr, size_t size, struct PIXEL* data);


struct PIXEL2* quant_high( struct PIXEL2* input, unsigned char* nr, size_t size, int bits);
double get_err2( struct PIXEL2* input, unsigned char* quantIds, size_t size, struct PIXEL2* centroids);
unsigned char closest2(struct PIXEL2 pixel, int quants, struct PIXEL2* data);
