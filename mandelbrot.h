#include <complex.h>

typedef struct extreme_coordinates {
    complex double lower_left;
    complex double upper_right;
} extremes_t;

void mandelbrot(const char *, const char *, const size_t, const size_t, const size_t, const complex double, const double);
extremes_t get_extreme_coordinates(const size_t, const size_t, const complex double, const double);
complex double coords_for_pixel(const size_t, const size_t, const complex double, const double, const size_t, const size_t);
size_t choose_escape_color(const complex double, const size_t);
