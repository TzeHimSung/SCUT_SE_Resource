%{
unsigned charCount = 0, wordCount = 0, lineCount = 0;
%}

word [^ \t\n]+
eol  \n
%%
{word}	{ wordCount++; charCount += yyleng; }
{eol}	{ charCount++; lineCount++; }
.	charCount++;
%%
main(argc,argv)
int argc;
char **argv;
{

	if (argc > 1) {
		FILE *file;

		file = fopen(argv[1], "r");
		if (!file) {
			fprintf(stderr,"could not open %s\n",argv[1]);
			exit(1);
		}
		yyin = file;
	}
	yylex();
	printf("%d %d %d\n",charCount, wordCount, lineCount);
	return 0;
}
