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

static void
assert_complex_points_equal(complex double expected, complex double actual)
{
    assert_doubles_equal(creal(expected), creal(actual));
    assert_doubles_equal(cimag(expected), cimag(actual));
}
    
START_TEST(test_complex_plane_coordinates)
{
    const int width = 800, height = 800;
    assert_complex_points_equal(   -2 - I * 2,
            coords_for_pixel(width, height,            0,              0));
    assert_complex_points_equal(    2 + I * 2,
            coords_for_pixel(width, height,        width,         height));
    assert_complex_points_equal(    0 + I * 0,
            coords_for_pixel(width, height,    width / 2,     height / 2));
    assert_complex_points_equal(0.665 + I * 0.855,
            coords_for_pixel(width, height,2 * width / 3, 5 * height / 7));
}
END_TEST

TCase
*mandelbrot_testcase(void)
{
    TCase *tc = tcase_create("Mandelbrot Test Case");

    tcase_add_test(tc, test_complex_plane_coordinates);

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
