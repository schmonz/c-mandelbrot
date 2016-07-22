#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "mandelbrot.h"

static int usage(char *progname) {
    printf("usage: %s <backend> <width> <height> <a> <b> <range>\n", basename(progname));
    return 77;
}

int main(int argc, char *argv[]) {
    if (argc == 7) {
        mandelbrot(
                argv[1],
                atoi(argv[2]),
                atoi(argv[3]),
                atof(argv[4]) + I * atof(argv[5]),
                atof(argv[6])
        );
    } else {
        return usage(argv[0]);
    }
}
