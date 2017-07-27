#include <complex.h>
#include <stdlib.h>

#include "graph.h"
#include "mandelbrot.h"

size_t
choose_escape_color(const complex double c, const size_t maximum_iterations)
{
    size_t escape = 0;

    complex double z = 0.0 + I * 0.0;

    for (complex double temp; escape < maximum_iterations; escape++) {
        temp = cpow(z, 2);

        if (cabs(temp) > 2)
            break;

        z = temp + c;
    }

    if (escape == maximum_iterations)
        escape = 0;

    if (escape == 0)
        return 0;
    else if (escape <= (maximum_iterations / 7))
        return 1;
    else if (escape <= (maximum_iterations / 5))
        return 2;
    else
        return 3;

    return escape;
}

void mandelbrot(const char *backend, const char *outputfile,
        const size_t width, const size_t height, const size_t iterations,
        const complex double center, const double range)
{
    graph_t image = graph_create(backend, width, height, center, range);

    for (size_t i = 0; i < image.width; i++) {
        for (size_t j = 0; j < image.height; j++) {
            complex double c = graph_get_coordinates(image, i, j);
            size_t colormap_entry = choose_escape_color(c, iterations);
            graph_set_pixel(image, i, j, colormap_entry);
        }
    }

    graph_write(image, outputfile);

    graph_destroy(image);
}
