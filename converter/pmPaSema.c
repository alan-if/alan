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
#include "util.h"

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


static char *(v3Keywords[]) = {"to", "is", "at", "in", "an", "by",
"if", "on", "do", "or", "of", "end", "not", "and", "isa", "add",
"the", "are", "has", "say", "set", "for", "use", "max", "min", "sum",
"verb", "does", "exit", "when", "else", "form", "name", "with",
"then", "step", "wait", "here", "list", "show", "from", "make",
"last", "word", "into", "each", "stop", "quit", "look", "save",
"this", "only", "event", "start", "check", "where", "every", "count",
"after", "until", "empty", "strip", "first", "words", "elsif",
"score", "actor", "syntax", "script", "option", "opaque", "taking",
"limits", "header", "nearby", "locate", "cancel", "depend", "visits",
"system", "random", "before", "options", "message", "article",
"entered", "extract", "include", "exclude", "restore", "restart",
"between", "current", "synonyms", "definite", "describe", "schedule",
"increase", "decrease", "contains", "location", "container",
"mentioned", "character", "depending", "attributes", "indefinite",
"characters", "description", NULL};

static void print(char string[])
{
    fprintf(outFile, "%s", string);
}

static void idPrint(char id[])
{
  int wordIndex = 0;

  while (v3Keywords[wordIndex] != NULL && strcmp(id, v3Keywords[wordIndex]) != 0)
    wordIndex++;
  if (v3Keywords[wordIndex] != NULL) fprintf(outFile, "'%s'", id);
  else fprintf(outFile, "%s", id);
}

