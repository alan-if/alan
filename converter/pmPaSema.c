/*----------------------------------------------------------------------*\

	pmPaSema.c

	ParserMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


#include "srcp.h"
#include "smScan.h"
#include "token.h"
#include "lmList.h"
#include "a2a3.h"

/* END %%IMPORT */

/* System dependencies
 * -------------------
 */

/* These datatypes should be defined to be unsigned integers of length 1, 2
 * and 4 bytes respectively.
 */
typedef unsigned char UByte1;
typedef unsigned short UByte2;
typedef unsigned int UByte4;

/* Token and Srcp definition */
#include "alanCommon.h"

/* Attribute stacks *\
\* ---------------- */
/* %%ATTRIBUTES */
/* The semantic attributes for grammar symbols */
typedef struct pmGrammar {
    int val;
    char *string;
} pmGrammar;

/* END %%ATTRIBUTES */

extern short pmStkP;
extern Token pmSySt[];
extern pmGrammar pmSeSt[];

/* %%DECLARATIONS - User data and routines */

#line 37 "alan.pmk"

static int val(char str[])
{
  int temp;
  sscanf(str, "%d", &temp);
  return(temp);
}

static int level = 0;
static void newline()
{
  int i;
  fprintf(outFile, "\n");
  if (level < 0) printf("Level = %d", level);
  for (i = 0; i < level; i++)
    fprintf(outFile, "  ");
}

static void indent()
{
  level++;
}

static void outdent()
{
  level--;
}


static char *(v3Keywords[]) = {
  "look", "quit", "in", "say", "locate", "make", "with", "empty",
  "container", "wait", "score", "save", "restore", "restart", "at",
  "to", "show", "use", "empty", "for", "each", "set", "start", "the", NULL};


static void idPrint(char id[])
{
  int wordIndex = 0;

  while (v3Keywords[wordIndex] != NULL && strcmp(id, v3Keywords[wordIndex]) != 0)
    wordIndex++;
  if (v3Keywords[wordIndex] != NULL) fprintf(outFile, "'%s'", id);
  else fprintf(outFile, id);
}

static void stringPrint(char string[])
{
  int ch, i;
  int endOfLine;
  char *remaining = string;
  Bool printed = FALSE;

  newline();
  endOfLine = 75-level*2;
  while (endOfLine < strlen(remaining)) {
    while(remaining[endOfLine] != ' ' && remaining[endOfLine] != '\0' && endOfLine > 0) endOfLine--;
    ch = remaining[endOfLine];
    remaining[endOfLine] = '\0';
    for (i = 0; i < endOfLine; i++) if (remaining[i] == '\n') remaining[i] = ' ';
    if (printed) fprintf(outFile, " ");
    fprintf(outFile, remaining);
    remaining[endOfLine] = ch;
    remaining = &remaining[endOfLine];
    while(*remaining == ' ' || *remaining == '\n') remaining++;
    newline();
    printed = TRUE;
    endOfLine = 75-level*2;
  }
  if (printed) fprintf(outFile, " ");
  for (i = 0; i < strlen(remaining); i++)
    if (remaining[i] == '\n')
      remaining[i] = ' ';
  fprintf(outFile, remaining);
}



/* END %%DECLARATIONS */


/*----------------------------------------------------------------------------
 * pmPaSema - The semantic actions
 *----------------------------------------------------------------------------
 */
