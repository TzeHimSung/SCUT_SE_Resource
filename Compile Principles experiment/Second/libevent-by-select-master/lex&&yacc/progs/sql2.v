#############################################################################
#                         A C A D E M I C   C O P Y
# 
# This file was produced by an ACADEMIC COPY of Parser Generator. It is for
# use in non-commercial software only. An ACADEMIC COPY of Parser Generator
# can only be used by a student, or a member of an academic community. If
# however you want to use Parser Generator for commercial purposes then you
# will need to purchase a license. For more information see the online help or
# go to the Bumble-Bee Software homepage at:
# 
# http://www.bumblebeesoftware.com
# 
# This notice must remain present in the file. It cannot be removed.
#############################################################################

#############################################################################
# sql2.v
# YACC verbose file generated from sql2.y.
# 
# Date: 06/06/06
# Time: 22:50:47
# 
# AYACC Version: 2.07
#############################################################################


##############################################################################
# Rules
##############################################################################

    0  $accept : sql_list $end

    1  sql_list : sql ';'
    2           | sql_list sql ';'

    3  sql : schema

    4  schema : CREATE SCHEMA AUTHORIZATION user opt_schema_element_list

    5  opt_schema_element_list :
    6                          | schema_element_list

    7  schema_element_list : schema_element
    8                      | schema_element_list schema_element

    9  schema_element : base_table_def
   10                 | view_def
   11                 | privilege_def

   12  base_table_def : CREATE TABLE table '(' base_table_element_commalist ')'

   13  base_table_element_commalist : base_table_element
   14                               | base_table_element_commalist ',' base_table_element

   15  base_table_element : column_def
   16                     | table_constraint_def

   17  column_def : column data_type column_def_opt_list

   18  column_def_opt_list :
   19                      | column_def_opt_list column_def_opt

   20  column_def_opt : NOT NULLX
   21                 | NOT NULLX UNIQUE
   22                 | NOT NULLX PRIMARY KEY
   23                 | DEFAULT literal
   24                 | DEFAULT NULLX
   25                 | DEFAULT USER
   26                 | CHECK '(' search_condition ')'
   27                 | REFERENCES table
   28                 | REFERENCES table '(' column_commalist ')'

   29  table_constraint_def : UNIQUE '(' column_commalist ')'
   30                       | PRIMARY KEY '(' column_commalist ')'
   31                       | FOREIGN KEY '(' column_commalist ')' REFERENCES table
   32                       | FOREIGN KEY '(' column_commalist ')' REFERENCES table '(' column_commalist ')'
   33                       | CHECK '(' search_condition ')'

   34  column_commalist : column
   35                   | column_commalist ',' column

   36  view_def : CREATE VIEW table opt_column_commalist AS query_spec opt_with_check_option

   37  opt_with_check_option :
   38                        | WITH CHECK OPTION

   39  opt_column_commalist :
   40                       | '(' column_commalist ')'

   41  privilege_def : GRANT privileges ON table TO grantee_commalist opt_with_grant_option

   42  opt_with_grant_option :
   43                        | WITH GRANT OPTION

   44  privileges : ALL PRIVILEGES
   45             | ALL
   46             | operation_commalist

   47  operation_commalist : operation
   48                      | operation_commalist ',' operation

   49  operation : SELECT
   50            | INSERT
   51            | DELETE
   52            | UPDATE opt_column_commalist
   53            | REFERENCES opt_column_commalist

   54  grantee_commalist : grantee
   55                    | grantee_commalist ',' grantee

   56  grantee : PUBLIC
   57          | user

   58  sql : cursor_def

   59  cursor_def : DECLARE cursor CURSOR FOR query_exp opt_order_by_clause

   60  opt_order_by_clause :
   61                      | ORDER BY ordering_spec_commalist

   62  ordering_spec_commalist : ordering_spec
   63                          | ordering_spec_commalist ',' ordering_spec

   64  ordering_spec : INTNUM opt_asc_desc
   65                | column_ref opt_asc_desc

   66  opt_asc_desc :
   67               | ASC
   68               | DESC

   69  sql : manipulative_statement

   70  manipulative_statement : close_statement
   71                         | commit_statement
   72                         | delete_statement_positioned
   73                         | delete_statement_searched
   74                         | fetch_statement
   75                         | insert_statement
   76                         | open_statement
   77                         | rollback_statement
   78                         | select_statement
   79                         | update_statement_positioned
   80                         | update_statement_searched

   81  close_statement : CLOSE cursor

   82  commit_statement : COMMIT WORK

   83  delete_statement_positioned : DELETE FROM table WHERE CURRENT OF cursor

   84  delete_statement_searched : DELETE FROM table opt_where_clause

   85  fetch_statement : FETCH cursor INTO target_commalist

   86  insert_statement : INSERT INTO table opt_column_commalist values_or_query_spec

   87  values_or_query_spec : VALUES '(' insert_atom_commalist ')'
   88                       | query_spec

   89  insert_atom_commalist : insert_atom
   90                        | insert_atom_commalist ',' insert_atom

   91  insert_atom : atom
   92              | NULLX

   93  open_statement : OPEN cursor

   94  rollback_statement : ROLLBACK WORK

   95  select_statement : SELECT opt_all_distinct selection INTO target_commalist table_exp

   96  opt_all_distinct :
   97                   | ALL
   98                   | DISTINCT

   99  update_statement_positioned : UPDATE table SET assignment_commalist WHERE CURRENT OF cursor

  100  assignment_commalist :
  101                       | assignment
  102                       | assignment_commalist ',' assignment

  103  assignment : column '=' scalar_exp
  104             | column '=' NULLX

  105  update_statement_searched : UPDATE table SET assignment_commalist opt_where_clause

  106  target_commalist : target
  107                   | target_commalist ',' target

  108  target : parameter_ref

  109  opt_where_clause :
  110                   | where_clause

  111  query_exp : query_term
  112            | query_exp UNION query_term
  113            | query_exp UNION ALL query_term

  114  query_term : query_spec
  115             | '(' query_exp ')'

  116  query_spec : SELECT opt_all_distinct selection table_exp

  117  selection : scalar_exp_commalist
  118            | '*'

  119  table_exp : from_clause opt_where_clause opt_group_by_clause opt_having_clause

  120  from_clause : FROM table_ref_commalist

  121  table_ref_commalist : table_ref
  122                      | table_ref_commalist ',' table_ref

  123  table_ref : table
  124            | table range_variable

  125  where_clause : WHERE search_condition

  126  opt_group_by_clause :
  127                      | GROUP BY column_ref_commalist

  128  column_ref_commalist : column_ref
  129                       | column_ref_commalist ',' column_ref

  130  opt_having_clause :
  131                    | HAVING search_condition

  132  search_condition :
  133                   | search_condition OR search_condition
  134                   | search_condition AND search_condition
  135                   | NOT search_condition
  136                   | '(' search_condition ')'
  137                   | predicate

  138  predicate : comparison_predicate
  139            | between_predicate
  140            | like_predicate
  141            | test_for_null
  142            | in_predicate
  143            | all_or_any_predicate
  144            | existence_test

  145  comparison_predicate : scalar_exp COMPARISON scalar_exp
  146                       | scalar_exp COMPARISON subquery

  147  between_predicate : scalar_exp NOT BETWEEN scalar_exp AND scalar_exp
  148                    | scalar_exp BETWEEN scalar_exp AND scalar_exp

  149  like_predicate : scalar_exp NOT LIKE atom opt_escape
  150                 | scalar_exp LIKE atom opt_escape

  151  opt_escape :
  152             | ESCAPE atom

  153  test_for_null : column_ref IS NOT NULLX
  154                | column_ref IS NULLX

  155  in_predicate : scalar_exp NOT IN '(' subquery ')'
  156               | scalar_exp IN '(' subquery ')'
  157               | scalar_exp NOT IN '(' atom_commalist ')'
  158               | scalar_exp IN '(' atom_commalist ')'

  159  atom_commalist : atom
  160                 | atom_commalist ',' atom

  161  all_or_any_predicate : scalar_exp COMPARISON any_all_some subquery

  162  any_all_some : ANY
  163               | ALL
  164               | SOME

  165  existence_test : EXISTS subquery

  166  subquery : '(' SELECT opt_all_distinct selection table_exp ')'

  167  scalar_exp : scalar_exp '+' scalar_exp
  168             | scalar_exp '-' scalar_exp
  169             | scalar_exp '*' scalar_exp
  170             | scalar_exp '/' scalar_exp
  171             | '+' scalar_exp
  172             | '-' scalar_exp
  173             | atom
  174             | column_ref
  175             | function_ref
  176             | '(' scalar_exp ')'

  177  scalar_exp_commalist : scalar_exp
  178                       | scalar_exp_commalist ',' scalar_exp

  179  atom : parameter_ref
  180       | literal
  181       | USER

  182  parameter_ref : parameter
  183                | parameter parameter
  184                | parameter INDICATOR parameter

  185  function_ref : AMMSC '(' '*' ')'
  186               | AMMSC '(' DISTINCT column_ref ')'
  187               | AMMSC '(' ALL scalar_exp ')'
  188               | AMMSC '(' scalar_exp ')'

  189  literal : STRING
  190          | INTNUM
  191          | APPROXNUM

  192  table : NAME
  193        | NAME '.' NAME

  194  column_ref : NAME
  195             | NAME '.' NAME
  196             | NAME '.' NAME '.' NAME

  197  data_type : CHARACTER
  198            | CHARACTER '(' INTNUM ')'
  199            | NUMERIC
  200            | NUMERIC '(' INTNUM ')'
  201            | NUMERIC '(' INTNUM ',' INTNUM ')'
  202            | DECIMAL
  203            | DECIMAL '(' INTNUM ')'
  204            | DECIMAL '(' INTNUM ',' INTNUM ')'
  205            | INTEGER
  206            | SMALLINT
  207            | FLOAT
  208            | FLOAT '(' INTNUM ')'
  209            | REAL
  210            | DOUBLE PRECISION

  211  column : NAME

  212  cursor : NAME

  213  parameter : PARAMETER

  214  range_variable : NAME

  215  user : NAME

  216  sql : WHENEVER NOT FOUND when_action
  217      | WHENEVER SQLERROR when_action

  218  when_action : GOTO NAME
  219              | CONTINUE


