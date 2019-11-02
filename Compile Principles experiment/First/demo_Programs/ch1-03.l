%{
/*
 * We expand upon the first example by adding recognition of some other
 * parts of speech.
 */

%}
%%

[\t ]+		/* ignore white space */ ;

is |
am |
are |
were |
was |
be |
being |
been |
do |
does |
did |
will |
would |
should |
can |
could |
has |
have |
had |
go		{ printf("%s: is a verb\n", yytext); }

very |
simply |
gently |
quietly |
calmly |
angrily		{ printf("%s: is an adverb\n", yytext); }

to |
from |
behind |
above |
below |
between |
below		{ printf("%s: is a preposition\n", yytext); }

if |
then |
and |
but |
or		{ printf("%s: is a conjunction\n", yytext); }

their |
my |
your |
his |
her |
its		{ printf("%s: is an adjective\n", yytext); }

I |
you |
he |
she |
we |
they		{ printf("%s: in a pronoun\n", yytext); }


[a-zA-Z]+ {
	 printf("%s:  don't recognize, might be a noun\n", yytext);
	}

\&.|\n		{ ECHO; /* normal default anyway */ }

%%

main()
{
	yylex();
}
