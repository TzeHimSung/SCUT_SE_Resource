
(* KWTBL - prepare keyword tables for use in lexical analyzers

   USAGE: kwtbl [<input-file] [>output-file]

   DESCRIPTION

   KWTBL is a tiny utility that helps you prepare keyword tables for use
   in lexical analyzers, such as yylex routines prepared with the TP Lex
   program.

   In languages with a large number of keywords it is often more efficient
   to analyze the reserved words using a general pattern, and then use a
   table lookup procedure to determine the actual code for the keyword.
   Also, large keyword tables in a Lex grammar can easily cause TP Lex to
   overflow; in such cases the KWTBL utility is useful.

   KWTBL reads in a list of (keyword, code) pairs, sorts keywords alpha-
   betically, and writes out two typed array constants, one containing
   the keyword strings and the other containing the corresponding codes.
   These tables may be accessed through a binary table lookup procedure
   which is appended to the end of the output file. The code for the
   table lookup procedure is assumed to be in the file 'KWTBL.COD' which
   must be present either in the current directory or in the directory
   from which KWTBL was executed; without this file, KWTBL does not
   generate the table lookup procedure.

   Consider a typical programming language with identifiers and certain
   reserved words which look like identifiers. The corresponding rules
   of your TP Lex program might look like:

   if		return( _IF_ );
   while	return( _WHILE_ );
   ...
   [A-Za-z]+	return( ID );

   Alternatively, you can use KWTBL to produce a keyword table lookup
   procedure. The input to KWTBL will be something like:

   if		_IF_
   while	_WHILE_
   ...

   Each keyword entry is on a separate line, and keyword and corresponding
   code are separated by whitespace (blanks and/or tabs). Empty lines are
   ignored. You do not have to sort the keyword entries; KWTBL will do that
   for you. The code for a keyword can actually be any character sequence
   which denotes a legal constant integer expression.

   From the keyword table, KWTBL produces two typed array constants:

   const

   nkws = ...;   { number of different keywords }
   kwsize = ...; { maximum size of keywords }

   kwtbl : array [ 1..nkws ] of String[ kwsize ] = (
     ..., 'if', ..., 'while', ...
   );

   kwcod : array [ 1..nkws ] of Integer = (
     ..., _IF_, ..., _WHILE_, ...
   );

   These tables may be accessed through the table lookup procedure (named
   kwlookup in the standard version of the KWTBL.COD file). The corresponding
   Lex code will now be:

   [A-Za-z]+	if kwlookup( yytext, code ) then
   		  return( code )
                else
                  return( ID );
*)

uses Dos;

(* Quicksort: *)

type

OrderPredicate = function (i, j : Integer) : Boolean;
SwapProc = procedure (i, j : Integer);

procedure quicksort(lo, hi: Integer;
                    less : OrderPredicate;
                    swap : SwapProc);
  (* derived from the quicksort routine in QSORT.PAS in the Turbo Pascal
     distribution *)
  procedure sort(l, r: Integer);
    var i, j, k : Integer;
    begin
      i := l; j := r; k := (l+r) DIV 2;
      repeat
        while less(i, k) do inc(i);
        while less(k, j) do dec(j);
        if i<=j then
          begin
            swap(i, j);
            if k=i then k := j (* pivot element swapped! *)
            else if k=j then k := i;
            inc(i); dec(j);
          end;
      until i>j;
      if l<j then sort(l,j);
      if i<r then sort(i,r);
    end(*sort*);
  begin
    if lo<hi then sort(lo,hi);
  end(*quicksort*);

const

maxkws = 100;	 (* maximum number of keywords *)
maxkwsize = 40;	 (* maximum keyword size; longer keywords are truncated *)
maxcodsize = 40; (* maximum code size; longer keyword codes are truncated *)

type

KwString = String[ maxkwsize ];
CodString = String [ maxcodsize ];

var

(* the keyword table: *)

nkws, kwsize : Integer;

kwtbl : array [ 1..maxkws ] of KwString;
kwcod : array [ 1..maxkws ] of CodString;

var stderr : Text;

procedure error( msg : String );
  (* write out an error message *)
  begin
    writeln( stderr, '*** ', msg );
  end(*error*);

procedure fatal( msg : String );
  (* write error message and halt program *)
  begin
    error( 'fatal: '+msg );
    halt ( 1 );
  end(*fatal*);

