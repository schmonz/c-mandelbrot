#include <gd.h>

#include "graph.h"

void
graph_backend_create(graph_t *graph)
{
    graph->image = gdImageCreate(graph->width, graph->height);
    for (size_t i = 0; i < NUM_COLORS; i++) {
        gdImageColorAllocate(graph->image,
                graph->colormap[i][0],
                graph->colormap[i][1],
                graph->colormap[i][2]);
    }
}

void
graph_backend_set_pixel(const graph_t graph,
        const size_t horizontal, const size_t vertical,
        const size_t colormap_entry)
{
    gdImageSetPixel(graph.image, horizontal, vertical, colormap_entry);
}

void
graph_backend_write(const graph_t graph, const char *outputfile)
{
    FILE *pngout = fopen(outputfile, "wb");
    gdImagePng(graph.image, pngout);
    fclose(pngout);
}

void
graph_backend_destroy(const graph_t graph)
{
    gdImageDestroy(graph.image);
}
