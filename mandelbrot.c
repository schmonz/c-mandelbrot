#include <gd.h>

#include <complex.h>
#include <stdbool.h>
#include <stdio.h>

#include "mandelbrot.h"

static void
color_all_pixels(gdImagePtr im, int color)
{
    gdImageFilledRectangle(im, 0, 0, WIDTH - 1, HEIGHT - 1, color);
}

static double
horizontal_pixel_to_x_value(int horizontal_pixel)
{
    const double minimum_x = -2.0;
    const double maximum_x =  2.0;

    return minimum_x
        + horizontal_pixel / (WIDTH / (maximum_x - minimum_x));
}

static double
vertical_pixel_to_y_value(int vertical_pixel)
{
    const double minimum_y = -2.0;
    const double maximum_y =  2.0;

    return minimum_y
        + vertical_pixel / (HEIGHT/ (maximum_y - minimum_y));
}

struct ComplexPoint
coords_for_pixel(size_t i, size_t j)
{
    struct ComplexPoint rp = {
        horizontal_pixel_to_x_value(i),
        vertical_pixel_to_y_value(j)
    };

    return rp;
}

static bool
this_point_is_good(struct ComplexPoint rp)
{
    const size_t maximum_iterations = 100;

    complex double z =  0.0 + I * 0.0;
    complex double c = rp.x + I * rp.y;

    complex double temp;

    for (size_t iteration = 0; iteration < maximum_iterations; iteration++) {
        temp = cpow(z, 2);

        if (cabs(temp) > 2)
            return false;

        z = temp + c;
    }

    return true;
}

static void
maybe_color_this_pixel(gdImagePtr im, int color, size_t i, size_t j)
{
    if (this_point_is_good(coords_for_pixel(i, j)))
        gdImageSetPixel(im, i, j, color);
}

void
draw_something()
{
    gdImagePtr im = gdImageCreate(WIDTH, HEIGHT);
    FILE *pngout;
    int white = gdImageColorAllocate(im, 255, 255, 255);
    int black = gdImageColorAllocate(im, 0, 0, 0);

    color_all_pixels(im, white);
    
    for (size_t i = 0; i < WIDTH; i++)
        for (size_t j = 0; j < HEIGHT; j++)
            maybe_color_this_pixel(im, black, i, j);
    
    pngout = fopen("pngelbrot.png", "wb");
    gdImagePng(im, pngout);
    
    fclose(pngout);
    gdImageDestroy(im);
}
