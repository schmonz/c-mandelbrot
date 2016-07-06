#include <gd.h>

#include <stdbool.h>
#include <stdio.h>

#include "mandelbrot.h"

static void
color_all_pixels(gdImagePtr im, int color)
{
    gdImageFilledRectangle(im, 0, 0, WIDTH - 1, HEIGHT - 1, color);
}

static bool
this_pixel_is_good(size_t x, size_t y)
{
    return ((x - y) % 5 == 0);
}

static void
maybe_color_this_pixel(gdImagePtr im, int color, size_t x, size_t y)
{
    if (this_pixel_is_good(x, y))
        gdImageSetPixel(im, x, y, color);
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

double
horizontal_pixel_to_x_value(int horizontal_pixel)
{
    const int minimum_x = -2;
    const int maximum_x =  2;

    return minimum_x
        + horizontal_pixel / (WIDTH / (maximum_x - minimum_x));
}

double
vertical_pixel_to_y_value(int vertical_pixel)
{
    const int minimum_y = -2;
    const int maximum_y =  2;

    return minimum_y
        + vertical_pixel / (HEIGHT/ (maximum_y - minimum_y));
}