##############################################################################
# States
##############################################################################

state 0
	$accept : . sql_list $end

	CLOSE  shift 1
	COMMIT  shift 2
	CREATE  shift 3
	DECLARE  shift 4
	DELETE  shift 5
	FETCH  shift 6
	INSERT  shift 7
	OPEN  shift 8
	ROLLBACK  shift 9
	SELECT  shift 10
	UPDATE  shift 11
	WHENEVER  shift 12

	sql_list  goto 13
	sql  goto 14
	schema  goto 15
	cursor_def  goto 16
	manipulative_statement  goto 17
	close_statement  goto 18
	commit_statement  goto 19
	delete_statement_positioned  goto 20
	delete_statement_searched  goto 21
	fetch_statement  goto 22
	insert_statement  goto 23
	open_statement  goto 24
	rollback_statement  goto 25
	select_statement  goto 26
	update_statement_positioned  goto 27
	update_statement_searched  goto 28


state 1
	close_statement : CLOSE . cursor

	NAME  shift 29

	cursor  goto 30


state 2
	commit_statement : COMMIT . WORK

	WORK  shift 31


state 3
	schema : CREATE . SCHEMA AUTHORIZATION user opt_schema_element_list

	SCHEMA  shift 32


state 4
	cursor_def : DECLARE . cursor CURSOR FOR query_exp opt_order_by_clause

	NAME  shift 29

	cursor  goto 33


state 5
	delete_statement_positioned : DELETE . FROM table WHERE CURRENT OF cursor
	delete_statement_searched : DELETE . FROM table opt_where_clause

	FROM  shift 34


state 6
	fetch_statement : FETCH . cursor INTO target_commalist

	NAME  shift 29

	cursor  goto 35


state 7
	insert_statement : INSERT . INTO table opt_column_commalist values_or_query_spec

	INTO  shift 36


state 8
	open_statement : OPEN . cursor

	NAME  shift 29

	cursor  goto 37


state 9
	rollback_statement : ROLLBACK . WORK

	WORK  shift 38


state 10
	select_statement : SELECT . opt_all_distinct selection INTO target_commalist table_exp
	opt_all_distinct : .  (96)

	ALL  shift 39
	DISTINCT  shift 40
	.  reduce 96

	opt_all_distinct  goto 41


state 11
	update_statement_positioned : UPDATE . table SET assignment_commalist WHERE CURRENT OF cursor
	update_statement_searched : UPDATE . table SET assignment_commalist opt_where_clause

	NAME  shift 42

	table  goto 43


state 12
	sql : WHENEVER . NOT FOUND when_action
	sql : WHENEVER . SQLERROR when_action

	NOT  shift 44
	SQLERROR  shift 45


state 13
	$accept : sql_list . $end  (0)
	sql_list : sql_list . sql ';'

	$end  accept
	CLOSE  shift 1
	COMMIT  shift 2
	CREATE  shift 3
	DECLARE  shift 4
	DELETE  shift 5
	FETCH  shift 6
	INSERT  shift 7
	OPEN  shift 8
	ROLLBACK  shift 9
	SELECT  shift 10
	UPDATE  shift 11
	WHENEVER  shift 12

	sql  goto 46
	schema  goto 15
	cursor_def  goto 16
	manipulative_statement  goto 17
	close_statement  goto 18
	commit_statement  goto 19
	delete_statement_positioned  goto 20
	delete_statement_searched  goto 21
	fetch_statement  goto 22
	insert_statement  goto 23
	open_statement  goto 24
	rollback_statement  goto 25
	select_statement  goto 26
	update_statement_positioned  goto 27
	update_statement_searched  goto 28


state 14
	sql_list : sql . ';'

	';'  shift 47


state 15
	sql : schema .  (3)

	.  reduce 3


state 16
	sql : cursor_def .  (58)

	.  reduce 58


state 17
	sql : manipulative_statement .  (69)

	.  reduce 69


state 18
	manipulative_statement : close_statement .  (70)

	.  reduce 70


state 19
	manipulative_statement : commit_statement .  (71)

	.  reduce 71


state 20
	manipulative_statement : delete_statement_positioned .  (72)

	.  reduce 72


state 21
	manipulative_statement : delete_statement_searched .  (73)

	.  reduce 73


state 22
	manipulative_statement : fetch_statement .  (74)

	.  reduce 74


state 23
	manipulative_statement : insert_statement .  (75)

	.  reduce 75


state 24
	manipulative_statement : open_statement .  (76)

	.  reduce 76


state 25
	manipulative_statement : rollback_statement .  (77)

	.  reduce 77


state 26
	manipulative_statement : select_statement .  (78)

	.  reduce 78


state 27
	manipulative_statement : update_statement_positioned .  (79)

	.  reduce 79


state 28
	manipulative_statement : update_statement_searched .  (80)

	.  reduce 80


state 29
	cursor : NAME .  (212)

	.  reduce 212


state 30
	close_statement : CLOSE cursor .  (81)

	.  reduce 81


state 31
	commit_statement : COMMIT WORK .  (82)

	.  reduce 82


state 32
	schema : CREATE SCHEMA . AUTHORIZATION user opt_schema_element_list

	AUTHORIZATION  shift 48


state 33
	cursor_def : DECLARE cursor . CURSOR FOR query_exp opt_order_by_clause

	CURSOR  shift 49


state 34
	delete_statement_positioned : DELETE FROM . table WHERE CURRENT OF cursor
	delete_statement_searched : DELETE FROM . table opt_where_clause

	NAME  shift 42

	table  goto 50


state 35
	fetch_statement : FETCH cursor . INTO target_commalist

	INTO  shift 51


state 36
	insert_statement : INSERT INTO . table opt_column_commalist values_or_query_spec

	NAME  shift 42

	table  goto 52


state 37
	open_statement : OPEN cursor .  (93)

	.  reduce 93


state 38
	rollback_statement : ROLLBACK WORK .  (94)

	.  reduce 94


state 39
	opt_all_distinct : ALL .  (97)

	.  reduce 97


state 40
	opt_all_distinct : DISTINCT .  (98)

	.  reduce 98


state 41
	select_statement : SELECT opt_all_distinct . selection INTO target_commalist table_exp

	'('  shift 53
	'*'  shift 54
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	selection  goto 67
	scalar_exp  goto 68
	parameter_ref  goto 69
	scalar_exp_commalist  goto 70
	function_ref  goto 71
	parameter  goto 72


state 42
	table : NAME .  (192)
	table : NAME . '.' NAME

	'.'  shift 73
	.  reduce 192


state 43
	update_statement_positioned : UPDATE table . SET assignment_commalist WHERE CURRENT OF cursor
	update_statement_searched : UPDATE table . SET assignment_commalist opt_where_clause

	SET  shift 74


state 44
	sql : WHENEVER NOT . FOUND when_action

	FOUND  shift 75


state 45
	sql : WHENEVER SQLERROR . when_action

	CONTINUE  shift 76
	GOTO  shift 77

	when_action  goto 78


state 46
	sql_list : sql_list sql . ';'

	';'  shift 79


state 47
	sql_list : sql ';' .  (1)

	.  reduce 1


state 48
	schema : CREATE SCHEMA AUTHORIZATION . user opt_schema_element_list

	NAME  shift 80

	user  goto 81


state 49
	cursor_def : DECLARE cursor CURSOR . FOR query_exp opt_order_by_clause

	FOR  shift 82


