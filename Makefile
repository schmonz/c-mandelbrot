APPROVAL_TESTS	=  approve_mandelbrot
THE_TESTS	=  check_mandelbrot
THE_PROGRAM	=  mandelbrot

CFLAGS		+= -g -O0 -Wall -Werror -Wextra -std=c99
GD_CFLAGS	:= $(shell pkg-config --cflags gdlib)
GD_LIBS		:= $(shell pkg-config --libs gdlib)

SILENT		=  @

all: check

check: ${THE_PROGRAM}
	${SILENT}./${THE_PROGRAM}
	${SILENT}./${APPROVAL_TESTS} pngelbrot.png

valgrind: ${THE_PROGRAM}
	${SILENT}valgrind --leak-check=full --show-leak-kinds=all ./${THE_PROGRAM}

clean:
	${SILENT}rm -f *.png *.o ${THE_PROGRAM}
	${SILENT}rm -rf *.dSYM

.PHONY: all check valgrind clean

${THE_PROGRAM}: mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${GD_CFLAGS} -o ${THE_PROGRAM} mandelbrot.c ${GD_LIBS}
