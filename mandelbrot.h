#include <sys/types.h>

#define WIDTH 100
#define HEIGHT 100

struct ComplexPoint {
    double x;
    double y;
};

void draw_something(void);
struct ComplexPoint coords_for_pixel(size_t, size_t);
