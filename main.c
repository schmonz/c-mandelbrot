#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "mandelbrot.h"

static int usage(char *progname) {
    printf("usage: %s <width> <height> <a> <b> <range>\n", basename(progname));
    return 77;
}

int main(int argc, char *argv[]) {
    if (argc == 6) {
        draw_something_gd(
                atoi(argv[1]),
                atoi(argv[2]),
                atof(argv[3]) + I * atof(argv[4]),
                atof(argv[5])
        );
    } else {
        return usage(argv[0]);
    }
}
