#include <string.h>

#include "graph.h"
#include "graph_cairo.h"
#include "graph_gd.h"
#include "graph_imlib2.h"

static graph_backend_t graph_backend;

static void
graph_backend_create(graph_t *graph, const char *desired_graph_backend)
{
    const size_t NUM_BACKENDS = 3;
    graph_backend_t graph_backends[NUM_BACKENDS];

    graph_backends[0] = graph_backend_cairo;
    graph_backends[1] = graph_backend_imlib2;
    graph_backends[2] = graph_backend_gd;

    for (size_t i = 0; i < NUM_BACKENDS; i++) {
        graph_backend = graph_backends[i];
        if (0 == strcmp(graph_backend.name, desired_graph_backend))
            break;
    }

    graph_backend.create(graph);
}

graph_t
graph_create(const char *desired_graph_backend,
        const size_t width, const size_t height,
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

    graph_backend_create(&graph, desired_graph_backend);

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
    graph_backend.set_pixel(graph, horizontal, vertical, colormap_entry);
}

void
graph_write(const graph_t graph, const char *outputfile)
{
    graph_backend.write(graph, outputfile);
}

void
graph_destroy(const graph_t graph)
{
    graph_backend.destroy(graph);
}
