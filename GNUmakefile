#SILENT		?= @
#LIBTOOL_SILENT	?= >/dev/null 2>&1
BACKEND		?= gd
OUTPUTFILE	?= pngelbrot.png

APPROVAL_TESTS	=  approve_mandelbrot
THE_TESTS	=  check_mandelbrot
THE_LIBRARY	=  libmandelbrot.${DLEXT}
THE_PROGRAM	=  main
DESTDIR		?= ${CURDIR}
RPATH		?= ${DESTDIR}

LIBTOOL		?= libtool ${LIBTOOL_SILENT}
CFLAGS		+= -g -O0 -Wall -Werror -Wextra -std=c99 ${OSX_CFLAGS}
LIBS		+= -ldl -lm
ifeq ($(shell uname), Darwin)
DLEXT		= dylib
else
DLEXT		= so
endif
OSX_INCLUDES	= /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include
OSX_CFLAGS	:= $(shell test -d ${OSX_INCLUDES} && echo -I${OSX_INCLUDES})
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

approval: .has_imagemagick ${THE_PROGRAM} graph_${BACKEND}.so
	${SILENT}./${THE_PROGRAM} graph_${BACKEND}.so ${OUTPUTFILE} 800 500 100 0.0 0.0 4.0
	${SILENT}./${APPROVAL_TESTS} ${OUTPUTFILE}

valgrind: ${THE_TESTS}
	${SILENT}valgrind --leak-check=full --show-leak-kinds=all ./${THE_TESTS}

clean:
	${SILENT}rm -f .has_* *.o *.a *.lo *.la *.so *.dylib ${THE_TESTS} ${THE_LIBRARY} ${THE_PROGRAM}
	${SILENT}rm -rf *.dSYM .libs

modules: graph_cairo.so graph_gd.so graph_imlib2.so

.PHONY: all check approval valgrind clean modules

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

check_mandelbrot.o: .has_check mandelbrot.h mandelbrot.c check_mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${CHECK_CFLAGS} -c check_mandelbrot.c

graph.lo: graph.h graph.c
	${SILENT}${LIBTOOL} --mode=compile --tag=CC ${CC} ${CFLAGS} -c graph.c

graph_cairo.lo: .has_cairo graph.h graph_cairo.c
	${SILENT}${LIBTOOL} --mode=compile --tag=CC ${CC} ${CFLAGS} ${CAIRO_CFLAGS} -c graph_cairo.c

graph_cairo.la: graph_cairo.lo
	${SILENT}${LIBTOOL} --mode=link --tag=CC ${CC} -module ${LDFLAGS} ${CAIRO_LIBS} -o graph_cairo.la graph_cairo.lo -version-info 0:0:0 -rpath ${RPATH}

graph_cairo.so: graph_cairo.la
	${SILENT}${LIBTOOL} --mode=install install -c .libs/graph_cairo.so ${DESTDIR}/graph_cairo.so

graph_gd.lo: .has_gd graph.h graph_gd.c
	${SILENT}${LIBTOOL} --mode=compile --tag=CC ${CC} ${CFLAGS} ${GD_CFLAGS} -c graph_gd.c

graph_gd.la: graph_gd.lo
	${SILENT}${LIBTOOL} --mode=link --tag=CC ${CC} -module ${LDFLAGS} ${GD_LIBS} -o graph_gd.la graph_gd.lo -version-info 0:0:0 -rpath ${RPATH}

graph_gd.so: graph_gd.la
	${SILENT}${LIBTOOL} --mode=install install -c .libs/graph_gd.so ${DESTDIR}/graph_gd.so

graph_imlib2.lo: .has_imlib2 graph.h graph_imlib2.c
	${SILENT}${LIBTOOL} --mode=compile --tag=CC ${CC} ${CFLAGS} ${IMLIB2_CFLAGS} -c graph_imlib2.c

graph_imlib2.la: graph_imlib2.lo
	${SILENT}${LIBTOOL} --mode=link --tag=CC ${CC} -module ${LDFLAGS} ${IMLIB2_LIBS} -o graph_imlib2.la graph_imlib2.lo -version-info 0:0:0 -rpath ${RPATH}

graph_imlib2.so: graph_imlib2.la
	${SILENT}${LIBTOOL} --mode=install install -c .libs/graph_imlib2.so ${DESTDIR}/graph_imlib2.so

main.o: mandelbrot.h main.c
	${SILENT}${CC} ${CFLAGS} -c main.c

mandelbrot.lo: graph.h mandelbrot.h mandelbrot.c
	${SILENT}${LIBTOOL} --mode=compile --tag=CC ${CC} ${CFLAGS} -c mandelbrot.c

mandelbrot_mpc.lo: .has_mpc mandelbrot.h mandelbrot_mpc.c
	${SILENT}${LIBTOOL} --mode=compile --tag=CC ${CC} ${CFLAGS} ${MPC_CFLAGS} -c mandelbrot_mpc.c

${THE_TESTS}: ${THE_LIBRARY} check_mandelbrot.o
	${SILENT}${CC} ${LDFLAGS} -L. -lmandelbrot ${LIBS} ${MPC_LIBS} ${CHECK_LIBS} check_mandelbrot.o -o ${THE_TESTS}

${THE_LIBRARY}: libmandelbrot.la
	${SILENT}${LIBTOOL} --mode=install install -c libmandelbrot.la ${DESTDIR}/libmandelbrot.la

libmandelbrot.la: modules graph.lo mandelbrot.lo mandelbrot_mpc.lo
	${SILENT}${LIBTOOL} --mode=link --tag=CC ${CC} ${LDFLAGS} -o libmandelbrot.la mandelbrot.lo graph.lo -version-info 0:0:0 -rpath ${RPATH}

${THE_PROGRAM}: ${THE_LIBRARY} main.o
	${SILENT}${CC} ${LDFLAGS} -L. -lmandelbrot ${LIBS} main.o -o ${THE_PROGRAM}
