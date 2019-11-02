
/* YREF.Y: YREF cross reference utility, V1.1 5-1-91 AG

   This is a sample Yacc program which produces complete cross reference
   listings of Yacc source (.y) files. It is based on the same grammar
   which has actually been used to implement Yacc itself, and thus might
   also be useful for other utilities which have to parse Yacc sources.
   The lexical analyzer for this program can be found in YREFLEX.L. The
   units YRefTools and YRefTables supply the additional routines and data
   structures used by the program. To compile the program, issue the
   commands:
     yacc yref
     lex yreflex
     tpc /m yref

   The YREF program reads and parses a Yacc source file and produces
   a listing of the source file (with linenumbers), followed by a cross
   reference table of all (literal and nonterminal) identifiers used in the
   grammar, in alphabetical order. For each symbol, YREF lists the type of
   the symbol (if type tags are used), and the corresponding line numbers
   where the symbol occurs. Line numbers marked with an asterisk denote
   places where the corresponding symbol is "defined" (the lines where a
   terminal is introduced in a %token definition, or a nonterminal appears
   on the left-hand side of a rule). Symbols which do not have at least
   one defining position are listed as "undefined symbols" at the end of
   the list; this is useful to check Yacc grammars for "completeness."

   YREF does not handle syntactic errors in the source file; it is
   assumed that the input file is a syntactically correct Yacc program.
   If a syntax error is encountered, YREF simply gives an error message
   indicating the offending position, and exits with return code 1.

   The command line syntax is as follows:

   YREF yacc-file[.Y] [output-file[.REF]]

   Default extensions .Y for the input, and .REF for the output file
   are supplied automatically. If the output file name is ommitted,
   it defaults to the name of the input file with new suffix .REF.

*/

