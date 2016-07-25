#include <complex.h>
#include <stdlib.h>

#include <mpc.h>

size_t
choose_escape_color_mpc(const complex double c, const size_t maximum_iterations)
{
    size_t escape = 0;

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
