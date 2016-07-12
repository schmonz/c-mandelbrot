APPROVAL_TESTS	=  approve_mandelbrot
THE_TESTS	=  check_mandelbrot
THE_LIBRARY	=  mandelbrot.a
THE_PROGRAM	=  main

CFLAGS		+= -g -O0 -Wall -Werror -Wextra -std=c99
GD_CFLAGS	:= $(shell pkg-config --cflags gdlib)
GD_LIBS		:= $(shell pkg-config --libs gdlib)
CHECK_CFLAGS	:= $(shell pkg-config --cflags check)
CHECK_LIBS	:= $(shell pkg-config --libs check)

SILENT		=  @

all: check approval

check: ${THE_TESTS}
	${SILENT}./${THE_TESTS}

approval: ${THE_PROGRAM}
	${SILENT}./${THE_PROGRAM}
	${SILENT}./${APPROVAL_TESTS} pngelbrot.png

valgrind: ${THE_TESTS}
	${SILENT}valgrind --leak-check=full --show-leak-kinds=all ./${THE_TESTS}

clean:
	${SILENT}rm -f *.o ${THE_TESTS} ${THE_LIBRARY} ${THE_PROGRAM}
	${SILENT}rm -rf *.dSYM

is-check-installed:
ifeq (, ${CHECK_LIBS})
	${SILENT}echo "Please install Check (https://libcheck.github.io/check/)." && false
endif

is-gd-installed:
ifeq (, ${GD_LIBS})
	${SILENT}echo "Please install GD (http://libgd.github.io)." && false
endif

.PHONY: all check approval valgrind clean is-check-installed is-gd-installed

${THE_TESTS}: is-check-installed ${THE_LIBRARY} check_mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${GD_CFLAGS} ${CHECK_CFLAGS} -o ${THE_TESTS} check_mandelbrot.c ${GD_LIBS} ${CHECK_LIBS} ${THE_LIBRARY}

${THE_LIBRARY}: is-gd-installed mandelbrot.h mandelbrot.c
	${SILENT}${CC} ${CFLAGS} ${GD_CFLAGS} -c mandelbrot.c
	${SILENT}ar rc ${THE_LIBRARY} mandelbrot.o
	${SILENT}ranlib ${THE_LIBRARY}

${THE_PROGRAM}: ${THE_LIBRARY} mandelbrot.h main.c
	${SILENT}${CC} ${CFLAGS} -o ${THE_PROGRAM} main.c ${GD_LIBS} ${THE_LIBRARY}
