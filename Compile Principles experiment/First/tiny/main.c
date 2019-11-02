#include "globals.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
 
int lineno = 1;
 
int main(int argc, char* argv[])
{
	TokenType token;
 
	do
	{
		token = getToken();
	} while (token != ENDFILE);
 
	return 0;
}
