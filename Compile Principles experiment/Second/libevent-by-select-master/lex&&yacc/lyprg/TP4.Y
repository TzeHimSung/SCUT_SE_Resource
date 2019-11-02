
/* Turbo Pascal 4.0 grammar (prototype) 5-1-91 AG

   This is an "almost complete" TP Yacc grammar for Turbo Pascal Version
   4.0. Although the grammar is incomplete, untested and I do not have a
   lexical analyzer for it yet, I decided to include it in the distribution
   for those of you who are planning to write a Turbo Pascal parser.

   You will notice that this is not an adaption of the Standard Pascal
   grammar in PAS.Y. Because of the many additional features in the
   Turbo Pascal language I decided to write a new grammar from scratch.

   Writing a Yacc grammar for Turbo Pascal is not as easy as it first
   seems to be, because the special language features often defy easy
   specification as an LALR(1) grammar. Taking the naive approach, you
   are bound to get a lot of parsing conflicts in your grammar, since
   some of the constructs just cannot be parsed unambigiously without
   making use of semantic information.

   One construct that causes trouble is the qualified identifier which,
   in many contexts, can also be interpreted as a record component
   designator. To fix this, I simply decided to treat both "ordinary" and
   qualified identifiers as a single ID token. Of course this means that
   the lexical analyzer will have to distinguish between ordinary and
   qualified identifiers (this can be done with some semantic processing
   to determine which identifiers are unit names).

   There is still one ambiguity left in the grammar (causing one shift/
   reduce conflict) which comes from type casts in variable designators.
   The construct in question has the form:

      ID ( variable )

   In certain contexts, this can both be interpreted as a variable and
   as a function or procedure call. At present, the generated parser
   will parse such a construct as a variable. This will not be any
   problem (at least I hope so) unless you actually do semantic processing
   in your parser.

   If you have any questions/comments/critizism concerning this grammar,
   direct them to ag@muwiinfa.geschichte.uni-mainz.de. Perhaps someone
   even extends the grammar towards Turbo Pascal 5.x/6.x (if you do, it
   would be kind if you let me have a copy of this grammar such that I can
   include it in the next distribution). */


%start compilation_unit

/* Lexical stuff: */

%token

/* The multiple character symbols below are used in the grammar for better
   readability; identifier synonyms are given for use in the lexical analyzer.
   These definitions must come before any other token definitions (note that
   TP Yacc starts counting user-defined literals at 257). */

'..' 257
':=' 258
'<=' 259
'<>' 260
'>=' 261

%{

const

DOTDOT = 257
DEFEQ  = 258
LEQ    = 259
NEQ    = 260
GEQ    = 261

%}

%token

/* Keywords are stropped with underscores to prevent conflicts with Turbo
   Pascal keywords. */

_AND_		_ABSOLUTE_	_ARRAY_			_BEGIN_
_CASE_		_CONST_		_DIV_			_DOWNTO_
_DO_		_ELSE_		_END_			_EXTERNAL_
_FILE_		_FORWARD_	_FOR_			_FUNCTION_
_GOTO_		_IF_		_IMPLEMENTATION_ 	_INTERFACE_
_IN_		_LABEL_		_MOD_			_NIL_
_NOT_		_OF_		_OR_			_PACKED_
_PROCEDURE_	_PROGRAM_	_RECORD_		_REPEAT_
_SET_		_SHL_		_SHR_			_STRING_
_THEN_		_TO_		_TYPE_			_UNIT_
_UNTIL_		_USES_		_VAR_			_WHILE_
_WITH_		_XOR_

%right _THEN_ _ELSE_            /* resolve dangling else */

%token

ID
UNSIGNED_INTEGER
UNSIGNED_REAL
STRING_CONST

%%

compilation_unit
	: program
        | unit
        ;

/* Programs: */

program	: program_heading uses_clause block '.'
	;

program_heading
	: /* empty */
        | _PROGRAM_ ID ';'
        | _PROGRAM_ ID '(' id_list ')' ';'
	;

uses_clause
	: /* empty */
        | _USES_ id_list ';'
        ;

id_list	: ID
	| id_list ',' ID
        ;

block	: decl_sect_list compound_stmt
	;

/* Units: */

