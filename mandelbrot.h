#include <complex.h>

#define NUM_COLORS 4

typedef struct extreme_coordinates {
    complex double lower_left;
    complex double upper_right;
} extremes_t;

enum ImageType { CAIRO, GD };

typedef struct graph {
    enum ImageType image_type;
    union {
        void *gd_image;	    /* gdImagePtr */
        void *cairo_image;	/* cairo_t *  */
    };
    const size_t width;
    const size_t height;
    const complex double center;
    const double range;
    const extremes_t extremes;
    const size_t colormap[NUM_COLORS][3];
} graph_t;

void mandelbrot(const char *, const char *, const size_t, const size_t, const size_t, const complex double, const double);
extremes_t get_extreme_coordinates(const size_t, const size_t, const complex double, const double);
graph_t graph_create(const char *, const size_t, const size_t, const complex double, const double);
void graph_destroy(const graph_t);
complex double graph_get_coordinates(graph_t, const size_t, const size_t);
size_t choose_escape_color(const complex double, const size_t);