state 50
	delete_statement_positioned : DELETE FROM table . WHERE CURRENT OF cursor
	delete_statement_searched : DELETE FROM table . opt_where_clause
	opt_where_clause : .  (109)

	WHERE  shift 83
	.  reduce 109

	opt_where_clause  goto 84
	where_clause  goto 85


state 51
	fetch_statement : FETCH cursor INTO . target_commalist

	PARAMETER  shift 62

	target_commalist  goto 86
	target  goto 87
	parameter_ref  goto 88
	parameter  goto 72


state 52
	insert_statement : INSERT INTO table . opt_column_commalist values_or_query_spec
	opt_column_commalist : .  (39)

	'('  shift 89
	.  reduce 39

	opt_column_commalist  goto 90


state 53
	scalar_exp : '(' . scalar_exp ')'

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 91
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 54
	selection : '*' .  (118)

	.  reduce 118


state 55
	scalar_exp : '+' . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 92
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 56
	scalar_exp : '-' . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 93
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 57
	column_ref : NAME .  (194)
	column_ref : NAME . '.' NAME
	column_ref : NAME . '.' NAME '.' NAME

	'.'  shift 94
	.  reduce 194


state 58
	literal : STRING .  (189)

	.  reduce 189


state 59
	literal : INTNUM .  (190)

	.  reduce 190


state 60
	literal : APPROXNUM .  (191)

	.  reduce 191


state 61
	function_ref : AMMSC . '(' '*' ')'
	function_ref : AMMSC . '(' DISTINCT column_ref ')'
	function_ref : AMMSC . '(' ALL scalar_exp ')'
	function_ref : AMMSC . '(' scalar_exp ')'

	'('  shift 95


state 62
	parameter : PARAMETER .  (213)

	.  reduce 213


state 63
	atom : USER .  (181)

	.  reduce 181


state 64
	atom : literal .  (180)

	.  reduce 180


state 65
	scalar_exp : column_ref .  (174)

	.  reduce 174


state 66
	scalar_exp : atom .  (173)

	.  reduce 173


state 67
	select_statement : SELECT opt_all_distinct selection . INTO target_commalist table_exp

	INTO  shift 96


state 68
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp
	scalar_exp_commalist : scalar_exp .  (177)

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	.  reduce 177


state 69
	atom : parameter_ref .  (179)

	.  reduce 179


state 70
	selection : scalar_exp_commalist .  (117)
	scalar_exp_commalist : scalar_exp_commalist . ',' scalar_exp

	','  shift 101
	.  reduce 117


state 71
	scalar_exp : function_ref .  (175)

	.  reduce 175


state 72
	parameter_ref : parameter .  (182)
	parameter_ref : parameter . parameter
	parameter_ref : parameter . INDICATOR parameter

	INDICATOR  shift 102
	PARAMETER  shift 62
	.  reduce 182

	parameter  goto 103


state 73
	table : NAME '.' . NAME

	NAME  shift 104


state 74
	update_statement_positioned : UPDATE table SET . assignment_commalist WHERE CURRENT OF cursor
	update_statement_searched : UPDATE table SET . assignment_commalist opt_where_clause
	assignment_commalist : .  (100)

	NAME  shift 105
	.  reduce 100

	column  goto 106
	assignment_commalist  goto 107
	assignment  goto 108


state 75
	sql : WHENEVER NOT FOUND . when_action

	CONTINUE  shift 76
	GOTO  shift 77

	when_action  goto 109


state 76
	when_action : CONTINUE .  (219)

	.  reduce 219


state 77
	when_action : GOTO . NAME

	NAME  shift 110


state 78
	sql : WHENEVER SQLERROR when_action .  (217)

	.  reduce 217


state 79
	sql_list : sql_list sql ';' .  (2)

	.  reduce 2


state 80
	user : NAME .  (215)

	.  reduce 215


state 81
	schema : CREATE SCHEMA AUTHORIZATION user . opt_schema_element_list
	opt_schema_element_list : .  (5)

	CREATE  shift 111
	GRANT  shift 112
	.  reduce 5

	opt_schema_element_list  goto 113
	schema_element_list  goto 114
	schema_element  goto 115
	base_table_def  goto 116
	view_def  goto 117
	privilege_def  goto 118


state 82
	cursor_def : DECLARE cursor CURSOR FOR . query_exp opt_order_by_clause

	'('  shift 119
	SELECT  shift 120

	query_spec  goto 121
	query_exp  goto 122
	query_term  goto 123


state 83
	delete_statement_positioned : DELETE FROM table WHERE . CURRENT OF cursor
	where_clause : WHERE . search_condition
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	CURRENT  shift 126
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 128
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 84
	delete_statement_searched : DELETE FROM table opt_where_clause .  (84)

	.  reduce 84


state 85
	opt_where_clause : where_clause .  (110)

	.  reduce 110


state 86
	fetch_statement : FETCH cursor INTO target_commalist .  (85)
	target_commalist : target_commalist . ',' target

	','  shift 139
	.  reduce 85


state 87
	target_commalist : target .  (106)

	.  reduce 106


state 88
	target : parameter_ref .  (108)

	.  reduce 108


state 89
	opt_column_commalist : '(' . column_commalist ')'

	NAME  shift 105

	column  goto 140
	column_commalist  goto 141


state 90
	insert_statement : INSERT INTO table opt_column_commalist . values_or_query_spec

	SELECT  shift 120
	VALUES  shift 142

	query_spec  goto 143
	values_or_query_spec  goto 144


state 91
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp
	scalar_exp : '(' scalar_exp . ')'

	')'  shift 145
	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100


state 92
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp
	scalar_exp : '+' scalar_exp .  (171)

	.  reduce 171


state 93
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp
	scalar_exp : '-' scalar_exp .  (172)

	.  reduce 172


state 94
	column_ref : NAME '.' . NAME
	column_ref : NAME '.' . NAME '.' NAME

	NAME  shift 146


state 95
	function_ref : AMMSC '(' . '*' ')'
	function_ref : AMMSC '(' . DISTINCT column_ref ')'
	function_ref : AMMSC '(' . ALL scalar_exp ')'
	function_ref : AMMSC '(' . scalar_exp ')'

	'('  shift 53
	'*'  shift 147
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	ALL  shift 148
	AMMSC  shift 61
	DISTINCT  shift 149
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 150
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 96
	select_statement : SELECT opt_all_distinct selection INTO . target_commalist table_exp

	PARAMETER  shift 62

	target_commalist  goto 151
	target  goto 87
	parameter_ref  goto 88
	parameter  goto 72


state 97
	scalar_exp : scalar_exp '*' . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 152
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 98
	scalar_exp : scalar_exp '+' . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 153
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 99
	scalar_exp : scalar_exp '-' . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 154
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 100
	scalar_exp : scalar_exp '/' . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 155
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 101
	scalar_exp_commalist : scalar_exp_commalist ',' . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 156
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 102
	parameter_ref : parameter INDICATOR . parameter

	PARAMETER  shift 62

	parameter  goto 157


state 103
	parameter_ref : parameter parameter .  (183)

	.  reduce 183


state 104
	table : NAME '.' NAME .  (193)

	.  reduce 193


state 105
	column : NAME .  (211)

	.  reduce 211


state 106
	assignment : column . '=' scalar_exp
	assignment : column . '=' NULLX

	'='  shift 158


state 107
	update_statement_positioned : UPDATE table SET assignment_commalist . WHERE CURRENT OF cursor
	assignment_commalist : assignment_commalist . ',' assignment
	update_statement_searched : UPDATE table SET assignment_commalist . opt_where_clause
	opt_where_clause : .  (109)

	','  shift 159
	WHERE  shift 160
	.  reduce 109

	opt_where_clause  goto 161
	where_clause  goto 85


state 108
	assignment_commalist : assignment .  (101)

	.  reduce 101


state 109
	sql : WHENEVER NOT FOUND when_action .  (216)

	.  reduce 216


state 110
	when_action : GOTO NAME .  (218)

	.  reduce 218


state 111
	base_table_def : CREATE . TABLE table '(' base_table_element_commalist ')'
	view_def : CREATE . VIEW table opt_column_commalist AS query_spec opt_with_check_option

	TABLE  shift 162
	VIEW  shift 163


state 112
	privilege_def : GRANT . privileges ON table TO grantee_commalist opt_with_grant_option

	ALL  shift 164
	DELETE  shift 165
	INSERT  shift 166
	REFERENCES  shift 167
	SELECT  shift 168
	UPDATE  shift 169

	privileges  goto 170
	operation_commalist  goto 171
	operation  goto 172


state 113
	schema : CREATE SCHEMA AUTHORIZATION user opt_schema_element_list .  (4)

	.  reduce 4


state 114
	opt_schema_element_list : schema_element_list .  (6)
	schema_element_list : schema_element_list . schema_element

	CREATE  shift 111
	GRANT  shift 112
	.  reduce 6

	schema_element  goto 173
	base_table_def  goto 116
	view_def  goto 117
	privilege_def  goto 118


