CC = gcc

CFLAGS = -Wall -std=c99 -g -D_GNU_SOURCE -lm

attractions: attractions.o pointlist.o point.o

attractions.o: attractions.c pointlist.c pointlist.h point.c point.h

pointlist.o: pointlist.c pointlist.h point.c point.h

point.o: point.c point.h

clean
	rm -f attractions.o pointlist.o point.o
	rm -f attractions
	rm -f output-*.txt