%{

{$I-}

uses YaccLib, LexLib, Dos, YRefTools, YRefTables;

procedure yyerror ( msg : String );
  begin
    writeln(msg , ' in line ', yylineno, ' at or near `', yytext, '''');
  end(*yyerror*);

var tag : Integer;       (* type tag *)
    symlineno : Integer; (* line number of last identifier *)

procedure scan ( var c : Char ); forward;
  (* scan for nonempty character, skipping comments *)

procedure skip ( delim : String ); forward;
  (* skip up to next occurrence of delim *)

procedure search ( delim : String ); forward;
  (* like skip, but retain found delimiter, and handle embedded strings *)

%}

/* Tokens of the Yacc language:

   Note the use of C_ID to distinguish identifiers which start a new rule.
   This is necessary because the parser is limited to one-symbol lookahead
   and hence could not determine whether an identifier is followed by a
   colon, starting a new rule, or is simply just another nonterminal or
   token identifier in the right-hand of a rule. Thus the lexical analyzer
   performs the necessary lookahead, and returns C_ID if the identifier is
   followed by a colon (skipping blanks and comments), and ID otherwise. */

%token
  ID		/* identifiers: {letter}{letter_or_digit}* */
  C_ID		/* identifier which forms left side of rule, i.e. is
		   followed by a colon */
  LITERAL       /* literals (strings enclosed in single or double quotes) */
  NUMBER	/* nonnegative integers: {digit}+ */
  PTOKEN PLEFT PRIGHT PNONASSOC PTYPE PSTART PPREC
  		/* reserved words: PTOKEN=%token, etc. */
  PP		/* source sections separator %% */
  LCURL		/* curly braces: %{ and %} */
  RCURL
  ',' ':' ';' '|' '{' '}' '<' '>' '='
		/* literals */

%start grammar

%%

grammar		: defs PP rules aux_procs
		;

aux_procs	: /* empty: aux_procs is optional */

		| PP { yyaccept; }

		;


defs		: /* empty */

		| defs def

		;

def		: PSTART ID

		| LCURL { search('%}'); } RCURL

		| PTOKEN tag token_list

		| PLEFT tag token_list

		| PRIGHT tag token_list

		| PNONASSOC tag token_list

		| PTYPE tag nonterm_list

                | PTYPE tag

		;

tag		: /* empty: type tag is optional */
				{ tag := 0; }
		| '<' ID '>'
                		{ tag := $2; }
		;

token_list	: token_num

		| token_list token_num

		| token_list ',' token_num

		;

token_num	: LITERAL opt_num

               	| ID
				{ add_ref($1, symlineno, true);
                                  set_type($1, tag); }
                  opt_num

		;

opt_num         : /* empty */

                | NUMBER

                ;

nonterm_list	: nonterm

		| nonterm_list nonterm

		| nonterm_list ',' nonterm

		;

nonterm		: ID
				{ add_ref($1, symlineno, false);
                                  set_type($1, tag); }
		;


rules		: rule1

		| LCURL { search('%}'); } RCURL rule1
					/* rules section may be prefixed
					   with `local' Turbo Pascal
					   declarations */
		| rules rule

		;

rule1		: C_ID
				{ add_ref($1, symlineno, true); }
		  ':' body prec

		;

rule		: rule1

		| '|' body prec

		;

body		: /* empty */

		| body LITERAL

		| body ID
				{ add_ref($2, symlineno, false); }
                | body action

		;

action		: '{' { search('}'); } '}'

		| '=' { skip(';'); }
                		/* old language feature; code must be
				   single statement ending with `;' */
		;

prec		: /* empty */

		| PPREC LITERAL opt_action

		| PPREC ID
				{ add_ref($2, symlineno, false); }
		  opt_action

		| prec ';'

		;

opt_action	: /* empty */

		| action

		;

%%

procedure scan ( var c : Char );
  label next;
  const tab = #9; nl = #10;
  begin
next:
    c := get_char;
    case c of
      ' ', tab, nl: goto next;
      '/': begin
             c := get_char;
             if c='*' then
               begin
                 skip('*/');
                 goto next;
               end
             else
               begin
                 unget_char(c);
                 unget_char('/');
                 c := '/';
               end;
           end;
      else unget_char(c);
    end;
  end(*scan*);

procedure skip ( delim : String );
  var i, j : Integer; c : Char;
  begin
    i := 1;
    while i<=length(delim) do
      begin
        c := get_char;
        if c=delim[i] then
          inc(i)
        else if c<>#0 then
          begin
            for j := i-1 downto 2 do unget_char(delim[j]);
            i := 1;
          end
        else
          exit;
      end;
  end(*skip*);

procedure search ( delim : String );
  var i, j : Integer; c : Char;
  begin
    i := 1;
    while i<=length(delim) do
      begin
        c := get_char;
        if c=delim[i] then
          inc(i)
        else if c<>#0 then
          begin
            for j := i-1 downto 2 do unget_char(delim[j]);
            i := 1;
            if c='''' then
              (* skip string *)
              skip('''');
          end
        else
          exit;
      end;
    for i := length(delim) downto 1 do
      unget_char(delim[i]);
  end(*search*);

(* Lexical analyzer: *)

{$I YRefLex}

(* Main program: *)

function addExt ( filename, ext : String ) : String;
  (* add default extension to filename *)
  var d : DirStr; n : NameStr; e : ExtStr;
  begin
    fsplit(filename, d, n, e);
    if e='' then e := '.'+ext;
    addExt := d+n+e;
  end(*addExt*);

function root ( filename : String ) : String;
  (* return filename with extension stripped off *)
  var d : DirStr; n : NameStr; e : ExtStr;
  begin
    fsplit(filename, d, n, e);
    root := d+n;
  end(*root*);

var result, lineno : Integer;
    yfile, reffile, line : String;

begin

  (* sign-on: *)

  writeln('YREF Version 1.0 [Mar 91], Copyright (c) 1991 by Albert Graef');

  (* parse command line: *)

  case paramCount of
    1 : begin
          yfile   := addExt(paramStr(1), 'y');
          reffile := root(paramStr(1))+'.ref';
        end;
    2 : begin
          yfile   := addExt(paramStr(1), 'y');
          reffile := addExt(paramStr(2), 'ref');
        end;
    else
      begin
        writeln('Usage: YREF yacc-file[.Y] [output-file[.REF]]');
        halt(0);
      end;
  end;

  (* open files: *)

  assign(yyinput,  yfile);
  assign(yyoutput, reffile);

  reset(yyinput);
  if ioresult<>0 then
    begin
      writeln('cannot open file '+yfile);
      halt(1);
    end;

  rewrite(yyoutput);
  if ioresult<>0 then
    begin
      writeln('cannot open file '+reffile);
      halt(1);
    end;

  (* produce numbered listing: *)

  lineno := 1;
  while not eof(yyinput) do
    begin
      readln(yyinput, line); 
      writeln(yyoutput, lineno:5, ':  ', line);
      inc(lineno);
    end;

  close(yyinput); reset(yyinput);

  (* parse: *)

  result := yyparse;

  (* produce cross reference listing *)

  if result=0 then ref_list;

  (* close files: *)

  close(yyinput); close(yyoutput);
  if result>0 then erase(yyoutput);

  (* terminate: *)

  if (result=0) and (n_undef>0) then
    writeln(n_undef, ' undefined symbol(s)');

  halt(result);

end.
