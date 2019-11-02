
/* Sample Yacc grammar for a simple desktop calculator; derived from a
   grammar in Aho et al: Compilers. Principles, Techniques and Tools (Sect.
   4.9).

   Lexical analyzer is in Lex program ExprLex.l.

   To compile parser and lexical analyzer, issue the following commands:

     yacc expr
     lex  exprlex
     tpc  expr

   Description: This program reads simple arithmetic expressions, constructed
   with real constants, operators +, -, *, /, unary - and parentheses
   (operators have their usual precedence, unary minus is highest), from
   standard input (one per line) and prints the result on standard output.
   Variables are denoted by a single letter (no distinction between upper and
   lowercase letters); they are assigned values through an assignment of the
   form <var>=<expr>.
   The program is terminated by entering an empty line. */

%{

uses YaccLib, LexLib;

var x : array [1..26] of Real;

%}

%token <Real> NUM       /* constants */
%token <Integer> ID     /* variables */
%type <Real> expr	/* expressions */

%left '+' '-'      	/* operators */
%left '*' '/'
%right UMINUS

%token ILLEGAL 		/* illegal token */

%%

input	: /* empty */
	| input '\n'		 { yyaccept; }
	| input expr '\n'	 { writeln($2:10:2); }
	| input ID '=' expr '\n' { x[$2] := $4; writeln($4:10:2); }
	| error '\n'             { yyerrok; }
	;

expr    :  expr '+' expr	 { $$ := $1 + $3; }
	|  expr '-' expr	 { $$ := $1 - $3; }
	|  expr '*' expr	 { $$ := $1 * $3; }
	|  expr '/' expr	 { $$ := $1 / $3; }
	|  '(' expr ')'		 { $$ := $2; }
	|  '-' expr              { $$ := -$2; }
           %prec UMINUS
	|  NUM                   { $$ := $1; }
        |  ID                    { $$ := x[$1]; }
	;

%%

{$I ExprLex}

var i : Integer;

begin
  for i := 1 to 26 do x[i] := 0.0;
  if yyparse=0 then { done };
end.
