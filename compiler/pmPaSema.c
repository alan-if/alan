/*----------------------------------------------------------------------*\

	pmPaSema.c

	ParserMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


#include "util.h"
#include "../interpreter/acode.h"

#include "types.h"
#include "smScan.h"
#include "token.h"

#include "lmList.h"

#include "alt.h"                /* ALT-nodes */
#include "evt.h"                /* EVT-nodes */
#include "lim.h"		/* LIM-nodes */
#include "msg.h"                /* MSG-nodes */
#include "opt.h"		/* OPTIONS */
#include "rul.h"                /* RUL-nodes */
#include "sco.h"                /* SCORES */
#include "str.h"		/* STRINGS */
#include "syn.h"                /* SYN-nodes */

#include "adv_x.h"
#include "add_x.h"
#include "atr_x.h"
#include "cla_x.h"
#include "chk_x.h"
#include "cnt_x.h"
#include "elm_x.h"
#include "exp_x.h"
#include "ext_x.h"
#include "id_x.h"
#include "ins_x.h"
#include "lst_x.h"
#include "prop_x.h"
#include "res_x.h"
#include "scr_x.h"
#include "stm_x.h"
#include "stp_x.h"
#include "stx_x.h"
#include "sym_x.h"
#include "vrb_x.h"
#include "whr_x.h"
#include "wht_x.h"

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
    Bool wordOrChar;
    What *wht;
    Where *whr;
    List *vrbs;
    Verb *vrb;
    int val;
    List *syns;
    Synonym *syn;
    List *stxs;
    Syntax *stx;
    char *str;
    List *stps;
    Step *stp;
    List *stms3;
    List *stms2;
    List *stms;
    Statement *stm;
    Srcp srcp2;
    Srcp srcp;
    List *set;
    List *scrs;
    Script *scr;
    SayForm sayForm;
    List *ruls;
    RulNod *rul;
    RestrictionKind restrictionKind;
    List *ress;
    Restriction *res;
    QualKind qual;
    Srcp pronounsSrcp;
    List *pronouns;
    Bool present;
    Bool opaque;
    OperatorKind op;
    List *nams;
    List *nam;
    List *msgs;
    Message *msg;
    Bool minus;
    Srcp mentionedSrcp;
    List *mentioned;
    List *lims;
    LimNod *lim;
    Bool isForm;
    List *inss;
    Instance *ins;
    Srcp indefiniteSrcp;
    Bool indefiniteIsForm;
    List *indefinite;
    List *idList2;
    List *idList;
    IdNode *id;
    List *form;
    Bool first;
    List *exts;
    Exit *ext;
    List *exps;
    Expression *exp;
    List *evts;
    EvtNod *evt;
    Srcp enteredSrcp;
    List *entered;
    List *elms;
    Element *elm;
    Srcp descriptionSrcp;
    List *descriptionDoes;
    List *descriptionChecks;
    Srcp descriptionCheckSrcp;
    Srcp definiteSrcp;
    Bool definiteIsForm;
    List *definite;
    List *cnts;
    Container *cnt;
    List *clas;
    Class *cla;
    List *chks;
    ChkNod *chk;
    List *cases;
    int bits;
    List *atrs;
    Attribute *atr;
    List *article;
    List *alts;
    AltNod *alt;
    AggregateKind agr;
    AddNode *add;
} pmGrammar;

/* END %%ATTRIBUTES */

extern short pmStkP;
extern Token pmSySt[];
extern pmGrammar pmSeSt[];

/* %%DECLARATIONS - User data and routines */

#line 78 "alan.pmk"