void pmPaSema(
int rule			/* IN production number */
)
{
#line 124 "alan.pmk"
    switch (rule) {
    case 3: { /* <optional_options> = 'OPTIONS' __genSym#0 <options>; */
#line 137 "alan.pmk"
 outdent(); newline(); newline(); 	break;}
    case 4: { /* __genSym#0 =; */
#line 135 "alan.pmk"
 fprintf(outFile, "Options"); indent(); newline(); 	break;}
    case 5: { /* <options> = <option>; */
#line 141 "alan.pmk"
 newline(); 	break;}
    case 7: { /* <option> = ID '.'; */
#line 146 "alan.pmk"

	fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string);
    	break;}
    case 8: { /* <option> = ID ID '.'; */
#line 150 "alan.pmk"

	fprintf(outFile, "%s %s.", pmSeSt[pmStkP+1].string, pmSeSt[pmStkP+2].string);
    	break;}
    case 9: { /* <option> = ID Integer '.'; */
#line 154 "alan.pmk"

	fprintf(outFile, "%s %d.", pmSeSt[pmStkP+1].string, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 26: { /* <default> = 'DEFAULT' __genSym#1 'ATTRIBUTES' <attributes>; */
#line 193 "alan.pmk"

	outdent();
	outdent();
	newline();
	fprintf(outFile, "End Add To.");
	newline();
    	break;}
    case 27: { /* __genSym#1 =; */
#line 184 "alan.pmk"

	newline();
	fprintf(outFile, "Add To Every thing");
	indent();
	newline();
	fprintf(outFile, "Is");
	indent();
    	break;}
    case 28: { /* <location_default> = 'LOCATION' __genSym#2 'ATTRIBUTES' <attributes>; */
#line 214 "alan.pmk"

	outdent();
	outdent();
	newline();
	fprintf(outFile, "End Add To.");
	newline();
    	break;}
    case 29: { /* __genSym#2 =; */
#line 205 "alan.pmk"

	newline();
	fprintf(outFile, "Add To Every location");
	indent();
	newline();
	fprintf(outFile, "Is");
	indent();
    	break;}
    case 30: { /* <object_default> = 'OBJECT' __genSym#3 'ATTRIBUTES' <attributes>; */
#line 235 "alan.pmk"

	outdent();
	outdent();
	newline();
	fprintf(outFile, "End Add To.");
	newline();
    	break;}
    case 31: { /* __genSym#3 =; */
#line 226 "alan.pmk"

	newline();
	fprintf(outFile, "Add To Every object");
	indent();
	newline();
	fprintf(outFile, "Is");
	indent();
    	break;}
    case 32: { /* <actor_default> = 'ACTOR' __genSym#4 'ATTRIBUTES' <attributes>; */
#line 256 "alan.pmk"

	outdent();
	outdent();
	newline();
	fprintf(outFile, "End Add To.");
	newline();
    	break;}
    case 33: { /* __genSym#4 =; */
#line 247 "alan.pmk"

	newline();
	fprintf(outFile, "Add To Every actor");
	indent();
	newline();
	fprintf(outFile, "Is");
	indent();
    	break;}
    case 34: { /* <attributes> = <attribute> '.'; */
#line 268 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 35: { /* <attributes> = <attributes> <attribute> '.'; */
#line 269 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 36: { /* <attribute> = ID; */
#line 273 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 37: { /* <attribute> = 'NOT' ID; */
#line 274 "alan.pmk"
 newline(); fprintf(outFile, "Not "); idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 38: { /* <attribute> = ID <optional_minus> Integer; */
#line 275 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); fprintf(outFile, " %s%s", pmSeSt[pmStkP+2].string, pmSySt[pmStkP+3].chars); 	break;}
    case 39: { /* <attribute> = ID STRING; */
#line 276 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); fprintf(outFile, " "); stringPrint(pmSySt[pmStkP+2].chars); 	break;}
    case 40: { /* <synonyms> = 'SYNONYMS' __genSym#5 <synonym_list>; */
#line 282 "alan.pmk"
 outdent(); newline(); 	break;}
    case 41: { /* __genSym#5 =; */
#line 281 "alan.pmk"
 newline(); fprintf(outFile, "Synonyms"); indent(); newline(); 	break;}
    case 44: { /* <synonym> = <id_list> '=' ID '.'; */
#line 289 "alan.pmk"
 fprintf(outFile, " = "); idPrint(pmSeSt[pmStkP+3].string); fprintf(outFile, "."); newline(); 	break;}
    case 49: { /* <syntax> = 'SYNTAX' __genSym#6 <syntax_list>; */
#line 307 "alan.pmk"
 outdent(); newline(); 	break;}
    case 50: { /* __genSym#6 =; */
#line 306 "alan.pmk"
 newline(); fprintf(outFile, "Syntax"); indent(); 	break;}
    case 54: { /* <syntax_item1> = ID '='; */
#line 317 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); fprintf(outFile, " = "); 	break;}
    case 58: { /* <syntax_element> = ID; */
#line 327 "alan.pmk"
 fprintf(outFile, " "); idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 59: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 328 "alan.pmk"
 fprintf(outFile, " ("); idPrint(pmSeSt[pmStkP+2].string); fprintf(outFile, ")"); fprintf(outFile, pmSeSt[pmStkP+4].string); 	break;}
    case 60: { /* <optional_indicators> =; */
#line 331 "alan.pmk"
 pmSeSt[pmStkP+1].string = malloc(1); strcpy(pmSeSt[pmStkP+1].string, ""); 	break;}
    case 61: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 333 "alan.pmk"

	    pmSeSt[pmStkP+1].string = realloc(pmSeSt[pmStkP+1].string,
		strlen(pmSeSt[pmStkP+1].string) + pmSeSt[pmStkP+2].string + 1);
	    strcat(pmSeSt[pmStkP+1].string, pmSeSt[pmStkP+2].string);
		break;}
    case 62: { /* <indicator> = '*'; */
#line 340 "alan.pmk"
 pmSeSt[pmStkP+1].string = "*"; 	break;}
    case 63: { /* <indicator> = '!'; */
#line 341 "alan.pmk"
 pmSeSt[pmStkP+1].string = "!"; 	break;}
    case 64: { /* <optional_class_restrictions> = '.'; */
#line 345 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 65: { /* <optional_class_restrictions> = 'WHERE' __genSym#7 <class_restrictions>; */
#line 347 "alan.pmk"
 outdent(); outdent(); newline(); 	break;}
    case 66: { /* __genSym#7 =; */
#line 346 "alan.pmk"
 indent(); newline(); fprintf(outFile, "Where "); indent(); 	break;}
    case 69: { /* __genSym#8 =; */
#line 352 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "And "); indent(); 	break;}
    case 70: { /* <class_restriction> = <class_restriction1> <class_restriction2>; */
#line 356 "alan.pmk"
 outdent(); 	break;}
    case 71: { /* <class_restriction1> = ID 'ISA'; */
#line 360 "alan.pmk"
 fprintf(outFile, "%s Isa ", pmSeSt[pmStkP+1].string); 	break;}
    case 73: { /* __genSym#9 =; */
#line 364 "alan.pmk"
 newline(); fprintf(outFile, "Else "); indent(); 	break;}
    case 74: { /* <classes> = <class_identifier>; */
#line 368 "alan.pmk"
 fprintf(outFile, pmSeSt[pmStkP+1].string); 	break;}
    case 75: { /* <classes> = <classes> 'OR' <class_identifier>; */
#line 369 "alan.pmk"
 fprintf(outFile, " -- ****OR**** %s", pmSeSt[pmStkP+3].string); 	break;}
    case 76: { /* <class_identifier> = 'OBJECT'; */
#line 373 "alan.pmk"
 pmSeSt[pmStkP+1].string = "object"; 	break;}
    case 77: { /* <class_identifier> = 'ACTOR'; */
#line 374 "alan.pmk"
 pmSeSt[pmStkP+1].string = "actor"; 	break;}
    case 78: { /* <class_identifier> = 'CONTAINER'; */
#line 375 "alan.pmk"
 pmSeSt[pmStkP+1].string = "container"; 	break;}
    case 81: { /* <class_identifier> = 'CONTAINER' 'OBJECT'; */
#line 376 "alan.pmk"
 pmSeSt[pmStkP+1].string = "**** container object ****"; 	break;}
    case 82: { /* <class_identifier> = 'CONTAINER' 'ACTOR'; */
#line 377 "alan.pmk"
 pmSeSt[pmStkP+1].string = "**** container actor ****"; 	break;}
    case 79: { /* <class_identifier> = 'INTEGER'; */
#line 378 "alan.pmk"
 pmSeSt[pmStkP+1].string = "integer"; 	break;}
    case 80: { /* <class_identifier> = 'STRING'; */
#line 379 "alan.pmk"
 pmSeSt[pmStkP+1].string = "string"; 	break;}
    case 86: { /* <verb_header> = 'VERB' __genSym#10 <id_list>; */
#line 391 "alan.pmk"
 indent(); 	break;}
    case 87: { /* __genSym#10 =; */
#line 391 "alan.pmk"
 newline(); fprintf(outFile, "Verb "); 	break;}
    case 92: { /* <verb_alternative> = <verb_alternative1> <simple_verb_body>; */
#line 402 "alan.pmk"
 outdent(); 	break;}
    case 93: { /* <verb_alternative1> = 'WHEN' ID; */
#line 405 "alan.pmk"
 newline(); fprintf(outFile, "When %s", pmSeSt[pmStkP+2].string); indent(); 	break;}
    case 95: { /* <verb_tail> = 'END' 'VERB' __genSym#11 <optional_id> '.'; */
#line 411 "alan.pmk"
 fprintf(outFile, "."); newline(); 	break;}
    case 96: { /* __genSym#11 =; */
#line 411 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End Verb"); 	break;}
    case 98: { /* <optional_checks> = <check1> <statements>; */
#line 417 "alan.pmk"
 outdent(); 	break;}
    case 99: { /* <optional_checks> = <check1> <check_list>; */
#line 418 "alan.pmk"
 outdent(); 	break;}
    case 100: { /* <check1> = 'CHECK'; */
#line 422 "alan.pmk"
 newline(); fprintf(outFile, "Check "); indent(); 	break;}
    case 103: { /* __genSym#12 =; */
#line 426 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "And "); indent(); 	break;}
    case 104: { /* <check> = <expression> 'ELSE' __genSym#13 <statements>; */
#line 429 "alan.pmk"
 outdent(); 	break;}
    case 105: { /* __genSym#13 =; */
#line 429 "alan.pmk"
 newline(); fprintf(outFile, "Else "); indent(); 	break;}
    case 108: { /* <does> = 'DOES' __genSym#14 <optional_qual> <statements>; */
#line 439 "alan.pmk"
 outdent(); 	break;}
    case 109: { /* __genSym#14 =; */
#line 439 "alan.pmk"
 newline(); fprintf(outFile, "Does"); indent(); 	break;}
    case 110: { /* <location> = <location_header> <location_body> <location_tail>; */
#line 444 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); 	break;}
    case 111: { /* <location_header> = <location_id> <optional_name>; */
#line 449 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 112: { /* <location_id> = 'LOCATION' ID; */
#line 454 "alan.pmk"

	newline(); newline();
	fprintf(outFile, "The %s Isa location" , pmSeSt[pmStkP+2].string);
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 120: { /* __genSym#15 =; */
#line 468 "alan.pmk"
 newline(); 	break;}
    case 121: { /* <location_tail> = 'END' 'LOCATION' <optional_id> '.'; */
#line 476 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End The "); 	break;}
    case 124: { /* <exit> = 'EXIT' __genSym#16 <id_list> <to_id> <optional_exit_body> '.'; */
#line 486 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 125: { /* __genSym#16 =; */
#line 485 "alan.pmk"
 newline(); fprintf(outFile, "Exit "); 	break;}
    case 126: { /* <to_id> = 'TO' ID; */
#line 489 "alan.pmk"
 fprintf(outFile, " To %s", pmSeSt[pmStkP+2].string); 	break;}
    case 128: { /* <optional_exit_body> = __genSym#17 <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 494 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End Exit%s%s", pmSeSt[pmStkP+6].string[0]?" ":"", pmSeSt[pmStkP+6].string); 	break;}
    case 129: { /* __genSym#17 =; */
#line 493 "alan.pmk"
 indent(); 	break;}
    case 130: { /* <object> = <object_header> <object_body> <object_tail>; */
#line 500 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); 	break;}
    case 131: { /* <object_header> = <object_id> <optional_where> <optional_names> <optional_where>; */
#line 505 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string;	break;}
    case 132: { /* <object_id> = 'OBJECT' ID; */
#line 510 "alan.pmk"

	newline(); newline(); fprintf(outFile, "The %s Isa object", pmSeSt[pmStkP+2].string);
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 139: { /* <object_body_part> = __genSym#18 <is> __genSym#19 <attributes>; */
#line 524 "alan.pmk"
 outdent(); 	break;}
    case 141: { /* __genSym#18 =; */
#line 524 "alan.pmk"
 newline(); 	break;}
    case 142: { /* __genSym#19 =; */
#line 524 "alan.pmk"
 indent(); 	break;}
    case 143: { /* <object_tail> = 'END' 'OBJECT' <optional_id> '.'; */
#line 533 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End The "); 	break;}
    case 145: { /* <optional_attributes> = <optional_attributes> <is> __genSym#20 <attributes>; */
#line 541 "alan.pmk"
 outdent(); 	break;}
    case 146: { /* __genSym#20 =; */
#line 541 "alan.pmk"
 indent(); newline(); 	break;}
    case 147: { /* <is> = 'IS'; */
#line 544 "alan.pmk"
 fprintf(outFile, "Is "); 	break;}
    case 148: { /* <is> = 'ARE'; */
#line 545 "alan.pmk"
 fprintf(outFile, "Are "); 	break;}
    case 149: { /* <is> = 'HAS'; */
#line 546 "alan.pmk"
 fprintf(outFile, "Has "); 	break;}
    case 152: { /* <description> = 'DESCRIPTION'; */
#line 554 "alan.pmk"
 newline(); fprintf(outFile, "Description"); newline(); 	break;}
    case 153: { /* <description> = 'DESCRIPTION' __genSym#21 <statements>; */
#line 555 "alan.pmk"
 outdent(); newline(); 	break;}
    case 154: { /* __genSym#21 =; */
#line 555 "alan.pmk"
 newline(); fprintf(outFile, "Description"); indent(); 	break;}
    case 162: { /* <name> = 'NAME' __genSym#22 <ids>; */
#line 573 "alan.pmk"
 newline(); 	break;}
    case 163: { /* __genSym#22 =; */
#line 573 "alan.pmk"
 fprintf(outFile, "Name "); 	break;}
    case 164: { /* <properties> = 'CONTAINER' __genSym#23 <container_body>; */
#line 578 "alan.pmk"
 outdent(); 	break;}
    case 165: { /* __genSym#23 =; */
#line 578 "alan.pmk"
 newline(); fprintf(outFile, "With Container"); indent(); 	break;}
    case 166: { /* <container> = <container_header> <container_body> <container_tail>; */
#line 582 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); newline(); 	break;}
    case 167: { /* <container_header> = 'CONTAINER' ID; */
#line 585 "alan.pmk"
 newline(); fprintf(outFile, "The %s", pmSeSt[pmStkP+2].string);
			indent(); newline();
			fprintf(outFile, "Container");
			pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string; indent(); 	break;}
    case 169: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */
#line 595 "alan.pmk"
 outdent(); outdent(); newline(); fprintf(outFile, "End The "); 	break;}
    case 171: { /* <optional_limits> = 'LIMITS' __genSym#24 <limits>; */
#line 599 "alan.pmk"
 outdent(); 	break;}
    case 172: { /* __genSym#24 =; */
#line 599 "alan.pmk"
 newline(); fprintf(outFile, "Limits"); indent(); 	break;}
    case 176: { /* __genSym#25 =; */
#line 606 "alan.pmk"
 fprintf(outFile, "Then"); indent(); 	break;}
    case 178: { /* <limit_attribute> = 'COUNT' Integer; */
#line 610 "alan.pmk"
 newline(); fprintf(outFile, "Count %s", pmSySt[pmStkP+2].chars); 	break;}
    case 180: { /* <optional_header> = 'HEADER' __genSym#26 <statements>; */
#line 614 "alan.pmk"
 outdent(); 	break;}
    case 181: { /* __genSym#26 =; */
#line 614 "alan.pmk"
 newline(); fprintf(outFile, "Header"); indent(); 	break;}
    case 183: { /* <optional_empty> = 'ELSE' __genSym#27 <statements>; */
#line 618 "alan.pmk"
 outdent(); 	break;}
    case 184: { /* __genSym#27 =; */
#line 618 "alan.pmk"
 newline(); fprintf(outFile, "Else"); indent(); 	break;}
    case 185: { /* <event> = <event_header> __genSym#28 <statements> <event_tail>; */
#line 623 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); 	break;}
    case 186: { /* __genSym#28 =; */
#line 623 "alan.pmk"
 indent(); 	break;}
    case 187: { /* <event_header> = 'EVENT' ID; */
#line 627 "alan.pmk"
 newline(); fprintf(outFile, "Event %s", pmSeSt[pmStkP+2].string);
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string; 	break;}
    case 188: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 632 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End Event "); 	break;}
    case 189: { /* <actor> = <actor_header> <actor_body> <actor_tail>; */
#line 638 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); 	break;}
    case 190: { /* <actor_header> = <actor_id> <optional_where> <optional_names> <optional_where>; */
#line 643 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 191: { /* <actor_id> = 'ACTOR' ID; */
#line 648 "alan.pmk"

	newline(); newline(); fprintf(outFile, "The %s Isa actor", pmSeSt[pmStkP+2].string);
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 196: { /* <actor_body_part> = __genSym#29 <is> __genSym#30 <attributes>; */
#line 662 "alan.pmk"
 outdent(); 	break;}
    case 199: { /* __genSym#29 =; */
#line 662 "alan.pmk"
 newline(); 	break;}
    case 200: { /* __genSym#30 =; */
#line 662 "alan.pmk"
 indent(); 	break;}
    case 201: { /* <actor_tail> = 'END' 'ACTOR' <optional_id> '.'; */
#line 669 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End The "); 	break;}
    case 205: { /* <script1> = 'SCRIPT' <integer_or_id> '.'; */
#line 684 "alan.pmk"
 newline(); fprintf(outFile, "Script %s", pmSeSt[pmStkP+2].string); indent(); 	break;}
    case 209: { /* <step> = 'STEP' __genSym#31 <statements>; */
#line 697 "alan.pmk"
 outdent(); 	break;}
    case 212: { /* __genSym#31 =; */
#line 697 "alan.pmk"
 newline(); fprintf(outFile, "Step"); indent(); 	break;}
    case 210: { /* <step> = <step_after_integer> <statements>; */
#line 698 "alan.pmk"
 outdent(); 	break;}
    case 211: { /* <step> = 'STEP' 'WAIT' 'UNTIL' __genSym#32 <expression> <statements>; */
#line 699 "alan.pmk"
 outdent(); 	break;}
    case 213: { /* __genSym#32 =; */
#line 699 "alan.pmk"
 newline(); fprintf(outFile, "Step"); indent(); 	break;}
    case 214: { /* <step_after_integer> = 'STEP' 'AFTER' Integer; */
#line 703 "alan.pmk"
 newline(); fprintf(outFile, "Step After %s", pmSySt[pmStkP+3].chars); indent(); 	break;}
    case 215: { /* <rule> = 'WHEN' __genSym#33 <expression> '=>' __genSym#34 <statements>; */
#line 708 "alan.pmk"
 outdent(); 	break;}
    case 216: { /* __genSym#33 =; */
#line 708 "alan.pmk"
 newline(); fprintf(outFile, "When "); 	break;}
    case 217: { /* __genSym#34 =; */
#line 708 "alan.pmk"
 fprintf(outFile, "=>"); indent(); newline(); 	break;}
    case 219: { /* __genSym#35 =; */
#line 713 "alan.pmk"
 newline(); newline(); fprintf(outFile, "Start "); 	break;}
    case 220: { /* __genSym#36 =; */
#line 713 "alan.pmk"
 fprintf(outFile, "."); indent(); newline(); 	break;}
    case 232: { /* <fullstop> = '.'; */
#line 738 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 233: { /* <output_statement> = STRING; */
#line 743 "alan.pmk"
 stringPrint(pmSySt[pmStkP+1].chars); 	break;}
    case 237: { /* __genSym#37 =; */
#line 744 "alan.pmk"
 newline(); fprintf(outFile, "Describe "); 	break;}
    case 238: { /* __genSym#38 =; */
#line 745 "alan.pmk"
 newline(); fprintf(outFile, "Say "); 	break;}
    case 239: { /* __genSym#39 =; */
#line 746 "alan.pmk"
 newline(); fprintf(outFile, "Say "); 	break;}
    case 240: { /* <special_statement> = 'QUIT' '.'; */
#line 750 "alan.pmk"
 newline(); fprintf(outFile, "Quit."); 	break;}
    case 241: { /* <special_statement> = 'LOOK' '.'; */
#line 751 "alan.pmk"
 newline(); fprintf(outFile, "Quit."); 	break;}
    case 242: { /* <special_statement> = 'SAVE' '.'; */
#line 752 "alan.pmk"
 newline(); fprintf(outFile, "Quit."); 	break;}
    case 243: { /* <special_statement> = 'RESTORE' '.'; */
#line 753 "alan.pmk"
 newline(); fprintf(outFile, "Quit."); 	break;}
    case 244: { /* <special_statement> = 'RESTART' '.'; */
#line 754 "alan.pmk"
 newline(); fprintf(outFile, "Quit."); 	break;}
    case 245: { /* <special_statement> = 'SCORE' __genSym#40 <optional_integer> '.'; */
#line 755 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 248: { /* __genSym#40 =; */
#line 755 "alan.pmk"
 newline(); fprintf(outFile, "Score "); 	break;}
    case 246: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 756 "alan.pmk"
 newline(); fprintf(outFile, "Visits %s.", pmSySt[pmStkP+2].chars); 	break;}
    case 247: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 757 "alan.pmk"
 newline(); fprintf(outFile, "****System**** "); stringPrint(pmSySt[pmStkP+2].chars); 	break;}
    case 251: { /* __genSym#41 =; */
#line 763 "alan.pmk"
 newline(); fprintf(outFile, "Empty "); 	break;}
    case 252: { /* __genSym#42 =; */
#line 763 "alan.pmk"
 fprintf(outFile, " "); 	break;}
    case 253: { /* __genSym#43 =; */
#line 764 "alan.pmk"
 newline(); fprintf(outFile, "Locate "); 	break;}
    case 254: { /* __genSym#44 =; */
#line 764 "alan.pmk"
 fprintf(outFile, " "); 	break;}
    case 261: { /* __genSym#45 =; */
#line 775 "alan.pmk"
 newline(); fprintf(outFile, "Make "); 	break;}
    case 262: { /* __genSym#46 =; */
#line 775 "alan.pmk"
 fprintf(outFile, " "); 	break;}
    case 263: { /* __genSym#47 =; */
#line 776 "alan.pmk"
 newline(); fprintf(outFile, "Set "); 	break;}
    case 264: { /* __genSym#48 =; */
#line 776 "alan.pmk"
 fprintf(outFile, " To "); 	break;}
    case 265: { /* __genSym#49 =; */
#line 777 "alan.pmk"
 newline(); fprintf(outFile, "Increase "); 	break;}
    case 266: { /* __genSym#50 =; */
#line 778 "alan.pmk"
 newline(); fprintf(outFile, "Decrease "); 	break;}
    case 269: { /* __genSym#51 =; */
#line 783 "alan.pmk"
 fprintf(outFile, " By "); 	break;}
    case 272: { /* <if_statement> = 'IF' __genSym#52 <expression> 'THEN' __genSym#53 <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 796 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End If."); 	break;}
    case 273: { /* __genSym#52 =; */
#line 793 "alan.pmk"
 newline(); fprintf(outFile, "If "); 	break;}
    case 274: { /* __genSym#53 =; */
#line 794 "alan.pmk"
 fprintf(outFile, " Then"); indent(); 	break;}
    case 280: { /* __genSym#54 =; */
#line 810 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "Elsif "); 	break;}
    case 281: { /* __genSym#55 =; */
#line 811 "alan.pmk"
 fprintf(outFile, " Then "); indent(); 	break;}
    case 284: { /* __genSym#56 =; */
#line 817 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "Else "); indent(); 	break;}
    case 285: { /* <depending_statement> = 'DEPENDING' 'ON' __genSym#57 <primary> __genSym#58 <depend_cases> 'END' 'DEPEND' '.'; */
#line 825 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End Depend."); 	break;}
    case 286: { /* __genSym#57 =; */
#line 822 "alan.pmk"
 newline(); fprintf(outFile, "Depending On "); 	break;}
    case 287: { /* __genSym#58 =; */
#line 823 "alan.pmk"
 indent(); newline(); 	break;}
    case 290: { /* __genSym#59 =; */
#line 830 "alan.pmk"
 newline(); 	break;}
    case 292: { /* <depend_case> = <right_hand_side> ':' __genSym#60 <statements>; */
#line 834 "alan.pmk"
 outdent(); 	break;}
    case 293: { /* __genSym#60 =; */
#line 834 "alan.pmk"
 fprintf(outFile, " : "); indent(); 	break;}
    case 291: { /* <depend_case> = 'ELSE' __genSym#61 <statements>; */
#line 835 "alan.pmk"
 outdent(); 	break;}
    case 294: { /* __genSym#61 =; */
#line 835 "alan.pmk"
 fprintf(outFile, "Else "); indent(); 	break;}
    case 296: { /* __genSym#62 =; */
#line 840 "alan.pmk"
 newline(); fprintf(outFile, "Use Script "); 	break;}
    case 298: { /* <optional_for_actor> = 'FOR' ID; */
#line 845 "alan.pmk"
 fprintf(outFile, "For %s", pmSeSt[pmStkP+2].string); 	break;}
    case 301: { /* __genSym#63 =; */
#line 851 "alan.pmk"
 fprintf(outFile, " Or "); 	break;}
    case 304: { /* __genSym#64 =; */
#line 856 "alan.pmk"
 fprintf(outFile, " And "); 	break;}
    case 307: { /* __genSym#65 =; */
#line 861 "alan.pmk"
 fprintf(outFile, " "); 	break;}
    case 314: { /* __genSym#66 =; */
#line 869 "alan.pmk"
 fprintf(outFile, " Between "); 	break;}
    case 315: { /* __genSym#67 =; */
#line 869 "alan.pmk"
 fprintf(outFile, " And "); 	break;}
    case 316: { /* __genSym#68 =; */
#line 870 "alan.pmk"
 fprintf(outFile, " Contains "); 	break;}
    case 322: { /* <primary> = '(' __genSym#69 <expression> ')'; */
#line 874 "alan.pmk"
 fprintf(outFile, ")"); 	break;}
    case 325: { /* __genSym#69 =; */
#line 874 "alan.pmk"
 fprintf(outFile, "("); 	break;}
    case 317: { /* <primary> = <optional_minus> Integer; */
#line 875 "alan.pmk"
 fprintf(outFile, "%s%s", pmSeSt[pmStkP+1].string, pmSySt[pmStkP+2].chars); 	break;}
    case 318: { /* <primary> = STRING; */
#line 876 "alan.pmk"
 stringPrint(pmSySt[pmStkP+1].chars); 	break;}
    case 326: { /* __genSym#70 =; */
#line 880 "alan.pmk"
 fprintf(outFile, "Random "); 	break;}
    case 327: { /* __genSym#71 =; */
#line 880 "alan.pmk"
 fprintf(outFile, "To "); 	break;}
    case 320: { /* <primary> = 'SCORE'; */
#line 881 "alan.pmk"
 fprintf(outFile, "Score"); 	break;}
    case 329: { /* <aggregate> = 'SUM' 'OF' ID; */
#line 885 "alan.pmk"
 fprintf(outFile, " Sum Of "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 330: { /* <aggregate> = 'MAX' 'OF' ID; */
#line 886 "alan.pmk"
 fprintf(outFile, " Max Of "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 328: { /* <aggregate> = 'COUNT'; */
#line 887 "alan.pmk"
 fprintf(outFile, " Count "); 	break;}
    case 331: { /* <something> = <optional_not> ID; */
#line 892 "alan.pmk"
 idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 332: { /* <what> = 'OBJECT'; */
#line 896 "alan.pmk"
 fprintf(outFile, "**** object ****"); 	break;}
    case 333: { /* <what> = 'LOCATION'; */
#line 897 "alan.pmk"
 fprintf(outFile, "Current Location"); 	break;}
    case 334: { /* <what> = 'ACTOR'; */
#line 898 "alan.pmk"
 fprintf(outFile, "Current Actor"); 	break;}
    case 335: { /* <what> = ID; */
#line 899 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 338: { /* <where> = 'HERE'; */
#line 908 "alan.pmk"
 fprintf(outFile, "Here"); 	break;}
    case 339: { /* <where> = 'NEARBY'; */
#line 909 "alan.pmk"
 fprintf(outFile, "Nearby"); 	break;}
    case 342: { /* __genSym#72 =; */
#line 910 "alan.pmk"
 fprintf(outFile, "At "); 	break;}
    case 343: { /* __genSym#73 =; */
#line 911 "alan.pmk"
 fprintf(outFile, "In "); 	break;}
    case 344: { /* <binop> = '+'; */
#line 915 "alan.pmk"
 fprintf(outFile, "+"); 	break;}
    case 345: { /* <binop> = '-'; */
#line 916 "alan.pmk"
 fprintf(outFile, "-"); 	break;}
    case 346: { /* <binop> = '*'; */
#line 917 "alan.pmk"
 fprintf(outFile, "*"); 	break;}
    case 347: { /* <binop> = '/'; */
#line 918 "alan.pmk"
 fprintf(outFile, "/"); 	break;}
    case 348: { /* <relop> = '<>'; */
#line 922 "alan.pmk"
 fprintf(outFile, "<>"); 	break;}
    case 349: { /* <relop> = '='; */
#line 923 "alan.pmk"
 fprintf(outFile, "="); 	break;}
    case 350: { /* <relop> = '=='; */
#line 924 "alan.pmk"
 fprintf(outFile, "=="); 	break;}
    case 351: { /* <relop> = '>='; */
#line 925 "alan.pmk"
 fprintf(outFile, ">="); 	break;}
    case 352: { /* <relop> = '<='; */
#line 926 "alan.pmk"
 fprintf(outFile, "<="); 	break;}
    case 353: { /* <relop> = '>'; */
#line 927 "alan.pmk"
 fprintf(outFile, ">"); 	break;}
    case 354: { /* <relop> = '<'; */
#line 928 "alan.pmk"
 fprintf(outFile, "<"); 	break;}
    case 356: { /* <optional_qual> = 'BEFORE'; */
#line 933 "alan.pmk"
 fprintf(outFile, " Before"); 	break;}
    case 357: { /* <optional_qual> = 'AFTER'; */
#line 934 "alan.pmk"
 fprintf(outFile, " After"); 	break;}
    case 358: { /* <optional_qual> = 'ONLY'; */
#line 935 "alan.pmk"
 fprintf(outFile, " Only"); 	break;}
    case 360: { /* <optional_not> = 'NOT'; */
#line 940 "alan.pmk"
 fprintf(outFile, "Not "); 	break;}
    case 361: { /* <optional_id> =; */
#line 944 "alan.pmk"
 pmSeSt[pmStkP+1].string = ""; 	break;}
    case 362: { /* <optional_id> = ID; */
#line 945 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 363: { /* <ids> = ID; */
#line 949 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 364: { /* <ids> = <ids> ID; */
#line 950 "alan.pmk"
 fprintf(outFile, " "); idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 365: { /* <id_list> = ID; */
#line 954 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 366: { /* <id_list> = <id_list> ',' ID; */
#line 955 "alan.pmk"
 fprintf(outFile, ", "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 368: { /* <optional_integer> = Integer; */
#line 960 "alan.pmk"
 fprintf(outFile, pmSySt[pmStkP+1].chars); 	break;}
    case 369: { /* <optional_minus> =; */
#line 964 "alan.pmk"
 pmSeSt[pmStkP+1].string = ""; 	break;}
    case 370: { /* <optional_minus> = '-'; */
#line 965 "alan.pmk"
 pmSeSt[pmStkP+1].string = "-"; 	break;}
    case 372: { /* <id_of> = ID 'OF'; */
#line 973 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); fprintf(outFile, " Of "); 	break;}
    case 373: { /* <integer_or_id> = Integer; */
#line 977 "alan.pmk"
 pmSeSt[pmStkP+1].string = malloc(strlen(pmSySt[pmStkP+1].chars)+2);
		sprintf(pmSeSt[pmStkP+1].string, "s%s", pmSySt[pmStkP+1].chars); 	break;}
    case 374: { /* <integer_or_id> = ID; */
#line 979 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 375: { /* ID = IDENT; */
#line 983 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSySt[pmStkP+1].chars; 	break;}
    case 376: { /* ID = 'DEFAULT'; */
#line 984 "alan.pmk"
 pmSeSt[pmStkP+1].string = "default"; 	break;}
    case 377: { /* ID = 'ARTICLE'; */
#line 985 "alan.pmk"
 pmSeSt[pmStkP+1].string = "article"; 	break;}
    case 378: { /* ID = 'MESSAGE'; */
#line 986 "alan.pmk"
 pmSeSt[pmStkP+1].string = "message"; 	break;}
    case 379: { /* ID = 'QUIT'; */
#line 987 "alan.pmk"
 pmSeSt[pmStkP+1].string = "quit"; 	break;}
    case 380: { /* ID = 'SAVE'; */
#line 988 "alan.pmk"
 pmSeSt[pmStkP+1].string = "save"; 	break;}
    case 381: { /* ID = 'RESTORE'; */
#line 989 "alan.pmk"
 pmSeSt[pmStkP+1].string = "restore"; 	break;}
    case 382: { /* ID = 'RESTART'; */
#line 990 "alan.pmk"
 pmSeSt[pmStkP+1].string = "restart"; 	break;}
    case 383: { /* ID = 'WAIT'; */
#line 991 "alan.pmk"
 pmSeSt[pmStkP+1].string = "wait"; 	break;}
    case 384: { /* ID = 'BETWEEN'; */
#line 992 "alan.pmk"
 pmSeSt[pmStkP+1].string = "between"; 	break;}
    case 385: { /* ID = 'CONTAINS'; */
#line 993 "alan.pmk"
 pmSeSt[pmStkP+1].string = "contains"; 	break;}
    case 386: { /* ID = 'ON'; */
#line 994 "alan.pmk"
 pmSeSt[pmStkP+1].string = "on"; 	break;}
    case 387: { /* ID = 'IN'; */
#line 995 "alan.pmk"
 pmSeSt[pmStkP+1].string = "in"; 	break;}
    case 388: { /* ID = 'AFTER'; */
#line 996 "alan.pmk"
 pmSeSt[pmStkP+1].string = "after"; 	break;}
    case 389: { /* ID = 'BEFORE'; */
#line 997 "alan.pmk"
 pmSeSt[pmStkP+1].string = "before"; 	break;}
    case 390: { /* ID = 'CHECK'; */
#line 998 "alan.pmk"
 pmSeSt[pmStkP+1].string = "check"; 	break;}
    case 391: { /* ID = 'DEPEND'; */
#line 999 "alan.pmk"
 pmSeSt[pmStkP+1].string = "depend"; 	break;}
    case 392: { /* ID = 'EXIT'; */
#line 1000 "alan.pmk"
 pmSeSt[pmStkP+1].string = "exit"; 	break;}
    case 393: { /* ID = 'FOR'; */
#line 1001 "alan.pmk"
 pmSeSt[pmStkP+1].string = "for"; 	break;}
    case 394: { /* ID = 'INTEGER'; */
#line 1002 "alan.pmk"
 pmSeSt[pmStkP+1].string = "integer"; 	break;}
    case 395: { /* ID = 'ISA'; */
#line 1003 "alan.pmk"
 pmSeSt[pmStkP+1].string = "isa"; 	break;}
    case 396: { /* ID = 'LIMITS'; */
#line 1004 "alan.pmk"
 pmSeSt[pmStkP+1].string = "limits"; 	break;}
    default: break; }
}/*pmPaSema()*/

