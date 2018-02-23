CC = gcc

CFLAGS = -Wall -std=c99 -g

hangman: hangman.o wordlist.o display.o

hangman.o: wordlist.h display.h

wordlist.o: wordlist.h

display.o: display.h

clean:
	rm -f hangman.o wordlist.o display.o
	rm -f hangman
	rm -f output-*.txt