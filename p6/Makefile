regular: regular.o pattern.o parse.o
	gcc regular.o pattern.o parse.o -o regular

regular.o: regular.c pattern.h parse.h
	gcc -Wall -std=c99 -g -c regular.c

pattern.o: pattern.c pattern.h
	gcc -Wall -std=c99 -g -c pattern.c

parse.o: parse.c parse.h
	gcc -Wall -std=c99 -g -c parse.c

clean:
	rm -f regular.o pattern.o parse.o
	rm -f regular
	rm -f output.txt
