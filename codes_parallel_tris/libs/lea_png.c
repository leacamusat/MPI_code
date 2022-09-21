#include "lea_png.h"
#include <stdlib.h>
#include <stdio.h>

#define TO_BYTE(f) (f < 1.0 ? (int)(256 * f) : 255)
#define MAKE_RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
#define MAKE_RGB(r, g, b) MAKE_RGBA(r, g, b, 0xff)

typedef struct tuple
{
    int min;
    int max;
} tuple_t;
typedef int (*pixel_compute_t)(tuple_t, int);

tuple_t min_max_matrix(matrix_t *mat)
{
    tuple_t t;
    switch (sizeof(int))
    {
    case 1:
        t.max = (int)INT8_MIN;
        t.min = (int)INT8_MAX;
        break;
    case 2:
        t.max = (int)INT16_MIN;
        t.min = (int)INT16_MAX;
        break;
    case 8:
        t.max = (int)INT64_MIN;
        t.min = (int)INT64_MAX;
        break;
    default:
        t.max = (int)INT32_MIN;
        t.min = (int)INT32_MAX;
    }
    for (int i = 0; i < mat->n; i++)
    {
        for (int j = 0; j < mat->m; j++)
        {
            if (mat->data[i][j] > t.max)
            {
                t.max = mat->data[i][j];
            }
            if (mat->data[i][j] < t.min)
            {
                t.min = mat->data[i][j];
            }
        }
    }
    return t;
}

int bw_pixel_compute(tuple_t span, int val)
{
    return 255 * ((val - span.min) > (span.max - span.min) / 2);
}
int grey_pixel_compute(tuple_t span, int val)
{
    double normalized = (double)(val - span.min) / (double)((long)span.max - (long)span.min);
    // Values in [0;256[
    return TO_BYTE(normalized);
}
int color_pixel_compute(tuple_t span, int val)
{
    double normalized = (double)(val - span.min) / (double)((long)span.max - (long)span.min);
    double tmp = 1 - normalized;
    int red = TO_BYTE(normalized), green = TO_BYTE(tmp), blue = 255;
    return MAKE_RGB(red, green, blue);
}
void save_png(const char *filename, colormap_t cm, matrix_t *img)
{
    libattopng_type_t palette;
    pixel_compute_t pc;
    switch (cm)
    {
    case BLACK_AND_WHITE:
        palette = PNG_GRAYSCALE;
        pc = bw_pixel_compute;
        break;
    case GREY:
        palette = PNG_GRAYSCALE;
        pc = grey_pixel_compute;
        break;
    case RGB:
        palette = PNG_RGB;
        pc = color_pixel_compute;
        break;
    default:
        perror("Unknown colormap\n");
        exit(1);
    }

    libattopng_t *png = libattopng_new(img->m, img->n, palette);
    tuple_t span = min_max_matrix(img);
    for (int i = 0; i < img->n; i++)
    {
        for (int j = 0; j < img->m; j++)
        {
            libattopng_set_pixel(png, j, i, pc(span, img->data[i][j]));
        }
    }

    libattopng_save(png, filename);
    libattopng_destroy(png);
}