state 115
	schema_element_list : schema_element .  (7)

	.  reduce 7


state 116
	schema_element : base_table_def .  (9)

	.  reduce 9


state 117
	schema_element : view_def .  (10)

	.  reduce 10


state 118
	schema_element : privilege_def .  (11)

	.  reduce 11


state 119
	query_term : '(' . query_exp ')'

	'('  shift 119
	SELECT  shift 120

	query_spec  goto 121
	query_exp  goto 174
	query_term  goto 123


state 120
	query_spec : SELECT . opt_all_distinct selection table_exp
	opt_all_distinct : .  (96)

	ALL  shift 39
	DISTINCT  shift 40
	.  reduce 96

	opt_all_distinct  goto 175


state 121
	query_term : query_spec .  (114)

	.  reduce 114


state 122
	cursor_def : DECLARE cursor CURSOR FOR query_exp . opt_order_by_clause
	query_exp : query_exp . UNION query_term
	query_exp : query_exp . UNION ALL query_term
	opt_order_by_clause : .  (60)

	ORDER  shift 176
	UNION  shift 177
	.  reduce 60

	opt_order_by_clause  goto 178


state 123
	query_exp : query_term .  (111)

	.  reduce 111


state 124
	search_condition : '(' . search_condition ')'
	scalar_exp : '(' . scalar_exp ')'
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 179
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 180
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 125
	search_condition : NOT . search_condition
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 181
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 126
	delete_statement_positioned : DELETE FROM table WHERE CURRENT . OF cursor

	OF  shift 182


state 127
	existence_test : EXISTS . subquery

	'('  shift 183

	subquery  goto 184


state 128
	where_clause : WHERE search_condition .  (125)
	search_condition : search_condition . OR search_condition
	search_condition : search_condition . AND search_condition

	OR  shift 185
	AND  shift 186
	.  reduce 125


state 129
	test_for_null : column_ref . IS NOT NULLX
	test_for_null : column_ref . IS NULLX
	scalar_exp : column_ref .  (174)

	IS  shift 187
	.  reduce 174


state 130
	comparison_predicate : scalar_exp . COMPARISON scalar_exp
	comparison_predicate : scalar_exp . COMPARISON subquery
	between_predicate : scalar_exp . NOT BETWEEN scalar_exp AND scalar_exp
	between_predicate : scalar_exp . BETWEEN scalar_exp AND scalar_exp
	like_predicate : scalar_exp . NOT LIKE atom opt_escape
	like_predicate : scalar_exp . LIKE atom opt_escape
	in_predicate : scalar_exp . NOT IN '(' subquery ')'
	in_predicate : scalar_exp . IN '(' subquery ')'
	in_predicate : scalar_exp . NOT IN '(' atom_commalist ')'
	in_predicate : scalar_exp . IN '(' atom_commalist ')'
	all_or_any_predicate : scalar_exp . COMPARISON any_all_some subquery
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	NOT  shift 188
	COMPARISON  shift 189
	BETWEEN  shift 190
	IN  shift 191
	LIKE  shift 192


state 131
	search_condition : predicate .  (137)

	.  reduce 137


state 132
	predicate : comparison_predicate .  (138)

	.  reduce 138


state 133
	predicate : between_predicate .  (139)

	.  reduce 139


state 134
	predicate : like_predicate .  (140)

	.  reduce 140


state 135
	predicate : test_for_null .  (141)

	.  reduce 141


state 136
	predicate : in_predicate .  (142)

	.  reduce 142


state 137
	predicate : all_or_any_predicate .  (143)

	.  reduce 143


state 138
	predicate : existence_test .  (144)

	.  reduce 144


state 139
	target_commalist : target_commalist ',' . target

	PARAMETER  shift 62

	target  goto 193
	parameter_ref  goto 88
	parameter  goto 72


state 140
	column_commalist : column .  (34)

	.  reduce 34


state 141
	column_commalist : column_commalist . ',' column
	opt_column_commalist : '(' column_commalist . ')'

	')'  shift 194
	','  shift 195


state 142
	values_or_query_spec : VALUES . '(' insert_atom_commalist ')'

	'('  shift 196


state 143
	values_or_query_spec : query_spec .  (88)

	.  reduce 88


state 144
	insert_statement : INSERT INTO table opt_column_commalist values_or_query_spec .  (86)

	.  reduce 86


state 145
	scalar_exp : '(' scalar_exp ')' .  (176)

	.  reduce 176


state 146
	column_ref : NAME '.' NAME .  (195)
	column_ref : NAME '.' NAME . '.' NAME

	'.'  shift 197
	.  reduce 195


state 147
	function_ref : AMMSC '(' '*' . ')'

	')'  shift 198


state 148
	function_ref : AMMSC '(' ALL . scalar_exp ')'

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 199
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 149
	function_ref : AMMSC '(' DISTINCT . column_ref ')'

	NAME  shift 57

	column_ref  goto 200


state 150
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp
	function_ref : AMMSC '(' scalar_exp . ')'

	')'  shift 201
	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100


state 151
	select_statement : SELECT opt_all_distinct selection INTO target_commalist . table_exp
	target_commalist : target_commalist . ',' target

	','  shift 139
	FROM  shift 202

	table_exp  goto 203
	from_clause  goto 204


state 152
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp '*' scalar_exp .  (169)
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	.  reduce 169


state 153
	scalar_exp : scalar_exp '+' scalar_exp .  (167)
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'/'  shift 100
	.  reduce 167


state 154
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp '-' scalar_exp .  (168)
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'/'  shift 100
	.  reduce 168


state 155
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp '/' scalar_exp .  (170)
	scalar_exp : scalar_exp . '/' scalar_exp

	.  reduce 170


state 156
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp
	scalar_exp_commalist : scalar_exp_commalist ',' scalar_exp .  (178)

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	.  reduce 178


state 157
	parameter_ref : parameter INDICATOR parameter .  (184)

	.  reduce 184


state 158
	assignment : column '=' . scalar_exp
	assignment : column '=' . NULLX

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	NULLX  shift 205
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 206
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 159
	assignment_commalist : assignment_commalist ',' . assignment

	NAME  shift 105

	column  goto 106
	assignment  goto 207


state 160
	update_statement_positioned : UPDATE table SET assignment_commalist WHERE . CURRENT OF cursor
	where_clause : WHERE . search_condition
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	CURRENT  shift 208
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 128
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 161
	update_statement_searched : UPDATE table SET assignment_commalist opt_where_clause .  (105)

	.  reduce 105


state 162
	base_table_def : CREATE TABLE . table '(' base_table_element_commalist ')'

	NAME  shift 42

	table  goto 209


state 163
	view_def : CREATE VIEW . table opt_column_commalist AS query_spec opt_with_check_option

	NAME  shift 42

	table  goto 210


state 164
	privileges : ALL . PRIVILEGES
	privileges : ALL .  (45)

	PRIVILEGES  shift 211
	.  reduce 45


state 165
	operation : DELETE .  (51)

	.  reduce 51


state 166
	operation : INSERT .  (50)

	.  reduce 50


state 167
	operation : REFERENCES . opt_column_commalist
	opt_column_commalist : .  (39)

	'('  shift 89
	.  reduce 39

	opt_column_commalist  goto 212


state 168
	operation : SELECT .  (49)

	.  reduce 49


state 169
	operation : UPDATE . opt_column_commalist
	opt_column_commalist : .  (39)

	'('  shift 89
	.  reduce 39

	opt_column_commalist  goto 213


state 170
	privilege_def : GRANT privileges . ON table TO grantee_commalist opt_with_grant_option

	ON  shift 214


state 171
	privileges : operation_commalist .  (46)
	operation_commalist : operation_commalist . ',' operation

	','  shift 215
	.  reduce 46


state 172
	operation_commalist : operation .  (47)

	.  reduce 47


state 173
	schema_element_list : schema_element_list schema_element .  (8)

	.  reduce 8


state 174
	query_exp : query_exp . UNION query_term
	query_exp : query_exp . UNION ALL query_term
	query_term : '(' query_exp . ')'

	')'  shift 216
	UNION  shift 177


state 175
	query_spec : SELECT opt_all_distinct . selection table_exp

	'('  shift 53
	'*'  shift 54
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	selection  goto 217
	scalar_exp  goto 68
	parameter_ref  goto 69
	scalar_exp_commalist  goto 70
	function_ref  goto 71
	parameter  goto 72


state 176
	opt_order_by_clause : ORDER . BY ordering_spec_commalist

	BY  shift 218


state 177
	query_exp : query_exp UNION . query_term
	query_exp : query_exp UNION . ALL query_term

	'('  shift 119
	ALL  shift 219
	SELECT  shift 120

	query_spec  goto 121
	query_term  goto 220


state 178
	cursor_def : DECLARE cursor CURSOR FOR query_exp opt_order_by_clause .  (59)

	.  reduce 59


