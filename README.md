# c-mandelbrot

![Build Status](https://github.com/schmonz/c-mandelbrot/actions/workflows/main.yml/badge.svg)

Lately I've been
[learning C](http://www.schmonz.com/2016/06/18/training-tdd-for-embedded-c/).
Recently I shared the story of
[how I got hooked on programming](http://www.schmonz.com/2007/06/29/when-programming-chose-me/)
on
[Developer on Fire 139](http://www.schmonz.com/talk/20160616-developer-on-fire/).
In so doing, I seem to have revived my own interest in the
[Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set).
Now I'm drawing it in C:

![Latest Image](https://github.com/schmonz/c-mandelbrot/raw/main/pngelbrot.png)

## TODO

7. Extract math backends: native C real math, native C complex math, GNU MPC.
7. Add performance tests for math and graphics.
7. Add math backend: [GNU Scientific Library](https://www.gnu.org/software/gsl/gsl.html).
7. Add a graphics backend that draws to a window on screen.
7. Profile: where are we spending our time?
7. Choose colormap size at runtime.
7. Compute escape times in parallel threads.
7. Improve safety and style of `graph_t.image` declaration.
7. Determine whether arbitrary-precision complex math can be made faster.
7. Determine when arbitrary-precision complex math is worth doing.
7. Use GNU Automake, Autoconf, Libtool.
7. If iterating over every pixel is slow, try:
    - parallelizing computation in multiple threads (with OpenMP or similar)
    - offloading to a GPU (with OpenCL or similar)
    - tracing the boundary
7. Auto-scale number of iterations with zoom level.
7. Fix Valgrind errors.
7. On OS X, try
   [Guard Malloc](https://developer.apple.com/library/mac/documentation/Performance/Conceptual/ManagingMemory/Articles/MallocDebug.html).
7. Try using
   [C++ ApprovalTests](https://github.com/approvals/ApprovalTests.cpp)
   to check the generated image.
7. Try `astyle`.
7. Try `cppcheck`.
7. In a Mac GUI, display the initial image (then maybe add features).
7. In an iPhone app, pinch to zoom (then maybe port to iPad).
7. Get ideas from [XaoS](http://matek.hu/xaos) or [Fractint](http://fractint.org).
7. Get optimization ideas from [mandelbrot-js](https://github.com/cslarsen/mandelbrot-js).
