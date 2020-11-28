# Simple SDL mini Makefile

CC=gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main

main: basics/pixel_operations.o  basics/sdl_basics.o filter.o segmentation.o extraction.o


clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