state 179
	search_condition : search_condition . OR search_condition
	search_condition : search_condition . AND search_condition
	search_condition : '(' search_condition . ')'

	')'  shift 221
	OR  shift 185
	AND  shift 186


state 180
	comparison_predicate : scalar_exp . COMPARISON scalar_exp
	comparison_predicate : scalar_exp . COMPARISON subquery
	between_predicate : scalar_exp . NOT BETWEEN scalar_exp AND scalar_exp
	between_predicate : scalar_exp . BETWEEN scalar_exp AND scalar_exp
	like_predicate : scalar_exp . NOT LIKE atom opt_escape
	like_predicate : scalar_exp . LIKE atom opt_escape
	in_predicate : scalar_exp . NOT IN '(' subquery ')'
	in_predicate : scalar_exp . IN '(' subquery ')'
	in_predicate : scalar_exp . NOT IN '(' atom_commalist ')'
	in_predicate : scalar_exp . IN '(' atom_commalist ')'
	all_or_any_predicate : scalar_exp . COMPARISON any_all_some subquery
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp
	scalar_exp : '(' scalar_exp . ')'

	')'  shift 145
	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	NOT  shift 188
	COMPARISON  shift 189
	BETWEEN  shift 190
	IN  shift 191
	LIKE  shift 192


state 181
	search_condition : search_condition . OR search_condition
	search_condition : search_condition . AND search_condition
	search_condition : NOT search_condition .  (135)

	.  reduce 135


state 182
	delete_statement_positioned : DELETE FROM table WHERE CURRENT OF . cursor

	NAME  shift 29

	cursor  goto 222


state 183
	subquery : '(' . SELECT opt_all_distinct selection table_exp ')'

	SELECT  shift 223


state 184
	existence_test : EXISTS subquery .  (165)

	.  reduce 165


state 185
	search_condition : search_condition OR . search_condition
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 224
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 186
	search_condition : search_condition AND . search_condition
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 225
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 187
	test_for_null : column_ref IS . NOT NULLX
	test_for_null : column_ref IS . NULLX

	NOT  shift 226
	NULLX  shift 227


state 188
	between_predicate : scalar_exp NOT . BETWEEN scalar_exp AND scalar_exp
	like_predicate : scalar_exp NOT . LIKE atom opt_escape
	in_predicate : scalar_exp NOT . IN '(' subquery ')'
	in_predicate : scalar_exp NOT . IN '(' atom_commalist ')'

	BETWEEN  shift 228
	IN  shift 229
	LIKE  shift 230


state 189
	comparison_predicate : scalar_exp COMPARISON . scalar_exp
	comparison_predicate : scalar_exp COMPARISON . subquery
	all_or_any_predicate : scalar_exp COMPARISON . any_all_some subquery

	'('  shift 231
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	ALL  shift 232
	AMMSC  shift 61
	ANY  shift 233
	PARAMETER  shift 62
	SOME  shift 234
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 235
	parameter_ref  goto 69
	subquery  goto 236
	any_all_some  goto 237
	function_ref  goto 71
	parameter  goto 72


state 190
	between_predicate : scalar_exp BETWEEN . scalar_exp AND scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 238
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 191
	in_predicate : scalar_exp IN . '(' subquery ')'
	in_predicate : scalar_exp IN . '(' atom_commalist ')'

	'('  shift 239


state 192
	like_predicate : scalar_exp LIKE . atom opt_escape

	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	atom  goto 240
	parameter_ref  goto 69
	parameter  goto 72


state 193
	target_commalist : target_commalist ',' target .  (107)

	.  reduce 107


state 194
	opt_column_commalist : '(' column_commalist ')' .  (40)

	.  reduce 40


state 195
	column_commalist : column_commalist ',' . column

	NAME  shift 105

	column  goto 241


state 196
	values_or_query_spec : VALUES '(' . insert_atom_commalist ')'

	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NULLX  shift 242
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	insert_atom_commalist  goto 243
	insert_atom  goto 244
	atom  goto 245
	parameter_ref  goto 69
	parameter  goto 72


state 197
	column_ref : NAME '.' NAME '.' . NAME

	NAME  shift 246


state 198
	function_ref : AMMSC '(' '*' ')' .  (185)

	.  reduce 185


state 199
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp
	function_ref : AMMSC '(' ALL scalar_exp . ')'

	')'  shift 247
	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100


state 200
	function_ref : AMMSC '(' DISTINCT column_ref . ')'

	')'  shift 248


state 201
	function_ref : AMMSC '(' scalar_exp ')' .  (188)

	.  reduce 188


state 202
	from_clause : FROM . table_ref_commalist

	NAME  shift 42

	table  goto 249
	table_ref_commalist  goto 250
	table_ref  goto 251


state 203
	select_statement : SELECT opt_all_distinct selection INTO target_commalist table_exp .  (95)

	.  reduce 95


state 204
	table_exp : from_clause . opt_where_clause opt_group_by_clause opt_having_clause
	opt_where_clause : .  (109)

	WHERE  shift 252
	.  reduce 109

	opt_where_clause  goto 253
	where_clause  goto 85


state 205
	assignment : column '=' NULLX .  (104)

	.  reduce 104


state 206
	assignment : column '=' scalar_exp .  (103)
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	.  reduce 103


state 207
	assignment_commalist : assignment_commalist ',' assignment .  (102)

	.  reduce 102


state 208
	update_statement_positioned : UPDATE table SET assignment_commalist WHERE CURRENT . OF cursor

	OF  shift 254


state 209
	base_table_def : CREATE TABLE table . '(' base_table_element_commalist ')'

	'('  shift 255


state 210
	view_def : CREATE VIEW table . opt_column_commalist AS query_spec opt_with_check_option
	opt_column_commalist : .  (39)

	'('  shift 89
	.  reduce 39

	opt_column_commalist  goto 256


state 211
	privileges : ALL PRIVILEGES .  (44)

	.  reduce 44


state 212
	operation : REFERENCES opt_column_commalist .  (53)

	.  reduce 53


state 213
	operation : UPDATE opt_column_commalist .  (52)

	.  reduce 52


state 214
	privilege_def : GRANT privileges ON . table TO grantee_commalist opt_with_grant_option

	NAME  shift 42

	table  goto 257


state 215
	operation_commalist : operation_commalist ',' . operation

	DELETE  shift 165
	INSERT  shift 166
	REFERENCES  shift 167
	SELECT  shift 168
	UPDATE  shift 169

	operation  goto 258


state 216
	query_term : '(' query_exp ')' .  (115)

	.  reduce 115


state 217
	query_spec : SELECT opt_all_distinct selection . table_exp

	FROM  shift 202

	table_exp  goto 259
	from_clause  goto 204


state 218
	opt_order_by_clause : ORDER BY . ordering_spec_commalist

	NAME  shift 57
	INTNUM  shift 260

	ordering_spec_commalist  goto 261
	ordering_spec  goto 262
	column_ref  goto 263


state 219
	query_exp : query_exp UNION ALL . query_term

	'('  shift 119
	SELECT  shift 120

	query_spec  goto 121
	query_term  goto 264


state 220
	query_exp : query_exp UNION query_term .  (112)

	.  reduce 112


state 221
	search_condition : '(' search_condition ')' .  (136)

	.  reduce 136


state 222
	delete_statement_positioned : DELETE FROM table WHERE CURRENT OF cursor .  (83)

	.  reduce 83


state 223
	subquery : '(' SELECT . opt_all_distinct selection table_exp ')'
	opt_all_distinct : .  (96)

	ALL  shift 39
	DISTINCT  shift 40
	.  reduce 96

	opt_all_distinct  goto 265


state 224
	search_condition : search_condition OR search_condition .  (133)
	search_condition : search_condition . OR search_condition
	search_condition : search_condition . AND search_condition

	AND  shift 186
	.  reduce 133


state 225
	search_condition : search_condition . OR search_condition
	search_condition : search_condition AND search_condition .  (134)
	search_condition : search_condition . AND search_condition

	.  reduce 134


state 226
	test_for_null : column_ref IS NOT . NULLX

	NULLX  shift 266


state 227
	test_for_null : column_ref IS NULLX .  (154)

	.  reduce 154


state 228
	between_predicate : scalar_exp NOT BETWEEN . scalar_exp AND scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 267
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 229
	in_predicate : scalar_exp NOT IN . '(' subquery ')'
	in_predicate : scalar_exp NOT IN . '(' atom_commalist ')'

	'('  shift 268


state 230
	like_predicate : scalar_exp NOT LIKE . atom opt_escape

	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	atom  goto 269
	parameter_ref  goto 69
	parameter  goto 72


state 231
	subquery : '(' . SELECT opt_all_distinct selection table_exp ')'
	scalar_exp : '(' . scalar_exp ')'

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	SELECT  shift 223
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 91
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 232
	any_all_some : ALL .  (163)

	.  reduce 163


state 233
	any_all_some : ANY .  (162)

	.  reduce 162


state 234
	any_all_some : SOME .  (164)

	.  reduce 164


