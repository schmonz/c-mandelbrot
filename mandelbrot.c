#include <gd.h>

#include <stdbool.h>
#include <stdio.h>

#define WIDTH 100
#define HEIGHT 100

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

static void
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

int
main(void)
{
    draw_something();
}
