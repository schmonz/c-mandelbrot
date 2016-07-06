#include <check.h>

#include "mandelbrot.h"

START_TEST(test_mandelbrot_stuff)
{
    ck_assert_int_eq(1, 2 - 1);
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
    
    /* XXX
     * get a prosthetic red test going
     * given a pixel between horizontal-0 and max, get X coordinate
     * same for Y
     * now for both together
     * now change the code to draw a circle
     */
}
