grammar alan;

options {
    language = Java;
}

@header {
  package se.alanif.grammar;
}

@lexer::header {
  package se.alanif.grammar;
}

adventure
	:	game_options? declaration* start ;

game_options
	:	(('OPTION') | 'OPTIONS') option+
	;

option	:	ID '.' 
	|	ID ID '.'
	|	ID INTEGER '.' 
	;

declaration
	:	messages 
	|	class_declaration 
	|	instance_declaration
	|	addition 
	|	rule 
	|	synonyms 
	|	syntax 
	|	verb 
	|	event 
	;


attribute_definition
	:	'NOT'? ID '.'
	|	ID '-'? INTEGER '.'
	|	ID STRING '.'
	|	ID ID '.'
	|	ID '{' set_members? '}' '.'
	;


set_members
	:	set_member ( ',' set_member )*
	;

set_member
	:	ID 
	|	'-'? INTEGER 
	|	STRING 
	;

synonyms
	:	'SYNONYMS' synonym_declaration+ 
	;

synonym_declaration
	:	id_list '=' ID '.' 
	;

messages
	:	'MESSAGE' message+ 
	;

message :	ID ':' statement+ 
	;

syntax	:	'SYNTAX' syntax_item+ 
             ;

syntax_item
	:	ID '=' syntax_element+ ('.' | 'WHERE' syntax_restrictions) 
	;

syntax_element
	:	ID 
	|	'(' ID ')' indicator*
	;

indicator
	:	('*') | '!'
	;

syntax_restrictions
	:	syntax_restriction ( 'AND' syntax_restriction )*
	;

syntax_restriction
	:	ID 'ISA' restriction_class 'ELSE' statement+ 
	;

restriction_class
	:	(ID) 
	|	'CONTAINER' 
	;

verb	:	'VERB' id_list verb_body 'END' 'VERB' ID? '.' 
	;

verb_body
	:	simple_verb_body 
	|	verb_alternative+
	;

simple_verb_body
	:	checks? does? 
	;


checks	:	'CHECK' check_list
	|	'CHECK' statement
	;

check_list
	:	check ( 'AND' check )*
	;

check	:	expression 'ELSE' statement+ 
	;

does	:	'DOES' qualifier? statement+ 
	;

verb_alternative
	:	'WHEN' ID simple_verb_body 
	;

class_declaration
	:	'EVERY' ID heritage? property* 'END' 'EVERY' ID? '.'
	;


addition
	:	'ADD' 'TO' 'EVERY'? ID heritage? property* 'END' 'ADD' 'TO'? ID? '.'
	;


instance_declaration
	:	'THE' ID heritage? property* 'END' 'THE' ID? '.' 
	;

heritage
	:	'ISA' ID '.'?
	;

property
	:	where '.'?
	|	is attribute_definition+
	|	name 
	|	description 
	|	entered 
	|	mentioned 
	|	'INDEFINITE'? article_or_form 
	|	'DEFINITE' article_or_form 
	|	container_properties 
	|	script 
	|	exit 
	|	verb 
	;


exit	:	'EXIT' id_list 'TO' ID (checks? does? 'END' 'EXIT' ID?)? '.' 
	;

is	:	('IS') | 'ARE' | 'HAS' 
	;

description
	:	'DESCRIPTION' checks? does?
	|	'DESCRIPTION' statement+
	;

article_or_form
	:	article 
	|	form 
	;

article	:	'ARTICLE' 
	|	'ARTICLE' statement+ 
	;

form	:	'FORM' 
	|	'FORM' statement+
	;

entered	:	'ENTERED' statement+ 
	;

mentioned
	:	'MENTIONED' statement+ 
	;

name	:	'NAME' ID+ '.'? 
	;

container_properties
	:	'WITH'? 'OPAQUE'? 'CONTAINER' ( container_body | '.' )
	;

container_body
	:	taking? limits? header? empty? extract? 
	;

taking	:	'TAKING' ID '.' 
	;

limits	:	'LIMITS' limit+ 
	;

limit	:	limit_attribute (('ELSE') | 'THEN') statement+
	;


limit_attribute
	:	attribute_definition 
	|	'COUNT' INTEGER
	;

header	:	'HEADER' statement+
	;

empty	:	('ELSE'|'EMPTY') statement+ 
	;

extract	:	'EXTRACT' checks? does? 
	|	'EXTRACT' statement+
	;

event	:	'EVENT' ID statement+ 'END' 'EVENT' ID? '.'
	;

script	:	'SCRIPT' ID '.'? description? step+ 
	;

step	:	'STEP' statement+ 
	|	'STEP' 'AFTER' INTEGER statement+ 
	|	'STEP' 'WAIT' 'UNTIL' expression statement+ 
	;

rule	:	'WHEN' expression (('=>') | 'THEN') statement+ ( 'END' 'WHEN' '.' )?
	;



start	:	'START' where '.' statement* 
	;

statement
	:	output_statement 
	|	special_statement 
	|	manipulation_statement 
	|	actor_statement 
	|	event_statement 
	|	assignment_statement 
	|	repetition_statement 
	|	conditional_statement 
	;

output_statement
	:	STRING 
	|	'DESCRIBE' what '.' 
	|	'SAY' (('THE') | 'AN')? expression '.' 
	|	'LIST' what '.' 
	|	'SHOW' ID '.' 
	;


