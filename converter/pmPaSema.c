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
  "container", "wait", "score", "save", "restore", "restart", "at", "of",
  "to", "show", "use", "empty", "for", "each", "set", "start", "the", NULL};


static void idPrint(char id[])
{
  int wordIndex = 0;

  while (v3Keywords[wordIndex] != NULL && strcmp(id, v3Keywords[wordIndex]) != 0)
    wordIndex++;
  if (v3Keywords[wordIndex] != NULL) fprintf(outFile, "'%s'", id);
  else fprintf(outFile, id);
}

static void onlyOneSpace(char string[])
{
  int i = 0;
  int j;

  while (string[i] != '\0') {
    while (string[i] != '\0' && !isspace(string[i])) i++;
    if (string[i] != '\0' && string[i+1] != '\0')
      string[i] = ' ';
      if (isspace(string[i+1])) {
	string[i+1] = ' ';
        j = i+1;
        while (string[j] != '\0' && isspace(string[j])) j++;
        if (string[j] == '\0')
	  string[i] = '\0';
        else {
          strcpy(&string[i+1], &string[j]);
	  i++;
	}
      }
    if (string[i] != '\0') i++;
  }
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
#line 151 "alan.pmk"
    switch (rule) {
    case 3: { /* <optional_options> = 'OPTIONS' __genSym#0 <options>; */
#line 164 "alan.pmk"
 outdent(); newline(); newline(); 	break;}
    case 4: { /* __genSym#0 =; */
#line 162 "alan.pmk"
 fprintf(outFile, "Options"); indent(); newline(); 	break;}
    case 5: { /* <options> = <option>; */
#line 168 "alan.pmk"
 newline(); 	break;}
    case 7: { /* <option> = ID '.'; */
#line 173 "alan.pmk"

	fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string);
    	break;}
    case 8: { /* <option> = ID ID '.'; */
#line 177 "alan.pmk"

	fprintf(outFile, "%s %s.", pmSeSt[pmStkP+1].string, pmSeSt[pmStkP+2].string);
    	break;}
    case 9: { /* <option> = ID Integer '.'; */
#line 181 "alan.pmk"

	fprintf(outFile, "%s %d.", pmSeSt[pmStkP+1].string, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 26: { /* <default> = 'DEFAULT' __genSym#1 'ATTRIBUTES' <attributes>; */
#line 220 "alan.pmk"

	outdent();
	outdent();
	newline();
	fprintf(outFile, "End Add To.");
	newline();
    	break;}
    case 27: { /* __genSym#1 =; */
#line 211 "alan.pmk"

	newline();
	fprintf(outFile, "Add To Every thing");
	indent();
	newline();
	fprintf(outFile, "Is");
	indent();
    	break;}
    case 28: { /* <location_default> = 'LOCATION' __genSym#2 'ATTRIBUTES' <attributes>; */
#line 241 "alan.pmk"

	outdent();
	outdent();
	newline();
	fprintf(outFile, "End Add To.");
	newline();
    	break;}
    case 29: { /* __genSym#2 =; */
#line 232 "alan.pmk"

	newline();
	fprintf(outFile, "Add To Every location");
	indent();
	newline();
	fprintf(outFile, "Is");
	indent();
    	break;}
    case 30: { /* <object_default> = 'OBJECT' __genSym#3 'ATTRIBUTES' <attributes>; */
#line 262 "alan.pmk"

	outdent();
	outdent();
	newline();
	fprintf(outFile, "End Add To.");
	newline();
    	break;}
    case 31: { /* __genSym#3 =; */
#line 253 "alan.pmk"

	newline();
	fprintf(outFile, "Add To Every object");
	indent();
	newline();
	fprintf(outFile, "Is");
	indent();
    	break;}
    case 32: { /* <actor_default> = 'ACTOR' __genSym#4 'ATTRIBUTES' <attributes>; */
#line 283 "alan.pmk"

	outdent();
	outdent();
	newline();
	fprintf(outFile, "End Add To.");
	newline();
    	break;}
    case 33: { /* __genSym#4 =; */
#line 274 "alan.pmk"

	newline();
	fprintf(outFile, "Add To Every actor");
	indent();
	newline();
	fprintf(outFile, "Is");
	indent();
    	break;}
    case 34: { /* <attributes> = <attribute> '.'; */
#line 295 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 35: { /* <attributes> = <attributes> <attribute> '.'; */
#line 296 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 36: { /* <attribute> = ID; */
#line 300 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 37: { /* <attribute> = 'NOT' ID; */
#line 301 "alan.pmk"
 newline(); fprintf(outFile, "Not "); idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 38: { /* <attribute> = ID <optional_minus> Integer; */
#line 302 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); fprintf(outFile, " %s%s", pmSeSt[pmStkP+2].string, pmSySt[pmStkP+3].chars); 	break;}
    case 39: { /* <attribute> = ID STRING; */
#line 303 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); fprintf(outFile, " "); stringPrint(pmSySt[pmStkP+2].chars); 	break;}
    case 40: { /* <synonyms> = 'SYNONYMS' __genSym#5 <synonym_list>; */
#line 309 "alan.pmk"
 outdent(); newline(); 	break;}
    case 41: { /* __genSym#5 =; */
#line 308 "alan.pmk"
 newline(); fprintf(outFile, "Synonyms"); indent(); newline(); 	break;}
    case 44: { /* <synonym> = <id_list> '=' ID '.'; */
#line 316 "alan.pmk"
 fprintf(outFile, " = "); idPrint(pmSeSt[pmStkP+3].string); fprintf(outFile, "."); newline(); 	break;}
    case 49: { /* <syntax> = 'SYNTAX' __genSym#6 <syntax_list>; */
#line 334 "alan.pmk"
 outdent(); newline(); 	break;}
    case 50: { /* __genSym#6 =; */
#line 333 "alan.pmk"
 newline(); fprintf(outFile, "Syntax"); indent(); 	break;}
    case 54: { /* <syntax_item1> = ID '='; */
#line 344 "alan.pmk"
 newline(); idPrint(pmSeSt[pmStkP+1].string); fprintf(outFile, " ="); 	break;}
    case 58: { /* <syntax_element> = ID; */
#line 354 "alan.pmk"
 fprintf(outFile, " "); idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 59: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 355 "alan.pmk"
 fprintf(outFile, " ("); idPrint(pmSeSt[pmStkP+2].string); fprintf(outFile, ")"); fprintf(outFile, pmSeSt[pmStkP+4].string); 	break;}
    case 60: { /* <optional_indicators> =; */
#line 358 "alan.pmk"
 pmSeSt[pmStkP+1].string = malloc(1); strcpy(pmSeSt[pmStkP+1].string, ""); 	break;}
    case 61: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 360 "alan.pmk"

	    pmSeSt[pmStkP+1].string = realloc(pmSeSt[pmStkP+1].string,
		strlen(pmSeSt[pmStkP+1].string) + strlen(pmSeSt[pmStkP+2].string) + 1);
	    strcat(pmSeSt[pmStkP+1].string, pmSeSt[pmStkP+2].string);
		break;}
    case 62: { /* <indicator> = '*'; */
#line 367 "alan.pmk"
 pmSeSt[pmStkP+1].string = "*"; 	break;}
    case 63: { /* <indicator> = '!'; */
#line 368 "alan.pmk"
 pmSeSt[pmStkP+1].string = "!"; 	break;}
    case 64: { /* <optional_class_restrictions> = '.'; */
#line 372 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 65: { /* <optional_class_restrictions> = 'WHERE' __genSym#7 <class_restrictions>; */
#line 374 "alan.pmk"
 outdent(); outdent(); newline(); 	break;}
    case 66: { /* __genSym#7 =; */
#line 373 "alan.pmk"
 indent(); newline(); fprintf(outFile, "Where "); indent(); 	break;}
    case 69: { /* __genSym#8 =; */
#line 379 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "And "); indent(); 	break;}
    case 70: { /* <class_restriction> = <class_restriction1> <class_restriction2>; */
#line 383 "alan.pmk"
 outdent(); 	break;}
    case 71: { /* <class_restriction1> = ID 'ISA'; */
#line 387 "alan.pmk"
 fprintf(outFile, "%s Isa ", pmSeSt[pmStkP+1].string); 	break;}
    case 73: { /* __genSym#9 =; */
#line 391 "alan.pmk"
 newline(); fprintf(outFile, "Else "); indent(); 	break;}
    case 74: { /* <classes> = <class_identifier>; */
#line 395 "alan.pmk"
 fprintf(outFile, pmSeSt[pmStkP+1].string); 	break;}
    case 75: { /* <classes> = <classes> 'OR' <class_identifier>; */
#line 396 "alan.pmk"
 fprintf(outFile, " -- ****OR**** %s", pmSeSt[pmStkP+3].string); 	break;}
    case 76: { /* <class_identifier> = 'OBJECT'; */
#line 400 "alan.pmk"
 pmSeSt[pmStkP+1].string = "object"; 	break;}
    case 77: { /* <class_identifier> = 'ACTOR'; */
#line 401 "alan.pmk"
 pmSeSt[pmStkP+1].string = "actor"; 	break;}
    case 78: { /* <class_identifier> = 'CONTAINER'; */
#line 402 "alan.pmk"
 pmSeSt[pmStkP+1].string = "container"; 	break;}
    case 81: { /* <class_identifier> = 'CONTAINER' 'OBJECT'; */
#line 403 "alan.pmk"
 pmSeSt[pmStkP+1].string = "**** container object ****"; 	break;}
    case 82: { /* <class_identifier> = 'CONTAINER' 'ACTOR'; */
#line 404 "alan.pmk"
 pmSeSt[pmStkP+1].string = "**** container actor ****"; 	break;}
    case 79: { /* <class_identifier> = 'INTEGER'; */
#line 405 "alan.pmk"
 pmSeSt[pmStkP+1].string = "integer"; 	break;}
    case 80: { /* <class_identifier> = 'STRING'; */
#line 406 "alan.pmk"
 pmSeSt[pmStkP+1].string = "string"; 	break;}
    case 86: { /* <verb_header> = 'VERB' __genSym#10 <id_list>; */
#line 418 "alan.pmk"
 indent(); 	break;}
    case 87: { /* __genSym#10 =; */
#line 418 "alan.pmk"
 newline(); fprintf(outFile, "Verb "); 	break;}
    case 92: { /* <verb_alternative> = <verb_alternative1> <simple_verb_body>; */
#line 429 "alan.pmk"
 outdent(); 	break;}
    case 93: { /* <verb_alternative1> = 'WHEN' ID; */
#line 432 "alan.pmk"
 newline(); fprintf(outFile, "When %s", pmSeSt[pmStkP+2].string); indent(); 	break;}
    case 95: { /* <verb_tail> = 'END' 'VERB' __genSym#11 <optional_id> '.'; */
#line 438 "alan.pmk"
 fprintf(outFile, "."); newline(); 	break;}
    case 96: { /* __genSym#11 =; */
#line 438 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End Verb"); 	break;}
    case 98: { /* <optional_checks> = <check1> <statements>; */
#line 444 "alan.pmk"
 outdent(); 	break;}
    case 99: { /* <optional_checks> = <check1> <check_list>; */
#line 445 "alan.pmk"
 outdent(); 	break;}
    case 100: { /* <check1> = 'CHECK'; */
#line 449 "alan.pmk"
 newline(); fprintf(outFile, "Check "); indent(); 	break;}
    case 103: { /* __genSym#12 =; */
#line 453 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "And "); indent(); 	break;}
    case 104: { /* <check> = <expression> 'ELSE' __genSym#13 <statements>; */
#line 456 "alan.pmk"
 outdent(); 	break;}
    case 105: { /* __genSym#13 =; */
#line 456 "alan.pmk"
 newline(); fprintf(outFile, "Else "); indent(); 	break;}
    case 108: { /* <does> = 'DOES' __genSym#14 <optional_qual> <statements>; */
#line 466 "alan.pmk"
 outdent(); 	break;}
    case 109: { /* __genSym#14 =; */
#line 466 "alan.pmk"
 newline(); fprintf(outFile, "Does"); indent(); 	break;}
    case 110: { /* <location> = <location_header> <location_body> <location_tail>; */
#line 471 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); 	break;}
    case 111: { /* <location_header> = <location_id> <optional_name>; */
#line 476 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 112: { /* <location_id> = 'LOCATION' ID; */
#line 481 "alan.pmk"

	newline(); newline();
	fprintf(outFile, "The %s Isa location" , pmSeSt[pmStkP+2].string);
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 117: { /* <location_body_part> = __genSym#15 <is> __genSym#16 <attributes>; */
#line 495 "alan.pmk"
 outdent(); 	break;}
    case 120: { /* __genSym#15 =; */
#line 495 "alan.pmk"
 newline(); 	break;}
    case 121: { /* __genSym#16 =; */
#line 495 "alan.pmk"
 indent(); 	break;}
    case 122: { /* <location_does> = 'DOES' __genSym#17 <optional_qual> <statements>; */
#line 503 "alan.pmk"
 outdent(); 	break;}
    case 123: { /* __genSym#17 =; */
#line 503 "alan.pmk"
 newline(); fprintf(outFile, "Entered"); indent(); 	break;}
    case 124: { /* <location_tail> = 'END' 'LOCATION' <optional_id> '.'; */
#line 508 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End The "); 	break;}
    case 127: { /* <exit> = 'EXIT' __genSym#18 <id_list> <to_id> <optional_exit_body> '.'; */
#line 518 "alan.pmk"
 fprintf(outFile, "."); newline(); 	break;}
    case 128: { /* __genSym#18 =; */
#line 517 "alan.pmk"
 newline(); fprintf(outFile, "Exit "); 	break;}
    case 129: { /* <to_id> = 'TO' ID; */
#line 521 "alan.pmk"
 fprintf(outFile, " To %s", pmSeSt[pmStkP+2].string); 	break;}
    case 131: { /* <optional_exit_body> = __genSym#19 <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 526 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End Exit%s%s", pmSeSt[pmStkP+6].string[0]?" ":"", pmSeSt[pmStkP+6].string); 	break;}
    case 132: { /* __genSym#19 =; */
#line 525 "alan.pmk"
 indent(); 	break;}
    case 133: { /* <object> = <object_header> <object_body> <object_tail>; */
#line 532 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); 	break;}
    case 134: { /* <object_header> = <object_id> <optional_where> <optional_names> <optional_where>; */
#line 537 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string;	break;}
    case 135: { /* <object_id> = 'OBJECT' ID; */
#line 542 "alan.pmk"

	newline(); newline(); fprintf(outFile, "The %s Isa object", pmSeSt[pmStkP+2].string);
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 142: { /* <object_body_part> = __genSym#20 <is> __genSym#21 <attributes>; */
#line 556 "alan.pmk"
 outdent(); 	break;}
    case 144: { /* __genSym#20 =; */
#line 556 "alan.pmk"
 newline(); 	break;}
    case 145: { /* __genSym#21 =; */
#line 556 "alan.pmk"
 indent(); 	break;}
    case 146: { /* <object_tail> = 'END' 'OBJECT' <optional_id> '.'; */
#line 565 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End The "); 	break;}
    case 148: { /* <optional_attributes> = <optional_attributes> <is> __genSym#22 <attributes>; */
#line 573 "alan.pmk"
 outdent(); 	break;}
    case 149: { /* __genSym#22 =; */
#line 573 "alan.pmk"
 indent(); newline(); 	break;}
    case 150: { /* <is> = 'IS'; */
#line 576 "alan.pmk"
 fprintf(outFile, "Is "); 	break;}
    case 151: { /* <is> = 'ARE'; */
#line 577 "alan.pmk"
 fprintf(outFile, "Are "); 	break;}
    case 152: { /* <is> = 'HAS'; */
#line 578 "alan.pmk"
 fprintf(outFile, "Has "); 	break;}
    case 155: { /* <description> = 'DESCRIPTION'; */
#line 586 "alan.pmk"
 newline(); fprintf(outFile, "Description"); newline(); 	break;}
    case 156: { /* <description> = 'DESCRIPTION' __genSym#23 <statements>; */
#line 587 "alan.pmk"
 outdent(); newline(); 	break;}
    case 157: { /* __genSym#23 =; */
#line 587 "alan.pmk"
 newline(); fprintf(outFile, "Description"); indent(); 	break;}
    case 159: { /* <article> = 'ARTICLE' __genSym#24 <statements>; */
#line 591 "alan.pmk"
 outdent(); newline(); 	break;}
    case 160: { /* __genSym#24 =; */
#line 591 "alan.pmk"
 newline(); fprintf(outFile, "Article"); indent(); 	break;}
    case 161: { /* <mentioned> = 'MENTIONED' __genSym#25 <statements>; */
#line 594 "alan.pmk"
 outdent(); newline(); 	break;}
    case 162: { /* __genSym#25 =; */
#line 594 "alan.pmk"
 newline(); fprintf(outFile, "Mentioned"); indent(); 	break;}
    case 167: { /* <name> = 'NAME' __genSym#26 <ids>; */
#line 605 "alan.pmk"
 newline(); 	break;}
    case 168: { /* __genSym#26 =; */
#line 605 "alan.pmk"
 fprintf(outFile, "Name "); 	break;}
    case 169: { /* <properties> = 'CONTAINER' __genSym#27 <container_body>; */
#line 610 "alan.pmk"
 outdent(); 	break;}
    case 170: { /* __genSym#27 =; */
#line 610 "alan.pmk"
 newline(); fprintf(outFile, "With Container"); indent(); 	break;}
    case 171: { /* <container> = <container_header> <container_body> <container_tail>; */
#line 614 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); newline(); 	break;}
    case 172: { /* <container_header> = 'CONTAINER' ID; */
#line 617 "alan.pmk"
 newline(); fprintf(outFile, "The %s", pmSeSt[pmStkP+2].string);
			indent(); newline();
			fprintf(outFile, "Container");
			pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string; indent(); 	break;}
    case 174: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */
#line 627 "alan.pmk"
 outdent(); outdent(); newline(); fprintf(outFile, "End The "); 	break;}
    case 176: { /* <optional_limits> = 'LIMITS' __genSym#28 <limits>; */
#line 631 "alan.pmk"
 outdent(); 	break;}
    case 177: { /* __genSym#28 =; */
#line 631 "alan.pmk"
 newline(); fprintf(outFile, "Limits"); indent(); 	break;}
    case 180: { /* <limit> = <limit_attribute> 'THEN' __genSym#29 <statements>; */
#line 638 "alan.pmk"
 outdent(); outdent(); 	break;}
    case 181: { /* __genSym#29 =; */
#line 638 "alan.pmk"
 indent(); newline(); fprintf(outFile, "Else"); indent(); 	break;}
    case 183: { /* <limit_attribute> = 'COUNT' Integer; */
#line 642 "alan.pmk"
 newline(); fprintf(outFile, "Count %s", pmSySt[pmStkP+2].chars); 	break;}
    case 185: { /* <optional_header> = 'HEADER' __genSym#30 <statements>; */
#line 646 "alan.pmk"
 outdent(); 	break;}
    case 186: { /* __genSym#30 =; */
#line 646 "alan.pmk"
 newline(); fprintf(outFile, "Header"); indent(); 	break;}
    case 188: { /* <optional_empty> = 'ELSE' __genSym#31 <statements>; */
#line 650 "alan.pmk"
 outdent(); 	break;}
    case 189: { /* __genSym#31 =; */
#line 650 "alan.pmk"
 newline(); fprintf(outFile, "Else"); indent(); 	break;}
    case 190: { /* <event> = <event_header> __genSym#32 <statements> <event_tail>; */
#line 655 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); newline(); 	break;}
    case 191: { /* __genSym#32 =; */
#line 655 "alan.pmk"
 indent(); 	break;}
    case 192: { /* <event_header> = 'EVENT' ID; */
#line 659 "alan.pmk"
 newline(); fprintf(outFile, "Event %s", pmSeSt[pmStkP+2].string);
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string; 	break;}
    case 193: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 664 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End Event "); 	break;}
    case 194: { /* <actor> = <actor_header> <actor_body> <actor_tail>; */
#line 670 "alan.pmk"
 fprintf(outFile, "%s.", pmSeSt[pmStkP+1].string); newline(); 	break;}
    case 195: { /* <actor_header> = <actor_id> <optional_where> <optional_names> <optional_where>; */
#line 675 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 196: { /* <actor_id> = 'ACTOR' ID; */
#line 680 "alan.pmk"

	newline(); newline(); fprintf(outFile, "The %s Isa actor", pmSeSt[pmStkP+2].string);
	indent(); newline();
	pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+2].string;
    	break;}
    case 201: { /* <actor_body_part> = __genSym#33 <is> __genSym#34 <attributes>; */
#line 694 "alan.pmk"
 outdent(); 	break;}
    case 204: { /* __genSym#33 =; */
#line 694 "alan.pmk"
 newline(); 	break;}
    case 205: { /* __genSym#34 =; */
#line 694 "alan.pmk"
 indent(); 	break;}
    case 206: { /* <actor_tail> = 'END' 'ACTOR' <optional_id> '.'; */
#line 701 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End The "); 	break;}
    case 210: { /* <script1> = 'SCRIPT' <integer_or_id> '.'; */
#line 716 "alan.pmk"
 newline(); fprintf(outFile, "Script %s", pmSeSt[pmStkP+2].string); indent(); 	break;}
    case 211: { /* <script2> = <optional_description> <step_list>; */
#line 720 "alan.pmk"
 outdent(); 	break;}
    case 214: { /* <step> = 'STEP' __genSym#35 <statements>; */
#line 729 "alan.pmk"
 outdent(); 	break;}
    case 217: { /* __genSym#35 =; */
#line 729 "alan.pmk"
 newline(); fprintf(outFile, "Step"); indent(); 	break;}
    case 215: { /* <step> = <step_after_integer> <statements>; */
#line 730 "alan.pmk"
 outdent(); 	break;}
    case 216: { /* <step> = 'STEP' 'WAIT' 'UNTIL' __genSym#36 <expression> <statements>; */
#line 731 "alan.pmk"
 outdent(); 	break;}
    case 218: { /* __genSym#36 =; */
#line 731 "alan.pmk"
 newline(); fprintf(outFile, "Step Wait Until "); indent(); 	break;}
    case 219: { /* <step_after_integer> = 'STEP' 'AFTER' Integer; */
#line 735 "alan.pmk"
 newline(); fprintf(outFile, "Step After %s", pmSySt[pmStkP+3].chars); indent(); 	break;}
    case 220: { /* <rule> = 'WHEN' __genSym#37 <expression> '=>' __genSym#38 <statements>; */
#line 740 "alan.pmk"
 outdent(); newline(); 	break;}
    case 221: { /* __genSym#37 =; */
#line 740 "alan.pmk"
 newline(); fprintf(outFile, "When "); 	break;}
    case 222: { /* __genSym#38 =; */
#line 740 "alan.pmk"
 fprintf(outFile, " Then"); indent(); 	break;}
    case 224: { /* __genSym#39 =; */
#line 745 "alan.pmk"
 newline(); newline(); fprintf(outFile, "Start "); 	break;}
    case 225: { /* __genSym#40 =; */
#line 745 "alan.pmk"
 fprintf(outFile, "."); indent(); 	break;}
    case 237: { /* <fullstop> = '.'; */
#line 770 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 238: { /* <output_statement> = STRING; */
#line 775 "alan.pmk"
 stringPrint(pmSySt[pmStkP+1].chars); 	break;}
    case 242: { /* __genSym#41 =; */
#line 776 "alan.pmk"
 newline(); fprintf(outFile, "Describe "); 	break;}
    case 243: { /* __genSym#42 =; */
#line 777 "alan.pmk"
 newline(); fprintf(outFile, "Say "); 	break;}
    case 244: { /* __genSym#43 =; */
#line 778 "alan.pmk"
 newline(); fprintf(outFile, "List "); 	break;}
    case 245: { /* <special_statement> = 'QUIT' '.'; */
#line 782 "alan.pmk"
 newline(); fprintf(outFile, "Quit."); 	break;}
    case 246: { /* <special_statement> = 'LOOK' '.'; */
#line 783 "alan.pmk"
 newline(); fprintf(outFile, "Look."); 	break;}
    case 247: { /* <special_statement> = 'SAVE' '.'; */
#line 784 "alan.pmk"
 newline(); fprintf(outFile, "Save."); 	break;}
    case 248: { /* <special_statement> = 'RESTORE' '.'; */
#line 785 "alan.pmk"
 newline(); fprintf(outFile, "Restore."); 	break;}
    case 249: { /* <special_statement> = 'RESTART' '.'; */
#line 786 "alan.pmk"
 newline(); fprintf(outFile, "Restart."); 	break;}
    case 250: { /* <special_statement> = 'SCORE' __genSym#44 <optional_integer> '.'; */
#line 787 "alan.pmk"
 fprintf(outFile, "."); 	break;}
    case 253: { /* __genSym#44 =; */
#line 787 "alan.pmk"
 newline(); fprintf(outFile, "Score "); 	break;}
    case 251: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 788 "alan.pmk"
 newline(); fprintf(outFile, "Visits %s.", pmSySt[pmStkP+2].chars); 	break;}
    case 252: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 789 "alan.pmk"
 newline(); fprintf(outFile, "****System**** "); stringPrint(pmSySt[pmStkP+2].chars); 	break;}
    case 256: { /* __genSym#45 =; */
#line 795 "alan.pmk"
 newline(); fprintf(outFile, "Empty "); 	break;}
    case 257: { /* __genSym#46 =; */
#line 795 "alan.pmk"
 fprintf(outFile, " "); 	break;}
    case 258: { /* __genSym#47 =; */
#line 796 "alan.pmk"
 newline(); fprintf(outFile, "Locate "); 	break;}
    case 259: { /* __genSym#48 =; */
#line 796 "alan.pmk"
 fprintf(outFile, " "); 	break;}
    case 262: { /* __genSym#49 =; */
#line 801 "alan.pmk"
 fprintf(outFile, " After "); 	break;}
    case 260: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 803 "alan.pmk"
 newline(); fprintf(outFile, "Cancel %s.", pmSeSt[pmStkP+2].string); 	break;}
    case 263: { /* <schedule1> = 'SCHEDULE' ID; */
#line 807 "alan.pmk"
 newline(); fprintf(outFile, "Schedule %s ", pmSeSt[pmStkP+2].string); 	break;}
    case 268: { /* __genSym#50 =; */
#line 812 "alan.pmk"
 newline(); fprintf(outFile, "Make "); 	break;}
    case 269: { /* __genSym#51 =; */
#line 812 "alan.pmk"
 fprintf(outFile, " "); 	break;}
    case 270: { /* __genSym#52 =; */
#line 813 "alan.pmk"
 newline(); fprintf(outFile, "Set "); 	break;}
    case 271: { /* __genSym#53 =; */
#line 813 "alan.pmk"
 fprintf(outFile, " To "); 	break;}
    case 272: { /* __genSym#54 =; */
#line 814 "alan.pmk"
 newline(); fprintf(outFile, "Increase "); 	break;}
    case 273: { /* __genSym#55 =; */
#line 815 "alan.pmk"
 newline(); fprintf(outFile, "Decrease "); 	break;}
    case 276: { /* __genSym#56 =; */
#line 820 "alan.pmk"
 fprintf(outFile, " By "); 	break;}
    case 279: { /* <if_statement> = 'IF' __genSym#57 <expression> 'THEN' __genSym#58 <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 833 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End If."); 	break;}
    case 280: { /* __genSym#57 =; */
#line 830 "alan.pmk"
 newline(); fprintf(outFile, "If "); 	break;}
    case 281: { /* __genSym#58 =; */
#line 831 "alan.pmk"
 fprintf(outFile, " Then"); indent(); 	break;}
    case 287: { /* __genSym#59 =; */
#line 847 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "Elsif "); 	break;}
    case 288: { /* __genSym#60 =; */
#line 848 "alan.pmk"
 fprintf(outFile, " Then "); indent(); 	break;}
    case 291: { /* __genSym#61 =; */
#line 854 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "Else "); indent(); 	break;}
    case 292: { /* <depending_statement> = 'DEPENDING' 'ON' __genSym#62 <primary> __genSym#63 <depend_cases> 'END' 'DEPEND' '.'; */
#line 862 "alan.pmk"
 outdent(); newline(); fprintf(outFile, "End Depend."); 	break;}
    case 293: { /* __genSym#62 =; */
#line 859 "alan.pmk"
 newline(); fprintf(outFile, "Depending On "); 	break;}
    case 294: { /* __genSym#63 =; */
#line 860 "alan.pmk"
 indent(); newline(); 	break;}
    case 297: { /* __genSym#64 =; */
#line 867 "alan.pmk"
 newline(); 	break;}
    case 299: { /* <depend_case> = <right_hand_side> ':' __genSym#65 <statements>; */
#line 871 "alan.pmk"
 outdent(); 	break;}
    case 300: { /* __genSym#65 =; */
#line 871 "alan.pmk"
 fprintf(outFile, " Then "); indent(); 	break;}
    case 298: { /* <depend_case> = 'ELSE' __genSym#66 <statements>; */
#line 872 "alan.pmk"
 outdent(); 	break;}
    case 301: { /* __genSym#66 =; */
#line 872 "alan.pmk"
 fprintf(outFile, "Else "); indent(); 	break;}
    case 303: { /* __genSym#67 =; */
#line 877 "alan.pmk"
 newline(); fprintf(outFile, "Use Script "); 	break;}
    case 304: { /* <script_reference> = <integer_or_id>; */
#line 881 "alan.pmk"
 fprintf(outFile, pmSeSt[pmStkP+1].string); 	break;}
    case 306: { /* <optional_for_actor> = 'FOR' ID; */
#line 886 "alan.pmk"
 fprintf(outFile, " For %s", pmSeSt[pmStkP+2].string); 	break;}
    case 309: { /* __genSym#68 =; */
#line 892 "alan.pmk"
 fprintf(outFile, " Or "); 	break;}
    case 312: { /* __genSym#69 =; */
#line 897 "alan.pmk"
 fprintf(outFile, " And "); 	break;}
    case 315: { /* __genSym#70 =; */
#line 902 "alan.pmk"
 fprintf(outFile, " "); 	break;}
    case 322: { /* __genSym#71 =; */
#line 910 "alan.pmk"
 fprintf(outFile, " Between "); 	break;}
    case 323: { /* __genSym#72 =; */
#line 910 "alan.pmk"
 fprintf(outFile, " And "); 	break;}
    case 324: { /* __genSym#73 =; */
#line 911 "alan.pmk"
 fprintf(outFile, " Contains "); 	break;}
    case 330: { /* <primary> = '(' __genSym#74 <expression> ')'; */
#line 915 "alan.pmk"
 fprintf(outFile, ")"); 	break;}
    case 333: { /* __genSym#74 =; */
#line 915 "alan.pmk"
 fprintf(outFile, "("); 	break;}
    case 325: { /* <primary> = <optional_minus> Integer; */
#line 916 "alan.pmk"
 fprintf(outFile, "%s%s", pmSeSt[pmStkP+1].string, pmSySt[pmStkP+2].chars); 	break;}
    case 326: { /* <primary> = STRING; */
#line 917 "alan.pmk"
 stringPrint(pmSySt[pmStkP+1].chars); 	break;}
    case 334: { /* __genSym#75 =; */
#line 920 "alan.pmk"
 fprintf(outFile, "Isa object, "); 	break;}
    case 335: { /* __genSym#76 =; */
#line 921 "alan.pmk"
 fprintf(outFile, "Random "); 	break;}
    case 336: { /* __genSym#77 =; */
#line 921 "alan.pmk"
 fprintf(outFile, " To "); 	break;}
    case 328: { /* <primary> = 'SCORE'; */
#line 922 "alan.pmk"
 fprintf(outFile, "Score"); 	break;}
    case 338: { /* <aggregate> = 'SUM' 'OF' ID; */
#line 926 "alan.pmk"
 fprintf(outFile, " (Sum Of "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 339: { /* <aggregate> = 'MAX' 'OF' ID; */
#line 927 "alan.pmk"
 fprintf(outFile, " (Max Of "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 337: { /* <aggregate> = 'COUNT'; */
#line 928 "alan.pmk"
 fprintf(outFile, " Count "); 	break;}
    case 340: { /* <something> = <optional_not> ID; */
#line 933 "alan.pmk"
 idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 341: { /* <what> = 'OBJECT'; */
#line 937 "alan.pmk"
 fprintf(outFile, "**** object ****"); 	break;}
    case 342: { /* <what> = 'LOCATION'; */
#line 938 "alan.pmk"
 fprintf(outFile, "Current Location"); 	break;}
    case 343: { /* <what> = 'ACTOR'; */
#line 939 "alan.pmk"
 fprintf(outFile, "Current Actor"); 	break;}
    case 344: { /* <what> = ID; */
#line 940 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 347: { /* <where> = 'HERE'; */
#line 949 "alan.pmk"
 fprintf(outFile, "Here"); 	break;}
    case 348: { /* <where> = 'NEARBY'; */
#line 950 "alan.pmk"
 fprintf(outFile, "Nearby"); 	break;}
    case 351: { /* __genSym#78 =; */
#line 951 "alan.pmk"
 fprintf(outFile, "At "); 	break;}
    case 352: { /* __genSym#79 =; */
#line 952 "alan.pmk"
 fprintf(outFile, "In "); 	break;}
    case 353: { /* <binop> = '+'; */
#line 956 "alan.pmk"
 fprintf(outFile, "+"); 	break;}
    case 354: { /* <binop> = '-'; */
#line 957 "alan.pmk"
 fprintf(outFile, "-"); 	break;}
    case 355: { /* <binop> = '*'; */
#line 958 "alan.pmk"
 fprintf(outFile, "*"); 	break;}
    case 356: { /* <binop> = '/'; */
#line 959 "alan.pmk"
 fprintf(outFile, "/"); 	break;}
    case 357: { /* <relop> = '<>'; */
#line 963 "alan.pmk"
 fprintf(outFile, "<>"); 	break;}
    case 358: { /* <relop> = '='; */
#line 964 "alan.pmk"
 fprintf(outFile, "="); 	break;}
    case 359: { /* <relop> = '=='; */
#line 965 "alan.pmk"
 fprintf(outFile, "=="); 	break;}
    case 360: { /* <relop> = '>='; */
#line 966 "alan.pmk"
 fprintf(outFile, ">="); 	break;}
    case 361: { /* <relop> = '<='; */
#line 967 "alan.pmk"
 fprintf(outFile, "<="); 	break;}
    case 362: { /* <relop> = '>'; */
#line 968 "alan.pmk"
 fprintf(outFile, ">"); 	break;}
    case 363: { /* <relop> = '<'; */
#line 969 "alan.pmk"
 fprintf(outFile, "<"); 	break;}
    case 365: { /* <optional_qual> = 'BEFORE'; */
#line 974 "alan.pmk"
 fprintf(outFile, " Before"); 	break;}
    case 366: { /* <optional_qual> = 'AFTER'; */
#line 975 "alan.pmk"
 fprintf(outFile, " After"); 	break;}
    case 367: { /* <optional_qual> = 'ONLY'; */
#line 976 "alan.pmk"
 fprintf(outFile, " Only"); 	break;}
    case 369: { /* <optional_not> = 'NOT'; */
#line 981 "alan.pmk"
 fprintf(outFile, "Not "); 	break;}
    case 370: { /* <optional_id> =; */
#line 985 "alan.pmk"
 pmSeSt[pmStkP+1].string = ""; 	break;}
    case 371: { /* <optional_id> = ID; */
#line 986 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 372: { /* <ids> = ID; */
#line 990 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 373: { /* <ids> = <ids> ID; */
#line 991 "alan.pmk"
 fprintf(outFile, " "); idPrint(pmSeSt[pmStkP+2].string); 	break;}
    case 374: { /* <id_list> = ID; */
#line 995 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); 	break;}
    case 375: { /* <id_list> = <id_list> ',' ID; */
#line 996 "alan.pmk"
 fprintf(outFile, ", "); idPrint(pmSeSt[pmStkP+3].string); 	break;}
    case 377: { /* <optional_integer> = Integer; */
#line 1001 "alan.pmk"
 fprintf(outFile, pmSySt[pmStkP+1].chars); 	break;}
    case 378: { /* <optional_minus> =; */
#line 1005 "alan.pmk"
 pmSeSt[pmStkP+1].string = ""; 	break;}
    case 379: { /* <optional_minus> = '-'; */
#line 1006 "alan.pmk"
 pmSeSt[pmStkP+1].string = "-"; 	break;}
    case 381: { /* <id_of> = ID 'OF'; */
#line 1014 "alan.pmk"
 idPrint(pmSeSt[pmStkP+1].string); fprintf(outFile, " Of "); 	break;}
    case 382: { /* <integer_or_id> = Integer; */
#line 1018 "alan.pmk"
 pmSeSt[pmStkP+1].string = malloc(strlen(pmSySt[pmStkP+1].chars)+2);
		sprintf(pmSeSt[pmStkP+1].string, "s%s", pmSySt[pmStkP+1].chars); 	break;}
    case 383: { /* <integer_or_id> = ID; */
#line 1020 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSeSt[pmStkP+1].string; 	break;}
    case 384: { /* ID = IDENT; */
#line 1024 "alan.pmk"
 pmSeSt[pmStkP+1].string = pmSySt[pmStkP+1].chars; 	break;}
    case 385: { /* ID = 'DEFAULT'; */
#line 1025 "alan.pmk"
 pmSeSt[pmStkP+1].string = "default"; 	break;}
    case 386: { /* ID = 'ARTICLE'; */
#line 1026 "alan.pmk"
 pmSeSt[pmStkP+1].string = "article"; 	break;}
    case 387: { /* ID = 'MESSAGE'; */
#line 1027 "alan.pmk"
 pmSeSt[pmStkP+1].string = "message"; 	break;}
    case 388: { /* ID = 'QUIT'; */
#line 1028 "alan.pmk"
 pmSeSt[pmStkP+1].string = "quit"; 	break;}
    case 389: { /* ID = 'SAVE'; */
#line 1029 "alan.pmk"
 pmSeSt[pmStkP+1].string = "save"; 	break;}
    case 390: { /* ID = 'RESTORE'; */
#line 1030 "alan.pmk"
 pmSeSt[pmStkP+1].string = "restore"; 	break;}
    case 391: { /* ID = 'RESTART'; */
#line 1031 "alan.pmk"
 pmSeSt[pmStkP+1].string = "restart"; 	break;}
    case 392: { /* ID = 'WAIT'; */
#line 1032 "alan.pmk"
 pmSeSt[pmStkP+1].string = "wait"; 	break;}
    case 393: { /* ID = 'BETWEEN'; */
#line 1033 "alan.pmk"
 pmSeSt[pmStkP+1].string = "between"; 	break;}
    case 394: { /* ID = 'CONTAINS'; */
#line 1034 "alan.pmk"
 pmSeSt[pmStkP+1].string = "contains"; 	break;}
    case 395: { /* ID = 'ON'; */
#line 1035 "alan.pmk"
 pmSeSt[pmStkP+1].string = "on"; 	break;}
    case 396: { /* ID = 'IN'; */
#line 1036 "alan.pmk"
 pmSeSt[pmStkP+1].string = "in"; 	break;}
    case 397: { /* ID = 'AFTER'; */
#line 1037 "alan.pmk"
 pmSeSt[pmStkP+1].string = "after"; 	break;}
    case 398: { /* ID = 'BEFORE'; */
#line 1038 "alan.pmk"
 pmSeSt[pmStkP+1].string = "before"; 	break;}
    case 399: { /* ID = 'CHECK'; */
#line 1039 "alan.pmk"
 pmSeSt[pmStkP+1].string = "check"; 	break;}
    case 400: { /* ID = 'DEPEND'; */
#line 1040 "alan.pmk"
 pmSeSt[pmStkP+1].string = "depend"; 	break;}
    case 401: { /* ID = 'EXIT'; */
#line 1041 "alan.pmk"
 pmSeSt[pmStkP+1].string = "exit"; 	break;}
    case 402: { /* ID = 'FOR'; */
#line 1042 "alan.pmk"
 pmSeSt[pmStkP+1].string = "for"; 	break;}
    case 403: { /* ID = 'INTEGER'; */
#line 1043 "alan.pmk"
 pmSeSt[pmStkP+1].string = "integer"; 	break;}
    case 404: { /* ID = 'ISA'; */
#line 1044 "alan.pmk"
 pmSeSt[pmStkP+1].string = "isa"; 	break;}
    case 405: { /* ID = 'LIMITS'; */
#line 1045 "alan.pmk"
 pmSeSt[pmStkP+1].string = "limits"; 	break;}
    default: break; }
}/*pmPaSema()*/

