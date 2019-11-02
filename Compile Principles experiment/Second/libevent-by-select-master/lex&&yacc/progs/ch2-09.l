%{
int comments, code, whiteSpace;
%}

%s COMMENT

%%
^[ \t]*"/*"	{ BEGIN COMMENT; /* enter comment eating state */ }
^[ \t]*"/*".*"*/"[ \t]*\n {
		 comments++; /* self-contained comment */
		 }

<COMMENT>"*/"[ \t]*\n	{ BEGIN 0;  comments++;}
<COMMENT>"*/"		{ BEGIN 0; }
<COMMENT>\n	{ comments++; }
<COMMENT>.\n	{ comments++; }

^[ \t]*\n	{ whiteSpace++; }

.+"/*".*"*/".*\n	{ code++; }
.*"/*".*"*/".+\n	{ code++; }
.+"/*".*\n		{ code++; BEGIN COMMENT; }
.\n			{ code++; }

.			; /* ignore everything else */
%%
main()
{
	yylex();
	printf("code: %d, comments %d, white space %d\n",
		code, comments, whiteSpace);
}
