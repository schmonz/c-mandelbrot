#include <complex.h>
#include <sys/types.h>

struct extreme_coordinates {
    complex double lower_left;
    complex double upper_right;
};
typedef struct extreme_coordinates extremes_t;

void draw_something(size_t, size_t);
extremes_t get_extreme_coordinates(size_t, size_t);
complex double coords_for_pixel(size_t, size_t, size_t, size_t);
