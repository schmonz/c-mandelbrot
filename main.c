#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "mandelbrot.h"

static int usage(char *progname) {
    printf("usage: %s <library> <width> <height> <iterations> <a> <b> <range>\n",
            basename(progname));
    return 77;
}

int main(int argc, char *argv[]) {
    if (argc == 8) {
        mandelbrot(
                argv[1],
                atoi(argv[2]),
                atoi(argv[3]),
                atoi(argv[4]),
                atof(argv[5]) + I * atof(argv[6]),
                atof(argv[7])
        );
    } else {
        return usage(argv[0]);
    }
}
