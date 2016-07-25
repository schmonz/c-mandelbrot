#include <Imlib2.h>

static void
backend_imlib2_create(graph_t *graph)
{
    graph->image = imlib_create_image(graph->width, graph->height);
    imlib_context_set_image(graph->image);
}

static void
backend_imlib2_set_pixel(const graph_t graph,
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

static void
backend_imlib2_write(const graph_t graph, const char *outputfile)
{
    (void)graph;
    imlib_image_set_format("png");
    imlib_save_image(outputfile);
}

static void
backend_imlib2_destroy(const graph_t graph)
{
    (void)graph;
    imlib_free_image();
}

static struct backend backend_imlib2 = {
    "imlib2",
    backend_imlib2_create,
    backend_imlib2_set_pixel,
    backend_imlib2_write,
    backend_imlib2_destroy,
};
