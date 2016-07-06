#include <gd.h>

#include <complex.h>
#include <stdbool.h>
#include <stdio.h>

#include "mandelbrot.h"

const size_t MAXIMUM_ITERATIONS = 100;

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

static size_t
is_in_mandelbrot_set(struct ComplexPoint rp)
{
    complex double z =  0.0 + I *  0.0;
    complex double c = rp.x + I * rp.y;

    complex double temp;

    for (size_t iteration = 0; iteration < MAXIMUM_ITERATIONS; iteration++) {
        temp = cpow(z, 2);

        if (cabs(temp) > 2)
            return iteration;

        z = temp + c;
    }

    return 0;
}

void
draw_something()
{
    gdImagePtr im = gdImageCreate(WIDTH, HEIGHT);
    FILE *pngout;
    int white = gdImageColorAllocate(im, 255, 255, 255);
    int black = gdImageColorAllocate(im,   0,   0,   0);
    int blue  = gdImageColorAllocate(im, 176, 229, 247);

    color_all_pixels(im, white);
    
    for (size_t i = 0; i < WIDTH; i++) {
        for (size_t j = 0; j < HEIGHT; j++) {
            size_t iterations = is_in_mandelbrot_set(coords_for_pixel(i, j));
            if (iterations == 0)
                gdImageSetPixel(im, i, j, black);
            else
                gdImageSetPixel(im, i, j, blue);
        }
    }
    
    pngout = fopen("pngelbrot.png", "wb");
    gdImagePng(im, pngout);
    
    fclose(pngout);
    gdImageDestroy(im);
}