state 235
	comparison_predicate : scalar_exp COMPARISON scalar_exp .  (145)
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	.  reduce 145


state 236
	comparison_predicate : scalar_exp COMPARISON subquery .  (146)

	.  reduce 146


state 237
	all_or_any_predicate : scalar_exp COMPARISON any_all_some . subquery

	'('  shift 183

	subquery  goto 270


state 238
	between_predicate : scalar_exp BETWEEN scalar_exp . AND scalar_exp
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	AND  shift 271


state 239
	in_predicate : scalar_exp IN '(' . subquery ')'
	in_predicate : scalar_exp IN '(' . atom_commalist ')'

	'('  shift 183
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	atom  goto 272
	parameter_ref  goto 69
	subquery  goto 273
	atom_commalist  goto 274
	parameter  goto 72


state 240
	like_predicate : scalar_exp LIKE atom . opt_escape
	opt_escape : .  (151)

	ESCAPE  shift 275
	.  reduce 151

	opt_escape  goto 276


state 241
	column_commalist : column_commalist ',' column .  (35)

	.  reduce 35


state 242
	insert_atom : NULLX .  (92)

	.  reduce 92


state 243
	values_or_query_spec : VALUES '(' insert_atom_commalist . ')'
	insert_atom_commalist : insert_atom_commalist . ',' insert_atom

	')'  shift 277
	','  shift 278


state 244
	insert_atom_commalist : insert_atom .  (89)

	.  reduce 89


state 245
	insert_atom : atom .  (91)

	.  reduce 91


state 246
	column_ref : NAME '.' NAME '.' NAME .  (196)

	.  reduce 196


state 247
	function_ref : AMMSC '(' ALL scalar_exp ')' .  (187)

	.  reduce 187


state 248
	function_ref : AMMSC '(' DISTINCT column_ref ')' .  (186)

	.  reduce 186


state 249
	table_ref : table .  (123)
	table_ref : table . range_variable

	NAME  shift 279
	.  reduce 123

	range_variable  goto 280


state 250
	from_clause : FROM table_ref_commalist .  (120)
	table_ref_commalist : table_ref_commalist . ',' table_ref

	','  shift 281
	.  reduce 120


state 251
	table_ref_commalist : table_ref .  (121)

	.  reduce 121


state 252
	where_clause : WHERE . search_condition
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 128
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 253
	table_exp : from_clause opt_where_clause . opt_group_by_clause opt_having_clause
	opt_group_by_clause : .  (126)

	GROUP  shift 282
	.  reduce 126

	opt_group_by_clause  goto 283


state 254
	update_statement_positioned : UPDATE table SET assignment_commalist WHERE CURRENT OF . cursor

	NAME  shift 29

	cursor  goto 284


state 255
	base_table_def : CREATE TABLE table '(' . base_table_element_commalist ')'

	NAME  shift 105
	CHECK  shift 285
	FOREIGN  shift 286
	PRIMARY  shift 287
	UNIQUE  shift 288

	base_table_element_commalist  goto 289
	base_table_element  goto 290
	column_def  goto 291
	table_constraint_def  goto 292
	column  goto 293


state 256
	view_def : CREATE VIEW table opt_column_commalist . AS query_spec opt_with_check_option

	AS  shift 294


state 257
	privilege_def : GRANT privileges ON table . TO grantee_commalist opt_with_grant_option

	TO  shift 295


state 258
	operation_commalist : operation_commalist ',' operation .  (48)

	.  reduce 48


state 259
	query_spec : SELECT opt_all_distinct selection table_exp .  (116)

	.  reduce 116


state 260
	ordering_spec : INTNUM . opt_asc_desc
	opt_asc_desc : .  (66)

	ASC  shift 296
	DESC  shift 297
	.  reduce 66

	opt_asc_desc  goto 298


state 261
	opt_order_by_clause : ORDER BY ordering_spec_commalist .  (61)
	ordering_spec_commalist : ordering_spec_commalist . ',' ordering_spec

	','  shift 299
	.  reduce 61


state 262
	ordering_spec_commalist : ordering_spec .  (62)

	.  reduce 62


state 263
	ordering_spec : column_ref . opt_asc_desc
	opt_asc_desc : .  (66)

	ASC  shift 296
	DESC  shift 297
	.  reduce 66

	opt_asc_desc  goto 300


state 264
	query_exp : query_exp UNION ALL query_term .  (113)

	.  reduce 113


state 265
	subquery : '(' SELECT opt_all_distinct . selection table_exp ')'

	'('  shift 53
	'*'  shift 54
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	selection  goto 301
	scalar_exp  goto 68
	parameter_ref  goto 69
	scalar_exp_commalist  goto 70
	function_ref  goto 71
	parameter  goto 72


state 266
	test_for_null : column_ref IS NOT NULLX .  (153)

	.  reduce 153


state 267
	between_predicate : scalar_exp NOT BETWEEN scalar_exp . AND scalar_exp
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	AND  shift 302


state 268
	in_predicate : scalar_exp NOT IN '(' . subquery ')'
	in_predicate : scalar_exp NOT IN '(' . atom_commalist ')'

	'('  shift 183
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	atom  goto 272
	parameter_ref  goto 69
	subquery  goto 303
	atom_commalist  goto 304
	parameter  goto 72


state 269
	like_predicate : scalar_exp NOT LIKE atom . opt_escape
	opt_escape : .  (151)

	ESCAPE  shift 275
	.  reduce 151

	opt_escape  goto 305


state 270
	all_or_any_predicate : scalar_exp COMPARISON any_all_some subquery .  (161)

	.  reduce 161


state 271
	between_predicate : scalar_exp BETWEEN scalar_exp AND . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 306
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 272
	atom_commalist : atom .  (159)

	.  reduce 159


state 273
	in_predicate : scalar_exp IN '(' subquery . ')'

	')'  shift 307


state 274
	in_predicate : scalar_exp IN '(' atom_commalist . ')'
	atom_commalist : atom_commalist . ',' atom

	')'  shift 308
	','  shift 309


state 275
	opt_escape : ESCAPE . atom

	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	atom  goto 310
	parameter_ref  goto 69
	parameter  goto 72


state 276
	like_predicate : scalar_exp LIKE atom opt_escape .  (150)

	.  reduce 150


state 277
	values_or_query_spec : VALUES '(' insert_atom_commalist ')' .  (87)

	.  reduce 87


state 278
	insert_atom_commalist : insert_atom_commalist ',' . insert_atom

	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NULLX  shift 242
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	insert_atom  goto 311
	atom  goto 245
	parameter_ref  goto 69
	parameter  goto 72


state 279
	range_variable : NAME .  (214)

	.  reduce 214


state 280
	table_ref : table range_variable .  (124)

	.  reduce 124


state 281
	table_ref_commalist : table_ref_commalist ',' . table_ref

	NAME  shift 42

	table  goto 249
	table_ref  goto 312


state 282
	opt_group_by_clause : GROUP . BY column_ref_commalist

	BY  shift 313


state 283
	table_exp : from_clause opt_where_clause opt_group_by_clause . opt_having_clause
	opt_having_clause : .  (130)

	HAVING  shift 314
	.  reduce 130

	opt_having_clause  goto 315


state 284
	update_statement_positioned : UPDATE table SET assignment_commalist WHERE CURRENT OF cursor .  (99)

	.  reduce 99


state 285
	table_constraint_def : CHECK . '(' search_condition ')'

	'('  shift 316


state 286
	table_constraint_def : FOREIGN . KEY '(' column_commalist ')' REFERENCES table
	table_constraint_def : FOREIGN . KEY '(' column_commalist ')' REFERENCES table '(' column_commalist ')'

	KEY  shift 317


state 287
	table_constraint_def : PRIMARY . KEY '(' column_commalist ')'

	KEY  shift 318


state 288
	table_constraint_def : UNIQUE . '(' column_commalist ')'

	'('  shift 319


state 289
	base_table_def : CREATE TABLE table '(' base_table_element_commalist . ')'
	base_table_element_commalist : base_table_element_commalist . ',' base_table_element

	')'  shift 320
	','  shift 321


state 290
	base_table_element_commalist : base_table_element .  (13)

	.  reduce 13


state 291
	base_table_element : column_def .  (15)

	.  reduce 15


state 292
	base_table_element : table_constraint_def .  (16)

	.  reduce 16


state 293
	column_def : column . data_type column_def_opt_list

	CHARACTER  shift 322
	DECIMAL  shift 323
	DOUBLE  shift 324
	FLOAT  shift 325
	INTEGER  shift 326
	NUMERIC  shift 327
	REAL  shift 328
	SMALLINT  shift 329

	data_type  goto 330


state 294
	view_def : CREATE VIEW table opt_column_commalist AS . query_spec opt_with_check_option

	SELECT  shift 120

	query_spec  goto 331


