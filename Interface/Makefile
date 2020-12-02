# Makefile

CC = gcc -fsanitize=address
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3 -export-dynamic
LDLIBS = `pkg-config --libs gtk+-3.0`

all: interface

interface: interface.c 

.PHONY: clean

clean:
	${RM} $(EXE)

# END
