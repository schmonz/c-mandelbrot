#include <string.h>

#include <cairo/cairo.h>
#include <gd.h>
#include <Imlib2.h>

#include "graph.h"

static void
backend_cairo_create(graph_t *graph)
{
    graph->image = cairo_create(
            cairo_image_surface_create(CAIRO_FORMAT_RGB24,
                graph->width, graph->height));

    cairo_set_line_width(graph->image, 0.1);
}

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
backend_imlib2_create(graph_t *graph)
{
    graph->image = imlib_create_image(graph->width, graph->height);

    imlib_context_set_image(graph->image);
}

static void
backend_cairo_set_pixel(const graph_t graph,
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

static void
backend_gd_set_pixel(const graph_t graph,
        const size_t horizontal, const size_t vertical,
        const size_t colormap_entry)
{
    gdImageSetPixel(graph.image, horizontal, vertical, colormap_entry);
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
backend_cairo_write(const graph_t graph, const char *outputfile)
{
    cairo_surface_write_to_png(cairo_get_target(graph.image),
            outputfile);
}

static void
backend_gd_write(const graph_t graph, const char *outputfile)
{
    FILE *pngout = fopen(outputfile, "wb");
    gdImagePng(graph.image, pngout);
    fclose(pngout);
}

static void
backend_imlib2_write(const graph_t graph, const char *outputfile)
{
    (void)graph;
    imlib_image_set_format("png");
    imlib_save_image(outputfile);
}

static void
backend_cairo_destroy(const graph_t graph)
{
    cairo_destroy(graph.image);
}

static void
backend_gd_destroy(const graph_t graph)
{
    gdImageDestroy(graph.image);
}

static void
backend_imlib2_destroy(const graph_t graph)
{
    (void)graph;
    imlib_free_image();
}

struct backend {
    const char *name;
    void (*create)(graph_t *);
    void (*set_pixel)(graph_t, size_t, size_t, size_t);
    void (*write)(graph_t, const char *);
    void (*destroy)(graph_t);
};

static struct backend backends[] = {
    {
        "cairo",
        backend_cairo_create,
        backend_cairo_set_pixel,
        backend_cairo_write,
        backend_cairo_destroy,
    },
    {
        "imlib2",
        backend_imlib2_create,
        backend_imlib2_set_pixel,
        backend_imlib2_write,
        backend_imlib2_destroy,
    },
    {
        "gd",
        backend_gd_create,
        backend_gd_set_pixel,
        backend_gd_write,
        backend_gd_destroy,
    },
};
static const size_t NUM_BACKENDS = sizeof(backends) / (sizeof(backends[0]));

static struct backend active_backend;

static void
graph_backend_create(graph_t *graph, const char *backend)
{
    for (size_t i = 0; i < NUM_BACKENDS; i++) {
        active_backend = backends[i];
        if (0 == strcmp(active_backend.name, backend))
            break;
    }

    active_backend.create(graph);
}

graph_t
graph_create(const char *backend, const size_t width, const size_t height,
        const complex double center, const double range)
{
    graph_t graph = {
        NULL,
        width,
        height,
        center,
        range,
        get_extreme_coordinates(width, height, center, range),
        {
            {   0,   0,   0 },
            { 176, 229, 247 },
            { 245, 137, 169 },
            { 154, 227, 194 },
        },
    };

    graph_backend_create(&graph, backend);

    return graph;
}

extremes_t
get_extreme_coordinates(const size_t width, const size_t height,
    const complex double center, const double range)
{
    extremes_t coords;

    if (width < height) {
        coords.lower_left =
            creal(center) - (range * width/height) / 2.0
            + I * cimag(center)
            - I * (range) / 2.0;
        coords.upper_right =
            creal(center) + (range * width/height) / 2.0
            + I * cimag(center)
            + I * (range) / 2.0;
    } else {
        coords.lower_left =
            creal(center) - (range) / 2.0
            + I * cimag(center)
            - I * (range * height/width) / 2.0;
        coords.upper_right =
            creal(center) + (range) / 2.0
            + I * cimag(center)
            + I * (range * height/width) / 2.0;
    }

    return coords;
}

complex double
graph_get_coordinates(const graph_t graph, const size_t i, const size_t j)
{
    const double minimum_a = creal(graph.extremes.lower_left);
    const double maximum_a = creal(graph.extremes.upper_right);
    const double minimum_b = cimag(graph.extremes.lower_left);
    const double maximum_b = cimag(graph.extremes.upper_right);

    return (minimum_a + i / (graph.width / (maximum_a - minimum_a)))
        + I * (minimum_b + j / (graph.height / (maximum_b - minimum_b)));
}

void
graph_set_pixel(const graph_t graph,
        const size_t horizontal, const size_t vertical,
        const size_t colormap_entry)
{
    active_backend.set_pixel(graph, horizontal, vertical, colormap_entry);
}

void
graph_write(const graph_t graph, const char *outputfile)
{
    active_backend.write(graph, outputfile);
}

void
graph_destroy(const graph_t graph)
{
    active_backend.destroy(graph);
}