state 295
	privilege_def : GRANT privileges ON table TO . grantee_commalist opt_with_grant_option

	NAME  shift 80
	PUBLIC  shift 332

	user  goto 333
	grantee_commalist  goto 334
	grantee  goto 335


state 296
	opt_asc_desc : ASC .  (67)

	.  reduce 67


state 297
	opt_asc_desc : DESC .  (68)

	.  reduce 68


state 298
	ordering_spec : INTNUM opt_asc_desc .  (64)

	.  reduce 64


state 299
	ordering_spec_commalist : ordering_spec_commalist ',' . ordering_spec

	NAME  shift 57
	INTNUM  shift 260

	ordering_spec  goto 336
	column_ref  goto 263


state 300
	ordering_spec : column_ref opt_asc_desc .  (65)

	.  reduce 65


state 301
	subquery : '(' SELECT opt_all_distinct selection . table_exp ')'

	FROM  shift 202

	table_exp  goto 337
	from_clause  goto 204


state 302
	between_predicate : scalar_exp NOT BETWEEN scalar_exp AND . scalar_exp

	'('  shift 53
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	AMMSC  shift 61
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	column_ref  goto 65
	atom  goto 66
	scalar_exp  goto 338
	parameter_ref  goto 69
	function_ref  goto 71
	parameter  goto 72


state 303
	in_predicate : scalar_exp NOT IN '(' subquery . ')'

	')'  shift 339


state 304
	in_predicate : scalar_exp NOT IN '(' atom_commalist . ')'
	atom_commalist : atom_commalist . ',' atom

	')'  shift 340
	','  shift 309


state 305
	like_predicate : scalar_exp NOT LIKE atom opt_escape .  (149)

	.  reduce 149


state 306
	between_predicate : scalar_exp BETWEEN scalar_exp AND scalar_exp .  (148)
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	.  reduce 148


state 307
	in_predicate : scalar_exp IN '(' subquery ')' .  (156)

	.  reduce 156


state 308
	in_predicate : scalar_exp IN '(' atom_commalist ')' .  (158)

	.  reduce 158


state 309
	atom_commalist : atom_commalist ',' . atom

	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	PARAMETER  shift 62
	USER  shift 63

	literal  goto 64
	atom  goto 341
	parameter_ref  goto 69
	parameter  goto 72


state 310
	opt_escape : ESCAPE atom .  (152)

	.  reduce 152


state 311
	insert_atom_commalist : insert_atom_commalist ',' insert_atom .  (90)

	.  reduce 90


state 312
	table_ref_commalist : table_ref_commalist ',' table_ref .  (122)

	.  reduce 122


state 313
	opt_group_by_clause : GROUP BY . column_ref_commalist

	NAME  shift 57

	column_ref  goto 342
	column_ref_commalist  goto 343


state 314
	opt_having_clause : HAVING . search_condition
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 344
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 315
	table_exp : from_clause opt_where_clause opt_group_by_clause opt_having_clause .  (119)

	.  reduce 119


state 316
	table_constraint_def : CHECK '(' . search_condition ')'
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 345
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 317
	table_constraint_def : FOREIGN KEY . '(' column_commalist ')' REFERENCES table
	table_constraint_def : FOREIGN KEY . '(' column_commalist ')' REFERENCES table '(' column_commalist ')'

	'('  shift 346


state 318
	table_constraint_def : PRIMARY KEY . '(' column_commalist ')'

	'('  shift 347


state 319
	table_constraint_def : UNIQUE '(' . column_commalist ')'

	NAME  shift 105

	column  goto 140
	column_commalist  goto 348


state 320
	base_table_def : CREATE TABLE table '(' base_table_element_commalist ')' .  (12)

	.  reduce 12


state 321
	base_table_element_commalist : base_table_element_commalist ',' . base_table_element

	NAME  shift 105
	CHECK  shift 285
	FOREIGN  shift 286
	PRIMARY  shift 287
	UNIQUE  shift 288

	base_table_element  goto 349
	column_def  goto 291
	table_constraint_def  goto 292
	column  goto 293


state 322
	data_type : CHARACTER .  (197)
	data_type : CHARACTER . '(' INTNUM ')'

	'('  shift 350
	.  reduce 197


state 323
	data_type : DECIMAL .  (202)
	data_type : DECIMAL . '(' INTNUM ')'
	data_type : DECIMAL . '(' INTNUM ',' INTNUM ')'

	'('  shift 351
	.  reduce 202


state 324
	data_type : DOUBLE . PRECISION

	PRECISION  shift 352


state 325
	data_type : FLOAT .  (207)
	data_type : FLOAT . '(' INTNUM ')'

	'('  shift 353
	.  reduce 207


state 326
	data_type : INTEGER .  (205)

	.  reduce 205


state 327
	data_type : NUMERIC .  (199)
	data_type : NUMERIC . '(' INTNUM ')'
	data_type : NUMERIC . '(' INTNUM ',' INTNUM ')'

	'('  shift 354
	.  reduce 199


state 328
	data_type : REAL .  (209)

	.  reduce 209


state 329
	data_type : SMALLINT .  (206)

	.  reduce 206


state 330
	column_def : column data_type . column_def_opt_list
	column_def_opt_list : .  (18)

	.  reduce 18

	column_def_opt_list  goto 355


state 331
	view_def : CREATE VIEW table opt_column_commalist AS query_spec . opt_with_check_option
	opt_with_check_option : .  (37)

	WITH  shift 356
	.  reduce 37

	opt_with_check_option  goto 357


state 332
	grantee : PUBLIC .  (56)

	.  reduce 56


state 333
	grantee : user .  (57)

	.  reduce 57


state 334
	privilege_def : GRANT privileges ON table TO grantee_commalist . opt_with_grant_option
	grantee_commalist : grantee_commalist . ',' grantee
	opt_with_grant_option : .  (42)

	','  shift 358
	WITH  shift 359
	.  reduce 42

	opt_with_grant_option  goto 360


state 335
	grantee_commalist : grantee .  (54)

	.  reduce 54


state 336
	ordering_spec_commalist : ordering_spec_commalist ',' ordering_spec .  (63)

	.  reduce 63


state 337
	subquery : '(' SELECT opt_all_distinct selection table_exp . ')'

	')'  shift 361


state 338
	between_predicate : scalar_exp NOT BETWEEN scalar_exp AND scalar_exp .  (147)
	scalar_exp : scalar_exp . '+' scalar_exp
	scalar_exp : scalar_exp . '-' scalar_exp
	scalar_exp : scalar_exp . '*' scalar_exp
	scalar_exp : scalar_exp . '/' scalar_exp

	'*'  shift 97
	'+'  shift 98
	'-'  shift 99
	'/'  shift 100
	.  reduce 147


state 339
	in_predicate : scalar_exp NOT IN '(' subquery ')' .  (155)

	.  reduce 155


state 340
	in_predicate : scalar_exp NOT IN '(' atom_commalist ')' .  (157)

	.  reduce 157


state 341
	atom_commalist : atom_commalist ',' atom .  (160)

	.  reduce 160


state 342
	column_ref_commalist : column_ref .  (128)

	.  reduce 128


state 343
	opt_group_by_clause : GROUP BY column_ref_commalist .  (127)
	column_ref_commalist : column_ref_commalist . ',' column_ref

	','  shift 362
	.  reduce 127


state 344
	opt_having_clause : HAVING search_condition .  (131)
	search_condition : search_condition . OR search_condition
	search_condition : search_condition . AND search_condition

	OR  shift 185
	AND  shift 186
	.  reduce 131


state 345
	table_constraint_def : CHECK '(' search_condition . ')'
	search_condition : search_condition . OR search_condition
	search_condition : search_condition . AND search_condition

	')'  shift 363
	OR  shift 185
	AND  shift 186


state 346
	table_constraint_def : FOREIGN KEY '(' . column_commalist ')' REFERENCES table
	table_constraint_def : FOREIGN KEY '(' . column_commalist ')' REFERENCES table '(' column_commalist ')'

	NAME  shift 105

	column  goto 140
	column_commalist  goto 364


state 347
	table_constraint_def : PRIMARY KEY '(' . column_commalist ')'

	NAME  shift 105

	column  goto 140
	column_commalist  goto 365


state 348
	table_constraint_def : UNIQUE '(' column_commalist . ')'
	column_commalist : column_commalist . ',' column

	')'  shift 366
	','  shift 195


state 349
	base_table_element_commalist : base_table_element_commalist ',' base_table_element .  (14)

	.  reduce 14


state 350
	data_type : CHARACTER '(' . INTNUM ')'

	INTNUM  shift 367


state 351
	data_type : DECIMAL '(' . INTNUM ')'
	data_type : DECIMAL '(' . INTNUM ',' INTNUM ')'

	INTNUM  shift 368


state 352
	data_type : DOUBLE PRECISION .  (210)

	.  reduce 210


state 353
	data_type : FLOAT '(' . INTNUM ')'

	INTNUM  shift 369


