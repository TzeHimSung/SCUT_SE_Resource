
%{

(* PASLEX.L: lexical analyzer for Pascal, adapted to TP Lex, 2-28-89 AG *)

%}

%{
(*
 * lex input file for pascal scanner
 *
 * extensions: to ways to spell "external" and "->" ok for "^".
 *
 *)
%}

%{

(* Note: Keywords are determined by scanning a keyword table, rather
   than including the keyword patterns in the Lex source which is done
   in the original version of this file. I prefer this method, because
   it makes the grammar itself more readable (handling case-insensitive
   keywords in Lex is quite cumbersome, e.g., you will have to write
   something like [Aa][Nn][Dd] to match the keyword `and'), and also
   produces a more (space-) efficient analyzer (184 states and 375
   transitions for the keyword pattern version, against only 40 states
   and 68 transitions for the keyword table version). *)

procedure commenteof;
  begin
    writeln('unexpected EOF inside comment at line ', yylineno);
  end(*commenteof*);

function upper(str : String) : String;
  (* converts str to uppercase *)
  var i : integer;
  begin
    for i := 1 to length(str) do
      str[i] := upCase(str[i]);
    upper := str
  end(*upper*);

function is_keyword(id : string; var token : integer) : boolean;
  (* checks whether id is Pascal keyword; if so, returns corresponding
     token number in token *)
  const
    id_len = 20;
  type
    Ident = string[id_len];
  const
    (* table of Pascal keywords: *)
    no_of_keywords = 39;
    keyword : array [1..no_of_keywords] of Ident = (
      'AND',       'ARRAY',     'BEGIN',    'CASE',
      'CONST',     'DIV',       'DO',       'DOWNTO',
      'ELSE',      'END',       'EXTERNAL', 'EXTERN',
      'FILE',      'FOR',       'FORWARD',  'FUNCTION',
      'GOTO',      'IF',        'IN',       'LABEL',
      'MOD',       'NIL',       'NOT',      'OF',
      'OR',        'OTHERWISE', 'PACKED',   'PROCEDURE',
      'PROGRAM',   'RECORD',    'REPEAT',   'SET',
      'THEN',      'TO',        'TYPE',     'UNTIL',
      'VAR',       'WHILE',     'WITH');
    keyword_token : array [1..no_of_keywords] of integer = (
      _AND,        _ARRAY,      _BEGIN,     _CASE,
      _CONST,      _DIV,        _DO,        _DOWNTO,
      _ELSE,       _END,        _EXTERNAL,  _EXTERNAL,
                                (* EXTERNAL: 2 spellings (see above)! *)
      _FILE,       _FOR,        _FORWARD,   _FUNCTION,
      _GOTO,       _IF,         _IN,        _LABEL,
      _MOD,        _NIL,        _NOT,       _OF,
      _OR,         _OTHERWISE,  _PACKED,    _PROCEDURE,
      _PROGRAM,    _RECORD,     _REPEAT,    _SET,
      _THEN,       _TO,         _TYPE,      _UNTIL,
      _VAR,        _WHILE,      _WITH);
  var m, n, k : integer;
  begin
    id := upper(id);
    (* binary search: *)
    m := 1; n := no_of_keywords;
    while m<=n do
      begin
        k := m+(n-m) div 2;
        if id=keyword[k] then
          begin
            is_keyword := true;
            token := keyword_token[k];
            exit
          end
        else if id>keyword[k] then
          m := k+1
        else
          n := k-1
      end;
    is_keyword := false
  end(*is_keyword*);

%}

NQUOTE    [^']

%%

%{

var c  : char;
    kw : integer;

%}

[a-zA-Z]([a-zA-Z0-9])*	if is_keyword(yytext, kw) then
                          return(kw)
                        else
                          return(IDENTIFIER);

":="			return(ASSIGNMENT);
'({NQUOTE}|'')+'	return(CHARACTER_STRING);
":"			return(COLON);
","			return(COMMA);
[0-9]+			return(DIGSEQ);
"."			return(DOT);
".."			return(DOTDOT);
"="			return(EQUAL);
">="			return(GE);
">"			return(GT);
"["			return(LBRAC);
"<="			return(LE);
"("			return(LPAREN);
"<"			return(LT);
"-"			return(MINUS);
"<>"			return(NOTEQUAL);
"+"			return(PLUS);
"]"			return(RBRAC);
[0-9]+"."[0-9]+		return(REALNUMBER);
")"			return(RPAREN);
";"			return(SEMICOLON);
"/"			return(SLASH);
"*"			return(STAR);
"**"			return(STARSTAR);
"->"            	|
"^"			return(UPARROW);

"(*"            	|
"{"			begin
                          repeat
			    c := get_char;
			    case c of
			      '}' : ;
			      '*' : begin
				      c := get_char;
				      if c=')' then exit else unget_char(c)
				    end;
                              #0 : begin
                                     commenteof;
                                     exit;
                                   end;
			    end;
			  until false
                        end;
[ \n\t\f]		;

.			return(ILLEGAL);
