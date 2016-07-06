#include <check.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "mandelbrot.h"

static void
assert_doubles_equal(double a, double b)
{
    const double epsilon = 0.000000001;
    if (fabs(a - b) < epsilon) {
        ck_assert(true);
    }
    else if (a > b) {
        ck_assert_msg(false, "first was bigger");
    }
    else {
        ck_assert_msg(false, "second was bigger");
    }
}
    
START_TEST(test_mandelbrot_stuff)
{
    complex double point = coords_for_pixel(0, 0);
    ck_assert_int_eq(-2, creal(point));
    ck_assert_int_eq(-2, cimag(point));

    point = coords_for_pixel(WIDTH, HEIGHT);
    ck_assert_int_eq( 2, creal(point));
    ck_assert_int_eq( 2, cimag(point));

    point = coords_for_pixel(WIDTH / 2, HEIGHT / 2);
    ck_assert_int_eq( 0, creal(point));
    ck_assert_int_eq( 0, cimag(point));

    point = coords_for_pixel(2 * WIDTH / 3, 5 * HEIGHT / 7);
    assert_doubles_equal(0.665, creal(point));
    assert_doubles_equal(0.855, cimag(point));
}
END_TEST

TCase
*mandelbrot_testcase(void)
{
    TCase *tc = tcase_create("Mandelbrot Test Case");

    tcase_add_test(tc, test_mandelbrot_stuff);

    return tc;
}

Suite
*mandelbrot_suite(void)
{
    Suite *s = suite_create("Mandelbrot Suite");

    suite_add_tcase(s, mandelbrot_testcase());

    return s;
}

int
main(void)
{
    int number_failed;

    SRunner *sr = srunner_create(mandelbrot_suite());
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed;
}
