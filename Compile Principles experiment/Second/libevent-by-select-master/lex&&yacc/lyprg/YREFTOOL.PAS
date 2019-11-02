
unit YRefTools;

(* This module supplies generic hash table and quicksort routines used
   by the YREF program. *)

interface

(* Quicksort: *)

type

OrderPredicate = function (i, j : Integer) : Boolean;
SwapProc = procedure (i, j : Integer);

procedure quicksort(lo, hi: Integer;
                    less : OrderPredicate;
                    swap : SwapProc);
  (* General inplace sorting procedure based on the quicksort algorithm.
     This procedure can be applied to any sequential data structure;
     only the corresponding routines less which compares, and swap which
     swaps two elements i,j of the target data structure, must be
     supplied as appropriate for the target data structure.
     - lo, hi: the lower and higher indices, indicating the elements to
       be sorted
     - less(i, j): should return true if element no. i `is less than'
       element no. j, and false otherwise; any total quasi-ordering may
       be supplied here (if neither less(i, j) nor less(j, i) then elements
       i and j are assumed to be `equal').
     - swap(i, j): should swap the elements with index i and j *)

(* Generic hash table routines (based on quadratic rehashing; hence the
   table size must be a prime number): *)

type

TableLookupProc = function(k : Integer) : String;
TableEntryProc  = procedure(k : Integer; symbol : String);

function key(symbol : String;
             table_size : Integer;
             lookup : TableLookupProc;
             entry  : TableEntryProc) : Integer;
  (* returns a hash table key for symbol; inserts the symbol into the
     table if necessary
     - table_size is the symbol table size and must be a fixed prime number
     - lookup is the table lookup procedure which should return the string
       at key k in the table ('' if entry is empty)
     - entry is the table entry procedure which is assumed to store the
       given symbol at the given location *)

function definedKey(symbol : String;
                    table_size : Integer;
                    lookup : TableLookupProc) : Boolean;
  (* checks the table to see if symbol is in the table *)

implementation

procedure fatal(msg : String);
  (* writes a fatal error message and exits *)
  begin
    writeln(msg);
    halt(1);
  end(*fatal*);

(* Quicksort: *)

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

(* Generic hash table routines: *)

function hash(str : String; table_size : Integer) : Integer;
  (* computes a hash key for str *)
  var i, key : Integer;
  begin
    key := 0;
    for i := 1 to length(str) do
      inc(key, ord(str[i]));
    hash := key mod table_size + 1;
  end(*hash*);

procedure newPos(var pos, incr, count : Integer; table_size : Integer);
  (* computes a new position in the table (quadratic rehashing)
     - pos: current position (+inc)
     - incr: current increment (+2)
     - count: current number of collisions (+1)
     quadratic rehashing formula for position of str after n collisions:
       pos(str, n) = (hash(str)+n^2) mod table_size +1
     note that n^2-(n-1)^2 = 2n-1 <=> n^2 = (n-1)^2 + (2n-1) for n>0,
     i.e. the increment inc=2n-1 increments by two in each collision *)
  begin
    inc(count);
    inc(pos, incr);
    if pos>table_size then pos := pos mod table_size + 1;
    inc(incr, 2)
  end(*newPos*);

function key(symbol : String;
             table_size : Integer;
             lookup : TableLookupProc;
             entry  : TableEntryProc) : Integer;
  var pos, incr, count : Integer;
  begin
    pos := hash(symbol, table_size);
    incr := 1;
    count := 0;
    while count<=table_size do
      if lookup(pos)='' then
        begin
          entry(pos, symbol);
          key := pos;
          exit
        end
      else if lookup(pos)=symbol then
        begin
          key := pos;
          exit
        end
      else
        newPos(pos, incr, count, table_size);
    fatal('symbol table overflow')
  end(*key*);

function definedKey(symbol : String;
                    table_size : Integer;
                    lookup : TableLookupProc) : Boolean;
  var pos, incr, count : Integer;
  begin
    pos := hash(symbol, table_size);
    incr := 1;
    count := 0;
    while count<=table_size do
      if lookup(pos)='' then
        begin
          definedKey := false;
          exit
        end
      else if lookup(pos)=symbol then
        begin
          definedKey := true;
          exit
        end
      else
        newPos(pos, incr, count, table_size);
    definedKey := false
  end(*definedKey*);

end(*YRefTools*).
