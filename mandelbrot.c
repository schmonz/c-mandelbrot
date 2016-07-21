#include <gd.h>

#include <complex.h>
#include <stdbool.h>

#ifdef USE_MPC
#include <mpc.h>
#endif

#include "mandelbrot.h"

const size_t MAXIMUM_ITERATIONS = 100;
int escape_colors[5];

static void
color_all_pixels(gdImagePtr im, size_t width, size_t height, int color)
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

#ifdef USE_MPC
size_t
count_escape(complex double c)
{
    /*
     * this seems slow! but maybe needed past a certain zoom level
     * then zoom and zoom and zoom and see when the image differs
     * if it's good, extract both impls into libs
     * choose best available at build/link time
     * detect MPC more accurately
     */
    size_t escape = 0;

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

    if (escape == MAXIMUM_ITERATIONS)
        escape = 0;

    return escape;
}

#else
size_t
count_escape(complex double c)
{
    size_t escape = 0;

    complex double z = 0.0 + I * 0.0;
    complex double temp;

    for (; escape < MAXIMUM_ITERATIONS; escape++) {
        temp = cpow(z, 2);

        if (cabs(temp) > 2)
            break;

        z = temp + c;
    }

    if (escape == MAXIMUM_ITERATIONS)
        escape = 0;

    return escape;
}
#endif

static int
choose_color_for_escape(size_t escape)
{
    if (escape == 0)
        return escape_colors[1];
    else if (escape <= (MAXIMUM_ITERATIONS / 7))
        return escape_colors[2];
    else if (escape <= (MAXIMUM_ITERATIONS / 5))
        return escape_colors[3];
    else
        return escape_colors[4];
}

void
draw_something(size_t width, size_t height, complex double center, double range)
{
    gdImagePtr im = gdImageCreate(width, height);
    FILE *pngout;
    escape_colors[0] = gdImageColorAllocate(im, 255, 255, 255);
    escape_colors[1] = gdImageColorAllocate(im,   0,   0,   0);
    escape_colors[2] = gdImageColorAllocate(im, 176, 229, 247);
    escape_colors[3] = gdImageColorAllocate(im, 245, 137, 169);
    escape_colors[4] = gdImageColorAllocate(im, 154, 227, 194);

    color_all_pixels(im, width, height, escape_colors[0]);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            size_t escape = count_escape(coords_for_pixel(width, height, center, range, i, j));
            gdImageSetPixel(im, i, j, choose_color_for_escape(escape));
        }
    }

    pngout = fopen("pngelbrot.png", "wb");
    gdImagePng(im, pngout);
 
    fclose(pngout);
    gdImageDestroy(im);
}
