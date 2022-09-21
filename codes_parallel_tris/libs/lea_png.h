#ifndef LEA
#define LEA

#include "libattopng.h"

typedef struct matrix
{
    int n; // Nombre de lignes
    int m; // Nombre de colonnes
    int **data;
} matrix_t;

typedef enum cm
{
    BLACK_AND_WHITE, // Noir et blanc
    GREY,            // Niveaux de gris
    RGB              // Couleurs RGB (heatmap)
} colormap_t;

void save_png(const char *filename, colormap_t cm, matrix_t *image);

#endif