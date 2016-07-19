#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "mandelbrot.h"

static int usage(char *progname) {
    printf("usage: %s <width-pixels> <height-pixels>\n", basename(progname));
    return 77;
}

int main(int argc, char *argv[]) {
    if (argc == 4) {
        draw_something(atoi(argv[1]), atoi(argv[2]), atof(argv[3]));
    } else {
        return usage(argv[0]);
    }
}
