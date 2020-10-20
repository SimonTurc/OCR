# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main

main: pixel_operations.o  sdl_basics.o filter.o segmentation.o

filter.o: pixel_operations.h sdl_basics.h filter.h

segmentation.o: pixel_operations.h sdl_basics.h segmentation.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
