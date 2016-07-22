#include <complex.h>
#include <stdbool.h>

#include <cairo/cairo.h>
#include <gd.h>

#ifdef USE_MPC
#include <mpc.h>
#endif

#include "mandelbrot.h"

const size_t MAXIMUM_ITERATIONS = 100;
const size_t NUM_COLORS = 5;
const char *OUTPUTFILE = "pngelbrot.png";

static int rgb_colors[NUM_COLORS][3] = {
    { 255, 255, 255 },
    {   0,   0,   0 },
    { 176, 229, 247 },
    { 245, 137, 169 },
    { 154, 227, 194 }
};

int gd_colors[NUM_COLORS];

static void
color_all_pixels_cairo(cairo_t *cr, double red, double green, double blue)
{
    cairo_set_source_rgb(cr, red, green, blue);
    cairo_paint(cr);
}

static void
color_all_pixels_gd(gdImagePtr im, size_t width, size_t height, int color)
{
    gdImageFilledRectangle(im, 0, 0, width - 1, height - 1, color);
}

static double
horizontal_pixel_to_x_value(extremes_t extremes, size_t width, int horizontal_pixel)
{
    const double minimum_x = creal(extremes.lower_left);
    const double maximum_x = creal(extremes.upper_right);

    return minimum_x
        + horizontal_pixel / (width / (maximum_x - minimum_x));
}

static double
vertical_pixel_to_y_value(extremes_t extremes, size_t height, int vertical_pixel)
{
    const double minimum_y = cimag(extremes.lower_left);
    const double maximum_y = cimag(extremes.upper_right);

    return minimum_y
        + vertical_pixel / (height / (maximum_y - minimum_y));
}

extremes_t
get_extreme_coordinates(size_t width, size_t height, complex double center, double range)
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
coords_for_pixel(size_t width, size_t height, complex double center, double range, size_t i, size_t j)
{
    extremes_t extremes = get_extreme_coordinates(width, height, center, range);
    return horizontal_pixel_to_x_value(extremes, width, i)
        + I * vertical_pixel_to_y_value(extremes, height, j);
}

size_t
count_escape(complex double c)
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

    for (; escape < MAXIMUM_ITERATIONS; escape++) {
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

    for (; escape < MAXIMUM_ITERATIONS; escape++) {
        temp = cpow(z, 2);

        if (cabs(temp) > 2)
            break;

        z = temp + c;
    }
#endif /* USE_MPC */

    if (escape == MAXIMUM_ITERATIONS)
        escape = 0;

    return escape;
}

static size_t
choose_color_cairo(size_t escape)
{
    if (escape == 0)
        return 1;
    else if (escape <= (MAXIMUM_ITERATIONS / 7))
        return 2;
    else if (escape <= (MAXIMUM_ITERATIONS / 5))
        return 3;
    else
        return 4;
}

static int
choose_color_gd(size_t escape)
{
    if (escape == 0)
        return gd_colors[1];
    else if (escape <= (MAXIMUM_ITERATIONS / 7))
        return gd_colors[2];
    else if (escape <= (MAXIMUM_ITERATIONS / 5))
        return gd_colors[3];
    else
        return gd_colors[4];
}

void
draw_something_cairo(size_t width, size_t height, complex double center, double range)
{
    cairo_surface_t *my_surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
    cairo_t *my_cairo = cairo_create(my_surface);
    double cairo_colors[NUM_COLORS][3];

    for (size_t i = 0; i < NUM_COLORS; i++) {
        for (size_t j = 0; j < 3; j++) {
            cairo_colors[i][j] = rgb_colors[i][j] / 255.0;
        }
    }

    color_all_pixels_cairo(my_cairo, cairo_colors[0][0], cairo_colors[0][1], cairo_colors[0][2]);

    cairo_set_line_width(my_cairo, 0.1);
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            size_t escape = count_escape(coords_for_pixel(width, height, center, range, i, j));
            size_t index = choose_color_cairo(escape);
            cairo_rectangle(my_cairo, i, j, 1, 1);
            cairo_set_source_rgb(my_cairo, cairo_colors[index][0], cairo_colors[index][1], cairo_colors[index][2]);
            cairo_fill(my_cairo);
        }
    }

    cairo_surface_write_to_png(my_surface, OUTPUTFILE);

    cairo_destroy(my_cairo);
    cairo_surface_destroy(my_surface);
}

void
draw_something_gd(size_t width, size_t height, complex double center, double range)
{
    gdImagePtr im = gdImageCreate(width, height);
    FILE *pngout;

    for (size_t i = 0; i < NUM_COLORS; i++) {
        gd_colors[i] = gdImageColorAllocate(im, rgb_colors[i][0], rgb_colors[i][1], rgb_colors[i][2]);
    }

    color_all_pixels_gd(im, width, height, gd_colors[0]);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            size_t escape = count_escape(coords_for_pixel(width, height, center, range, i, j));
            gdImageSetPixel(im, i, j, choose_color_gd(escape));
        }
    }

    pngout = fopen(OUTPUTFILE, "wb");
    gdImagePng(im, pngout);
 
    fclose(pngout);
    gdImageDestroy(im);
}
