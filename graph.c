#include <string.h>

#include <cairo/cairo.h>
#include <gd.h>
#include <Imlib2.h>

#include "graph.h"

static void
backend_cairo_create(graph_t *graph)
{
    graph->image_type = CAIRO;

    graph->image = cairo_create(
            cairo_image_surface_create(CAIRO_FORMAT_RGB24,
                graph->width, graph->height));

    cairo_set_line_width(graph->image, 0.1);
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
backend_cairo_write(const graph_t graph, const char *outputfile)
{
    cairo_surface_write_to_png(cairo_get_target(graph.image),
            outputfile);
}

static void
backend_cairo_destroy(const graph_t graph)
{
    cairo_destroy(graph.image);
}

static void
graph_backend_create(graph_t *graph, const char *backend)
{
    if (0 == strcmp("cairo", backend)) {
        backend_cairo_create(graph);
    } else if (0 == strcmp("imlib2", backend)) {
        graph->image_type = IMLIB2;

        graph->image = imlib_create_image(graph->width, graph->height);

        imlib_context_set_image(graph->image);
    } else {
        graph->image_type = GD;

        graph->image = gdImageCreate(graph->width, graph->height);

        for (size_t i = 0; i < NUM_COLORS; i++) {
            gdImageColorAllocate(graph->image,
                    graph->colormap[i][0],
                    graph->colormap[i][1],
                    graph->colormap[i][2]);
        }
    }
}

graph_t
graph_create(const char *backend, const size_t width, const size_t height,
        const complex double center, const double range)
{
    graph_t graph = {
        0,
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
    switch (graph.image_type) {
        case CAIRO:
            backend_cairo_set_pixel(graph, horizontal, vertical, colormap_entry);
            break;
        case GD:
            gdImageSetPixel(graph.image, horizontal, vertical,
                    colormap_entry);
            break;
        case IMLIB2:
            imlib_context_set_color(
                    graph.colormap[colormap_entry][0],
                    graph.colormap[colormap_entry][1],
                    graph.colormap[colormap_entry][2],
                    255);
            imlib_image_fill_rectangle(horizontal, vertical, 1, 1);
            break;
        default:
            break;
    }
}

void
graph_write(const graph_t graph, const char *outputfile)
{
    FILE *pngout;

    switch (graph.image_type) {
        case CAIRO:
            backend_cairo_write(graph, outputfile);
            break;
        case GD:
            pngout = fopen(outputfile, "wb");
            gdImagePng(graph.image, pngout);
            fclose(pngout);
            break;
        case IMLIB2:
            imlib_image_set_format("png");
            imlib_save_image(outputfile);
            break;
        default:
            break;
    }
}

void
graph_destroy(const graph_t graph)
{
    switch (graph.image_type) {
        case CAIRO:
            backend_cairo_destroy(graph);
            break;
        case GD:
            gdImageDestroy(graph.image);
            break;
        case IMLIB2:
            imlib_free_image();
            break;
        default:
            break;
    }
}

