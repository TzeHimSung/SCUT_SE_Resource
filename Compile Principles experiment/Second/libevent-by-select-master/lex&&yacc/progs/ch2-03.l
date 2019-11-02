%{
/*
 * ch2-03.l
 *
 * The word counter example for multiple files
 *
 */

unsigned long charCount = 0, wordCount = 0, lineCount = 0;

#undef yywrap	/* sometimes a macro by default */

%}

word [^ \t\n]+
eol  \n
%%
{word}	{ wordCount++; charCount += yyleng; }
{eol}	{ charCount++; lineCount++; }
.	charCount++;
%%

char **fileList;
unsigned currentFile = 0;
unsigned nFiles;
unsigned long totalCC = 0;
unsigned long totalWC = 0;
unsigned long totalLC = 0;

main(argc,argv)
int argc;
char **argv;
{
        FILE *file;

        fileList = argv+1;
        nFiles = argc-1;

        if (argc == 2) {
		/*
		 * we handle the single file case differently from
		 * the multiple file case since we don't need to
		 * print a summary line
		 */
		currentFile = 1;
		file = fopen(argv[1], "r");
		if (!file) {
			fprintf(stderr,"could not open %s\n",argv[1]);
			exit(1);
		}
		yyin = file;
        }
        if (argc > 2)
		yywrap();	/* open first file */

	yylex();
        /*
         * once again, we handle zero or one file
	 * differently from multiple files.
         */
        if (argc > 2) {
		printf("%8lu %8lu %8lu %s\n", lineCount, wordCount,
			charCount, fileList[currentFile-1]);
		totalCC += charCount;
		totalWC += wordCount;
		totalLC += lineCount;
		printf("%8lu %8lu %8lu total\n",totalLC, totalWC, totalCC);
	} else
		printf("%8lu %8lu %8lu\n",lineCount, wordCount, charCount);

	return 0;
}

/*
 * the lexer calls yywrap to handle EOF conditions (e.g., to
 * connect to a new file, as we do in this case.)
 */

yywrap()
{
	FILE *file;

	if ((currentFile != 0) && (nFiles > 1) && (currentFile < nFiles)) {
		/*
		 * we print out the statistics for the previous file.
		 */
		printf("%8lu %8lu %8lu %s\n", lineCount, wordCount,
			charCount, fileList[currentFile-1]);
		totalCC += charCount;
		totalWC += wordCount;
		totalLC += lineCount;
		charCount = wordCount = lineCount = 0;
		fclose(yyin);	/* done with that file */
	}

	while (fileList[currentFile] != (char *)0) {
		file = fopen(fileList[currentFile++], "r");
		if (file != NULL) {
			yyin = file;
			break;
		}
		fprintf(stderr,
			"could not open %s\n",
			fileList[currentFile-1]);
	}
	return (file ? 0 : 1);	/* 0 means there's more input */
}
