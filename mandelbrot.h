#include <sys/types.h>

#define WIDTH 100
#define HEIGHT 100

struct RealPoint {
    double x;
    double y;
};

void draw_something(void);
struct RealPoint coords_for_pixel(size_t, size_t);
