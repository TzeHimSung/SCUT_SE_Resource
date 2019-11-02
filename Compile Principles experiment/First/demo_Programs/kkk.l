%%
[\n\t ]	;
\".*\"  {printf("quoted_string");}

.	ECHO;
%%
main()
{
	yylex();
}
