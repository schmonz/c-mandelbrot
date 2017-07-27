SILENT		?= @
BACKEND		?= gd
OUTPUTFILE	?= pngelbrot.png

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
MPC		:= $(shell cpp -dM ${GD_CFLAGS} -include mpc.h </dev/null 2>/dev/null | grep MPC_VERSION_STRING)
MPC_CFLAGS	= ${GD_CFLAGS}
MPC_LIBS	=  -lmpc -lmpfr -lgmp

all: check approval

check: ${THE_TESTS}
	${SILENT}./${THE_TESTS}

approval: .has_imagemagick ${THE_PROGRAM}
	${SILENT}./${THE_PROGRAM} ${BACKEND} ${OUTPUTFILE} 800 500 100 0.0 0.0 4.0
	${SILENT}./${APPROVAL_TESTS} ${OUTPUTFILE}

valgrind: ${THE_TESTS}
	${SILENT}valgrind --leak-check=full --show-leak-kinds=all ./${THE_TESTS}

clean:
	${SILENT}rm -f .has_* *.o ${THE_TESTS} ${THE_LIBRARY} ${THE_PROGRAM}
	${SILENT}rm -rf *.dSYM

.PHONY: all check approval valgrind clean

.has_cairo:
ifeq (, ${CAIRO_LIBS})
	${SILENT}echo "Please install Cairo (https://www.cairographics.org)." && false
else
	${SILENT}touch .has_cairo
endif

.has_check:
ifeq (, ${CHECK_LIBS})
	${SILENT}echo "Please install Check (https://libcheck.github.io/check/)." && false
else
	${SILENT}touch .has_check
endif

.has_gd:
ifeq (, ${GD_LIBS})
	${SILENT}echo "Please install GD (http://libgd.github.io)." && false
else
	${SILENT}touch .has_gd
endif

.has_imagemagick:
ifeq (, ${IMAGEMAGICK})
	${SILENT}echo "Please install ImageMagick (http://www.imagemagick.org)." && false
else
	${SILENT}touch .has_imagemagick
endif

.has_imlib2:
ifeq (, ${IMLIB2_LIBS})
	${SILENT}echo "Please install Imlib 2 (http://docs.enlightenment.org/api/imlib2/html/)." && false
else
	${SILENT}touch .has_imlib2
endif

.has_mpc:
ifeq (, ${MPC})
	${SILENT}echo "Please install GNU MPC (http://www.multiprecision.org)." && false
else
	${SILENT}touch .has_mpc
endif

check_mandelbrot.o: .has_check mandelbrot.h mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${CHECK_CFLAGS} -c check_mandelbrot.c

graph.o: graph.h graph.c
	${SILENT}${CC} ${CFLAGS} -c graph.c

graph_cairo.o: .has_cairo graph.h graph_cairo.h graph_cairo.c
	${SILENT}${CC} ${CFLAGS} ${CAIRO_CFLAGS} -c graph_cairo.c

graph_gd.o: .has_gd graph.h graph_gd.h graph_gd.c
	${SILENT}${CC} ${CFLAGS} ${GD_CFLAGS} -c graph_gd.c

graph_imlib2.o: .has_imlib2 graph.h graph_imlib2.h graph_imlib2.c
	${SILENT}${CC} ${CFLAGS} ${IMLIB2_CFLAGS} -c graph_imlib2.c

main.o: mandelbrot.h main.c
	${SILENT}${CC} ${CFLAGS} -c main.c

mandelbrot.o: mandelbrot.h mandelbrot.c
	${SILENT}${CC} ${CFLAGS} -c mandelbrot.c

mandelbrot_mpc.o: .has_mpc mandelbrot.h mandelbrot_mpc.c
	${SILENT}${CC} ${CFLAGS} ${MPC_CFLAGS} -c mandelbrot_mpc.c

${THE_TESTS}: ${THE_LIBRARY} check_mandelbrot.o
	${SILENT}${CC} ${LDFLAGS} ${THE_LIBRARY} ${LIBS} ${CAIRO_LIBS} ${GD_LIBS} ${IMLIB2_LIBS} ${MPC_LIBS} ${CHECK_LIBS} check_mandelbrot.o -o ${THE_TESTS}

${THE_LIBRARY}: graph.o graph_cairo.o graph_gd.o graph_imlib2.o mandelbrot.o mandelbrot_mpc.o
	${SILENT}ar rc ${THE_LIBRARY} graph.o graph_cairo.o graph_gd.o graph_imlib2.o mandelbrot.o mandelbrot_mpc.o
	${SILENT}ranlib ${THE_LIBRARY}

${THE_PROGRAM}: ${THE_LIBRARY} main.o
	${SILENT}${CC} ${LDFLAGS} ${THE_LIBRARY} ${LIBS} ${CAIRO_LIBS} ${GD_LIBS} ${IMLIB2_LIBS} ${MPC_LIBS} main.o -o ${THE_PROGRAM}
