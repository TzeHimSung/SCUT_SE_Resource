
%{

(* Lex demonstration program for the use of start states, taken from the UNIX
   manual. This program copies standard input to standard output, and changes
   the word `magic' to `first', `second' or `third' on any line that starts
   with the letter a, b or c, respectively.

   Try it out, e.g., by issuing the command `magic' and typing in the following
   lines:

     This is a magic word.
     a This is a magic word.
     b This is a magic word.
     c This is a magic word.

   The respond should be:

     This is a magic word.
     a This is a first word.
     b This is a second word.
     c This is a third word.

   To compile this program: lex magic
                            tpc magic

*)

uses LexLib;

%}

%S AA BB CC

%%

^a		begin
		  echo; start(AA);
		end;
^b		begin
		  echo; start(BB);
		end;
^c		begin
		  echo; start(CC);
		end;
\n		begin
		  echo; start(0);
		end;
<AA>magic	write('first');
<BB>magic	write('second');
<CC>magic	write('third');
.		echo;

%%

begin
  if yylex=0 then ;
end.
