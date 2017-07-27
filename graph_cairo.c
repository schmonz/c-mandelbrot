#include <cairo/cairo.h>

#include "graph.h"

void
graph_backend_create(graph_t *graph)
{
    graph->image = cairo_create(
            cairo_image_surface_create(CAIRO_FORMAT_RGB24,
                graph->width, graph->height));
    cairo_set_line_width(graph->image, 0.1);
}

void
graph_backend_set_pixel(const graph_t graph,
        const size_t horizontal, const size_t vertical,
        const size_t colormap_entry)
{
    cairo_rectangle(graph.image, horizontal, vertical, 1, 1);
    cairo_set_source_rgb(graph.image,
            graph.colormap[colormap_entry][0] / 255.0,
            graph.colormap[colormap_entry][1] / 255.0,
            graph.colormap[colormap_entry][2] / 255.0);
    cairo_fill(graph.image);
}

void
graph_backend_write(const graph_t graph, const char *outputfile)
{
    cairo_surface_write_to_png(cairo_get_target(graph.image), outputfile);
}

void
graph_backend_destroy(const graph_t graph)
{
    cairo_surface_destroy(cairo_get_target(graph.image));
    cairo_destroy(graph.image);
}
