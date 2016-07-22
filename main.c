#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "mandelbrot.h"

static int usage(char *progname) {
    printf("usage: %s <library> <filename> <width> <height> <iterations> <a> <b> <range>\n",
            basename(progname));
    return 77;
}

int main(int argc, char *argv[]) {
    if (argc == 9) {
        mandelbrot(
                argv[1],
                argv[2],
                atoi(argv[3]),
                atoi(argv[4]),
                atoi(argv[5]),
                atof(argv[6]) + I * atof(argv[7]),
                atof(argv[8])
        );
    } else {
        return usage(argv[0]);
    }
}
