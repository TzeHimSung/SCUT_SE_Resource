%{
unsigned verbose;
unsigned fname;
char *progName;

int myinput(char *buf, int max);
#undef YY_INPUT
#define YY_INPUT(buf,result,max) (result = myinput(buf,max))
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

/* provide a chunk of stuff to flex */
/* it handles unput itself, so we pass in an argument at a time */
int
myinput(char *buf, int max)
{
	int len, copylen;

	if (targv >= arglim)
		return 0;	/* EOF */
	len = strlen(*targv)-offset;	/* amount of current arg */
	if(len >= max)
		copylen = max-1;
	else
		copylen = len;
	if(len > 0)
		memcpy(buf, targv[0]+offset, copylen);
	if(targv[0][offset+copylen] == '\0') {	/* end of arg */
		buf[copylen] = ' ';
		copylen++;
		offset = 0;
		targv++;
	}
	return copylen;
}
