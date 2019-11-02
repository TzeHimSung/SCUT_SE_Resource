%{
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "util.h"

char tokenString[MAXTOKENLEN + 1];
%}

digit		[0-9]
number		{digit}+
letter		[a-zA-Z]
identifier	{letter}[a-zA-Z0-9]*
newline		\n
whitespace	[ \t]

%%

"if"			{return IF;}
"then"			{return THEN;}
"else"			{return ELSE;}
"end"			{return END;}
"repeat"		{return REPEAT;}
"until"			{return UNTIL;}
"read"			{return READ;}
"write"			{return WRITE;}
":="			{return ASSIGN;}
"="			{return EQ;}
"<"			{return LT;}
"+"			{return PLUS;}
"-"			{return MINUS;}
"*"			{return TIMES;}
"/"			{return OVER;}
"("			{return LPAREN;}
")"			{return RPAREN;}
";"			{return SEMI;}
"while"			{return WHILE;}
"endwhile"			{return ENDWHILE;}
"do"			{return DO;}
"to"			{return TO;}
"downto"			{return DOWNTO;}
"for"			{return FOR;}
"enddo"			{return ENDDO;}
{number}		{return NUM;}
{identifier}		{return ID;}
{newline}		{lineno++;}
{whitespace}	{ /* Do nothing */ }
"{"			{ char c;
			  int count = 1;
			  do
			  {
				  c = input();
				  if (c == EOF) break;
				  else if (c == '\n') lineno++;
				  else if (c == '{') count++;
				  else if (c == '}') count--;
			  } while (count != 0);
			}
.			{return ERROR;}

%%

TokenType getToken(void)
{
	TokenType currentToken;
	currentToken = yylex();
	strncpy(tokenString, yytext, MAXTOKENLEN);
	printf("%d: ", lineno);
	printToken(currentToken, tokenString);

	return currentToken;
}