manipulation_statement
	:	'EMPTY' what where? '.' 
	|	'LOCATE' what where '.' 
	|	'INCLUDE' primary 'IN' what '.' 
	|	'REMOVE' primary 'FROM' what '.' 
	;

event_statement
	:	'CANCEL' ID '.' 
	|	'SCHEDULE' ID where? 'AFTER' expression '.' 
	;

assignment_statement
	:	'MAKE' what something '.' 
	|	'STRIP' (('FIRST') | 'LAST' )? expression? (('WORD') | 'WORDS' | 'CHARACTER' | 'CHARACTERS' )? 'FROM' expression ( 'INTO' expression )? '.' 
	|	'INCREASE' attribute_reference ( 'BY' expression )? '.' 
	|	'DECREASE' attribute_reference ( 'BY' expression )? '.' 
	|	'SET' attribute_reference 'TO' expression '.' 
	;

conditional_statement
	:	if_statement 
	|	depending_statement 
	;

if_statement
	:	'IF' expression 'THEN' statement+ elsif_part* else_part? 'END' 'IF' '.' 
	;

elsif_part
	:	'ELSIF' expression 'THEN' statement+
	;

else_part
	:	'ELSE' statement+
	;

depending_statement
	:	'DEPENDING' 'ON' primary depend_case+ 'END' (('DEPEND') | 'DEPENDING') '.' 
	;


depend_case
	:	right_hand_side ':' statement+ 
	|	'ELSE' statement+
	;

repetition_statement
	:	for_each ID filters? 'DO' statement+ 'END' for_each '.'?
	;

for_each:	'FOR' 'EACH' //| 'FOR' | 'EACH'
	;

actor_statement
	:	'STOP' what '.' 
	|	'USE' 'SCRIPT' ID ( 'FOR' what )? '.' 
	;


special_statement
	:	'QUIT' '.' 
	|	'LOOK' '.' 
	|	'SAVE' '.' 
	|	'RESTORE' '.' 
	|	'RESTART' '.' 
	|	'SCORE' INTEGER? '.' 
	|	'VISITS' INTEGER '.' 
	|	'SYSTEM' STRING '.' 
	;

expression
	:	term ( 'OR' term )*
	;

term	:	factor ( 'AND' factor )*
	;

factor	:	arithmetic factor_operation?
	;
             
factor_operation 
	:	'NOT'? where 
	|	'NOT'? relop arithmetic 
	|	'NOT'? 'CONTAINS' arithmetic 
	|	'NOT'? 'BETWEEN' arithmetic 'AND' arithmetic
	;

arithmetic
	:	aggregate filters 
	|	primary logic_operation?
	;

logic_operation
	:	'ISA' ID 
	|	is something 
	|	binop arithmetic
	;
        
filters : filter ( ',' filter )*
        ;

filter 	:	'NOT'? where 
	|	'NOT'? 'ISA' ID 
	|	is something 
	;

right_hand_side
	:	filter
	|	'NOT'? relop primary 
	|	'NOT'? 'CONTAINS' factor 
	|	'NOT'? 'BETWEEN' factor 'AND' factor 
	;

primary	:	literal 
	|	what 
	|	'SCORE' 
	|	'(' expression ')' 
	|	'RANDOM' primary 'TO' primary 
	;

literal	:	'-'? INTEGER
	|	STRING
	;
	
aggregate
	:	'COUNT' 
	|	aggregator 'OF' ID 
	;

aggregator
	:	'MAX' 
	|	'MIN' 
	|	'SUM' 
	;

something
	:	'NOT'? ID 
	;

what	:	simple_what 
	|	attribute_reference 
	;

simple_what
	:	ID
	|	'THIS' 
	|	'CURRENT' 'ACTOR' 
	|	'CURRENT' 'LOCATION' 
	;

attribute_reference
	:	(ID 'OF')+  simple_what
	|	simple_what (':' ID)+
	;
	
where	:	'HERE' 
	|	'NEARBY' 
	|	'AT' what 
	|	'IN' what 
	;

binop	:	'+' 
	|	'-' 
	|	'*' 
	|	'/' 
	;

relop	:	'<>' 
	|	'=' 
	|	'==' 
	|	'>=' 
	|	'<=' 
	|	'>' 
	|	'<' 
	;

qualifier
	:	( 'BEFORE') | 'AFTER' | 'ONLY'
	;

id_list
	: ID ( ',' ID )*
        ;

ID
	:	IDENTIFIER ;

//ID : 'LOCATION' 
//   | 'ACTOR' 
//   | 'OPAQUE' 
//   | 'ON' 
//   | 'OF' 
//   | 'FIRST' 
//   | 'INTO' 
//   | 'TAKING'
//   | IDENTIFIER 
//       ;


INTEGER
	:	DIGIT+
	;

fragment IDENTIFIER
	:	LETTER (LETTER | DIGIT | '_')*
	//|	'\'' ([^\'\n]!'\'''\'')* ('\'' ! '\n')
	;

fragment DIGIT
	:	'0'..'9';

fragment LETTER
	:	'a'..'z' | 'A'..'Z' ; //\xe0-\xf6\xf8-\xfe] ;

COMMENT
	:	'--' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;} ; 

STRING 
	:	'"' ~'"' '"' ;

WHITESPACE
	:	(' ' | '\r' | '\t' | '\u000C' | '\n') {$channel=HIDDEN;} 
	;