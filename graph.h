#include <complex.h>

typedef struct extreme_coordinates {
    complex double lower_left;
    complex double upper_right;
} extremes_t;

#define NUM_COLORS 4
typedef struct graph {
    void *image;
    const size_t width;
    const size_t height;
    const complex double center;
    const double range;
    const extremes_t extremes;
    const size_t colormap[NUM_COLORS][3];
} graph_t;

typedef struct graph_backend {
    const char *name;
    void (*create)(graph_t *);
    void (*set_pixel)(graph_t, size_t, size_t, size_t);
    void (*write)(graph_t, const char *);
    void (*destroy)(graph_t);
} graph_backend_t;

extremes_t get_extreme_coordinates(const size_t, const size_t, const complex double, const double);
graph_t graph_create(const char *, const size_t, const size_t, const complex double, const double);
void graph_destroy(const graph_t);
complex double graph_get_coordinates(graph_t, const size_t, const size_t);
void graph_set_pixel(const graph_t, const size_t, const size_t, const size_t);
void graph_write(const graph_t, const char *);