state 354
	data_type : NUMERIC '(' . INTNUM ')'
	data_type : NUMERIC '(' . INTNUM ',' INTNUM ')'

	INTNUM  shift 370


state 355
	column_def : column data_type column_def_opt_list .  (17)
	column_def_opt_list : column_def_opt_list . column_def_opt

	NOT  shift 371
	CHECK  shift 372
	DEFAULT  shift 373
	REFERENCES  shift 374
	.  reduce 17

	column_def_opt  goto 375


state 356
	opt_with_check_option : WITH . CHECK OPTION

	CHECK  shift 376


state 357
	view_def : CREATE VIEW table opt_column_commalist AS query_spec opt_with_check_option .  (36)

	.  reduce 36


state 358
	grantee_commalist : grantee_commalist ',' . grantee

	NAME  shift 80
	PUBLIC  shift 332

	user  goto 333
	grantee  goto 377


state 359
	opt_with_grant_option : WITH . GRANT OPTION

	GRANT  shift 378


state 360
	privilege_def : GRANT privileges ON table TO grantee_commalist opt_with_grant_option .  (41)

	.  reduce 41


state 361
	subquery : '(' SELECT opt_all_distinct selection table_exp ')' .  (166)

	.  reduce 166


state 362
	column_ref_commalist : column_ref_commalist ',' . column_ref

	NAME  shift 57

	column_ref  goto 379


state 363
	table_constraint_def : CHECK '(' search_condition ')' .  (33)

	.  reduce 33


state 364
	table_constraint_def : FOREIGN KEY '(' column_commalist . ')' REFERENCES table
	table_constraint_def : FOREIGN KEY '(' column_commalist . ')' REFERENCES table '(' column_commalist ')'
	column_commalist : column_commalist . ',' column

	')'  shift 380
	','  shift 195


state 365
	table_constraint_def : PRIMARY KEY '(' column_commalist . ')'
	column_commalist : column_commalist . ',' column

	')'  shift 381
	','  shift 195


state 366
	table_constraint_def : UNIQUE '(' column_commalist ')' .  (29)

	.  reduce 29


state 367
	data_type : CHARACTER '(' INTNUM . ')'

	')'  shift 382


state 368
	data_type : DECIMAL '(' INTNUM . ')'
	data_type : DECIMAL '(' INTNUM . ',' INTNUM ')'

	')'  shift 383
	','  shift 384


state 369
	data_type : FLOAT '(' INTNUM . ')'

	')'  shift 385


state 370
	data_type : NUMERIC '(' INTNUM . ')'
	data_type : NUMERIC '(' INTNUM . ',' INTNUM ')'

	')'  shift 386
	','  shift 387


state 371
	column_def_opt : NOT . NULLX
	column_def_opt : NOT . NULLX UNIQUE
	column_def_opt : NOT . NULLX PRIMARY KEY

	NULLX  shift 388


state 372
	column_def_opt : CHECK . '(' search_condition ')'

	'('  shift 389


state 373
	column_def_opt : DEFAULT . literal
	column_def_opt : DEFAULT . NULLX
	column_def_opt : DEFAULT . USER

	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NULLX  shift 390
	USER  shift 391

	literal  goto 392


state 374
	column_def_opt : REFERENCES . table
	column_def_opt : REFERENCES . table '(' column_commalist ')'

	NAME  shift 42

	table  goto 393


state 375
	column_def_opt_list : column_def_opt_list column_def_opt .  (19)

	.  reduce 19


state 376
	opt_with_check_option : WITH CHECK . OPTION

	OPTION  shift 394


state 377
	grantee_commalist : grantee_commalist ',' grantee .  (55)

	.  reduce 55


state 378
	opt_with_grant_option : WITH GRANT . OPTION

	OPTION  shift 395


state 379
	column_ref_commalist : column_ref_commalist ',' column_ref .  (129)

	.  reduce 129


state 380
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' . REFERENCES table
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' . REFERENCES table '(' column_commalist ')'

	REFERENCES  shift 396


state 381
	table_constraint_def : PRIMARY KEY '(' column_commalist ')' .  (30)

	.  reduce 30


state 382
	data_type : CHARACTER '(' INTNUM ')' .  (198)

	.  reduce 198


state 383
	data_type : DECIMAL '(' INTNUM ')' .  (203)

	.  reduce 203


state 384
	data_type : DECIMAL '(' INTNUM ',' . INTNUM ')'

	INTNUM  shift 397


state 385
	data_type : FLOAT '(' INTNUM ')' .  (208)

	.  reduce 208


state 386
	data_type : NUMERIC '(' INTNUM ')' .  (200)

	.  reduce 200


state 387
	data_type : NUMERIC '(' INTNUM ',' . INTNUM ')'

	INTNUM  shift 398


state 388
	column_def_opt : NOT NULLX .  (20)
	column_def_opt : NOT NULLX . UNIQUE
	column_def_opt : NOT NULLX . PRIMARY KEY

	PRIMARY  shift 399
	UNIQUE  shift 400
	.  reduce 20


state 389
	column_def_opt : CHECK '(' . search_condition ')'
	search_condition : .  (132)

	'('  shift 124
	'+'  shift 55
	'-'  shift 56
	NAME  shift 57
	STRING  shift 58
	INTNUM  shift 59
	APPROXNUM  shift 60
	NOT  shift 125
	AMMSC  shift 61
	EXISTS  shift 127
	PARAMETER  shift 62
	USER  shift 63
	.  reduce 132

	literal  goto 64
	search_condition  goto 401
	column_ref  goto 129
	atom  goto 66
	scalar_exp  goto 130
	parameter_ref  goto 69
	predicate  goto 131
	comparison_predicate  goto 132
	between_predicate  goto 133
	like_predicate  goto 134
	test_for_null  goto 135
	in_predicate  goto 136
	all_or_any_predicate  goto 137
	existence_test  goto 138
	function_ref  goto 71
	parameter  goto 72


state 390
	column_def_opt : DEFAULT NULLX .  (24)

	.  reduce 24


state 391
	column_def_opt : DEFAULT USER .  (25)

	.  reduce 25


state 392
	column_def_opt : DEFAULT literal .  (23)

	.  reduce 23


state 393
	column_def_opt : REFERENCES table .  (27)
	column_def_opt : REFERENCES table . '(' column_commalist ')'

	'('  shift 402
	.  reduce 27


state 394
	opt_with_check_option : WITH CHECK OPTION .  (38)

	.  reduce 38


state 395
	opt_with_grant_option : WITH GRANT OPTION .  (43)

	.  reduce 43


state 396
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' REFERENCES . table
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' REFERENCES . table '(' column_commalist ')'

	NAME  shift 42

	table  goto 403


state 397
	data_type : DECIMAL '(' INTNUM ',' INTNUM . ')'

	')'  shift 404


state 398
	data_type : NUMERIC '(' INTNUM ',' INTNUM . ')'

	')'  shift 405


state 399
	column_def_opt : NOT NULLX PRIMARY . KEY

	KEY  shift 406


state 400
	column_def_opt : NOT NULLX UNIQUE .  (21)

	.  reduce 21


state 401
	column_def_opt : CHECK '(' search_condition . ')'
	search_condition : search_condition . OR search_condition
	search_condition : search_condition . AND search_condition

	')'  shift 407
	OR  shift 185
	AND  shift 186


state 402
	column_def_opt : REFERENCES table '(' . column_commalist ')'

	NAME  shift 105

	column  goto 140
	column_commalist  goto 408


state 403
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' REFERENCES table .  (31)
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' REFERENCES table . '(' column_commalist ')'

	'('  shift 409
	.  reduce 31


state 404
	data_type : DECIMAL '(' INTNUM ',' INTNUM ')' .  (204)

	.  reduce 204


state 405
	data_type : NUMERIC '(' INTNUM ',' INTNUM ')' .  (201)

	.  reduce 201


state 406
	column_def_opt : NOT NULLX PRIMARY KEY .  (22)

	.  reduce 22


state 407
	column_def_opt : CHECK '(' search_condition ')' .  (26)

	.  reduce 26


state 408
	column_def_opt : REFERENCES table '(' column_commalist . ')'
	column_commalist : column_commalist . ',' column

	')'  shift 410
	','  shift 195


state 409
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' REFERENCES table '(' . column_commalist ')'

	NAME  shift 105

	column  goto 140
	column_commalist  goto 411


state 410
	column_def_opt : REFERENCES table '(' column_commalist ')' .  (28)

	.  reduce 28


state 411
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' REFERENCES table '(' column_commalist . ')'
	column_commalist : column_commalist . ',' column

	')'  shift 412
	','  shift 195


state 412
	table_constraint_def : FOREIGN KEY '(' column_commalist ')' REFERENCES table '(' column_commalist ')' .  (32)

	.  reduce 32


##############################################################################
# Summary
##############################################################################

97 token(s), 91 nonterminal(s)
220 grammar rule(s), 413 state(s)


##############################################################################
# End of File
##############################################################################
