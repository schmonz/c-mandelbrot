#include <Imlib2.h>

#include "graph.h"

void
graph_backend_create(graph_t *graph)
{
    graph->image = imlib_create_image(graph->width, graph->height);
    imlib_context_set_image(graph->image);
}

void
graph_backend_set_pixel(const graph_t graph,
        const size_t horizontal, const size_t vertical,
        const size_t colormap_entry)
{
    imlib_context_set_color(
            graph.colormap[colormap_entry][0],
            graph.colormap[colormap_entry][1],
            graph.colormap[colormap_entry][2],
            255);
    imlib_image_fill_rectangle(horizontal, vertical, 1, 1);
}

void
graph_backend_write(const graph_t graph, const char *outputfile)
{
    (void)graph;
    imlib_image_set_format("png");
    imlib_save_image(outputfile);
}

void
graph_backend_destroy(const graph_t graph)
{
    (void)graph;
    imlib_free_image();
}