static void stringPrint(char string[])
{
  int ch, i;
  int endOfLine;
  char *remaining = string;
  Bool printed = FALSE;

  newline();
  onlyOneSpace(string);

  endOfLine = 75-level*2;
  while (endOfLine < strlen(remaining)) {
    while(remaining[endOfLine] != ' ' && remaining[endOfLine] != '\0' && endOfLine > 0) endOfLine--;
    ch = remaining[endOfLine];
    remaining[endOfLine] = '\0';
    for (i = 0; i < endOfLine; i++) if (remaining[i] == '\n') remaining[i] = ' ';
    if (printed) print(" ");
    print(remaining);
    remaining[endOfLine] = ch;
    remaining = &remaining[endOfLine];
    while(*remaining == ' ' || *remaining == '\n') remaining++;
    newline();
    printed = TRUE;
    endOfLine = 75-level*2;
  }
  if (printed) print(" ");
  for (i = 0; i < strlen(remaining); i++)
    if (remaining[i] == '\n')
      remaining[i] = ' ';
  print(remaining);
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
#line 167 "alan.pmk"
    switch (rule) {
    case 3: { /* <optional_options> = 'OPTIONS' __genSym#0 <options>; */
#line 180 "alan.pmk"
 outdent(); newline(); newline(); 	break;}
    case 4: { /* __genSym#0 =; */
#line 178 "alan.pmk"
 print("Options"); indent(); newline(); 	break;}
    case 5: { /* <options> = <option>; */
#line 184 "alan.pmk"
 newline(); 	break;}
    case 7: { /* <option> = ID '.'; */
#line 189 "alan.pmk"

	idPrint(pmSeSt[pmStkP+1].string); print(".");
    	break;}
    case 8: { /* <option> = ID ID '.'; */
#line 193 "alan.pmk"

	idPrint(pmSeSt[pmStkP+1].string); print(" "); idPrint(pmSeSt[pmStkP+2].string); printf(".");
    	break;}
    case 9: { /* <option> = ID Integer '.'; */
#line 197 "alan.pmk"

	idPrint(pmSeSt[pmStkP+1].string); print(" "); print(pmSySt[pmStkP+2].chars); print(".");
    	break;}
    case 26: { /* <default> = 'DEFAULT' __genSym#1 'ATTRIBUTES' <attributes>; */
#line 236 "alan.pmk"

	outdent();
	outdent();
	newline();
	print("End Add To.");
	newline();
    	break;}
    case 27: { /* __genSym#1 =; */
#line 227 "alan.pmk"

	newline();
	print("Add To Every thing");
	indent();
	newline();
	print("Is");
	indent();
    	break;}
    case 28: { /* <location_default> = 'LOCATION' __genSym#2 'ATTRIBUTES' <attributes>; */
#line 257 "alan.pmk"

	outdent();
	outdent();
	newline();
	print("End Add To.");
	newline();
    	break;}
    case 29: { /* __genSym#2 =; */
#line 248 "alan.pmk"

	newline();
	print("Add To Every location");
	indent();
	newline();
	print("Is");
	indent();
    	break;}
    case 30: { /* <object_default> = 'OBJECT' __genSym#3 'ATTRIBUTES' <attributes>; */
#line 278 "alan.pmk"

	outdent();
	outdent();
	newline();
	print("End Add To.");
	newline();
    	break;}
    case 31: { /* __genSym#3 =; */
#line 269 "alan.pmk"

	newline();
	print("Add To Every object");
	indent();
	newline();
	print("Is");
	indent();
    	break;}
    case 32: { /* <actor_default> = 'ACTOR' __genSym#4 'ATTRIBUTES' <attributes>; */
#line 299 "alan.pmk"

	outdent();
	outdent();
	newline();
	print("End Add To.");
	newline();
    	break;}
    case 33: { /* __genSym#4 =; */
#line 290 "alan.pmk"

	newline();
	print("Add To Every actor");
	indent();
	newline();
	print("Is");
	indent();
    	break;}
    case 34: { /* <attributes> = <attribute> '.'; */
#line 311 "alan.pmk"
 print("."); 	break;}
    case 35: { /* <attributes> = <attributes> <attribute> '.'; */
#line 312 "alan.pmk"
 print("."); 	break;}
    case 36: { /* <attribute> = ID; */
#line 316 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 37: { /* <attribute> = 'NOT' ID; */
#line 317 "alan.pmk"
 newline(); print("Not "); idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 38: { /* <attribute> = ID <optional_minus> Integer; */
#line 318 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); print(" "); print(pmSeSt[pmStkP+2].string); print(" "); print(pmSySt[pmStkP+3].chars); 	break;}
    case 39: { /* <attribute> = ID STRING; */
#line 319 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); print(" "); stringPrint(pmSySt[pmStkP+2].chars); 	break;}
    case 40: { /* <synonyms> = 'SYNONYMS' __genSym#5 <synonym_list>; */
#line 325 "alan.pmk"
 outdent(); newline(); 	break;}
    case 41: { /* __genSym#5 =; */
#line 324 "alan.pmk"
 newline(); print("Synonyms"); indent(); newline(); 	break;}
    case 44: { /* <synonym> = <id_list> '=' ID '.'; */
#line 332 "alan.pmk"
 print(" = "); idPrint(pmSeSt[pmStkP+3].string); print("."); newline(); 	break;}
    case 49: { /* <syntax> = 'SYNTAX' __genSym#6 <syntax_list>; */
#line 350 "alan.pmk"
 outdent(); newline(); 	break;}
    case 50: { /* __genSym#6 =; */
#line 349 "alan.pmk"
 newline(); print("Syntax"); indent(); 	break;}
    case 54: { /* <syntax_item1> = ID '='; */
#line 360 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); print(" ="); 	break;}
    case 58: { /* <syntax_element> = ID; */
#line 370 "alan.pmk"
 print(" "); idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 59: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 371 "alan.pmk"
 print(" ("); idPrint(pmSeSt[pmStkP+2].string); print(")"); print(pmSeSt[pmStkP+4].string); 	break;}
    case 60: { /* <optional_indicators> =; */
#line 374 "alan.pmk"
 pmSeSt[pmStkP+1].string = malloc(1); strcpy(pmSeSt[pmStkP+1].string, ""); 	break;}
    case 61: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 376 "alan.pmk"

	    pmSeSt[pmStkP+1].string = realloc(pmSeSt[pmStkP+1].string,
		strlen(pmSeSt[pmStkP+1].string) + strlen(pmSeSt[pmStkP+2].string) + 1);
	    strcat(pmSeSt[pmStkP+1].string, pmSeSt[pmStkP+2].string);
		break;}
    case 62: { /* <indicator> = '*'; */
#line 383 "alan.pmk"
 pmSeSt[pmStkP+1].string = "*"; 	break;}
    case 63: { /* <indicator> = '!'; */
#line 384 "alan.pmk"
 pmSeSt[pmStkP+1].string = "!"; 	break;}
    case 64: { /* <optional_class_restrictions> = '.'; */
#line 388 "alan.pmk"
 print("."); 	break;}
    case 65: { /* <optional_class_restrictions> = 'WHERE' __genSym#7 <class_restrictions>; */
#line 390 "alan.pmk"
 outdent(); outdent(); newline(); 	break;}
    case 66: { /* __genSym#7 =; */
#line 389 "alan.pmk"
 indent(); newline(); print("Where "); indent(); 	break;}
    case 69: { /* __genSym#8 =; */
#line 395 "alan.pmk"
 outdent(); newline(); print("And "); indent(); 	break;}
    case 70: { /* <class_restriction> = <class_restriction1> <class_restriction2>; */
#line 399 "alan.pmk"
 outdent(); 	break;}
    case 71: { /* <class_restriction1> = ID 'ISA'; */
#line 403 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); print(" Isa "); 	break;}
    case 73: { /* __genSym#9 =; */
#line 407 "alan.pmk"
 newline(); print("Else "); indent(); 	break;}
    case 74: { /* <classes> = <class_identifier>; */
#line 411 "alan.pmk"
 print(pmSeSt[pmStkP+1].string); 	break;}
    case 75: { /* <classes> = <classes> 'OR' <class_identifier>; */
#line 412 "alan.pmk"
 print(" ****OR**** "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 76: { /* <class_identifier> = 'OBJECT'; */
#line 416 "alan.pmk"
 pmSeSt[pmStkP+1].string = "object"; 	break;}
    case 77: { /* <class_identifier> = 'ACTOR'; */
#line 417 "alan.pmk"
 pmSeSt[pmStkP+1].string = "actor"; 	break;}
    case 78: { /* <class_identifier> = 'CONTAINER'; */
#line 418 "alan.pmk"
 pmSeSt[pmStkP+1].string = "container"; 	break;}
    case 81: { /* <class_identifier> = 'CONTAINER' 'OBJECT'; */
#line 419 "alan.pmk"
 pmSeSt[pmStkP+1].string = "**** container object ****"; 	break;}
    case 82: { /* <class_identifier> = 'CONTAINER' 'ACTOR'; */
#line 420 "alan.pmk"
 pmSeSt[pmStkP+1].string = "**** container actor ****"; 	break;}
    case 79: { /* <class_identifier> = 'INTEGER'; */
#line 421 "alan.pmk"
 pmSeSt[pmStkP+1].string = "integer"; 	break;}
    case 80: { /* <class_identifier> = 'STRING'; */
#line 422 "alan.pmk"
 pmSeSt[pmStkP+1].string = "string"; 	break;}
    case 86: { /* <verb_header> = 'VERB' __genSym#10 <id_list>; */
#line 434 "alan.pmk"
 indent(); 	break;}
    case 87: { /* __genSym#10 =; */
#line 434 "alan.pmk"
 newline(); print("Verb "); 	break;}
    case 92: { /* <verb_alternative> = <verb_alternative1> <simple_verb_body>; */
#line 445 "alan.pmk"
 outdent(); 	break;}
    case 93: { /* <verb_alternative1> = 'WHEN' ID; */
#line 448 "alan.pmk"
 newline(); print("When "); idPrint(pmSeSt[pmStkP+2].string); indent(); 	break;}
    case 95: { /* <verb_tail> = 'END' 'VERB' __genSym#11 <optional_id> '.'; */
#line 454 "alan.pmk"
 print("."); newline(); 	break;}
    case 96: { /* __genSym#11 =; */
#line 454 "alan.pmk"
 outdent(); newline(); print("End Verb"); 	break;}
    case 98: { /* <optional_checks> = <check1> <statements>; */
#line 460 "alan.pmk"
 outdent(); 	break;}
    case 99: { /* <optional_checks> = <check1> <check_list>; */
#line 461 "alan.pmk"
 outdent(); 	break;}
    case 100: { /* <check1> = 'CHECK'; */
#line 465 "alan.pmk"
 newline(); print("Check "); indent(); 	break;}
    case 103: { /* __genSym#12 =; */
#line 469 "alan.pmk"
 outdent(); newline(); print("And "); indent(); 	break;}
    case 104: { /* <check> = <expression> 'ELSE' __genSym#13 <statements>; */
#line 472 "alan.pmk"
 outdent(); 	break;}
    case 105: { /* __genSym#13 =; */
#line 472 "alan.pmk"
 newline(); print("Else "); indent(); 	break;}
    case 108: { /* <does> = 'DOES' __genSym#14 <optional_qual> <statements>; */
#line 482 "alan.pmk"
 outdent(); 	break;}
    case 109: { /* __genSym#14 =; */
#line 482 "alan.pmk"
 newline(); print("Does"); indent(); 	break;}
    case 110: { /* <location> = <location_header> <location_body> <location_tail>; */
#line 487 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); print("."); 	break;}
    case 111: { /* <location_header> = <location_id> <optional_name>; */
#line 492 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 112: { /* <location_id> = 'LOCATION' ID; */
#line 497 "alan.pmk"

	newline(); newline();
	print("The "); idPrint(pmSeSt[pmStkP+2].string); print(" Isa location");
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 117: { /* <location_body_part> = __genSym#15 <is> __genSym#16 <attributes>; */
#line 511 "alan.pmk"
 outdent(); 	break;}
    case 120: { /* __genSym#15 =; */
#line 511 "alan.pmk"
 newline(); 	break;}
    case 121: { /* __genSym#16 =; */
#line 511 "alan.pmk"
 indent(); 	break;}
    case 122: { /* <location_does> = 'DOES' __genSym#17 <optional_qual> <statements>; */
#line 519 "alan.pmk"
 outdent(); 	break;}
    case 123: { /* __genSym#17 =; */
#line 519 "alan.pmk"
 newline(); print("Entered"); indent(); 	break;}
    case 124: { /* <location_tail> = 'END' 'LOCATION' <optional_id> '.'; */
#line 524 "alan.pmk"
 outdent(); newline(); print("End The "); 	break;}
    case 127: { /* <exit> = 'EXIT' __genSym#18 <id_list> <to_id> <optional_exit_body> '.'; */
#line 534 "alan.pmk"
 print("."); newline(); 	break;}
    case 128: { /* __genSym#18 =; */
#line 533 "alan.pmk"
 newline(); print("Exit "); 	break;}
    case 129: { /* <to_id> = 'TO' ID; */
#line 537 "alan.pmk"
 print(" To "); idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 131: { /* <optional_exit_body> = __genSym#19 <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 542 "alan.pmk"
 outdent(); newline(); print("End Exit"); if (pmSeSt[pmStkP+6].string[0] != '\0') {print(" "); idPrint(pmSeSt[pmStkP+6].string); } 	break;}
    case 132: { /* __genSym#19 =; */
#line 541 "alan.pmk"
 indent(); 	break;}
    case 133: { /* <object> = <object_header> <object_body> <object_tail>; */
#line 548 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); print("."); 	break;}
    case 134: { /* <object_header> = <object_id> <optional_where> <optional_names> <optional_where>; */
#line 553 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string;	break;}
    case 135: { /* <object_id> = 'OBJECT' ID; */
#line 558 "alan.pmk"

	newline(); newline(); print("The "); idPrint(pmSeSt[pmStkP+2].string); print(" Isa object");
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 142: { /* <object_body_part> = __genSym#20 <is> __genSym#21 <attributes>; */
#line 572 "alan.pmk"
 outdent(); 	break;}
    case 144: { /* __genSym#20 =; */
#line 572 "alan.pmk"
 newline(); 	break;}
    case 145: { /* __genSym#21 =; */
#line 572 "alan.pmk"
 indent(); 	break;}
    case 146: { /* <object_tail> = 'END' 'OBJECT' <optional_id> '.'; */
#line 581 "alan.pmk"
 outdent(); newline(); print("End The "); 	break;}
    case 148: { /* <optional_attributes> = <optional_attributes> <is> __genSym#22 <attributes>; */
#line 589 "alan.pmk"
 outdent(); 	break;}
    case 149: { /* __genSym#22 =; */
#line 589 "alan.pmk"
 indent(); newline(); 	break;}
    case 150: { /* <is> = 'IS'; */
#line 592 "alan.pmk"
 print("Is "); 	break;}
    case 151: { /* <is> = 'ARE'; */
#line 593 "alan.pmk"
 print("Are "); 	break;}
    case 152: { /* <is> = 'HAS'; */
#line 594 "alan.pmk"
 print("Has "); 	break;}
    case 155: { /* <description> = 'DESCRIPTION'; */
#line 602 "alan.pmk"
 newline(); print("Description"); newline(); 	break;}
    case 156: { /* <description> = 'DESCRIPTION' __genSym#23 <statements>; */
#line 603 "alan.pmk"
 outdent(); newline(); 	break;}
    case 157: { /* __genSym#23 =; */
#line 603 "alan.pmk"
 newline(); print("Description"); indent(); 	break;}
    case 159: { /* <article> = 'ARTICLE' __genSym#24 <statements>; */
#line 607 "alan.pmk"
 outdent(); newline(); 	break;}
    case 160: { /* __genSym#24 =; */
#line 607 "alan.pmk"
 newline(); print("Article"); indent(); 	break;}
    case 161: { /* <mentioned> = 'MENTIONED' __genSym#25 <statements>; */
#line 610 "alan.pmk"
 outdent(); newline(); 	break;}
    case 162: { /* __genSym#25 =; */
#line 610 "alan.pmk"
 newline(); print("Mentioned"); indent(); 	break;}
    case 167: { /* <name> = 'NAME' __genSym#26 <ids>; */
#line 621 "alan.pmk"
 newline(); 	break;}
    case 168: { /* __genSym#26 =; */
#line 621 "alan.pmk"
 print("Name "); 	break;}
    case 169: { /* <properties> = 'CONTAINER' __genSym#27 <container_body>; */
#line 626 "alan.pmk"
 outdent(); 	break;}
    case 170: { /* __genSym#27 =; */
#line 626 "alan.pmk"
 newline(); print("With Container"); indent(); 	break;}
    case 171: { /* <container> = <container_header> <container_body> <container_tail>; */
#line 630 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); print("."); newline(); 	break;}
    case 172: { /* <container_header> = 'CONTAINER' ID; */
#line 633 "alan.pmk"
 newline(); print("The "); idPrint(pmSeSt[pmStkP+2].string);
			indent(); newline();
			print("Container");
			pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string; indent(); 	break;}
    case 174: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */
#line 643 "alan.pmk"
 outdent(); outdent(); newline(); print("End The "); 	break;}
    case 176: { /* <optional_limits> = 'LIMITS' __genSym#28 <limits>; */
#line 647 "alan.pmk"
 outdent(); 	break;}
    case 177: { /* __genSym#28 =; */
#line 647 "alan.pmk"
 newline(); print("Limits"); indent(); 	break;}
    case 180: { /* <limit> = <limit_attribute> 'THEN' __genSym#29 <statements>; */
#line 654 "alan.pmk"
 outdent(); outdent(); 	break;}
    case 181: { /* __genSym#29 =; */
#line 654 "alan.pmk"
 indent(); newline(); print("Else"); indent(); 	break;}
    case 183: { /* <limit_attribute> = 'COUNT' Integer; */
#line 658 "alan.pmk"
 newline(); print("Count "); print(pmSySt[pmStkP+2].chars); 	break;}
    case 185: { /* <optional_header> = 'HEADER' __genSym#30 <statements>; */
#line 662 "alan.pmk"
 outdent(); 	break;}
    case 186: { /* __genSym#30 =; */
#line 662 "alan.pmk"
 newline(); print("Header"); indent(); 	break;}
    case 188: { /* <optional_empty> = 'ELSE' __genSym#31 <statements>; */
#line 666 "alan.pmk"
 outdent(); 	break;}
    case 189: { /* __genSym#31 =; */
#line 666 "alan.pmk"
 newline(); print("Else"); indent(); 	break;}
    case 190: { /* <event> = <event_header> __genSym#32 <statements> <event_tail>; */
#line 671 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); print("."); newline(); 	break;}
    case 191: { /* __genSym#32 =; */
#line 671 "alan.pmk"
 indent(); 	break;}
    case 192: { /* <event_header> = 'EVENT' ID; */
#line 675 "alan.pmk"
 newline(); print("Event "); idPrint(pmSeSt[pmStkP+2].string);
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string; 	break;}
    case 193: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 680 "alan.pmk"
 outdent(); newline(); print("End Event "); 	break;}
    case 194: { /* <actor> = <actor_header> <actor_body> <actor_tail>; */
#line 686 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); print("."); newline(); 	break;}
    case 195: { /* <actor_header> = <actor_id> <optional_where> <optional_names> <optional_where>; */
#line 691 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 196: { /* <actor_id> = 'ACTOR' ID; */
#line 696 "alan.pmk"

	newline(); newline(); print("The "); idPrint(pmSeSt[pmStkP+2].string); print(" Isa actor");
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 201: { /* <actor_body_part> = __genSym#33 <is> __genSym#34 <attributes>; */
#line 710 "alan.pmk"
 outdent(); 	break;}
    case 204: { /* __genSym#33 =; */
#line 710 "alan.pmk"
 newline(); 	break;}
    case 205: { /* __genSym#34 =; */
#line 710 "alan.pmk"
 indent(); 	break;}
    case 206: { /* <actor_tail> = 'END' 'ACTOR' <optional_id> '.'; */
#line 717 "alan.pmk"
 outdent(); newline(); print("End The "); 	break;}
    case 210: { /* <script1> = 'SCRIPT' <integer_or_id> '.'; */
#line 732 "alan.pmk"
 newline(); print("Script "); print(pmSeSt[pmStkP+2].string); print("."); indent(); 	break;}
    case 211: { /* <script2> = <optional_description> <step_list>; */
#line 736 "alan.pmk"
 outdent(); 	break;}
    case 214: { /* <step> = 'STEP' __genSym#35 <statements>; */
#line 745 "alan.pmk"
 outdent(); 	break;}
    case 217: { /* __genSym#35 =; */
#line 745 "alan.pmk"
 newline(); print("Step"); indent(); 	break;}
    case 215: { /* <step> = <step_after_integer> <statements>; */
#line 746 "alan.pmk"
 outdent(); 	break;}
    case 216: { /* <step> = 'STEP' 'WAIT' 'UNTIL' __genSym#36 <expression> <statements>; */
#line 747 "alan.pmk"
 outdent(); 	break;}
    case 218: { /* __genSym#36 =; */
#line 747 "alan.pmk"
 newline(); print("Step Wait Until "); indent(); 	break;}
    case 219: { /* <step_after_integer> = 'STEP' 'AFTER' Integer; */
#line 751 "alan.pmk"
 newline(); print("Step After "); print(pmSySt[pmStkP+3].chars); indent(); 	break;}
    case 220: { /* <rule> = 'WHEN' __genSym#37 <expression> '=>' __genSym#38 <statements>; */
#line 756 "alan.pmk"
 outdent(); newline(); 	break;}
    case 221: { /* __genSym#37 =; */
#line 756 "alan.pmk"
 newline(); print("When "); 	break;}
    case 222: { /* __genSym#38 =; */
#line 756 "alan.pmk"
 print(" Then"); indent(); 	break;}
    case 224: { /* __genSym#39 =; */
#line 761 "alan.pmk"
 newline(); newline(); print("Start "); 	break;}
    case 225: { /* __genSym#40 =; */
#line 761 "alan.pmk"
 print("."); indent(); 	break;}
    case 237: { /* <fullstop> = '.'; */
#line 786 "alan.pmk"
 print("."); 	break;}
    case 238: { /* <output_statement> = STRING; */
#line 791 "alan.pmk"
 stringPrint(pmSySt[pmStkP+1].chars); 	break;}
    case 242: { /* __genSym#41 =; */
#line 792 "alan.pmk"
 newline(); print("Describe "); 	break;}
    case 243: { /* __genSym#42 =; */
#line 793 "alan.pmk"
 newline(); print("Say "); 	break;}
    case 244: { /* __genSym#43 =; */
#line 794 "alan.pmk"
 newline(); print("List "); 	break;}
    case 245: { /* <special_statement> = 'QUIT' '.'; */
#line 798 "alan.pmk"
 newline(); print("Quit."); 	break;}
    case 246: { /* <special_statement> = 'LOOK' '.'; */
#line 799 "alan.pmk"
 newline(); print("Look."); 	break;}
    case 247: { /* <special_statement> = 'SAVE' '.'; */
#line 800 "alan.pmk"
 newline(); print("Save."); 	break;}
    case 248: { /* <special_statement> = 'RESTORE' '.'; */
#line 801 "alan.pmk"
 newline(); print("Restore."); 	break;}
    case 249: { /* <special_statement> = 'RESTART' '.'; */
#line 802 "alan.pmk"
 newline(); print("Restart."); 	break;}
    case 250: { /* <special_statement> = 'SCORE' __genSym#44 <optional_integer> '.'; */
#line 803 "alan.pmk"
 print("."); 	break;}
    case 253: { /* __genSym#44 =; */
#line 803 "alan.pmk"
 newline(); print("Score "); 	break;}
    case 251: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 804 "alan.pmk"
 newline(); print("Visits "); print(pmSySt[pmStkP+2].chars); print("."); 	break;}
    case 252: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 805 "alan.pmk"
 newline(); print("****System**** "); stringPrint(pmSySt[pmStkP+2].chars); 	break;}
    case 256: { /* __genSym#45 =; */
#line 811 "alan.pmk"
 newline(); print("Empty "); 	break;}
    case 257: { /* __genSym#46 =; */
#line 811 "alan.pmk"
 print(" "); 	break;}
    case 258: { /* __genSym#47 =; */
#line 812 "alan.pmk"
 newline(); print("Locate "); 	break;}
    case 259: { /* __genSym#48 =; */
#line 812 "alan.pmk"
 print(" "); 	break;}
    case 262: { /* __genSym#49 =; */
#line 817 "alan.pmk"
 print("After "); 	break;}
    case 260: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 819 "alan.pmk"
 newline(); print("Cancel "); idPrint(pmSeSt[pmStkP+2].string); print("."); 	break;}
    case 263: { /* <schedule1> = 'SCHEDULE' ID; */
#line 823 "alan.pmk"
 newline(); print("Schedule "); idPrint(pmSeSt[pmStkP+2].string); print(" "); 	break;}
    case 268: { /* __genSym#50 =; */
#line 828 "alan.pmk"
 newline(); print("Make "); 	break;}
    case 269: { /* __genSym#51 =; */
#line 828 "alan.pmk"
 print(" "); 	break;}
    case 270: { /* __genSym#52 =; */
#line 829 "alan.pmk"
 newline(); print("Set "); 	break;}
    case 271: { /* __genSym#53 =; */
#line 829 "alan.pmk"
 print(" To "); 	break;}
    case 272: { /* __genSym#54 =; */
#line 830 "alan.pmk"
 newline(); print("Increase "); 	break;}
    case 273: { /* __genSym#55 =; */
#line 831 "alan.pmk"
 newline(); print("Decrease "); 	break;}
    case 276: { /* __genSym#56 =; */
#line 836 "alan.pmk"
 print(" By "); 	break;}
    case 279: { /* <if_statement> = 'IF' __genSym#57 <expression> 'THEN' __genSym#58 <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 849 "alan.pmk"
 outdent(); newline(); print("End If."); 	break;}
    case 280: { /* __genSym#57 =; */
#line 846 "alan.pmk"
 newline(); print("If "); 	break;}
    case 281: { /* __genSym#58 =; */
#line 847 "alan.pmk"
 print(" Then"); indent(); 	break;}
    case 287: { /* __genSym#59 =; */
#line 863 "alan.pmk"
 outdent(); newline(); print("Elsif "); 	break;}
    case 288: { /* __genSym#60 =; */
#line 864 "alan.pmk"
 print(" Then "); indent(); 	break;}
    case 291: { /* __genSym#61 =; */
#line 870 "alan.pmk"
 outdent(); newline(); print("Else "); indent(); 	break;}
    case 292: { /* <depending_statement> = 'DEPENDING' 'ON' __genSym#62 <primary> __genSym#63 <depend_cases> 'END' 'DEPEND' '.'; */
#line 878 "alan.pmk"
 outdent(); newline(); print("End Depend."); 	break;}
    case 293: { /* __genSym#62 =; */
#line 875 "alan.pmk"
 newline(); print("Depending On "); 	break;}
    case 294: { /* __genSym#63 =; */
#line 876 "alan.pmk"
 indent(); newline(); 	break;}
    case 297: { /* __genSym#64 =; */
#line 883 "alan.pmk"
 newline(); 	break;}
    case 299: { /* <depend_case> = <right_hand_side> ':' __genSym#65 <statements>; */
#line 887 "alan.pmk"
 outdent(); 	break;}
    case 300: { /* __genSym#65 =; */
#line 887 "alan.pmk"
 print(" Then "); indent(); 	break;}
    case 298: { /* <depend_case> = 'ELSE' __genSym#66 <statements>; */
#line 888 "alan.pmk"
 outdent(); 	break;}
    case 301: { /* __genSym#66 =; */
#line 888 "alan.pmk"
 print("Else "); indent(); 	break;}
    case 303: { /* __genSym#67 =; */
#line 893 "alan.pmk"
 newline(); print("Use Script "); 	break;}
    case 304: { /* <script_reference> = <integer_or_id>; */
#line 897 "alan.pmk"
 print(pmSeSt[pmStkP+1].string); 	break;}
    case 306: { /* <optional_for_actor> = 'FOR' ID; */
#line 902 "alan.pmk"
 print(" For "); idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 309: { /* __genSym#68 =; */
#line 908 "alan.pmk"
 print(" Or "); 	break;}
    case 312: { /* __genSym#69 =; */
#line 913 "alan.pmk"
 print(" And "); 	break;}
    case 315: { /* __genSym#70 =; */
#line 918 "alan.pmk"
 print(" "); 	break;}
    case 322: { /* __genSym#71 =; */
#line 926 "alan.pmk"
 print(" Between "); 	break;}
    case 323: { /* __genSym#72 =; */
#line 926 "alan.pmk"
 print(" And "); 	break;}
    case 324: { /* __genSym#73 =; */
#line 927 "alan.pmk"
 print(" Contains "); 	break;}
    case 330: { /* <primary> = '(' __genSym#74 <expression> ')'; */
#line 931 "alan.pmk"
 print(")"); 	break;}
    case 333: { /* __genSym#74 =; */
#line 931 "alan.pmk"
 print("("); 	break;}
    case 325: { /* <primary> = <optional_minus> Integer; */
#line 932 "alan.pmk"
 print(pmSeSt[pmStkP+1].string); print(pmSySt[pmStkP+2].chars); 	break;}
    case 326: { /* <primary> = STRING; */
#line 933 "alan.pmk"
 stringPrint(pmSySt[pmStkP+1].chars); 	break;}
    case 334: { /* __genSym#75 =; */
#line 936 "alan.pmk"
 print("Isa object, "); 	break;}
    case 335: { /* __genSym#76 =; */
#line 937 "alan.pmk"
 print("Random "); 	break;}
    case 336: { /* __genSym#77 =; */
#line 937 "alan.pmk"
 print(" To "); 	break;}
    case 328: { /* <primary> = 'SCORE'; */
#line 938 "alan.pmk"
 print("Score"); 	break;}
    case 338: { /* <aggregate> = 'SUM' 'OF' ID; */
#line 942 "alan.pmk"
 print(" (Sum Of "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 339: { /* <aggregate> = 'MAX' 'OF' ID; */
#line 943 "alan.pmk"
 print(" (Max Of "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 337: { /* <aggregate> = 'COUNT'; */
#line 944 "alan.pmk"
 print(" Count "); 	break;}
    case 340: { /* <something> = <optional_not> ID; */
#line 949 "alan.pmk"
 idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 341: { /* <what> = 'OBJECT'; */
#line 953 "alan.pmk"
 print("**** object ****"); 	break;}
    case 342: { /* <what> = 'LOCATION'; */
#line 954 "alan.pmk"
 print("Current Location"); 	break;}
    case 343: { /* <what> = 'ACTOR'; */
#line 955 "alan.pmk"
 print("Current Actor"); 	break;}
    case 344: { /* <what> = ID; */
#line 956 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 346: { /* <optional_where> = <where>; */
#line 962 "alan.pmk"
 print(" "); 	break;}
    case 347: { /* <where> = 'HERE'; */
#line 966 "alan.pmk"
 print("Here"); 	break;}
    case 348: { /* <where> = 'NEARBY'; */
#line 967 "alan.pmk"
 print("Nearby"); 	break;}
    case 351: { /* __genSym#78 =; */
#line 968 "alan.pmk"
 print("At "); 	break;}
    case 352: { /* __genSym#79 =; */
#line 969 "alan.pmk"
 print("In "); 	break;}
    case 353: { /* <binop> = '+'; */
#line 973 "alan.pmk"
 print("+"); 	break;}
    case 354: { /* <binop> = '-'; */
#line 974 "alan.pmk"
 print("-"); 	break;}
    case 355: { /* <binop> = '*'; */
#line 975 "alan.pmk"
 print("*"); 	break;}
    case 356: { /* <binop> = '/'; */
#line 976 "alan.pmk"
 print("/"); 	break;}
    case 357: { /* <relop> = '<>'; */
#line 980 "alan.pmk"
 print("<>"); 	break;}
    case 358: { /* <relop> = '='; */
#line 981 "alan.pmk"
 print("="); 	break;}
    case 359: { /* <relop> = '=='; */
#line 982 "alan.pmk"
 print("=="); 	break;}
    case 360: { /* <relop> = '>='; */
#line 983 "alan.pmk"
 print(">="); 	break;}
    case 361: { /* <relop> = '<='; */
#line 984 "alan.pmk"
 print("<="); 	break;}
    case 362: { /* <relop> = '>'; */
#line 985 "alan.pmk"
 print(">"); 	break;}
    case 363: { /* <relop> = '<'; */
#line 986 "alan.pmk"
 print("<"); 	break;}
    case 365: { /* <optional_qual> = 'BEFORE'; */
#line 991 "alan.pmk"
 print(" Before"); 	break;}
    case 366: { /* <optional_qual> = 'AFTER'; */
#line 992 "alan.pmk"
 print(" After"); 	break;}
    case 367: { /* <optional_qual> = 'ONLY'; */
#line 993 "alan.pmk"
 print(" Only"); 	break;}
    case 369: { /* <optional_not> = 'NOT'; */
#line 998 "alan.pmk"
 print("Not "); 	break;}
    case 370: { /* <optional_id> =; */
#line 1002 "alan.pmk"
 pmSeSt[pmStkP+1].string = ""; 	break;}
    case 371: { /* <optional_id> = ID; */
#line 1003 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 372: { /* <ids> = ID; */
#line 1007 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 373: { /* <ids> = <ids> ID; */
#line 1008 "alan.pmk"
 print(" "); idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 374: { /* <id_list> = ID; */
#line 1012 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 375: { /* <id_list> = <id_list> ',' ID; */
#line 1013 "alan.pmk"
 print(", "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 377: { /* <optional_integer> = Integer; */
#line 1018 "alan.pmk"
 print(pmSySt[pmStkP+1].chars); 	break;}
    case 378: { /* <optional_minus> =; */
#line 1022 "alan.pmk"
 pmSeSt[pmStkP+1].string = ""; 	break;}
    case 379: { /* <optional_minus> = '-'; */
#line 1023 "alan.pmk"
 pmSeSt[pmStkP+1].string = "-"; 	break;}
    case 381: { /* <id_of> = ID 'OF'; */
#line 1031 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); print(" Of "); 	break;}
    case 382: { /* <integer_or_id> = Integer; */
#line 1035 "alan.pmk"
 pmSeSt[pmStkP+1].string = malloc(strlen(pmSySt[pmStkP+1].chars)+2);
		sprintf(pmSeSt[pmStkP+1].string, "s%s", pmSySt[pmStkP+1].chars); 	break;}
    case 383: { /* <integer_or_id> = ID; */
#line 1037 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 384: { /* ID = IDENT; */
#line 1041 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSySt[pmStkP+1].chars; 	break;}
    case 385: { /* ID = 'DEFAULT'; */
#line 1042 "alan.pmk"
 pmSeSt[pmStkP+1].string = "default"; 	break;}
    case 386: { /* ID = 'ARTICLE'; */
#line 1043 "alan.pmk"
 pmSeSt[pmStkP+1].string = "article"; 	break;}
    case 387: { /* ID = 'MESSAGE'; */
#line 1044 "alan.pmk"
 pmSeSt[pmStkP+1].string = "message"; 	break;}
    case 388: { /* ID = 'QUIT'; */
#line 1045 "alan.pmk"
 pmSeSt[pmStkP+1].string = "quit"; 	break;}
    case 389: { /* ID = 'SAVE'; */
#line 1046 "alan.pmk"
 pmSeSt[pmStkP+1].string = "save"; 	break;}
    case 390: { /* ID = 'RESTORE'; */
#line 1047 "alan.pmk"
 pmSeSt[pmStkP+1].string = "restore"; 	break;}
    case 391: { /* ID = 'RESTART'; */
#line 1048 "alan.pmk"
 pmSeSt[pmStkP+1].string = "restart"; 	break;}
    case 392: { /* ID = 'WAIT'; */
#line 1049 "alan.pmk"
 pmSeSt[pmStkP+1].string = "wait"; 	break;}
    case 393: { /* ID = 'BETWEEN'; */
#line 1050 "alan.pmk"
 pmSeSt[pmStkP+1].string = "between"; 	break;}
    case 394: { /* ID = 'CONTAINS'; */
#line 1051 "alan.pmk"
 pmSeSt[pmStkP+1].string = "contains"; 	break;}
    case 395: { /* ID = 'ON'; */
#line 1052 "alan.pmk"
 pmSeSt[pmStkP+1].string = "on"; 	break;}
    case 396: { /* ID = 'IN'; */
#line 1053 "alan.pmk"
 pmSeSt[pmStkP+1].string = "in"; 	break;}
    case 397: { /* ID = 'AFTER'; */
#line 1054 "alan.pmk"
 pmSeSt[pmStkP+1].string = "after"; 	break;}
    case 398: { /* ID = 'BEFORE'; */
#line 1055 "alan.pmk"
 pmSeSt[pmStkP+1].string = "before"; 	break;}
    case 399: { /* ID = 'CHECK'; */
#line 1056 "alan.pmk"
 pmSeSt[pmStkP+1].string = "check"; 	break;}
    case 400: { /* ID = 'DEPEND'; */
#line 1057 "alan.pmk"
 pmSeSt[pmStkP+1].string = "depend"; 	break;}
    case 401: { /* ID = 'EXIT'; */
#line 1058 "alan.pmk"
 pmSeSt[pmStkP+1].string = "exit"; 	break;}
    case 402: { /* ID = 'FOR'; */
#line 1059 "alan.pmk"
 pmSeSt[pmStkP+1].string = "for"; 	break;}
    case 403: { /* ID = 'INTEGER'; */
#line 1060 "alan.pmk"
 pmSeSt[pmStkP+1].string = "integer"; 	break;}
    case 404: { /* ID = 'ISA'; */
#line 1061 "alan.pmk"
 pmSeSt[pmStkP+1].string = "isa"; 	break;}
    case 405: { /* ID = 'LIMITS'; */
#line 1062 "alan.pmk"
 pmSeSt[pmStkP+1].string = "limits"; 	break;}
    default: break; }
}/*pmPaSema()*/

