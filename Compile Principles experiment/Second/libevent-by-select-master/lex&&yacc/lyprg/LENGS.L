
%{

(* This is a Lex demonstration program taken from the UNIX manual which
   which counts words in a file.
   Usage: lengs <input-file
   This will produce a histogram of word lengths in the input file.
   To compile: lex lengs
               tpc lengs *)

uses LexLib;

var lengs : array [1..100] of Integer;

%}

%%

[a-zA-Z]+	inc(lengs[yyleng]);
.		|
\n		;

%%

var i : Integer;

begin
  for i := 1 to 100 do lengs[i] := 0;
  if yylex=0 then ;
  writeln('Length':6, ' ':3, 'No. words':6);
  for i := 1 to 100 do if lengs[i]>0 then
    writeln(i:6, ' ':3, lengs[i]:6);
end.
