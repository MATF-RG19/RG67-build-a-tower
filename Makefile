PROGRAM = build_tower
CC      = gcc
CFLAGS  = -g -ansi -Wall -I/usr/X11R6/include -I/usr/pkg/include -std=c99
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o image.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(PROGRAM)  main.o image.o $(LDLIBS)

image.o: image.c image.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c image.c -o image.o

main.o: main.c image.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c main.c -o main.o

.PHONY: clean

clean:
	-rm *.o $(PROGRAM)

