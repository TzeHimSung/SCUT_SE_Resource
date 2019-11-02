%%
main()
{
	extern double sqrt(), exp(), log();

	addfunc("sqrt", sqrt);
	addfunc("exp", exp);
	addfunc("log", log);
	yyparse();
}

addfunc(name, func)
char *name;
double (*func)();
{
	struct symtab *sp = symlook(name);
	sp->funcptr = func;
}
