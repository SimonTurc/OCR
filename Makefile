#MAKEFILE
CC =gcc -fsanitize=address
CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -Werror -std=c99 -O3 -export-dynamic
LDFLAGS = -pthread -lpthread
LDLIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl` -lSDL_image -lSDL_gfx

SRC = Image/basics/pixel_operations.c  Image/basics/sdl_basics.c Image/filter.c Image/segmentation.c Image/extraction.c Image/matrix_resize.c Image/skew.c Image/prediction.c text.c

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ} -lm

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program
	${RM} main.d     # remove main program

-include ${DEP}
