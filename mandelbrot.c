#include <complex.h>
#include <stdlib.h>

#ifdef USE_MPC
#include <mpc.h>
#endif

#include "graph.h"
#include "mandelbrot.h"

size_t
choose_escape_color(const complex double c, const size_t maximum_iterations)
{
    size_t escape = 0;

#ifdef USE_MPC
    const mpfr_prec_t precision = 53;
    mpc_rnd_t rounding_mode = MPC_RNDNN;

    mpc_t my_z, my_temp, my_c;
    mpfr_t my_mpfr_absolute_value;
    mpc_init2(my_z, precision), mpc_init2(my_temp, precision),
        mpc_init2(my_c, precision);
    mpfr_init2(my_mpfr_absolute_value, precision);

    mpc_set_dc(my_c, c, rounding_mode);
    mpc_set_dc(my_z, 0.0 + I * 0.0, rounding_mode);

    for (; escape < maximum_iterations; escape++) {
        mpc_pow_d(my_temp, my_z, 2.0, rounding_mode);

        mpc_abs(my_mpfr_absolute_value, my_temp, rounding_mode);
        if (mpfr_get_d(my_mpfr_absolute_value, rounding_mode) > 2)
            break;

        mpc_add(my_z, my_temp, my_c, rounding_mode);
    }

    mpfr_clear(my_mpfr_absolute_value);
    mpc_clear(my_c);
    mpc_clear(my_temp);
    mpc_clear(my_z);
#else
    complex double z = 0.0 + I * 0.0;
    complex double temp;

    for (; escape < maximum_iterations; escape++) {
        temp = cpow(z, 2);

        if (cabs(temp) > 2)
            break;

        z = temp + c;
    }
#endif /* USE_MPC */

    if (escape == maximum_iterations)
        escape = 0;

    if (escape == 0)
        return 0;
    else if (escape <= (maximum_iterations / 7))
        return 1;
    else if (escape <= (maximum_iterations / 5))
        return 2;
    else
        return 3;

    return escape;
}

void mandelbrot(const char *backend, const char *outputfile,
        const size_t width, const size_t height, const size_t iterations,
        const complex double center, const double range)
{
    graph_t image = graph_create(backend, width, height, center, range);

    for (size_t i = 0; i < image.width; i++) {
        for (size_t j = 0; j < image.height; j++) {
            complex double c = graph_get_coordinates(image, i, j);
            size_t colormap_entry = choose_escape_color(c, iterations);
            graph_set_pixel(image, i, j, colormap_entry);
        }
    }

    graph_write(image, outputfile);

    graph_destroy(image);
}
