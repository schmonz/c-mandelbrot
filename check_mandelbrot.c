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
    const int width = 800, height = width;
    const double range = 4.0;
    complex double center = 0 + I * 0;

    extremes_t foo = get_extreme_coordinates(width, height, center, range);
    assert_complex_points_equal(-2 - I * 2, foo.lower_left);
    assert_complex_points_equal( 2 + I * 2, foo.upper_right);

    assert_complex_points_equal(    0 + I * 0,
            coords_for_pixel(width, height, center, range,     width / 2,     height / 2));
    assert_complex_points_equal(0.665 + I * 0.855,
            coords_for_pixel(width, height, center, range, 2 * width / 3, 5 * height / 7));
}
END_TEST

START_TEST(test_aspect_ratio_in_portrait)
{
    const int width = 500, height = 800;
    const double range = 2.0;
    complex double center = 0.25 + I * -0.3;

    extremes_t foo = get_extreme_coordinates(width, height, center, range);
    assert_complex_points_equal(-0.375 - I * 1.3, foo.lower_left);
    assert_complex_points_equal( 0.875 + I * 0.7, foo.upper_right);
}
END_TEST

START_TEST(test_aspect_ratio_in_landscape)
{
    const int width = 600, height = 450;
    const double range = 4.0;
    complex double center = -0.25 + I * 0.3;

    extremes_t foo = get_extreme_coordinates(width, height, center, range);
    assert_complex_points_equal(-2.25 - I * 1.2, foo.lower_left);
    assert_complex_points_equal( 1.75 + I * 1.8, foo.upper_right);
}
END_TEST

START_TEST(test_some_known_points)
{
    size_t iterations = 100;
    complex double is_in_set = 0.0 + I * 0.0;
    complex double is_not_in_set = 1.5 + I * 1.5;
    ck_assert_int_eq(1, choose_escape_color(is_in_set, iterations));
    ck_assert_int_ne(1, choose_escape_color(is_not_in_set, iterations));
}
END_TEST

TCase
*mandelbrot_testcase(void)
{
    TCase *tc = tcase_create("Mandelbrot Test Case");

    tcase_add_test(tc, test_complex_plane_coordinates);
    tcase_add_test(tc, test_aspect_ratio_in_portrait);
    tcase_add_test(tc, test_aspect_ratio_in_landscape);
    tcase_add_test(tc, test_some_known_points);

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
