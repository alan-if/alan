grammar alan;

adventure : optional_options declaration+ start ;

optional_options :
                   | gensym0 (option)+
                       ;

gensym0 : 'OPTIONS' 
           | 'OPTION' 
               ;

option : ID '.' 
         | ID ID '.' 
         | ID INTEGER '.' 
             ;

declaration : messages 
              | class 
              | instance 
              | rule 
              | synonyms 
              | syntax 
              | verb 
              | addition 
              | event 
                  ;

attributes : ( attribute_definition '.' )+ ;

attribute_definition : ID '.'
                       | 'NOT' ID '.'
                       | ID optional_minus INTEGER '.'
                       | ID STRING '.'
                       | ID ID '.'
//                       | ID '{' optional_members '\}' '.'
                           ;

optional_members : 
                   | set_members 
                       ;

set_members : set_member ( ',' set_member )*
                  ;

set_member : ID 
             | optional_minus INTEGER 
             | STRING 
                 ;

synonyms : 'SYNONYMS' synonym_declaration+ 
               ;

synonym_declaration : id_list '=' ID '.' 
                          ;

messages : 'MESSAGE' message+ 
               ;

message : ID ':' statements 
              ;

syntax : 'SYNTAX' syntax_list 
             ;

syntax_list : syntax_item+
                  ;

syntax_item : ID '=' syntax_elements optional_syntax_restrictions 
	;

syntax_elements : syntax_element+
	;

syntax_element : ID 
                 | '(' ID ')' optional_indicators 
                     ;

optional_indicators : indicator*
                          ;

indicator : '*' 
            | '!' 
                ;

syntax_restriction_clauses : syntax_restriction ( 'AND' syntax_restriction )*
                                 ;

syntax_restriction : ID 'ISA' restriction_class 'ELSE' statements 
                         ;

restriction_class : ID 
                    | 'CONTAINER' 
                        ;

optional_syntax_restrictions : '.' 
                               | 'WHERE' syntax_restriction_clauses 
                                   ;

verb : verb_header verb_body verb_tail 
           ;

verb_header : 'VERB' id_list 
                  ;

verb_body : simple_verb_body 
            | verb_alternatives 
                ;

verb_alternatives : verb_alternative+
                        ;

verb_alternative : 'WHEN' ID simple_verb_body 
                       ;

simple_verb_body : optional_checks? optional_does 
                       ;

verb_tail : 'END' 'VERB' optional_id '.' 
                ;

optional_checks : 'CHECK' ( statements | check_list )
                      ;

check_list : check ( 'AND' check )*
                 ;

check : expression 'ELSE' statements 
            ;

optional_does : 
                | does 
                    ;

does : 'DOES' optional_qual statements 
           ;

class : 'EVERY' ID optional_heritage properties class_tail 
            ;

class_tail : 'END' 'EVERY' optional_id optional_period
                 ;

addition : 'ADD' 'TO' genSym2 ID optional_heritage properties 
                  add_tail 
               ;

genSym2 : 
           | 'EVERY' 
               ;

add_tail : 'END' 'ADD' genSym3 optional_id optional_period
               ;

genSym3 : 
           | 'TO' 
               ;

instance : 'THE' ID optional_heritage properties  instance_tail 
               ;

instance_tail : 'END' 'THE' optional_id optional_period
                    ;

optional_heritage : 
                    | heritage 
                        ;

heritage : 'ISA' ID optional_period 
               ;

properties : property*
                 ;

property : where optional_period 
           | container_properties 
           | description 
           | optional_indefinite article_or_form 
           | name 
           | entered 
           | mentioned 
           | 'DEFINITE' article_or_form 
           | is attributes 
           | script 
           | exit 
           | verb 
               ;

optional_indefinite : 
           | 'INDEFINITE' 
               ;

exit : 'EXIT' id_list 'TO' ID optional_exit_body '.' 
           ;

optional_exit_body : 
                     | optional_checks optional_does 'END' 'EXIT' 
                            optional_id 
                         ;

optional_attributes : ( is attributes )*
                          ;

is : 'IS' 
     | 'ARE' 
     | 'HAS' 
         ;

optional_description : 
                       | description 
                           ;

description : 'DESCRIPTION' optional_checks optional_does 
              | 'DESCRIPTION' statements 
                  ;

article_or_form : article 
                  | form 
                      ;

