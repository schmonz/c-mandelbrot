#include <gd.h>

#include <complex.h>
#include <stdbool.h>
#include <stdio.h>

#include "mandelbrot.h"

const size_t MAXIMUM_ITERATIONS = 100;
int escape_time_colors[5];

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
count_escape_time(struct ComplexPoint rp)
{
    complex double z =  0.0 + I *  0.0;
    complex double c = rp.x + I * rp.y;

    complex double temp;

    for (size_t escape_time = 0; escape_time < MAXIMUM_ITERATIONS; escape_time++) {
        temp = cpow(z, 2);

        if (cabs(temp) > 2)
            return escape_time;

        z = temp + c;
    }

    return 0;
}

void
draw_something()
{
    gdImagePtr im = gdImageCreate(WIDTH, HEIGHT);
    FILE *pngout;
    escape_time_colors[0] = gdImageColorAllocate(im, 255, 255, 255);
    escape_time_colors[1] = gdImageColorAllocate(im,   0,   0,   0);
    escape_time_colors[2] = gdImageColorAllocate(im, 176, 229, 247);
    escape_time_colors[4] = gdImageColorAllocate(im, 154, 227, 194);
    escape_time_colors[3] = gdImageColorAllocate(im, 245, 137, 169);

    color_all_pixels(im, escape_time_colors[0]);
    
    for (size_t i = 0; i < WIDTH; i++) {
        for (size_t j = 0; j < HEIGHT; j++) {
            size_t escape_time = count_escape_time(coords_for_pixel(i, j));
            if (escape_time == 0)
                gdImageSetPixel(im, i, j, escape_time_colors[1]);
            else if (escape_time <= (MAXIMUM_ITERATIONS / 7))
                gdImageSetPixel(im, i, j, escape_time_colors[2]);
            else if (escape_time <= (MAXIMUM_ITERATIONS / 5))
                gdImageSetPixel(im, i, j, escape_time_colors[3]);
            else
                gdImageSetPixel(im, i, j, escape_time_colors[4]);
        }
    }
    
    pngout = fopen("pngelbrot.png", "wb");
    gdImagePng(im, pngout);
    
    fclose(pngout);
    gdImageDestroy(im);
}
