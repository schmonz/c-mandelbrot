CFLAGS		:= $(shell pkg-config --cflags gdlib)
LIBS		:= $(shell pkg-config --libs gdlib)

CFLAGS		+= -g -O0 -Wall -Werror -Wextra -std=c99

SILENT		= @

all: mandelbrot

mandelbrot: mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${LIBS} -o mandelbrot mandelbrot.c
	./mandelbrot
	open pngelbrot.png