static int val(char str[])
{
  int temp;

  sscanf(str, "%d", &temp);
  return(temp);
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
#line 192 "alan.pmk"
    switch (rule) {
    case 8: { /* <option> = ID '.'; */
#line 212 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 9: { /* <option> = ID ID '.'; */
#line 217 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 10: { /* <option> = ID Integer '.'; */
#line 222 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 18: { /* <declaration> = <synonyms>; */
#line 235 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns);	break;}
    case 14: { /* <declaration> = <messages>; */
#line 238 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 19: { /* <declaration> = <syntax>; */
#line 241 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 20: { /* <declaration> = <verb>; */
#line 244 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VERB_LIST); 	break;}
    case 15: { /* <declaration> = <class>; */
#line 247 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLASS_LIST); 	break;}
    case 21: { /* <declaration> = <addition>; */
#line 250 "alan.pmk"
 adv.adds = concat(adv.adds, pmSeSt[pmStkP+1].add, ADD_LIST); 	break;}
    case 16: { /* <declaration> = <instance>; */
#line 253 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSTANCE_LIST); 	break;}
    case 22: { /* <declaration> = <event>; */
#line 256 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVENT_LIST); 	break;}
    case 17: { /* <declaration> = <rule>; */
#line 259 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULE_LIST); 	break;}
    case 23: { /* <attributes> = <attribute definition> '.'; */
#line 267 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, ATTRIBUTE_LIST);
    	break;}
    case 24: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 272 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATTRIBUTE_LIST);
    	break;}
    case 25: { /* <attribute definition> = ID; */
#line 279 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, TRUE);
    	break;}
    case 26: { /* <attribute definition> = 'NOT' ID; */
#line 284 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+2].id->srcp,	pmSeSt[pmStkP+2].id, FALSE);
    	break;}
    case 27: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 289 "alan.pmk"

	if (pmSeSt[pmStkP+2].minus)
		pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, -val(pmSySt[pmStkP+3].chars));
	else
		pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, val(pmSySt[pmStkP+3].chars));
    	break;}
    case 28: { /* <attribute definition> = ID STRING; */
#line 297 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newStringAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 29: { /* <attribute definition> = ID ID; */
#line 301 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newReferenceAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+2].id);
    	break;}
    case 30: { /* <attribute definition> = ID '{' <optional members> '}'; */
#line 306 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newSetAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].set);
    	break;}
    case 31: { /* <optional members> =; */
#line 313 "alan.pmk"
 pmSeSt[pmStkP+1].set = NULL; 	break;}
    case 32: { /* <optional members> = <set_members>; */
#line 315 "alan.pmk"
 pmSeSt[pmStkP+1].set = pmSeSt[pmStkP+1].set; 	break;}
    case 33: { /* <set_members> = <set_member>; */
#line 320 "alan.pmk"

	pmSeSt[pmStkP+1].set = concat(NULL, pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 34: { /* <set_members> = <set_members> ',' <set_member>; */
#line 325 "alan.pmk"

	pmSeSt[pmStkP+1].set = concat(pmSeSt[pmStkP+1].set, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 35: { /* <set_member> = ID; */
#line 332 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhatExpression(pmSeSt[pmStkP+1].id->srcp, newWhat(&pmSeSt[pmStkP+1].id->srcp, WHAT_ID, pmSeSt[pmStkP+1].id));
    	break;}
    case 36: { /* <set_member> = <optional_minus> Integer; */
#line 337 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].minus?-val(pmSySt[pmStkP+2].chars):val(pmSySt[pmStkP+2].chars));
    	break;}
    case 37: { /* <set_member> = STRING; */
#line 342 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].fpos, pmSySt[pmStkP+1].len);
    	break;}
    case 38: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 350 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 39: { /* <synonym_list> = <synonym_declaration>; */
#line 357 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, SYNONYM_LIST);
    	break;}
    case 40: { /* <synonym_list> = <synonym_list> <synonym_declaration>; */
#line 362 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNONYM_LIST);
    	break;}
    case 41: { /* <synonym_declaration> = <id_list> '=' ID '.'; */
#line 369 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			     			pmSeSt[pmStkP+1].idList,
			      			pmSeSt[pmStkP+3].id);
    	break;}
    case 42: { /* <messages> = 'MESSAGE' <message_list>; */
#line 379 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 43: { /* <message_list> = <message>; */
#line 386 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, MESSAGE_LIST);
    	break;}
    case 44: { /* <message_list> = <message_list> <message>; */
#line 391 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MESSAGE_LIST);
    	break;}
    case 45: { /* <message> = ID ':' <statements>; */
#line 398 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newMessage(&pmSySt[pmStkP+2].srcp,
				      	pmSeSt[pmStkP+1].id,
					pmSeSt[pmStkP+3].stms);
    	break;}
    case 46: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 408 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 47: { /* <syntax_list> = <syntax_item>; */
#line 415 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, SYNTAX_LIST);
    	break;}
    case 48: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 420 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, SYNTAX_LIST);
    	break;}
    case 49: { /* <syntax_item> = ID '=' <syntax_elements> <optional_syntax_restrictions>; */
#line 427 "alan.pmk"

	pmSeSt[pmStkP+1].stx = newSyntax(pmSySt[pmStkP+2].srcp,
				  	pmSeSt[pmStkP+1].id,
				  	concat(pmSeSt[pmStkP+3].elms,
				        	newEndOfSyntax(),
						ELEMENT_LIST),
					pmSeSt[pmStkP+4].ress,
					pmSeSt[pmStkP+4].srcp);
    	break;}
    case 50: { /* <syntax_elements> = <syntax_element>; */
#line 440 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, ELEMENT_LIST);
    	break;}
    case 51: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 445 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                ELEMENT_LIST);
    	break;}
    case 52: { /* <syntax_element> = ID; */
#line 454 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newWordElement(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id);
    	break;}
    case 53: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 460 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newParameterElement(pmSeSt[pmStkP+2].id->srcp,
					   pmSeSt[pmStkP+2].id,
					   pmSeSt[pmStkP+4].bits);
    	break;}
    case 54: { /* <optional_indicators> =; */
#line 469 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 55: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 474 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 56: { /* <indicator> = '*'; */
#line 481 "alan.pmk"

	pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 57: { /* <indicator> = '!'; */
#line 486 "alan.pmk"

	pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 58: { /* <syntax_restriction_clauses> = <syntax_restriction>; */
#line 499 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, RESTRICTION_LIST);
    	break;}
    case 59: { /* <syntax_restriction_clauses> = <syntax_restriction_clauses> 'AND' <syntax_restriction>; */
#line 504 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, RESTRICTION_LIST);
    	break;}
    case 60: { /* <syntax_restriction> = ID 'ISA' <restriction_class> 'ELSE' <statements>; */
#line 512 "alan.pmk"

	pmSeSt[pmStkP+1].res = newRestriction(pmSySt[pmStkP+2].srcp,
			  			  pmSeSt[pmStkP+1].id,
						  pmSeSt[pmStkP+3].restrictionKind,
						  pmSeSt[pmStkP+3].id,
						  pmSeSt[pmStkP+5].stms);
    	break;}
    case 61: { /* <restriction_class> = ID; */
#line 523 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].restrictionKind = ID_RESTRICTION;
    	break;}
    case 62: { /* <restriction_class> = 'CONTAINER'; */
#line 529 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
	pmSeSt[pmStkP+1].restrictionKind = CONTAINER_RESTRICTION;
    	break;}
    case 63: { /* <optional_syntax_restrictions> = '.'; */
#line 537 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 64: { /* <optional_syntax_restrictions> = 'WHERE' <syntax_restriction_clauses>; */
#line 542 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 65: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 552 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newVerb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (compareStrings(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 66: { /* <verb_header> = 'VERB' <id_list>; */
#line 565 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 67: { /* <verb_body> = <simple_verb_body>; */
#line 574 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&pmSeSt[pmStkP+1].srcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   ALTERNATIVE_LIST);
    	break;}
    case 68: { /* <verb_body> = <verb_alternatives>; */
#line 585 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 69: { /* <verb_alternatives> = <verb_alternative>; */
#line 592 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, ALTERNATIVE_LIST);
    	break;}
    case 70: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 597 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				ALTERNATIVE_LIST);
    	break;}
    case 71: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 606 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 72: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 617 "alan.pmk"

	if (pmSeSt[pmStkP+2].stms == NULL) /* No DOES */
	  pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	else
	  pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 73: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */
