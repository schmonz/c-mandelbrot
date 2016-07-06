CFLAGS		:= $(shell pkg-config --cflags gdlib)
LIBS		:= $(shell pkg-config --libs gdlib)

CFLAGS		+= -g -O0 -Wall -Werror -Wextra -std=c99

SILENT		= @

all: check

check: mandelbrot
	${SILENT}./mandelbrot
	${SILENT}open pngelbrot.png

valgrind: mandelbrot
	${SILENT}valgrind --leak-check=full --show-leak-kinds=all ./mandelbrot

clean:
	${SILENT}rm -f *.png *.a *.o mandelbrot
	${SILENT}rm -rf *.dSYM

.PHONY: all check valgrind clean

mandelbrot: mandelbrot.c
	${SILENT}${CC} ${CFLAGS} -o mandelbrot mandelbrot.c ${LIBS}
