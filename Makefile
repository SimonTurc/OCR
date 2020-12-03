# Simple SDL mini Makefile

CC=gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS= -pthread -lpthread
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lSDL_gfx

all: main

main: Image/basics/pixel_operations.o  Image/basics/sdl_basics.o Image/filter.o Image/segmentation.o Image/extraction.o Image/matrix_resize.o Image/skew.o Image/prediction.o -lm

clean:
	${RM} *.o
	${RM} Image/*.o
	${RM} Image/*.d
	${RM} Neural_network/*.o
	${RM} Neural_network/*.d
	${RM} *.d
	${RM} main

# END
