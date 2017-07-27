#include "graph.h"

void graph_backend_cairo_create(graph_t *);

void graph_backend_cairo_set_pixel(const graph_t,
        const size_t, const size_t, const size_t);

void graph_backend_cairo_write(const graph_t, const char *);

void graph_backend_cairo_destroy(const graph_t);

graph_backend_t graph_backend_cairo;