unit	: unit_heading interface_part implementation_part
	  initialization_part '.'
	;

unit_heading
	: _UNIT_ ID ';'
	;

interface_part
	: _INTERFACE_ uses_clause int_decl_sect_list
        ;

implementation_part
	: _IMPLEMENTATION_ decl_sect_list
        ;

initialization_part
	: compound_stmt
        | _END_
        ;

/* Declaration sections: */

decl_sect_list
	: /* empty */
        | decl_sect_list decl_sect
        ;

decl_sect
	: label_decl_sect
	| const_decl_sect
	| type_decl_sect
	| var_decl_sect
	| proc_decl
	| func_decl
	;

label_decl_sect
	: _LABEL_ label_list ';'
	;

label_list
	: label
        | label_list ',' label
        ;

label	: UNSIGNED_INTEGER
		/* must be decimal integer in the range 0..9999 */
	| ID
        ;

const_decl_sect
	: _CONST_ const_decl
	| const_decl_sect const_decl
	;

type_decl_sect
	: _TYPE_ type_decl
	| type_decl_sect type_decl
	;

var_decl_sect
	: _VAR_ var_decl
	| var_decl_sect var_decl
	;

/* Interface declaration sections: */

/* These appear instead of normal declaration sections in the interface
   part of a unit. The difference is that there are no label declarations
   and for procedures and functions only the headings are given. */

int_decl_sect_list
	: /* empty */
	| int_decl_sect_list int_decl_sect
	;

int_decl_sect
	: const_decl_sect
	| type_decl_sect
	| var_decl_sect
	| proc_heading
	| func_heading
	;

/* Constant declarations: */

const_decl
	: ID '=' const ';'
        | ID ':' type '=' typed_const ';'
        ;

const	: unsigned_number
	| sign unsigned_number
        | ID
        | sign ID
        | STRING_CONST
	;

unsigned_number
	: UNSIGNED_INTEGER
        | UNSIGNED_REAL
        ;

sign	: '+' | '-'
	;

typed_const
	: const
	| array_const
	| record_const
        | set_const
	;

array_const
	: '(' typed_const_list ')'
	;

typed_const_list
	: typed_const
	| typed_const_list ',' typed_const
	;

record_const
	: '(' const_field_list ')'
	;

const_field_list
	: const_field
	| const_field_list ',' const_field
	;

const_field
	: ID ':' typed_const
	;

set_const
	: '[' const_elem_list ']'
        ;

const_elem_list
	: /* empty */
        | const_elem_list1
        ;

const_elem_list1
	: const_elem
	| const_elem_list1 ',' const_elem
	;

const_elem
	: const
	| const '..' const
	;

/* Type declarations: */

type_decl
	: ID '=' type ';'
	;

type	: simple_type
	| pointer_type
        | structured_type
        | string_type
        ;

simple_type
	: ID
	| const '..' const	/* subrange */
	| '(' id_list ')'	/* enumeration */
	;

pointer_type
	: '^' ID
	;

structured_type
	: unpacked_structured_type
        | _PACKED_ unpacked_structured_type
        ;

unpacked_structured_type
	: array_type
	| record_type
	| set_type
	| file_type
	;

array_type
	: _ARRAY_ '[' simple_type_list ']' _OF_ type
	;

simple_type_list
	: simple_type
	| simple_type_list ',' simple_type
	;

record_type
	: _RECORD_ field_list _END_
	;

field_list
	: fixed_part
	| variant_part
	| fixed_part ';' variant_part
	;

fixed_part
	: record_section
	| fixed_part ';' record_section
	;

record_section
	: /* empty */
	| id_list ':' type
	;

variant_part
	: _CASE_ tag_field _OF_ variant_list
	;

tag_field
	: ID
	| ID ':' ID
	;

variant_list
	: variant
	| variant_list ';' variant
	;

variant	: /* empty */
	| case_tag_list ':' '(' field_list ')'
	;

case_tag_list
	: const
        | case_tag_list ',' const
        ;

set_type
	: _SET_ _OF_ simple_type
	;

file_type
	: _FILE_ _OF_ type
	| _FILE_
	;

string_type
	: _STRING_
	| _STRING_ '[' const ']'
	;

/* Variable declarations: */