#line 630 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 74: { /* <optional_checks> =; */
#line 638 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].srcp = nulsrcp;
    	break;}
    case 75: { /* <optional_checks> = 'CHECK' <statements>; */
#line 644 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), CHECK_LIST);
    	break;}
    case 76: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 650 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 77: { /* <check_list> = <check>; */
#line 658 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, CHECK_LIST);
    	break;}
    case 78: { /* <check_list> = <check_list> 'AND' <check>; */
#line 663 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHECK_LIST);
    	break;}
    case 79: { /* <check> = <expression> 'ELSE' <statements>; */
#line 670 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 80: { /* <optional_does> =; */
#line 678 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].srcp = nulsrcp;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 81: { /* <optional_does> = <does>; */
#line 685 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+1].qual;
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 82: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 694 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 83: { /* <class> = 'EVERY' ID <optional_heritage> <properties> <class_tail>; */
#line 705 "alan.pmk"

	pmSeSt[pmStkP+1].cla = newClass(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newProps(pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].pronounsSrcp,
					pmSeSt[pmStkP+4].pronouns,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].descriptionCheckSrcp,
					pmSeSt[pmStkP+4].descriptionChecks,
					pmSeSt[pmStkP+4].descriptionSrcp,
					pmSeSt[pmStkP+4].descriptionDoes,
					pmSeSt[pmStkP+4].enteredSrcp,
					pmSeSt[pmStkP+4].entered,
					pmSeSt[pmStkP+4].mentionedSrcp,
					pmSeSt[pmStkP+4].mentioned,
					pmSeSt[pmStkP+4].definiteSrcp,
					pmSeSt[pmStkP+4].definite,
					pmSeSt[pmStkP+4].definiteIsForm,
					pmSeSt[pmStkP+4].indefiniteSrcp,
					pmSeSt[pmStkP+4].indefinite,
					pmSeSt[pmStkP+4].indefiniteIsForm,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 84: { /* <class_tail> = 'END' 'EVERY' <optional_id> __genSym#1; */
#line 740 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 87: { /* <addition> = 'ADD' 'TO' __genSym#2 ID <optional_heritage> <properties> <add_tail>; */
#line 747 "alan.pmk"

	pmSeSt[pmStkP+1].add = newAdd(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+4].id,
				pmSeSt[pmStkP+5].id,
				newProps(pmSeSt[pmStkP+6].whr,
					pmSeSt[pmStkP+6].nams,
					pmSeSt[pmStkP+6].pronounsSrcp,
					pmSeSt[pmStkP+6].pronouns,
					pmSeSt[pmStkP+6].atrs,
					pmSeSt[pmStkP+6].cnt,
					pmSeSt[pmStkP+6].descriptionCheckSrcp,
					pmSeSt[pmStkP+6].descriptionChecks,
					pmSeSt[pmStkP+6].descriptionSrcp,
					pmSeSt[pmStkP+6].descriptionDoes,
					pmSeSt[pmStkP+6].enteredSrcp,
					pmSeSt[pmStkP+6].entered,
					pmSeSt[pmStkP+6].mentionedSrcp,
					pmSeSt[pmStkP+6].mentioned,
					pmSeSt[pmStkP+6].definiteSrcp,
					pmSeSt[pmStkP+6].definite,
					pmSeSt[pmStkP+6].definiteIsForm,
					pmSeSt[pmStkP+6].indefiniteSrcp,
					pmSeSt[pmStkP+6].indefinite,
					pmSeSt[pmStkP+6].indefiniteIsForm,
					pmSeSt[pmStkP+6].exts,
					pmSeSt[pmStkP+6].vrbs,
					pmSeSt[pmStkP+6].scrs));
	if (pmSeSt[pmStkP+7].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+4].id, pmSeSt[pmStkP+7].id))
		lmLog(&pmSeSt[pmStkP+7].id->srcp, 200, sevWAR, pmSeSt[pmStkP+4].id->string);
    	break;}
    case 90: { /* <add_tail> = 'END' 'ADD' __genSym#3 <optional_id> __genSym#4; */
#line 782 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+4].id; 	break;}
    case 95: { /* <instance> = 'THE' ID <optional_heritage> <properties> <instance tail>; */
#line 789 "alan.pmk"

	pmSeSt[pmStkP+1].ins = newInstance(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newProps(pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].pronounsSrcp,
					pmSeSt[pmStkP+4].pronouns,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].descriptionCheckSrcp,
					pmSeSt[pmStkP+4].descriptionChecks,
					pmSeSt[pmStkP+4].descriptionSrcp,
					pmSeSt[pmStkP+4].descriptionDoes,
					pmSeSt[pmStkP+4].enteredSrcp,
					pmSeSt[pmStkP+4].entered,
					pmSeSt[pmStkP+4].mentionedSrcp,
					pmSeSt[pmStkP+4].mentioned,
					pmSeSt[pmStkP+4].definiteSrcp,
					pmSeSt[pmStkP+4].definite,
					pmSeSt[pmStkP+4].definiteIsForm,
					pmSeSt[pmStkP+4].indefiniteSrcp,
					pmSeSt[pmStkP+4].indefinite,
					pmSeSt[pmStkP+4].indefiniteIsForm,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 96: { /* <instance tail> = 'END' 'THE' <optional_id> __genSym#5; */
#line 824 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 99: { /* <optional_heritage> =; */
#line 829 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 100: { /* <optional_heritage> = <heritage>; */
#line 834 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 101: { /* <heritage> = 'ISA' ID __genSym#6; */
#line 841 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 104: { /* <properties> =; */
#line 846 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out all fields */
    	break;}
    case 105: { /* <properties> = <properties> <property>; */
#line 851 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+2].nam, NAME_LIST);

        if (pmSeSt[pmStkP+2].pronouns != NULL) {
            if (pmSeSt[pmStkP+1].pronouns != NULL) 
                /* WHERE clause already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "PRONOUN", "class/instance", NULL);
	    else {
		pmSeSt[pmStkP+1].pronouns = combine(pmSeSt[pmStkP+1].pronouns, pmSeSt[pmStkP+2].pronouns);
		pmSeSt[pmStkP+1].pronounsSrcp = pmSeSt[pmStkP+2].pronounsSrcp;
	    }
        }

        if (pmSeSt[pmStkP+2].whr != NULL) {
            if (pmSeSt[pmStkP+1].whr != NULL) 
                /* WHERE clause already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "WHERE", "class/instance", NULL);
	    else
	        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+2].whr;
        }

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);

	if (pmSeSt[pmStkP+2].cnt != NULL) {
            if (pmSeSt[pmStkP+1].cnt != NULL)
                /* CONTAINER properties already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "CONTAINER properties", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+2].cnt;
        }

	if (pmSeSt[pmStkP+2].descriptionChecks != NULL) {
            if(pmSeSt[pmStkP+1].descriptionChecks != NULL)
                /* DESCRIPTION CHECK already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp2, 204, sevERR, "DESCRIPTION CHECK", "class/instance", NULL);
	    else {
	        pmSeSt[pmStkP+1].descriptionChecks = pmSeSt[pmStkP+2].descriptionChecks;
		pmSeSt[pmStkP+1].descriptionCheckSrcp = pmSeSt[pmStkP+2].srcp2;
	    }
        }

	if (pmSeSt[pmStkP+2].descriptionDoes != NULL) {
            if (pmSeSt[pmStkP+1].descriptionDoes != NULL)
	        /* DESCRIPTION CHECK already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].descriptionDoes = pmSeSt[pmStkP+2].descriptionDoes;
		pmSeSt[pmStkP+1].descriptionSrcp = pmSeSt[pmStkP+2].srcp;
        }

	if (pmSeSt[pmStkP+2].entered != NULL) {
            if (pmSeSt[pmStkP+1].entered != NULL)
                /* ENTERED already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "ENTERED", "class/instance", NULL);
	    else {
                pmSeSt[pmStkP+1].entered = pmSeSt[pmStkP+2].entered;
		pmSeSt[pmStkP+1].enteredSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].mentioned != NULL) {
            if (pmSeSt[pmStkP+1].mentioned != NULL)
                /* MENTIONED already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "MENTIONED", "class/instance", NULL);
	    else {
                pmSeSt[pmStkP+1].mentioned = pmSeSt[pmStkP+2].mentioned;
		pmSeSt[pmStkP+1].mentionedSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].indefinite != NULL) {
            if (pmSeSt[pmStkP+1].indefinite != NULL)
                /* Indefinite Article/Form already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "[Indefinite] Article/Form", "class/instance", NULL);
            else { 
	        pmSeSt[pmStkP+1].indefinite = pmSeSt[pmStkP+2].indefinite;
	        pmSeSt[pmStkP+1].indefiniteIsForm = pmSeSt[pmStkP+2].indefiniteIsForm;
		pmSeSt[pmStkP+1].indefiniteSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].definite != NULL) {
            if (pmSeSt[pmStkP+1].definite != NULL)
                /* Definite Article/Form already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "Definite Article/Form", "class/instance", NULL);
            else { 
	        pmSeSt[pmStkP+1].definite = pmSeSt[pmStkP+2].definite;
	        pmSeSt[pmStkP+1].definiteIsForm = pmSeSt[pmStkP+2].definiteIsForm;
		pmSeSt[pmStkP+1].definiteSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXIT_LIST);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VERB_LIST);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, SCRIPT_LIST);
    	break;}
    case 110: { /* <property> = <name>; */
#line 952 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 111: { /* <property> = <pronoun>; */
#line 958 "alan.pmk"
{ List *idList = pmSeSt[pmStkP+1].idList;
	Srcp pronounsSrcp = pmSeSt[pmStkP+1].pronounsSrcp;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].pronouns = idList;
	pmSeSt[pmStkP+1].pronounsSrcp = pronounsSrcp;
    }	break;}
    case 106: { /* <property> = <where> __genSym#7; */
#line 966 "alan.pmk"
{
        Where *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 115: { /* <property> = <is> <attributes>; */
#line 976 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 107: { /* <property> = <container_properties>; */
#line 982 "alan.pmk"
{
        Container *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 108: { /* <property> = <description>; */
#line 992 "alan.pmk"
{
        Srcp srcp2 = pmSeSt[pmStkP+1].srcp2;
        List *descriptionChecks = pmSeSt[pmStkP+1].chks;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;
        List *descriptionDoes = pmSeSt[pmStkP+1].stms;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */

	pmSeSt[pmStkP+1].srcp2 = srcp2;
	pmSeSt[pmStkP+1].descriptionChecks = descriptionChecks;
	pmSeSt[pmStkP+1].srcp = srcp;
	pmSeSt[pmStkP+1].descriptionDoes = descriptionDoes;
    }	break;}
    case 112: { /* <property> = <entered>; */
#line 1007 "alan.pmk"
{
        List *entered = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].entered = entered;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 113: { /* <property> = <mentioned>; */
#line 1017 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].mentioned = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 109: { /* <property> = __genSym#8 <article_or_form>; */
#line 1027 "alan.pmk"
{ List *indefinite = pmSeSt[pmStkP+2].stms;
	Bool isForm = pmSeSt[pmStkP+2].isForm;
        Srcp srcp = pmSeSt[pmStkP+2].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].indefinite = indefinite;
	pmSeSt[pmStkP+1].indefiniteIsForm = isForm;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 114: { /* <property> = 'DEFINITE' <article_or_form>; */
#line 1039 "alan.pmk"
{ List *definite = pmSeSt[pmStkP+2].stms;
	Bool isForm = pmSeSt[pmStkP+2].isForm;
        Srcp srcp = pmSeSt[pmStkP+2].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].definite = definite;
	pmSeSt[pmStkP+1].definiteIsForm = isForm;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 117: { /* <property> = <exit>; */
#line 1051 "alan.pmk"
{ Exit *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 118: { /* <property> = <verb>; */
#line 1057 "alan.pmk"
{ Verb *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 116: { /* <property> = <script>; */
#line 1063 "alan.pmk"
{ Script *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 123: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 1073 "alan.pmk"

	pmSeSt[pmStkP+1].ext = newExit(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].idList,
			   pmSeSt[pmStkP+4].id,
			   pmSeSt[pmStkP+5].chks,
			   pmSeSt[pmStkP+5].stms);
	if (pmSeSt[pmStkP+5].id != NULL) { /* END-id given */
	    if (compareStrings(pmSeSt[pmStkP+2].str, pmSeSt[pmStkP+5].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].str);
	}
    	break;}
    case 124: { /* <optional_exit_body> =; */
#line 1088 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 125: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 1095 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 126: { /* <optional_attributes> =; */
#line 1110 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 127: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 1115 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 128: { /* <is> = 'IS'; */
#line 1122 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 129: { /* <is> = 'ARE'; */
#line 1127 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 130: { /* <is> = 'HAS'; */
#line 1132 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 131: { /* <optional_description> =; */
#line 1139 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 132: { /* <optional_description> = <description>; */
#line 1145 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 133: { /* <description> = 'DESCRIPTION' <optional_checks> <optional_does>; */
#line 1153 "alan.pmk"

	if (pmSeSt[pmStkP+2].chks == NULL && pmSeSt[pmStkP+3].stms == NULL) {
	  pmSeSt[pmStkP+1].chks = NULL;
	  pmSeSt[pmStkP+1].stms = concat(NULL, newStatement(&pmSySt[pmStkP+1].srcp,
						NOP_STATEMENT), STATEMENT_LIST);
          pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
          pmSeSt[pmStkP+1].srcp2 = nulsrcp;
	} else {
	  if (pmSeSt[pmStkP+2].chks != NULL)
	    pmSeSt[pmStkP+1].srcp2 = pmSeSt[pmStkP+2].srcp;
	  else
	    pmSeSt[pmStkP+1].srcp2 = nulsrcp;
	  if (pmSeSt[pmStkP+3].stms != NULL)
	    pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+3].srcp;
	  else
	    pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	  pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
	  pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;

	  if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	    lmLog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
	}
    	break;}
    case 134: { /* <description> = 'DESCRIPTION' <statements>; */
#line 1178 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].srcp2 = nulsrcp;
    	break;}
    case 135: { /* <article_or_form> = <article>; */
#line 1186 "alan.pmk"
 pmSeSt[pmStkP+1].isForm = FALSE; 	break;}
    case 136: { /* <article_or_form> = <form>; */
#line 1187 "alan.pmk"
 pmSeSt[pmStkP+1].isForm = TRUE; 	break;}
    case 137: { /* <article> = 'ARTICLE'; */
#line 1192 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newStatement(&pmSySt[pmStkP+1].srcp,
							NOP_STATEMENT), STATEMENT_LIST);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 138: { /* <article> = 'ARTICLE' <statements>; */
#line 1199 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 139: { /* <form> = 'FORM'; */
#line 1207 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newStatement(&pmSySt[pmStkP+1].srcp,
						 NOP_STATEMENT), STATEMENT_LIST);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 140: { /* <form> = 'FORM' <statements>; */
#line 1214 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 141: { /* <entered> = 'ENTERED' <statements>; */
#line 1223 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 142: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 1232 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 143: { /* <optional_names> =; */
#line 1241 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 144: { /* <optional_names> = <optional_names> <name>; */
#line 1246 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					pmSeSt[pmStkP+2].nam, NAME_LIST);
    	break;}
    case 145: { /* <name> = 'NAME' <ids> <optional_full_stop>; */
#line 1254 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 146: { /* <pronoun> = 'PRONOUN' <id_list> <optional_full_stop>; */
#line 1261 "alan.pmk"

	pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+2].idList;
	pmSeSt[pmStkP+1].pronounsSrcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 149: { /* <container_properties> = __genSym#9 <optionally_opaque> 'CONTAINER' <container_body>; */
#line 1275 "alan.pmk"

	pmSeSt[pmStkP+1].cnt = newContainer(newContainerBody(pmSySt[pmStkP+3].srcp,
					pmSeSt[pmStkP+2].opaque,
					pmSeSt[pmStkP+4].id,
					pmSeSt[pmStkP+4].lims,
					pmSeSt[pmStkP+4].stms,
					pmSeSt[pmStkP+4].stms2,
					pmSeSt[pmStkP+4].chks,
					pmSeSt[pmStkP+4].stms3));
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+3].srcp;
    	break;}
    case 152: { /* <optionally_opaque> =; */
#line 1291 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = FALSE; 	break;}
    case 153: { /* <optionally_opaque> = 'OPAQUE'; */
#line 1293 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = TRUE; 	break;}
    case 154: { /* <container_body> = <optional_taking> <optional_limits> <optional_header> <optional_empty> <optional_extract>; */
#line 1299 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+2].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+3].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+4].stms;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+5].chks;
	pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+5].stms;
    	break;}
    case 156: { /* <optional_taking> =; */
#line 1313 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 157: { /* <optional_taking> = 'TAKING' ID '.'; */
#line 1318 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 158: { /* <optional_limits> =; */
#line 1326 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 159: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1331 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 160: { /* <limits> = <limit>; */
#line 1338 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIMIT_LIST);
    	break;}
    case 161: { /* <limits> = <limits> <limit>; */
#line 1343 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMIT_LIST);
    	break;}
    case 162: { /* <limit> = <limit_attribute> <else_or_then> <statements>; */
#line 1350 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSeSt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 163: { /* <else_or_then> = 'ELSE'; */
#line 1358 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 164: { /* <else_or_then> = 'THEN'; */
#line 1359 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 165: { /* <limit_attribute> = <attribute definition>; */
#line 1364 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 166: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1369 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSySt[pmStkP+1].srcp, newId(pmSySt[pmStkP+1].srcp, "count"), val(pmSySt[pmStkP+2].chars));
    	break;}
    case 167: { /* <optional_header> =; */
#line 1376 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 168: { /* <optional_header> = 'HEADER' <statements>; */
#line 1381 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 169: { /* <optional_empty> =; */
#line 1388 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 170: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1393 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 171: { /* <optional_extract> =; */
#line 1400 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 172: { /* <optional_extract> = 'EXTRACT' <optional_checks> <optional_does>; */
#line 1406 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
	if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	  lmLog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
    	break;}
    case 173: { /* <optional_extract> = 'EXTRACT' <statements>; */
#line 1414 "alan.pmk"

	pmSeSt[pmStkP+1].chk = NULL;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 174: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1424 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 175: { /* <event_header> = 'EVENT' ID; */
#line 1437 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 176: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1445 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 177: { /* <script> = 'SCRIPT' ID __genSym#10 <optional_description> <step_list>; */
#line 1453 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newScript(&pmSySt[pmStkP+1].srcp,
			          pmSeSt[pmStkP+2].id,
			          pmSeSt[pmStkP+4].stms,
			          pmSeSt[pmStkP+5].stps);
    	break;}
    case 180: { /* <step_list> = <step>; */
#line 1463 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, STEP_LIST);
    	break;}
    case 181: { /* <step_list> = <step_list> <step>; */
#line 1468 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STEP_LIST);
    	break;}
    case 182: { /* <step> = 'STEP' <statements>; */
#line 1475 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 183: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1483 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 184: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1491 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 185: { /* <rule> = 'WHEN' <expression> <then> <statements> <optional_end_when>; */
#line 1502 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 192: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1522 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 193: { /* <optional_statements> =; */
#line 1531 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 194: { /* <optional_statements> = <statements>; */
#line 1536 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 195: { /* <statements> = <statement>; */
#line 1543 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, STATEMENT_LIST);
    	break;}
    case 196: { /* <statements> = <statements> <statement>; */
#line 1548 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STATEMENT_LIST);
    	break;}
    case 197: { /* <statement> = <output_statement>; */
#line 1555 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 199: { /* <statement> = <manipulation_statement>; */
#line 1560 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 202: { /* <statement> = <assignment_statement>; */
#line 1565 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 204: { /* <statement> = <conditional_statement>; */
#line 1570 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 203: { /* <statement> = <repetition_statement>; */
#line 1575 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 200: { /* <statement> = <actor_statement>; */
#line 1580 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 201: { /* <statement> = <event_statement>; */
#line 1585 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 198: { /* <statement> = <special_statement>; */
#line 1590 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 205: { /* <output_statement> = STRING; */
#line 1598 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, PRINT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 206: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1605 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newDescribeStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 207: { /* <output_statement> = 'SAY' <say_form> <expression> '.'; */
#line 1610 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.say.form = pmSeSt[pmStkP+2].sayForm;
    	break;}
    case 208: { /* <output_statement> = 'LIST' <primary> '.'; */
#line 1617 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, LIST_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].exp;
    	break;}
    case 209: { /* <output_statement> = 'SHOW' ID '.'; */
#line 1623 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SHOW_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.show.filename = pmSeSt[pmStkP+2].id;
    	break;}
    case 210: { /* <output_statement> = 'STYLE' <style> '.'; */
#line 1629 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStyleStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].val);
    	break;}
    case 211: { /* <style> = 'NORMAL'; */
#line 1635 "alan.pmk"
 pmSeSt[pmStkP+1].val = NORMAL_STYLE; 	break;}
    case 212: { /* <style> = 'EMPHASIZED'; */
#line 1636 "alan.pmk"
 pmSeSt[pmStkP+1].val = EMPHASIZED_STYLE; 	break;}
    case 213: { /* <style> = 'PREFORMATTED'; */
#line 1637 "alan.pmk"
 pmSeSt[pmStkP+1].val = PREFORMATTED_STYLE; 	break;}
    case 214: { /* <style> = 'ALERT'; */
#line 1638 "alan.pmk"
 pmSeSt[pmStkP+1].val = ALERT_STYLE; 	break;}
    case 215: { /* <style> = 'QUOTE'; */
#line 1639 "alan.pmk"
 pmSeSt[pmStkP+1].val = QUOTE_STYLE; 	break;}
    case 216: { /* <say_form> =; */
#line 1644 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_SIMPLE; 	break;}
    case 217: { /* <say_form> = 'THE'; */
#line 1646 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_DEFINITE; 	break;}
    case 218: { /* <say_form> = 'AN'; */
#line 1648 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_INDEFINITE; 	break;}
    case 219: { /* <manipulation_statement> = 'EMPTY' <primary> <optional_where> '.'; */
#line 1655 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newEmptyStatement(pmSySt[pmStkP+1].srcp,
							  pmSeSt[pmStkP+2].exp,
							  pmSeSt[pmStkP+3].whr);
    	break;}
    case 220: { /* <manipulation_statement> = 'LOCATE' <primary> <where> '.'; */
#line 1662 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newLocateStatement(pmSySt[pmStkP+1].srcp,
							   pmSeSt[pmStkP+2].exp,
							   pmSeSt[pmStkP+3].whr);
    	break;}
    case 221: { /* <manipulation_statement> = 'INCLUDE' <primary> 'IN' <what> '.'; */
#line 1669 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newIncludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 222: { /* <manipulation_statement> = 'EXCLUDE' <primary> 'FROM' <what> '.'; */
#line 1676 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newExcludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 224: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1687 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SCHEDULE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 223: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1695 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, CANCEL_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 225: { /* <assignment_statement> = 'MAKE' <primary> <something> '.'; */
#line 1705 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, MAKE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].present;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 229: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1713 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SET_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 227: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1720 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, INCREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 228: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1727 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DECREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 226: { /* <assignment_statement> = 'STRIP' <optional_first_or_last> <optional_expression> <optional_word_or_character> 'FROM' <expression> <optional_into> '.'; */
#line 1734 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STRIP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.strip.first = pmSeSt[pmStkP+2].first;
	pmSeSt[pmStkP+1].stm->fields.strip.count = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.wordOrChar = pmSeSt[pmStkP+4].wordOrChar;
	pmSeSt[pmStkP+1].stm->fields.strip.from = pmSeSt[pmStkP+6].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.into = pmSeSt[pmStkP+7].exp;
    	break;}
    case 230: { /* <optional_by_clause> =; */
#line 1746 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 231: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1751 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 232: { /* <optional_first_or_last> =; */
#line 1758 "alan.pmk"
 pmSeSt[pmStkP+1].first = TRUE; 	break;}
    case 233: { /* <optional_first_or_last> = 'FIRST'; */
#line 1760 "alan.pmk"
 pmSeSt[pmStkP+1].first = TRUE; 	break;}
    case 234: { /* <optional_first_or_last> = 'LAST'; */
#line 1762 "alan.pmk"
 pmSeSt[pmStkP+1].first = FALSE; 	break;}
    case 235: { /* <optional_word_or_character> =; */
#line 1767 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 236: { /* <optional_word_or_character> = 'WORD'; */
#line 1769 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 237: { /* <optional_word_or_character> = 'WORDS'; */
#line 1771 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 238: { /* <optional_word_or_character> = 'CHARACTER'; */
#line 1773 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = FALSE; 	break;}
    case 239: { /* <optional_word_or_character> = 'CHARACTERS'; */
#line 1775 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = FALSE; 	break;}
    case 240: { /* <optional_into> =; */
#line 1780 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 241: { /* <optional_into> = 'INTO' <expression>; */
#line 1783 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp; 	break;}
    case 242: { /* <conditional_statement> = <if_statement>; */
#line 1789 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 243: { /* <conditional_statement> = <depending_statement>; */
#line 1794 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 244: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 1802 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	if (pmSeSt[pmStkP+5].stms == NULL)
	    pmSeSt[pmStkP+1].stm->fields.iff.els = pmSeSt[pmStkP+6].stms;
	else {
	    /* Connect the else_part to the elsif_list */
	    (tailOf(pmSeSt[pmStkP+5].stms))->element.stm->fields.iff.els =
			pmSeSt[pmStkP+6].stms;
	    pmSeSt[pmStkP+5].stms->next = NULL;
	    pmSeSt[pmStkP+1].stm->fields.iff.els = pmSeSt[pmStkP+5].stms;
	}
    	break;}
    case 245: { /* <optional_elsif_list> =; */
#line 1820 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 246: { /* <optional_elsif_list> = <elsif_list>; */
#line 1825 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 247: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 1832 "alan.pmk"
{
	Statement *stm;

	stm = newStatement(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, STATEMENT_LIST);
    }	break;}
    case 248: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 1845 "alan.pmk"
{
	Statement *stm;

	stm = newStatement(&pmSySt[pmStkP+2].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+5].stms;
	stm->fields.iff.els = NULL;

	/* Now connect it to the previous ELSIFs, I.e connect the */
	/* else-part of the last element in the elsif_list to this */
	/* IF-statement and also concat this element to the list so */
	/* we'll know were the end is next time */
	(tailOf(pmSeSt[pmStkP+1].stms))->element.stm->fields.iff.els = concat(NULL, stm, STATEMENT_LIST);
	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, stm, STATEMENT_LIST);
    }	break;}
    case 249: { /* <optional_else_part> =; */
#line 1864 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 250: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 1869 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 251: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' __genSym#12 '.'; */
#line 1876 "alan.pmk"
{ Statement *stm;
	stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPEND_STATEMENT);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 254: { /* <depend_cases> = <depend_case>; */
#line 1887 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, CASE_LIST);
    	break;}
    case 255: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1892 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASE_LIST);
    	break;}
    case 257: { /* <depend_case> = <right_hand_side> 'THEN' <statements>; */
#line 1899 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+2].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 256: { /* <depend_case> = 'ELSE' <statements>; */
#line 1906 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 258: { /* <repetition_statement> = <for_each> ID <optional_loop_restrictions> 'DO' <statements> 'END' <for_each> __genSym#13; */
#line 1916 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSeSt[pmStkP+1].srcp, EACH_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.each.stms = pmSeSt[pmStkP+5].stms;
	pmSeSt[pmStkP+1].stm->fields.each.loopId = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.each.classId = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.each.filters = pmSeSt[pmStkP+3].exps;
    	break;}
    case 261: { /* <optional_loop_restrictions> =; */
#line 1927 "alan.pmk"

	pmSeSt[pmStkP+1].exps = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 262: { /* <optional_loop_restrictions> = <filters>; */
#line 1933 "alan.pmk"

	pmSeSt[pmStkP+1].exps = pmSeSt[pmStkP+1].exps;
	/* While refactoring, for now, use first element */
	if (pmSeSt[pmStkP+1].exps->element.exp->kind == ISA_EXPRESSION)
	    pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].exps->element.exp->fields.isa.class;
    	break;}
    case 263: { /* <for_each> = 'FOR'; */
#line 1943 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 264: { /* <for_each> = 'EACH'; */
#line 1945 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 265: { /* <for_each> = 'FOR' 'EACH'; */
#line 1947 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 267: { /* <actor_statement> = 'USE' 'SCRIPT' ID <optional_for_actor> '.'; */
#line 1953 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, USE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.actorExp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 266: { /* <actor_statement> = 'STOP' <what> '.'; */
#line 1960 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STOP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.stop.actor = pmSeSt[pmStkP+2].exp;
    	break;}
    case 268: { /* <optional_for_actor> =; */
#line 1968 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 269: { /* <optional_for_actor> = 'FOR' <what>; */
#line 1973 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 270: { /* <special_statement> = 'QUIT' '.'; */
#line 1981 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, QUIT_STATEMENT);
    	break;}
    case 271: { /* <special_statement> = 'LOOK' '.'; */
#line 1986 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, LOOK_STATEMENT);
    	break;}
    case 272: { /* <special_statement> = 'SAVE' '.'; */
#line 1991 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAVE_STATEMENT);
    	break;}
    case 273: { /* <special_statement> = 'RESTORE' '.'; */
#line 1996 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTORE_STATEMENT);
    	break;}
    case 274: { /* <special_statement> = 'RESTART' '.'; */
#line 2001 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTART_STATEMENT);
    	break;}
    case 275: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 2006 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SCORE_STATEMENT);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scoreCount;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 276: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 2017 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, VISITS_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 277: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 2023 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SYSTEM_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 278: { /* <optional_expression> =; */
#line 2034 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 279: { /* <optional_expression> = <expression>; */
#line 2037 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp; 	break;}
    case 280: { /* <expression> = <term>; */
#line 2042 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 281: { /* <expression> = <expression> 'OR' <term>; */
#line 2047 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
						pmSeSt[pmStkP+1].exp,
						FALSE,
						OR_OPERATOR,
						pmSeSt[pmStkP+3].exp);
    	break;}
    case 282: { /* <term> = <factor>; */
#line 2058 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 283: { /* <term> = <term> 'AND' <factor>; */
#line 2063 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
					  pmSeSt[pmStkP+1].exp,
					  FALSE,
					  AND_OPERATOR,
					  pmSeSt[pmStkP+3].exp);
    	break;}
    case 284: { /* <factor> = <arithmetic>; */
#line 2074 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 286: { /* <factor> = <factor> <optional_not> <relop> <arithmetic>; */
#line 2079 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+3].srcp,
					    pmSeSt[pmStkP+1].exp,
					    pmSeSt[pmStkP+2].present,
					    pmSeSt[pmStkP+3].op,
					    pmSeSt[pmStkP+4].exp);
    	break;}
    case 285: { /* <factor> = <factor> <optional_not> <where>; */
#line 2088 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhereExpression(pmSeSt[pmStkP+3].srcp,
					   pmSeSt[pmStkP+1].exp,
					   pmSeSt[pmStkP+2].present,
					   pmSeSt[pmStkP+3].whr);
    	break;}
    case 288: { /* <factor> = <factor> <optional_not> 'BETWEEN' <arithmetic> 'AND' <arithmetic>; */
#line 2096 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBetweenExpression(pmSySt[pmStkP+3].srcp,
					     pmSeSt[pmStkP+1].exp,
					     pmSeSt[pmStkP+2].present,
					     pmSeSt[pmStkP+4].exp,
					     pmSeSt[pmStkP+6].exp);
    	break;}
    case 287: { /* <factor> = <factor> <optional_not> 'CONTAINS' <arithmetic>; */
#line 2105 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+3].srcp,
					    pmSeSt[pmStkP+1].exp,
					    pmSeSt[pmStkP+2].present,
					    CONTAINS_OPERATOR,
					    pmSeSt[pmStkP+4].exp);
    	break;}
    case 289: { /* <arithmetic> = <primary>; */
#line 2117 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 291: { /* <arithmetic> = <primary> 'ISA' ID; */
#line 2122 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].exp, FALSE, pmSeSt[pmStkP+3].id);
    	break;}
    case 292: { /* <arithmetic> = <primary> <is> <something>; */
#line 2127 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id,
					 pmSeSt[pmStkP+3].present, pmSeSt[pmStkP+1].exp);
    	break;}
    case 293: { /* <arithmetic> = <arithmetic> <binop> <primary>; */
#line 2133 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+2].srcp,
						pmSeSt[pmStkP+1].exp,
						FALSE,
						pmSeSt[pmStkP+2].op,
						pmSeSt[pmStkP+3].exp);
    	break;}
    case 290: { /* <arithmetic> = <aggregate> <filters>; */
#line 2142 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAggregateExpression(pmSeSt[pmStkP+1].srcp,
							pmSeSt[pmStkP+1].agr,
							pmSeSt[pmStkP+1].id,
							pmSeSt[pmStkP+2].exps);
    	break;}
    case 294: { /* <filters> = <filter>; */
#line 2153 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(NULL, pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 295: { /* <filters> = <filters> ',' <filter>; */
#line 2158 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(pmSeSt[pmStkP+1].exps, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 297: { /* <filter expression> = <optional_not> <where>; */
#line 2181 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhereExpression(pmSeSt[pmStkP+2].srcp,
						      NULL,
						      pmSeSt[pmStkP+1].present,
						      pmSeSt[pmStkP+2].whr);
    	break;}
    case 298: { /* <filter expression> = <optional_not> 'ISA' ID; */
#line 2189 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, NULL, pmSeSt[pmStkP+1].present, pmSeSt[pmStkP+3].id);
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 299: { /* <filter expression> = <is> <something>; */
#line 2195 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+1].srcp,
						pmSeSt[pmStkP+2].id,
						pmSeSt[pmStkP+2].present, NULL);
    	break;}
    case 301: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 2204 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+2].srcp,
						     NULL,
						     pmSeSt[pmStkP+1].present,
						     pmSeSt[pmStkP+2].op,
						     pmSeSt[pmStkP+3].exp);
    	break;}
    case 303: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 2213 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBetweenExpression(pmSySt[pmStkP+2].srcp,
						      NULL,
						      pmSeSt[pmStkP+1].present,
						      pmSeSt[pmStkP+3].exp,
						      pmSeSt[pmStkP+5].exp);
    	break;}
    case 302: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 2222 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
						     NULL,
						     pmSeSt[pmStkP+1].present,
						     CONTAINS_OPERATOR,
						     pmSeSt[pmStkP+3].exp);

    	break;}
    case 308: { /* <primary> = '(' <expression> ')'; */
#line 2237 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 304: { /* <primary> = <optional_minus> Integer; */
#line 2242 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp,
					      pmSeSt[pmStkP+1].minus?
							-val(pmSySt[pmStkP+2].chars):
							val(pmSySt[pmStkP+2].chars));
    	break;}
    case 305: { /* <primary> = STRING; */
#line 2250 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp,
					     pmSySt[pmStkP+1].fpos,
					     pmSySt[pmStkP+1].len);
    	break;}
    case 306: { /* <primary> = <what>; */
#line 2257 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 310: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 2262 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newRandomRangeExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp, pmSeSt[pmStkP+4].exp);
    	break;}
    case 309: { /* <primary> = 'RANDOM' 'IN' <primary>; */
#line 2267 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newRandomInExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+3].exp);
    	break;}
    case 307: { /* <primary> = 'SCORE'; */
#line 2272 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newScoreExpression(pmSySt[pmStkP+1].srcp);
    	break;}
    case 312: { /* <aggregate> = <aggregator> 'OF' ID; */
#line 2280 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 311: { /* <aggregate> = 'COUNT'; */
#line 2287 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = COUNT_AGGREGATE;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 313: { /* <aggregator> = 'MAX'; */
#line 2296 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MAX_AGGREGATE;
    	break;}
    case 314: { /* <aggregator> = 'MIN'; */
#line 2302 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MIN_AGGREGATE;
    	break;}
    case 315: { /* <aggregator> = 'SUM'; */
#line 2308 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = SUM_AGGREGATE;
    	break;}
    case 316: { /* <something> = <optional_not> ID; */
#line 2317 "alan.pmk"

	pmSeSt[pmStkP+1].present = pmSeSt[pmStkP+1].present;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 317: { /* <what> = <simple_what>; */
#line 2325 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhatExpression(pmSeSt[pmStkP+1].srcp, pmSeSt[pmStkP+1].wht);
    	break;}
    case 318: { /* <what> = <attribute_reference>; */
#line 2330 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 319: { /* <simple_what> = ID; */
#line 2337 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSeSt[pmStkP+1].id->srcp, WHAT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 321: { /* <simple_what> = 'CURRENT' 'ACTOR'; */
#line 2343 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_ACTOR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 322: { /* <simple_what> = 'CURRENT' 'LOCATION'; */
#line 2349 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_LOCATION, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 320: { /* <simple_what> = 'THIS'; */
#line 2355 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_THIS, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 323: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2364 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].id, FALSE, pmSeSt[pmStkP+3].exp);
    	break;}
    case 324: { /* <attribute_reference> = <what> ':' ID; */
#line 2369 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id, FALSE, pmSeSt[pmStkP+1].exp);
    	break;}
    case 325: { /* <optional_where> =; */
#line 2376 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&nulsrcp, FALSE, WHERE_DEFAULT, NULL);
    	break;}
    case 326: { /* <optional_where> = <where>; */
#line 2381 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 327: { /* <where> = <optional_directly> 'HERE'; */
#line 2388 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].present, WHERE_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 328: { /* <where> = <optional_directly> 'NEARBY'; */
#line 2394 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].present, WHERE_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 329: { /* <where> = <optional_directly> 'AT' <what>; */
#line 2400 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].present, WHERE_AT, pmSeSt[pmStkP+3].exp);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 330: { /* <where> = <optional_directly> 'IN' <what>; */
#line 2406 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].present, WHERE_IN, pmSeSt[pmStkP+3].exp);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 331: { /* <binop> = '+'; */
#line 2414 "alan.pmk"

	pmSeSt[pmStkP+1].op = PLUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 332: { /* <binop> = '-'; */
#line 2420 "alan.pmk"

	pmSeSt[pmStkP+1].op = MINUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 333: { /* <binop> = '*'; */
#line 2426 "alan.pmk"

	pmSeSt[pmStkP+1].op = MULT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 334: { /* <binop> = '/'; */
#line 2432 "alan.pmk"

	pmSeSt[pmStkP+1].op = DIV_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 335: { /* <relop> = '<>'; */
#line 2441 "alan.pmk"

        pmSeSt[pmStkP+1].op   = NE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 336: { /* <relop> = '='; */
#line 2447 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EQ_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 337: { /* <relop> = '=='; */
#line 2453 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EXACT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 338: { /* <relop> = '>='; */
#line 2459 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 339: { /* <relop> = '<='; */
#line 2465 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 340: { /* <relop> = '>'; */
#line 2471 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 341: { /* <relop> = '<'; */
#line 2477 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 342: { /* <optional_qual> =; */
#line 2487 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 343: { /* <optional_qual> = 'BEFORE'; */
#line 2492 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 344: { /* <optional_qual> = 'AFTER'; */
#line 2498 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 345: { /* <optional_qual> = 'ONLY'; */
#line 2504 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 346: { /* <optional_not> =; */
#line 2513 "alan.pmk"

        pmSeSt[pmStkP+1].present = FALSE;
    	break;}
    case 347: { /* <optional_not> = 'NOT'; */
#line 2518 "alan.pmk"

        pmSeSt[pmStkP+1].present = TRUE;
    	break;}
    case 348: { /* <optional_directly> =; */
#line 2525 "alan.pmk"

        pmSeSt[pmStkP+1].present = FALSE;
    	break;}
    case 349: { /* <optional_directly> = 'DIRECTLY'; */
#line 2530 "alan.pmk"

        pmSeSt[pmStkP+1].present = TRUE;
    	break;}
    case 350: { /* <optional_id> =; */
#line 2537 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 351: { /* <optional_id> = ID; */
#line 2542 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 352: { /* <ids> = ID; */
#line 2549 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 353: { /* <ids> = <ids> ID; */
#line 2554 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, ID_LIST);
    	break;}
    case 354: { /* <id_list> = ID; */
#line 2561 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 355: { /* <id_list> = <id_list> ',' ID; */
#line 2567 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, ID_LIST);
    	break;}
    case 356: { /* <optional_integer> =; */
#line 2575 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 357: { /* <optional_integer> = Integer; */
#line 2580 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 358: { /* <optional_minus> =; */
#line 2587 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 359: { /* <optional_minus> = '-'; */
#line 2592 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 360: { /* ID = Identifier; */
#line 2599 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 361: { /* ID = 'LOCATION'; */
#line 2604 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 362: { /* ID = 'ACTOR'; */
#line 2609 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 363: { /* ID = 'OPAQUE'; */
#line 2614 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 364: { /* ID = 'ON'; */
#line 2619 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 365: { /* ID = 'OF'; */
#line 2624 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 366: { /* ID = 'FIRST'; */
#line 2629 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 367: { /* ID = 'INTO'; */
#line 2634 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 368: { /* ID = 'TAKING'; */
#line 2639 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    default: break; }
}/*pmPaSema()*/

