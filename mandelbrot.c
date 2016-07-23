#include <complex.h>
#include <string.h>

#include <cairo/cairo.h>
#include <gd.h>

#ifdef USE_MPC
#include <mpc.h>
#endif

#include "mandelbrot.h"

size_t
choose_escape_color(const complex double c, const size_t maximum_iterations)
{
    size_t escape = 0;

#ifdef USE_MPC
    const mpfr_prec_t precision = 53;
    mpc_rnd_t rounding_mode = MPC_RNDNN;

    mpc_t my_z, my_temp, my_c;
    mpfr_t my_mpfr_absolute_value;
    mpc_init2(my_z, precision), mpc_init2(my_temp, precision),
        mpc_init2(my_c, precision);
    mpfr_init2(my_mpfr_absolute_value, precision);

    mpc_set_dc(my_c, c, rounding_mode);
    mpc_set_dc(my_z, 0.0 + I * 0.0, rounding_mode);

    for (; escape < maximum_iterations; escape++) {
        mpc_pow_d(my_temp, my_z, 2.0, rounding_mode);

        mpc_abs(my_mpfr_absolute_value, my_temp, rounding_mode);
        if (mpfr_get_d(my_mpfr_absolute_value, rounding_mode) > 2)
            break;

        mpc_add(my_z, my_temp, my_c, rounding_mode);
    }

    mpfr_clear(my_mpfr_absolute_value);
    mpc_clear(my_c);
    mpc_clear(my_temp);
    mpc_clear(my_z);
#else
    complex double z = 0.0 + I * 0.0;
    complex double temp;

    for (; escape < maximum_iterations; escape++) {
        temp = cpow(z, 2);

        if (cabs(temp) > 2)
            break;

        z = temp + c;
    }
#endif /* USE_MPC */

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

static void
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

static void
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