var_decl
	: id_list ':' type absolute_clause ';'
	;

absolute_clause
	: /* empty */
        | _ABSOLUTE_ UNSIGNED_INTEGER ':' UNSIGNED_INTEGER
        | _ABSOLUTE_ ID
        ;

/* Procedure and function declarations: */

proc_decl
	: proc_heading proc_block ';'
	;

func_decl
	: func_heading proc_block ';'
	;

proc_heading
	: _PROCEDURE_ ID fp_list ';'
	;

func_heading
	: _FUNCTION_ ID fp_list ':' fptype ';'
	;

proc_block         	/* omitted inline and interrupt */
	: block
	| _EXTERNAL_
	| _FORWARD_
	;

fp_list : /* empty */
	| '(' fp_sect_list ')'
	;

fp_sect_list
	: fp_sect
	| fp_sect_list ';' fp_sect
	;

fp_sect	: id_list ':' fptype
	| _VAR_ id_list ':' fptype
	| _VAR_ id_list
	;

fptype	: ID
	| _STRING_
	;

/* Statements: */

stmt	: unlabelled_stmt
	| label ':' unlabelled_stmt
        ;

unlabelled_stmt
	: /* empty */
	| assignment
	| proc_call
	| goto_stmt
	| compound_stmt
	| if_stmt
	| case_stmt
	| repeat_stmt
	| while_stmt
	| for_stmt
	| with_stmt
	;

assignment
	: variable ':=' expr
	;

proc_call
	: ID param_list
	;

param_list
	: /* empty */
	| '(' expr_list ')'
	;

expr_list
	: expr
	| expr_list ',' expr
	;

goto_stmt
	: _GOTO_ label
	;

compound_stmt
	: _BEGIN_ stmt_list _END_
	;

stmt_list
	: stmt
	| stmt_list ';' stmt
	;

if_stmt	: _IF_ expr _THEN_ stmt
	| _IF_ expr _THEN_ stmt _ELSE_ stmt
	;

case_stmt
	: _CASE_ expr _OF_ case_list else_case _END_
	;

case_list
	: case
	| case_list ';' case
	;

case	: /* empty */
	| case_label_list ':' stmt
	;

case_label_list
	: case_label
        | case_label_list ',' case_label
        ;

case_label
	: const
        | const '..' const
        ;

else_case
	: /* empty */
	| _ELSE_ stmt
	| _ELSE_ stmt ';'
	;

repeat_stmt
	: _REPEAT_ stmt_list _UNTIL_ expr
	;

while_stmt
	: _WHILE_ expr _DO_ stmt
	;

for_stmt
	: _FOR_ ID ':=' expr _TO_ expr _DO_ stmt
	| _FOR_ ID ':=' expr _DOWNTO_ expr _DO_ stmt
	;

with_stmt
	: _WITH_ variable_list _DO_ stmt
	;

variable_list
	: variable
        | variable_list ',' variable
        ;

/* Variables: */

variable
	: ID
	| variable '[' expr_list ']'		/* array component */
	| variable '.' ID			/* record field */
        | variable '^'				/* pointer value */
	| ID '(' variable ')'		        /* type cast */
	;

/* Expressions: */

expr	: simple_expr
	| simple_expr relop simple_expr
        ;

relop	: '=' | '<>' | '<' | '>' | '<=' | '>=' | _IN_
        ;

simple_expr
	: term
        | sign term
        | simple_expr addop term
        ;

addop	: '+' | '-' | _OR_ | _XOR_
	;

term	: factor
	| term mulop factor
        ;

mulop	: '*' | '/' | _DIV_ | _MOD_ | _SHL_ | _SHR_
	;

/* Parameterless function calls, and function calls looking like type
   casts are caught as variables. */

factor	: variable
	| UNSIGNED_INTEGER
        | UNSIGNED_REAL
        | STRING_CONST
        | '[' elem_list ']'
        | _NIL_
        | '@' variable
        | '(' expr ')'
        | ID '(' expr_list ')'
        | _NOT_ factor
        ;

elem_list
	: /* empty */
        | elem_list1
        ;

elem_list1
	: elem
	| elem_list1 ',' elem
	;

elem	: expr
	| expr '..' expr
	;

