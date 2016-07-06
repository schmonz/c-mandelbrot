#include <check.h>

#include "mandelbrot.h"
    
START_TEST(test_mandelbrot_stuff)
{
    ck_assert_int_eq(-2, horizontal_pixel_to_x_value(0));
    ck_assert_int_eq( 2, horizontal_pixel_to_x_value(WIDTH));
    ck_assert_int_eq( 0, horizontal_pixel_to_x_value(WIDTH / 2));

    ck_assert_int_eq(-2, vertical_pixel_to_y_value(0));
    ck_assert_int_eq( 2, vertical_pixel_to_y_value(HEIGHT));
    ck_assert_int_eq( 0, vertical_pixel_to_y_value(HEIGHT / 2));

    // given pixel (50, 50), get value (0, 0)
    // XXX now draw a circle
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
