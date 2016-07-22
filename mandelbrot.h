#include <complex.h>

typedef struct extreme_coordinates {
    complex double lower_left;
    complex double upper_right;
} extremes_t;

void mandelbrot(const char *,const char *, size_t, size_t, size_t, complex double, double);
extremes_t get_extreme_coordinates(size_t, size_t, complex double, double);
complex double coords_for_pixel(size_t, size_t, complex double, double, size_t, size_t);
size_t choose_escape_color(complex double, size_t);
