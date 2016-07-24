APPROVAL_TESTS	=  approve_mandelbrot
THE_TESTS	=  check_mandelbrot
THE_LIBRARY	=  mandelbrot.a
THE_PROGRAM	=  main

CFLAGS		+= -g -O0 -Wall -Werror -Wextra -std=c99
LIBS		+= -lm
CAIRO_CFLAGS	:= $(shell pkg-config --cflags cairo 2>/dev/null)
CAIRO_LIBS	:= $(shell pkg-config --libs cairo 2>/dev/null)
CHECK_CFLAGS	:= $(shell pkg-config --cflags check 2>/dev/null)
CHECK_LIBS	:= $(shell pkg-config --libs check 2>/dev/null)
GD_CFLAGS	:= $(shell pkg-config --cflags gdlib 2>/dev/null)
GD_LIBS		:= $(shell pkg-config --libs gdlib 2>/dev/null)
IMAGEMAGICK	:= $(shell compare 2>/dev/null | grep ImageMagick)
IMLIB2_CFLAGS	:= $(shell pkg-config --cflags imlib2 2>/dev/null)
IMLIB2_LIBS	:= $(shell pkg-config --libs imlib2 2>/dev/null)

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
ifeq (, ${IMAGEMAGICK})
	${SILENT}echo "Please install ImageMagick (http://www.imagemagick.org)." && false
endif

is-imlib2-installed:
ifeq (, ${IMLIB2_LIBS})
	${SILENT}echo "Please install Imlib 2 (http://docs.enlightenment.org/api/imlib2/html/)." && false
endif

is-mpc-installed:
ifeq (yes, ${BUILD_WITH_MPC}) # XXX a little too phony
MPC_CFLAGS	=  -DUSE_MPC ${GD_CFLAGS}
MPC_LIBS	=  -lmpc -lmpfr -lgmp
endif

.PHONY: all check approval valgrind clean is-cairo-installed is-check-installed is-gd-installed is-imagemagick-installed is-imlib2-installed is-mpc-installed

${THE_TESTS}: is-check-installed ${THE_LIBRARY} check_mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${CHECK_CFLAGS} -o ${THE_TESTS} check_mandelbrot.c ${THE_LIBRARY} ${LIBS} ${CAIRO_LIBS} ${GD_LIBS} ${IMLIB2_LIBS} ${MPC_LIBS} ${CHECK_LIBS}

${THE_LIBRARY}: is-cairo-installed is-gd-installed is-imlib2-installed is-mpc-installed graph.h graph.c mandelbrot.h mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${CAIRO_CFLAGS} ${GD_CFLAGS} ${IMLIB2_CFLAGS} -c graph.c
	${SILENT}${CC} ${CFLAGS} ${MPC_CFLAGS} -c mandelbrot.c
	${SILENT}ar rc ${THE_LIBRARY} graph.o mandelbrot.o
	${SILENT}ranlib ${THE_LIBRARY}

${THE_PROGRAM}: ${THE_LIBRARY} mandelbrot.h main.c
	${SILENT}${CC} ${CFLAGS} -o ${THE_PROGRAM} main.c ${THE_LIBRARY} ${LIBS} ${CAIRO_LIBS} ${GD_LIBS} ${IMLIB2_LIBS} ${MPC_LIBS}
