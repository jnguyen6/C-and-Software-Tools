CC = gcc

CFLAGS = -Wall -std=c99 -g -D_GNU_SOURCE

LDFLAGS = -lm

attractions: attractions.o pointlist.o point.o

attractions.o: pointlist.h point.h

pointlist.o: pointlist.h point.h

point.o: point.h

clean:
	rm -f attractions.o pointlist.o point.o
	rm -f attractions
	rm -f output.txt