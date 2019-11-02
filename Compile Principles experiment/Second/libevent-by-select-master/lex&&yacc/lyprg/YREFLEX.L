
%{

(* Lexical analyzer for the YREF program, V1.1 4-30-91 AG
   implements a lexical analyzer for Yacc grammars *)

function yywrap : Boolean;
  begin
    yywrap := true; (* files are closed by main program *)
  end(*yywrap*);

%}

L			[A-Za-z_]
D			[0-9]
Q			\\([0-7]{1,3}|.)
P			[\\%]

%%

%{

var c : Char;

%}

  (* whitespace: *)

[ \t\n]			;

  (* comments: *)

"/*"			skip('*/');

  (* identifiers, literals and numbers: *)

{L}({L}|{D})*		begin
                          symlineno := yylineno;
			  yylval := key(yytext, max_syms, lookup, entry);
                          scan(c);
                          if c=':' then
			    return(C_ID)
			  else
			    return(ID);
			end;

'([^'\n\\]|{Q})+'	|
\"([^"\n\\]|{Q})+\"	return(LITERAL);

{D}+			return(NUMBER);

  (* keywords (various synonyms): *)

{P}token		|
{P}term			|
{P}0			return(PTOKEN);

{P}left			|
{P}<			return(PLEFT);

{P}right		|
{P}>			return(PRIGHT);

{P}nonassoc		|
{P}binary		|
{P}2			return(PNONASSOC);

{P}type			return(PTYPE);

{P}start		return(PSTART);

{P}prec			|
{P}=			return(PPREC);

{P}{P}			return(PP);

{P}\{			return(LCURL);

{P}\}			return(RCURL);

  (* others: *)

.			returnc(yytext[1]);
