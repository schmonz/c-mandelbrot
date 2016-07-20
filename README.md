# c-mandelbrot

Lately I've been [learning C](http://www.schmonz.com/2016/06/18/training-tdd-for-embedded-c/).
Recently I shared the story of
[how I got hooked on programming](http://www.schmonz.com/2007/06/29/when-programming-chose-me/)
on [Developer on Fire 139](http://www.schmonz.com/talk/20160616-developer-on-fire/).
In so doing, I seem to have revived my own interest in the
[Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set).
Now I'm drawing it in C:

![Latest Image](https://github.com/schmonz/c-mandelbrot/raw/master/pngelbrot.png)

## TODO

7. Fix Valgrind errors.
7. Try using
   [C++ ApprovalTests](https://github.com/approvals/ApprovalTests.cpp)
   to check the generated image.
7. Try another graphics library
   ([Cairo](https://www.cairographics.org/FAQ/#minimal_C_program)?
   [AA-lib](http://aa-project.sourceforge.net/aalib/)?).
7. Try `astyle`.
7. Try `cppcheck`.
7. Use higher-precision math (after zooming in far enough that it matters).
7. If iterating over every pixel is slow, try:
    - parallelizing computation in multiple threads (with OpenMP or similar)
    - offloading to a GPU (with OpenCL or similar)
    - tracing the boundary
7. Auto-scale number of iterations with zoom level.
7. In a Mac GUI, display the initial image (then maybe add features).
7. In an iPhone app, pinch to zoom (then maybe port to iPad).
