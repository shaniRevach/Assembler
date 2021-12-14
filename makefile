assembler: main.o assembler.o first_parsing.o second_parsing.o writing.o help_functions.o
	gcc -g -Wall -ansi -pedantic main.o assembler.o first_parsing.o second_parsing.o writing.o help_functions.o -o assembler

main.o: main.c globals.h definitions.h
	gcc -c -Wall -ansi -pedantic main.c -o main.o

assembler.o: assembler.c definitions.h declarations.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

first_parsing.o: first_parsing.c definitions.h declarations.h
	gcc -c -Wall -ansi -pedantic first_parsing.c -o first_parsing.o

second_parsing.o: second_parsing.c definitions.h declarations.h
	gcc -c -Wall -ansi -pedantic second_parsing.c -o second_parsing.o

writing.o: writing.c definitions.h declarations.h
	gcc -c -Wall -ansi -pedantic writing.c -o writing.o

helpfunc.o: helpfunc.c definitions.h declarations.h
	gcc -c -Wall -ansi -pedantic helpfunc.c -o helpfunc.o
