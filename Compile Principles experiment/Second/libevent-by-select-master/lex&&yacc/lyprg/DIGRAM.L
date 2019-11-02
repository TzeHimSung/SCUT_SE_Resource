
%{

(* Lex demonstration program for the use of the REJECT routine, taken from
   the UNIX manual. This program produces a digram table of the input file
   (counts all pairs of lowercase letters).
   Usage: digram <input-file
   To compile: lex digram
               tpc digram *)

uses LexLib;

var digram : array ['a'..'z','a'..'z'] of Integer;

%}

%%

[a-z][a-z]	begin
		  inc(digram[yytext[1],yytext[2]]);
		  reject;
		end;
.		|
\n		;

%%

var c,d : char;
begin
  for c := 'a' to 'z' do for d := 'a' to 'z' do digram[c,d] := 0;
  if yylex=0 then
    for c := 'a' to 'z' do for d := 'a' to 'z' do
      if digram[c,d]<>0 then
	writeln(c,d,': ',digram[c,d]);
end.
