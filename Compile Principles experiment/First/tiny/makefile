scanner.exe: main.o lex.yy.o util.o
	gcc main.o lex.yy.o util.o -o scanner -lfl
main.o: main.c globals.h util.h
	gcc main.c -c
util.o: util.c util.h globals.h
	gcc util.c -c
lex.yy.o: tiny.l
	flex tiny.l
	gcc lex.yy.c -c
