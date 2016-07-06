THE_PROGRAM	= mandelbrot

CFLAGS		:= $(shell pkg-config --cflags gdlib)
LIBS		:= $(shell pkg-config --libs gdlib)

CFLAGS		+= -g -O0 -Wall -Werror -Wextra -std=c99

SILENT		= @

all: check

check: ${THE_PROGRAM}
	${SILENT}./${THE_PROGRAM}
	${SILENT}open pngelbrot.png

valgrind: ${THE_PROGRAM}
	${SILENT}valgrind --leak-check=full --show-leak-kinds=all ./${THE_PROGRAM}

clean:
	${SILENT}rm -f *.png *.a *.o ${THE_PROGRAM}
	${SILENT}rm -rf *.dSYM

.PHONY: all check valgrind clean

${THE_PROGRAM}: mandelbrot.c
	${SILENT}${CC} ${CFLAGS} -o ${THE_PROGRAM} mandelbrot.c ${LIBS}
