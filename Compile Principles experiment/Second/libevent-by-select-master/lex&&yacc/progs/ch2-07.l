%s MAGIC

%%
<MAGIC>.+	{ BEGIN 0; printf("Magic:"); ECHO; }
magic		BEGIN MAGIC;
.+		ECHO;
%%

main()
{
	yylex();
}
