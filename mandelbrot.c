#include <complex.h>
#include <string.h>

#include <cairo/cairo.h>
#include <gd.h>

#ifdef USE_MPC
#include <mpc.h>
#endif

#include "mandelbrot.h"

static const size_t NUM_COLORS = 4;
static const int rgb_colors[NUM_COLORS][3] = {
    {   0,   0,   0 },
    { 176, 229, 247 },
    { 245, 137, 169 },
    { 154, 227, 194 }
};

static void
populate_colors_cairo(double colors[NUM_COLORS][3])
{
    for (size_t i = 0; i < NUM_COLORS; i++)
        for (size_t j = 0; j < 3; j++)
            colors[i][j] = rgb_colors[i][j] / 255.0;
}

static void
populate_colors_gd(gdImagePtr im, int colors[NUM_COLORS])
{
    for (size_t i = 0; i < NUM_COLORS; i++)
        colors[i] = gdImageColorAllocate(im, rgb_colors[i][0], rgb_colors[i][1], rgb_colors[i][2]);
}

static void
set_pixel_cairo(cairo_t *cr, const size_t horizontal_pixel, const size_t vertical_pixel, const double color[])
{
    cairo_rectangle(cr, horizontal_pixel, vertical_pixel, 1, 1);
    cairo_set_source_rgb(cr, color[0], color[1], color[2]);
    cairo_fill(cr);
}

static void
set_pixel_gd(gdImagePtr im, const size_t horizontal_pixel, const size_t vertical_pixel, const int color)
{
    gdImageSetPixel(im, horizontal_pixel, vertical_pixel, color);
}

static double
horizontal_pixel_to_x_value(const extremes_t extremes, const size_t width, const size_t horizontal_pixel)
{
    const double minimum_x = creal(extremes.lower_left);
    const double maximum_x = creal(extremes.upper_right);

    return minimum_x
        + horizontal_pixel / (width / (maximum_x - minimum_x));
}

static double
vertical_pixel_to_y_value(const extremes_t extremes, const size_t height, const size_t vertical_pixel)
{
    const double minimum_y = cimag(extremes.lower_left);
    const double maximum_y = cimag(extremes.upper_right);

    return minimum_y
        + vertical_pixel / (height / (maximum_y - minimum_y));
}

extremes_t
get_extreme_coordinates(const size_t width, const size_t height, const complex double center, const double range)
{
    extremes_t coords;

    if (width < height) {
        coords.lower_left = creal(center) - (range * width/height) / 2.0
            + I * cimag(center)
            - I * (range) / 2.0;
        coords.upper_right = creal(center) + (range * width/height) / 2.0
            + I * cimag(center)
            + I * (range) / 2.0;
    } else {
        coords.lower_left = creal(center) - (range) / 2.0
            + I * cimag(center)
            - I * (range * height/width) / 2.0;
        coords.upper_right = creal(center) + (range) / 2.0
            + I * cimag(center)
            + I * (range * height/width) / 2.0;
    }

    return coords;
}

complex double
coords_for_pixel(const size_t width, const size_t height, const complex double center, const double range, const size_t i, const size_t j)
{
    extremes_t extremes = get_extreme_coordinates(width, height, center, range);
    return horizontal_pixel_to_x_value(extremes, width, i)
        + I * vertical_pixel_to_y_value(extremes, height, j);
}

size_t
choose_escape_color(const complex double c, const size_t maximum_iterations)
{
    size_t escape = 0;

#ifdef USE_MPC
    const mpfr_prec_t precision = 53;
    mpc_rnd_t rounding_mode = MPC_RNDNN;

    mpc_t my_z, my_temp, my_c;
    mpfr_t my_mpfr_absolute_value;
    mpc_init2(my_z, precision), mpc_init2(my_temp, precision), mpc_init2(my_c, precision);
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

static void
mandelbrot_cairo(const char *outputfile, const size_t width, const size_t height, const size_t iterations, const complex double center, const double range)
{
    cairo_surface_t *target = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
    cairo_t *cr = cairo_create(target);
    cairo_set_line_width(cr, 0.1);
    double colors[NUM_COLORS][3];

    populate_colors_cairo(colors);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            complex double coords = coords_for_pixel(width, height, center, range, i, j);
            set_pixel_cairo(cr, i, j, colors[choose_escape_color(coords, iterations)]);
        }
    }

    cairo_surface_write_to_png(target, outputfile);

    cairo_destroy(cr);
    cairo_surface_destroy(target);
}

static void
mandelbrot_gd(const char *outputfile, const size_t width, const size_t height, const size_t iterations, const complex double center, const double range)
{
    gdImagePtr im = gdImageCreate(width, height);
    FILE *pngout;
    int colors[NUM_COLORS];

    populate_colors_gd(im, colors);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            complex double coords = coords_for_pixel(width, height, center, range, i, j);
            set_pixel_gd(im, i, j, colors[choose_escape_color(coords, iterations)]);
        }
    }

    pngout = fopen(outputfile, "wb");
    gdImagePng(im, pngout);
 
    fclose(pngout);
    gdImageDestroy(im);
}

void mandelbrot(const char *backend, const char *outputfile, const size_t width, const size_t height, const size_t iterations, const complex double center, const double range)
{
    if (0 == strcmp("cairo", backend))
        mandelbrot_cairo(outputfile, width, height, iterations, center, range);
    else
        mandelbrot_gd(outputfile, width, height, iterations, center, range);
}
