all: part4 part3 part2 part1

part4: part4.o string_parser.o
	gcc -g -Wall -o part4 part4.o string_parser.o 

part3: part3.o string_parser.o
	gcc -g -Wall -o part3 part3.o string_parser.o 

part2: part2.o string_parser.o
	gcc -g -Wall -o part2 part2.o string_parser.o 

part1: part1.o string_parser.o
	gcc -g -Wall -o part1 part1.o string_parser.o 


string_parser.o: string_parser.c string_parser.h
	gcc -g -c string_parser.c

part4.o: part4.c
	gcc -g -c part4.c

part3.o: part3.c
	gcc -g -c part3.c
	
part2.o: part2.c
	gcc -g -c part2.c

part1.o: part1.c
	gcc -g -c part1.c

clean:
	rm -f core *.o part4 part3 part2 part1