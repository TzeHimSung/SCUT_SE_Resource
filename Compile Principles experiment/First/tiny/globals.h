#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
typedef enum
{
    ENDFILE, ERROR,
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    ID, NUM,
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI,
    WHILE, DO, TO, DOWNTO, FOR, ENDDO, ENDWHILE, MOD
} TokenType;
extern lineno;

/* The max size of identifier of reserved word */
#define MAXTOKENLEN 50

#endif