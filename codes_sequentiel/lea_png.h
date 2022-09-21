#ifndef LEA
#define LEA

#include "libattopng.h"

typedef struct matrix
{
    int n;
    int m;
    int **data;
} matrix_t;

typedef enum cm
{
    BLACK_AND_WHITE,
    GREY,
    RGB
} colormap_t;

void save_png(const char *filename, colormap_t cm, matrix_t *image);

#endif