article : 'ARTICLE' 
          | 'ARTICLE' statements 
              ;

form : 'FORM' 
       | 'FORM' statements 
           ;

entered : 'ENTERED' statements 
              ;

mentioned : 'MENTIONED' statements 
                ;

optional_names : name*
	;

name : 'NAME' ids optional_full_stop 
           ;

optional_full_stop : 
                     | '.' 
                         ;

container_properties : optional_with optional_opaque 'CONTAINER' 
                              container_body 
                           ;

optional_with : 
           | 'WITH' 
               ;

optional_opaque : 
                    | 'OPAQUE' 
                        ;

container_body : optional_taking optional_limits optional_header 
                        optional_empty optional_extract 
                 | '.' 
                     ;

optional_taking : 
                  | 'TAKING' ID '.' 
                      ;

optional_limits : 
                  | 'LIMITS' limits 
                      ;

limits : limit+
             ;

limit : limit_attribute else_or_then statements 
            ;

else_or_then : 'ELSE' 
               | 'THEN' 
                   ;

limit_attribute : attribute_definition 
                  | 'COUNT' INTEGER 
                      ;

optional_header : 
                  | 'HEADER' statements 
                      ;

optional_empty : 
                 | 'ELSE' statements 
                     ;

optional_extract : 
                   | 'EXTRACT' optional_checks optional_does 
                   | 'EXTRACT' statements 
                       ;

event : event_header statements event_tail 
            ;

event_header : 'EVENT' ID 
                   ;

event_tail : 'END' 'EVENT' optional_id '.' 
                 ;

script : 'SCRIPT' ID optional_period optional_description step_list 
             ;

optional_period : 
            | '.' 
                ;

step_list : step+
                ;

step : 'STEP' statements 
       | 'STEP' 'AFTER' INTEGER statements 
       | 'STEP' 'WAIT' 'UNTIL' expression statements 
           ;

rule : 'WHEN' expression then statements optional_end_when 
           ;

then : '=>' 
       | 'THEN' 
           ;

optional_end_when : 
                    | 'END' 'WHEN' optional_period 
                        ;

start : 'START' where '.' optional_statements 
            ;

optional_statements : 
                      | statements 
                          ;

statements : statement+
                 ;

statement : output_statement 
            | special_statement 
            | manipulation_statement 
            | actor_statement 
            | event_statement 
            | assignment_statement 
            | repetition_statement 
            | conditional_statement 
                ;

output_statement : STRING 
                   | 'DESCRIBE' what '.' 
                   | 'SAY' say_form expression '.' 
                   | 'LIST' what '.' 
                   | 'SHOW' ID '.' 
                       ;

say_form : 
           | 'THE' 
           | 'AN' 
               ;

manipulation_statement : 'EMPTY' what optional_where '.' 
                         | 'LOCATE' what where '.' 
                         | 'INCLUDE' primary 'IN' what '.' 
                         | 'REMOVE' primary 'FROM' what '.' 
                             ;

event_statement : 'CANCEL' ID '.' 
                  | 'SCHEDULE' ID optional_where 'AFTER' expression 
                         '.' 
                      ;

assignment_statement : 'MAKE' what something '.' 
                       | 'STRIP' optional_first_or_last 
                              optional_expression 
                              optional_word_or_character 'FROM' expression 
                              optional_into '.' 
                       | 'INCREASE' attribute_reference 
                              optional_by_clause '.' 
                       | 'DECREASE' attribute_reference 
                              optional_by_clause '.' 
                       | 'SET' attribute_reference 'TO' expression 
                              '.' 
                           ;

optional_by_clause : 
                     | 'BY' expression 
                         ;

optional_first_or_last : 
                         | 'FIRST' 
                         | 'LAST' 
                             ;

optional_word_or_character : 
                             | 'WORD' 
                             | 'WORDS' 
                             | 'CHARACTER' 
                             | 'CHARACTERS' 
                                 ;

optional_into : 
                | 'INTO' expression 
                    ;

conditional_statement : if_statement 
                        | depending_statement 
                            ;

if_statement : 'IF' expression 'THEN' statements 
                      optional_elsif_list optional_else_part 'END' 'IF' 
                      '.' 
                   ;

optional_elsif_list
	:	( 'ELSIF' expression 'THEN' statements )*
	;

optional_else_part : 
                     | 'ELSE' statements 
                         ;

