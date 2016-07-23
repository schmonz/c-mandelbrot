#include <string.h>

#include <cairo/cairo.h>
#include <gd.h>

#include "graph.h"

graph_t
graph_create(const char *backend, const size_t width, const size_t height,
        const complex double center, const double range)
{
    graph_t image = {
        0,
        { NULL },
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

    if (0 == strcmp("cairo", backend)) {
        image.image_type = CAIRO;

        image.cairo_image = cairo_create(
                cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height));

        cairo_set_line_width(image.cairo_image, 0.1);
    } else {
        image.image_type = GD;

        image.gd_image = gdImageCreate(width, height);

        for (size_t i = 0; i < NUM_COLORS; i++) {
            gdImageColorAllocate(image.gd_image,
                    image.colormap[i][0],
                    image.colormap[i][1],
                    image.colormap[i][2]);
        }
    }

    return image;
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

static double
graph_get_a(const graph_t graph, const size_t horizontal)
{
    const double minimum_a = creal(graph.extremes.lower_left);
    const double maximum_a = creal(graph.extremes.upper_right);

    return minimum_a
        + horizontal / (graph.width / (maximum_a - minimum_a));
}

static double
graph_get_b(const graph_t graph, const size_t vertical)
{
    const double minimum_b = cimag(graph.extremes.lower_left);
    const double maximum_b = cimag(graph.extremes.upper_right);

    return minimum_b
        + vertical / (graph.height / (maximum_b - minimum_b));
}

complex double
graph_get_coordinates(const graph_t graph, const size_t i, const size_t j)
{
    return graph_get_a(graph, i) + I * graph_get_b(graph, j);
}

void
graph_set_pixel(const graph_t graph,
        const size_t horizontal, const size_t vertical,
        const size_t colormap_entry)
{
    switch (graph.image_type) {
        case CAIRO:
            cairo_rectangle(graph.cairo_image, horizontal, vertical, 1, 1);
            cairo_set_source_rgb(graph.cairo_image,
                    graph.colormap[colormap_entry][0] / 255.0,
                    graph.colormap[colormap_entry][1] / 255.0,
                    graph.colormap[colormap_entry][2] / 255.0);
            cairo_fill(graph.cairo_image);
            break;
        case GD:
            gdImageSetPixel(graph.gd_image, horizontal, vertical,
                    colormap_entry);
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
            cairo_surface_write_to_png(cairo_get_target(graph.cairo_image),
                    outputfile);
            break;
        case GD:
            pngout = fopen(outputfile, "wb");
            gdImagePng(graph.gd_image, pngout);
            fclose(pngout);
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
            cairo_destroy(graph.cairo_image);
            break;
        case GD:
            gdImageDestroy(graph.gd_image);
            break;
        default:
            break;
    }
}

