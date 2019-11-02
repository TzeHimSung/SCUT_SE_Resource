
#include "util.h"
#include <stdio.h>
#include "globals.h"
 
void printToken(TokenType token, char* tokenString)
{
	switch(token)
	{
		case WHILE:
		case DO:
		case TO:
		case DOWNTO:
		case FOR:
		case ENDDO:
		case ENDWHILE:
		case MOD:
		case IF:
		case THEN:
		case ELSE:
		case END:
		case REPEAT:
		case UNTIL:
		case READ:
		case WRITE:
			printf("\treversed word: %s\n", tokenString);
			break;
		case ID:
			printf("\tidentifier: %s\n", tokenString);
			break;
		case NUM:
			printf("\tnumber: %s\n", tokenString);
			break;
		case ASSIGN:
		case EQ:
		case LT:
		case PLUS:
		case MINUS:
		case TIMES:
		case OVER:
		case LPAREN:
		case RPAREN:
		case SEMI:
			printf("\toperator: %s\n", tokenString);
	}
}