procedure split( line : String; var kw : KwString; var cod : CodString );
  (* split a line in keyword and code part *)
  const tab = ^I;
  var i : Integer;
  begin
    kw := ''; i := 1;
    while (i<=length(line)) and (line[i]<>' ') and (line[i]<>tab) do
      begin
        if length(kw)<maxkwsize then kw := kw+line[i];
        inc(i);
      end;
    while (i<=length(line)) and ((line[i]=' ') or (line[i]=tab)) do inc(i);
    cod := copy( line, i, length(line) );
  end(*split*);

function path( filename : String ) : String;
  (* return the drive/directory part of a filename *)
  var d : DirStr; n : NameStr; e : ExtStr;
  begin
    fsplit( filename, d, n, e );
    path := d;
  end(*path*);

function intStr( i : Integer ) : String;
  (* convert an integer to a string *)
  var s : String;
  begin
    Str( i, s );
    intStr := s;
  end(*intStr*);

(* Routines to sort the keyword table: *)

{$F+}
function less( i, j : Integer ) : Boolean;
  begin
    less := kwtbl[i]<kwtbl[j];
  end(*less*);
procedure swap( i, j : Integer );
  var kw : KwString; cod : CodString;
  begin
    kw := kwtbl[i]; cod := kwcod[i];
    kwtbl[i] := kwtbl[j]; kwcod[i] := kwcod[j];
    kwtbl[j] := kw; kwcod[j] := cod;
  end(*swap*);
{$F-}

var line : String; i, lineno, actcol : Integer; codfile : Text;

begin
  (* open stderr device: *)
  assign( stderr, '' ); rewrite( stderr );
  TextRec(stderr).handle := 2;
  TextRec(stderr).bufSize := 1;
  (* read in the keyword table from standard input: *)
  nkws := 0; kwsize := 0; lineno := 0;
  while not eof do
    begin
      readln( line ); inc( lineno );
      if line<>'' then
        begin
          inc( nkws );
          if nkws>maxkws then fatal( 'too many keywords' );
          split( line, kwtbl[ nkws ], kwcod[ nkws ] );
          if ( kwtbl[ nkws ]='' ) or ( kwcod[ nkws ] = '' ) then
            begin
              error( 'error in line '+intStr( lineno ) );
              dec( nkws );
            end
	  else if length(kwtbl[nkws])>kwsize then
	    kwsize := length(kwtbl[nkws]);
        end;
    end;
  (* sort the keyword table: *)
  quicksort( 1, nkws, less, swap );
  (* produce output code: *)
  if nkws=0 then fatal( 'nothing to do' );
  writeln;
  writeln( 'const' );
  writeln;
  writeln('nkws = ', nkws, ';' );
  writeln('kwsize = ', kwsize, ';' );
  writeln;
  writeln('kwtbl : array [ 1..nkws ] of String[ kwsize ] = (' );
  write( '  ' ); actcol := 3;
  for i := 1 to nkws do
    begin
      if actcol+length(kwtbl[i])>79 then
        begin
          writeln; write( '  ' ); actcol := 3;
        end;
      write( '''', kwtbl[i], '''' ); inc( actcol, length( kwtbl[i] )+2 );
      if i<nkws then
        begin
          write( ', ' ); inc( actcol, 2 );
        end;
    end;
  writeln;
  writeln( ');' );
  writeln;
  writeln('kwcod : array [ 1..nkws ] of Integer = (' );
  write( '  ' ); actcol := 3;
  for i := 1 to nkws do
    begin
      if actcol+length(kwcod[i])>79 then
        begin
          writeln; write( '  ' ); actcol := 3;
        end;
      write( kwcod[i] ); inc( actcol, length( kwcod[i] ) );
      if i<nkws then
        begin
          write( ', ' ); inc( actcol, 2 );
        end;
    end;
  writeln;
  writeln( ');' );
  writeln;
  (* copy KWTBL.COD file to the output: *)
  {$I-}
  assign( codfile, 'KWTBL.COD' ); reset( codfile );
  if ioresult<>0 then
    begin
      assign( codfile, path( paramStr(0) )+'KWTBL.COD' ); reset( codfile );
      if ioresult<>0 then halt;
    end;
  while not eof( codfile ) do
    begin
      readln( codfile, line );
      writeln( line );
    end;
  close( codfile );
end.
