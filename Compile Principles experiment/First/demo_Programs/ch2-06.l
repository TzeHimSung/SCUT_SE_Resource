%{
#undef input
#undef unput
unsigned verbose;
unsigned fname;
char *progName;
%}

%s FNAME

%%
[ ]+         /* ignore blanks */ ;
<FNAME>[ ]+  /* ignore blanks */ ;

-h	|
"-?"	|
-help	{ printf("usage is: %s [-help | -h | -? ] [-verbose | -v]"
	 " [(-file| -f) filename]\n", progName);
	}
-v	|
-verbose { printf("verbose mode is on\n"); verbose = 1; }


-f	|
-file	{ BEGIN FNAME; fname = 1; }

<FNAME>[^ ]+ { printf("use file %s\n", yytext); BEGIN 0; fname = 2;}

[^ ]+  ECHO;
%%
char **targv;	/* remembers arguments */
char **arglim;	/* end of arguments */

main(int argc, char **argv)
{
	progName = *argv;
	targv = argv+1;
	arglim = argv+argc;
	yylex();
	if(fname < 2)
		printf("No filename given\n");
}

static unsigned offset = 0;

int
input(void)
{
	char c;

	if (targv >= arglim)
		return(0);	/* EOF */
	/* end of argument, move to the next */
	if ((c = targv[0][offset++]) != '\0')
		return(c);
	targv++;
	offset = 0;
	return(' ');
}

/* simple unput only backs up, doesn't allow you to */
/* put back different text */
void
unput(int ch)
{

	/* AT&T lex sometimes puts back the EOF ! */
	if(ch == 0)
		return;	/* ignore, can't put back EOF */
	if (offset) {	/* back up in current arg */
		offset--;
		return;
	}

	targv--;	/* back to previous arg */
	offset = strlen(*targv);
}
