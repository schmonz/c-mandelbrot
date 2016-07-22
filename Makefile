APPROVAL_TESTS	=  approve_mandelbrot
THE_TESTS	=  check_mandelbrot
THE_LIBRARY	=  mandelbrot.a
THE_PROGRAM	=  main

CFLAGS		+= -g -O0 -Wall -Werror -Wextra -std=c99
CAIRO_CFLAGS	:= $(shell pkg-config --cflags cairo)
CAIRO_LIBS	:= $(shell pkg-config --libs cairo)
CHECK_CFLAGS	:= $(shell pkg-config --cflags check)
CHECK_LIBS	:= $(shell pkg-config --libs check)
GD_CFLAGS	:= $(shell pkg-config --cflags gdlib)
GD_LIBS		:= $(shell pkg-config --libs gdlib)
IMAGEMAGICK_PATH:= $(shell pkg-config --variable=exec_prefix ImageMagick)

BUILD_WITH_MPC	?= no

SILENT		=  @

all: check approval

check: ${THE_TESTS}
	${SILENT}./${THE_TESTS}

approval: is-imagemagick-installed ${THE_PROGRAM}
	${SILENT}./${THE_PROGRAM} gd pngelbrot.png 800 500 100 0.0 0.0 4.0
	${SILENT}./${APPROVAL_TESTS} pngelbrot.png

valgrind: ${THE_TESTS}
	${SILENT}valgrind --leak-check=full --show-leak-kinds=all ./${THE_TESTS}

clean:
	${SILENT}rm -f *.o ${THE_TESTS} ${THE_LIBRARY} ${THE_PROGRAM}
	${SILENT}rm -rf *.dSYM

is-cairo-installed:
ifeq (, ${CAIRO_LIBS})
	${SILENT}echo "Please install Cairo (https://www.cairographics.org)." && false
endif

is-check-installed:
ifeq (, ${CHECK_LIBS})
	${SILENT}echo "Please install Check (https://libcheck.github.io/check/)." && false
endif

is-gd-installed:
ifeq (, ${GD_LIBS})
	${SILENT}echo "Please install GD (http://libgd.github.io)." && false
endif

is-imagemagick-installed:
ifeq (, ${IMAGEMAGICK_PATH})
	${SILENT}echo "Please install ImageMagick (http://www.imagemagick.org)." && false
endif

is-mpc-installed:
ifeq (yes, ${BUILD_WITH_MPC}) # XXX a little too phony
MPC_CFLAGS	=  -DUSE_MPC ${GD_CFLAGS}
MPC_LIBS	=  -lmpc -lmpfr -lgmp
endif

.PHONY: all check approval valgrind clean is-cairo-installed is-check-installed is-gd-installed is-mpc-installed

${THE_TESTS}: is-check-installed ${THE_LIBRARY} check_mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${CAIRO_CFLAGS} ${GD_CFLAGS} ${CHECK_CFLAGS} -o ${THE_TESTS} check_mandelbrot.c ${CAIRO_LIBS} ${GD_LIBS} ${MPC_LIBS} ${CHECK_LIBS} ${THE_LIBRARY}

${THE_LIBRARY}: is-cairo-installed is-gd-installed is-mpc-installed mandelbrot.h mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${CAIRO_CFLAGS} ${GD_CFLAGS} ${MPC_CFLAGS} -c mandelbrot.c
	${SILENT}ar rc ${THE_LIBRARY} mandelbrot.o
	${SILENT}ranlib ${THE_LIBRARY}

${THE_PROGRAM}: ${THE_LIBRARY} mandelbrot.h main.c
	${SILENT}${CC} ${CFLAGS} -o ${THE_PROGRAM} main.c ${CAIRO_LIBS} ${GD_LIBS} ${MPC_LIBS} ${THE_LIBRARY}
