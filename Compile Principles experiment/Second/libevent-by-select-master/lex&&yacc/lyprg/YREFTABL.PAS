
unit YRefTables;

(* This module implements the symbol and cross reference table used
   by the YREF program. *)

interface

const

max_syms = 997; (* symbol table size (prime number!) *)

function lookup ( k : Integer ) : String;
procedure entry ( k : Integer; symbol : String );
(* lookup and entry routines for the symbol table *)

procedure add_ref ( k : Integer; lineno : Integer; mark : Boolean );
  (* Enter an occurrence for symbol k into the table; the mark flag indicates
     whether this occurrence should be marked as a "defining" occurrence. *)

procedure set_type ( k : Integer; t : Integer );
  (* set the type of symbol k *)

procedure ref_list;
  (* Produces the cross reference listing in alphabetical order. *)

var n_undef : Integer;
  (* number of undefined symbols *)

implementation

uses LexLib, YRefTools;

(* Basic data structures and routines: *)

type

StrPtr  = ^String;               (* dynamic strings *)
RefList = ^RefNode;              (* symbol occurrence lists *)
RefNode = record
            lineno : Integer;    (* line number *)
            mark   : Boolean;    (* mark flag *)
            next   : RefList;    (* link to next list element *)
          end;

function newStr ( str : String ) : StrPtr;
  (* returns a dynamic copy of str (only the number of bytes actually
     needed are allocated) *)
  var strp : StrPtr;
  begin
    getmem(strp, succ(length(str)));
    move(str, strp^, succ(length(str)));
    newStr := strp;
  end(*newStr*);

procedure append ( var L : RefList; lineno : Integer; mark : Boolean );
  (* append an entry to the given occurrence list (eliminate dups) *)
  begin
    if L=nil then
      begin
        new(L);
        L^.lineno := lineno;
        L^.mark   := mark;
        L^.next   := nil;
      end
    else if L^.lineno=lineno then
      L^.mark := L^.mark or mark
    else
      append(L^.next, lineno, mark);
  end(*append*);

(* Symbol and cross reference table: *)

var

sym_table : array [1..max_syms] of StrPtr;  (* symbol table (nil denotes
                                               empty entry) *)
ref_table : array [1..max_syms] of RefList; (* cross reference table *)

defined   : array [1..max_syms] of Boolean; (* defined symbols *)

sym_type  : array [1..max_syms] of Integer; (* symbol types *)

n_syms    : Integer;                        (* number of symbols in the
                                               table *)
sym_no    : array [1..max_syms] of Integer; (* symbol keys sorted in
                                               alphabetical order *)

(* Comparison and swap routines to sort the symbol table: *)

{$F+}
function sym_less ( i, j : Integer ) : Boolean;
{$F-}
  begin
    sym_less := sym_table[sym_no[i]]^<sym_table[sym_no[j]]^
  end(*sym_less*);

{$F+}
procedure sym_swap ( i, j : Integer );
{$F-}
  var x : Integer;
  begin
    x := sym_no[i]; sym_no[i] := sym_no[j]; sym_no[j] := x;
  end(*sym_swap*);

procedure sort;
  (* sort symbols into sym_no array *)
  var k : Integer;
  begin
    n_syms := 0;
    for k := 1 to max_syms do
      if (sym_table[k]<>nil) and (ref_table[k]<>nil) then
        begin
          inc(n_syms);
          sym_no[n_syms] := k;
        end;
    quicksort(1, n_syms, sym_less, sym_swap);
  end(*sort*);

(* Interface routines: *)

function lookup ( k : Integer ) : String;
  begin
    if sym_table[k]=nil then
      lookup := ''
    else
      lookup := sym_table[k]^
  end(*lookup*);

procedure entry ( k : Integer; symbol : String );
  begin
    sym_table[k] := newStr(symbol)
  end(*entry*);

procedure add_ref ( k : Integer; lineno : Integer; mark : Boolean );
  begin
    append(ref_table[k], lineno, mark);
    defined[k] := defined[k] or mark;
  end(*add_ref*);

procedure set_type ( k : Integer; t : Integer );
  begin
    sym_type[k] := t;
  end(*set_type*);

procedure ref_list;
  const tab = #9;
  var i : Integer;
      L : RefList;
  begin
    sort;
    writeln(yyoutput);
    for i := 1 to n_syms do
      begin
        write(yyoutput, '  ', sym_table[sym_no[i]]^);
        if sym_type[sym_no[i]]<>0 then
          write(yyoutput, ' <', sym_table[sym_type[sym_no[i]]]^, '>');
        write(yyoutput, ' ');
        L := ref_table[sym_no[i]];
        while L<>nil do
          with L^ do
            begin
              write(yyoutput, ' ', lineno);
              if mark then write(yyoutput, '*');
              L := next;
            end;
        writeln(yyoutput);
      end;
    n_undef := 0;
    for i := 1 to n_syms do
      if not defined[sym_no[i]] and
         (sym_table[sym_no[i]]^<>'error') then
        inc(n_undef);
    if n_undef>0 then
      begin
        writeln(yyoutput);
        writeln(yyoutput, '  undefined symbols:');
        writeln(yyoutput);
        for i := 1 to n_syms do
          if not defined[sym_no[i]] and
             (sym_table[sym_no[i]]^<>'error') then
            writeln(yyoutput, '  ', sym_table[sym_no[i]]^);
      end;
  end(*ref_list*);

var k : Integer;
begin
  for k := 1 to max_syms do
    begin
      sym_table[k] := nil;
      ref_table[k] := nil;
      defined[k]   := false;
      sym_type[k]  := 0;
    end;
end(*YRefTables*).
