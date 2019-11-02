#
# Makefile for TINY
# Gnu C Version
# K. Louden 2/3/98
#

CC = gcc

CFLAGS =
 	
LEX = lex
YACC = yacc

OBJS = main.o util.o scan.o parse.o

tiny.out: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o tiny.out
	
main.o: main.c globals.h util.h scan.h
	$(CC) $(CFLAGS) -c main.c
	
util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: lex.yy.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c lex.yy.c -o scan.o

parse.o: y.tab.c parse.h scan.h globals.h util.h
	$(CC) $(CFLAGS) -c y.tab.c -o parse.o
	
clean:
	-rm tiny.out
	-rm $(OBJS) lex.yy.c y.tab.c y.tab.h

all: tiny.out

