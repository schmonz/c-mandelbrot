#include <complex.h>
#include <sys/types.h>

typedef struct extreme_coordinates {
    complex double lower_left;
    complex double upper_right;
} extremes_t;

void draw_something(size_t, size_t, double);
extremes_t get_extreme_coordinates(size_t, size_t, double);
complex double coords_for_pixel(size_t, size_t, double, size_t, size_t);
