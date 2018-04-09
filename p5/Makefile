all: encode decode

encode: encode.o bits.o codes.o
	gcc encode.o bits.o codes.o -o encode

encode.o: encode.c bits.h codes.h
	gcc -Wall -std=c99 -g -c encode.c

bits.o: bits.c bits.h
	gcc -Wall -std=c99 -g -c bits.c
	
codes.o: codes.c codes.h
	gcc -Wall -std=c99 -g -c codes.c
	
decode: decode.o bits.o codes.o
	gcc decode.o bits.o codes.o -o decode
	
decode.o: decode.c bits.h codes.h
	gcc -Wall -std=c99 -g -c decode.c

clean:
	rm -f encode.o codes.o bits.o
	rm -f encode
	rm -f decode.o codes.o bits.o
	rm -f decode
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt
