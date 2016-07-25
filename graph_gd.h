#include <gd.h>

static void
backend_gd_create(graph_t *graph)
{
    graph->image = gdImageCreate(graph->width, graph->height);
    for (size_t i = 0; i < NUM_COLORS; i++) {
        gdImageColorAllocate(graph->image,
                graph->colormap[i][0],
                graph->colormap[i][1],
                graph->colormap[i][2]);
    }
}

static void
backend_gd_set_pixel(const graph_t graph,
        const size_t horizontal, const size_t vertical,
        const size_t colormap_entry)
{
    gdImageSetPixel(graph.image, horizontal, vertical, colormap_entry);
}

static void
backend_gd_write(const graph_t graph, const char *outputfile)
{
    FILE *pngout = fopen(outputfile, "wb");
    gdImagePng(graph.image, pngout);
    fclose(pngout);
}

static void
backend_gd_destroy(const graph_t graph)
{
    gdImageDestroy(graph.image);
}

static struct backend backend_gd = {
    "gd",
    backend_gd_create,
    backend_gd_set_pixel,
    backend_gd_write,
    backend_gd_destroy,
};
