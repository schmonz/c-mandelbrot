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
    struct ComplexPoint point = coords_for_pixel(0, 0);
    ck_assert_int_eq(-2, point.x);
    ck_assert_int_eq(-2, point.y);

    point = coords_for_pixel(WIDTH, HEIGHT);
    ck_assert_int_eq( 2, point.x);
    ck_assert_int_eq( 2, point.y);

    point = coords_for_pixel(WIDTH / 2, HEIGHT / 2);
    ck_assert_int_eq( 0, point.x);
    ck_assert_int_eq( 0, point.y);

    point = coords_for_pixel(2 * WIDTH / 3, 5 * HEIGHT / 7);
    assert_doubles_equal(0.664, point.x);
    assert_doubles_equal(0.856, point.y);
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
