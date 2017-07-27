#include <check.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "graph.h"
#include "mandelbrot.h"

START_TEST(test_some_known_points)
{
    size_t iterations = 2;
    complex double is_in_set = 0.0 + I * 0.0;
    complex double is_not_in_set = 1.5 + I * 1.5;

    ck_assert_int_eq(0, choose_escape_color(is_in_set, iterations));
    ck_assert_int_ne(0, choose_escape_color(is_not_in_set, iterations));
}
END_TEST

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

    graph_t graph = graph_create("graph_gd.so", width, height, 0 + I * 0, 4.0);

    assert_complex_points_equal(-2 - I * 2, graph.extremes.lower_left);
    assert_complex_points_equal( 2 + I * 2, graph.extremes.upper_right);

    assert_complex_points_equal(    0 + I * 0,
            graph_get_coordinates(graph, width / 2, height / 2));
    assert_complex_points_equal(0.665 + I * 0.855,
            graph_get_coordinates(graph, 2 * width / 3, 5 * height / 7));

    graph_destroy(graph);
}
END_TEST

START_TEST(test_aspect_ratio_in_portrait)
{
    const int width = 500, height = 800;

    graph_t graph = graph_create("graph_gd.so", width, height, 0.25 + I * -0.3, 2.0);

    assert_complex_points_equal(-0.375 - I * 1.3, graph.extremes.lower_left);
    assert_complex_points_equal( 0.875 + I * 0.7, graph.extremes.upper_right);

    graph_destroy(graph);
}
END_TEST

START_TEST(test_aspect_ratio_in_landscape)
{
    const int width = 600, height = 450;

    graph_t graph = graph_create("graph_gd.so", width, height, -0.25 + I * 0.3, 4.0);

    assert_complex_points_equal(-2.25 - I * 1.2, graph.extremes.lower_left);
    assert_complex_points_equal( 1.75 + I * 1.8, graph.extremes.upper_right);

    graph_destroy(graph);
}
END_TEST

TCase
*mandelbrot_algorithm(void)
{
    TCase *tc = tcase_create("Mandelbrot Algorithm");

    tcase_add_test(tc, test_some_known_points);

    return tc;
}

TCase
*complex_plane(void)
{
    TCase *tc = tcase_create("Complex Plane");

    tcase_add_test(tc, test_complex_plane_coordinates);
    tcase_add_test(tc, test_aspect_ratio_in_portrait);
    tcase_add_test(tc, test_aspect_ratio_in_landscape);

    return tc;
}

Suite
*mandelbrot_suite(void)
{
    Suite *s = suite_create("Mandelbrot Suite");

    suite_add_tcase(s, mandelbrot_algorithm());
    suite_add_tcase(s, complex_plane());

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