depending_statement : 'DEPENDING' 'ON' primary depend_cases 'END' 
                             genSym12 '.' 
                          ;

genSym12 : 'DEPEND' 
            | 'DEPENDING' 
                ;

depend_cases : depend_case+
                   ;

depend_case : right_hand_side ':' statements 
              | 'ELSE' statements
                  ;

repetition_statement : for_each ID filters? 'DO' 
                              statements 'END' for_each optional_period 
                           ;

for_each : 'FOR' | 'EACH' | 'FOR' 'EACH'
               ;

actor_statement : 'STOP' what '.' 
                  | 'USE' 'SCRIPT' ID optional_for_actor '.' 
                      ;

optional_for_actor : 
                     | 'FOR' what 
                         ;

special_statement : 'QUIT' '.' 
                    | 'LOOK' '.' 
                    | 'SAVE' '.' 
                    | 'RESTORE' '.' 
                    | 'RESTART' '.' 
                    | 'SCORE' optional_INTEGER '.' 
                    | 'VISITS' INTEGER '.' 
                    | 'SYSTEM' STRING '.' 
                        ;

optional_expression : 
                      | expression 
                          ;

expression : term ( 'OR' term )*
                 ;

term : factor ( 'AND' factor )*
           ;

factor : arithmetic ( optional_not factor_operation )*
             ;
             
factor_operation 
	: optional_not where 
        | optional_not relop arithmetic 
        | optional_not 'CONTAINS' arithmetic 
        | optional_not 'BETWEEN' arithmetic 'AND' arithmetic
        ;

arithmetic
	: primary 
        | aggregate filters 
        | primary 'ISA' ID 
        | primary is something 
        | primary binop arithmetic
        ;

filters : filter ( ',' filter )*
              ;

filter : optional_not where 
                    | optional_not 'ISA' ID 
                    | is something 
                        ;

right_hand_side : filter expression 
                  | optional_not relop primary 
                  | optional_not 'CONTAINS' factor 
                  | optional_not 'BETWEEN' factor 'AND' factor 
                      ;

primary : optional_minus INTEGER 
          | STRING 
          | what 
          | 'SCORE' 
          | '(' expression ')' 
          | 'RANDOM' primary 'TO' primary 
              ;

aggregate : 'COUNT' 
            | aggregator 'OF' ID 
                ;

aggregator : 'MAX' 
             | 'MIN' 
             | 'SUM' 
                 ;

something : optional_not ID 
                ;

what : simple_what 
       | attribute_reference 
           ;

simple_what : ID 
              | 'THIS' 
              | 'CURRENT' 'ACTOR' 
              | 'CURRENT' 'LOCATION' 
                  ;

attribute_reference : ID 'OF' what 
                      //| what ':' ID 
                          ;

optional_where : 
                 | where 
                     ;

where : 'HERE' 
        | 'NEARBY' 
        | 'AT' what 
        | 'IN' what 
            ;

binop : '+' 
        | '-' 
        | '*' 
        | '/' 
            ;

relop : '<>' 
        | '=' 
        | '==' 
        | '>=' 
        | '<=' 
        | '>' 
        | '<' 
            ;

optional_qual : 
                | 'BEFORE' 
                | 'AFTER' 
                | 'ONLY' 
                    ;

optional_not : 
               | 'NOT' 
                   ;

optional_id : 
              | ID 
                  ;

ids : ID+
          ;

id_list : ID ( ',' ID )*
              ;

optional_INTEGER : 
                   | INTEGER 
                       ;

optional_minus : 
                 | '-' 
                     ;

ID : IDENTIFIER 
   | 'LOCATION' 
   | 'ACTOR' 
   | 'OPAQUE' 
   | 'ON' 
   | 'OF' 
   | 'FIRST' 
   | 'INTO' 
   | 'TAKING' 
       ;


INTEGER :
	|	DIGIT+
    ;

IDENTIFIER
    	: LETTER (LETTER | DIGIT | '_')*
    	//| '\'' ([^\'\n]!'\'''\'')* ('\'' ! '\n')
    	;

fragment DIGIT : '0'..'9';

fragment LETTER	: 'a'..'z' | 'A'..'Z' ; //\xe0-\xf6\xf8-\xfe] ;

COMMENT : '--' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;} ; 

STRING 	: '"' ~'"' '"' ;
