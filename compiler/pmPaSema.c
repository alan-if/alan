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
    What *wht;
    Where *whr;
    Bool wordOrChar;
    List *vrbs;
    Verb *vrb;
    int val;
    List *syns;
    SynNod *syn;
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
    ResNod *res;
    QualKind qual;
    Bool opaque;
    OperatorKind op;
    Bool not;
    List *nams;
    List *nam;
    List *msgs;
    MsgNod *msg;
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
    ExtNod *ext;
    List *exps;
    ExpressionKind expKd;
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
    ClaNod *cla;
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
#line 191 "alan.pmk"
    switch (rule) {
    case 8: { /* <option> = ID '.'; */
#line 211 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 9: { /* <option> = ID ID '.'; */
#line 216 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 10: { /* <option> = ID Integer '.'; */
#line 221 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 18: { /* <declaration> = <synonyms>; */
#line 234 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns);	break;}
    case 14: { /* <declaration> = <messages>; */
#line 237 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 19: { /* <declaration> = <syntax>; */
#line 240 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 20: { /* <declaration> = <verb>; */
#line 243 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VERB_LIST); 	break;}
    case 15: { /* <declaration> = <class>; */
#line 246 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLASS_LIST); 	break;}
    case 21: { /* <declaration> = <addition>; */
#line 249 "alan.pmk"
 adv.adds = concat(adv.adds, pmSeSt[pmStkP+1].add, ADD_LIST); 	break;}
    case 16: { /* <declaration> = <instance>; */
#line 252 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSTANCE_LIST); 	break;}
    case 22: { /* <declaration> = <event>; */
#line 255 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVENT_LIST); 	break;}
    case 17: { /* <declaration> = <rule>; */
#line 258 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULE_LIST); 	break;}
    case 23: { /* <attributes> = <attribute definition> '.'; */
#line 266 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, ATTRIBUTE_LIST);
    	break;}
    case 24: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 271 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATTRIBUTE_LIST);
    	break;}
    case 25: { /* <attribute definition> = ID; */
#line 278 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, TRUE);
    	break;}
    case 26: { /* <attribute definition> = 'NOT' ID; */
#line 283 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+2].id->srcp,	pmSeSt[pmStkP+2].id, FALSE);
    	break;}
    case 27: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 288 "alan.pmk"

	if (pmSeSt[pmStkP+2].minus)
		pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, -val(pmSySt[pmStkP+3].chars));
	else
		pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, val(pmSySt[pmStkP+3].chars));
    	break;}
    case 28: { /* <attribute definition> = ID STRING; */
#line 296 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newStringAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 29: { /* <attribute definition> = ID ID; */
#line 300 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newReferenceAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+2].id);
    	break;}
    case 30: { /* <attribute definition> = ID '{' <optional members> '}'; */
#line 305 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newSetAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].set);
    	break;}
    case 31: { /* <optional members> =; */
#line 312 "alan.pmk"
 pmSeSt[pmStkP+1].set = NULL; 	break;}
    case 32: { /* <optional members> = <set_members>; */
#line 314 "alan.pmk"
 pmSeSt[pmStkP+1].set = pmSeSt[pmStkP+1].set; 	break;}
    case 33: { /* <set_members> = <set_member>; */
#line 319 "alan.pmk"

	pmSeSt[pmStkP+1].set = concat(NULL, pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 34: { /* <set_members> = <set_members> ',' <set_member>; */
#line 324 "alan.pmk"

	pmSeSt[pmStkP+1].set = concat(pmSeSt[pmStkP+1].set, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 35: { /* <set_member> = ID; */
#line 331 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhatExpression(pmSeSt[pmStkP+1].id->srcp, newWhat(&pmSeSt[pmStkP+1].id->srcp, WHAT_ID, pmSeSt[pmStkP+1].id));
    	break;}
    case 36: { /* <set_member> = <optional_minus> Integer; */
#line 336 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].minus?-val(pmSySt[pmStkP+2].chars):val(pmSySt[pmStkP+2].chars));
    	break;}
    case 37: { /* <set_member> = STRING; */
#line 341 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].fpos, pmSySt[pmStkP+1].len);
    	break;}
    case 38: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 349 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 39: { /* <synonym_list> = <synonym_declaration>; */
#line 356 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, SYNONYM_LIST);
    	break;}
    case 40: { /* <synonym_list> = <synonym_list> <synonym_declaration>; */
#line 361 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNONYM_LIST);
    	break;}
    case 41: { /* <synonym_declaration> = <id_list> '=' ID '.'; */
#line 368 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			     			pmSeSt[pmStkP+1].idList,
			      			pmSeSt[pmStkP+3].id);
    	break;}
    case 42: { /* <messages> = 'MESSAGE' <message_list>; */
#line 378 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 43: { /* <message_list> = <message>; */
#line 385 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, MESSAGE_LIST);
    	break;}
    case 44: { /* <message_list> = <message_list> <message>; */
#line 390 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MESSAGE_LIST);
    	break;}
    case 45: { /* <message> = ID ':' <statements>; */
#line 397 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 46: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 407 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 47: { /* <syntax_list> = <syntax_item>; */
#line 414 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, SYNTAX_LIST);
    	break;}
    case 48: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 419 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, SYNTAX_LIST);
    	break;}
    case 49: { /* <syntax_item> = ID '=' <syntax_elements> <optional_syntax_restrictions>; */
#line 426 "alan.pmk"

	pmSeSt[pmStkP+1].stx = newSyntax(pmSySt[pmStkP+2].srcp,
				  	pmSeSt[pmStkP+1].id,
				  	concat(pmSeSt[pmStkP+3].elms,
				        	newElement(&pmSeSt[pmStkP+1].id->srcp,
							   END_OF_SYNTAX,
							   NULL,
							   FALSE),
						ELEMENT_LIST),
					pmSeSt[pmStkP+4].ress,
					pmSeSt[pmStkP+4].srcp);
    	break;}
    case 50: { /* <syntax_elements> = <syntax_element>; */
#line 442 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, ELEMENT_LIST);
    	break;}
    case 51: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 447 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                ELEMENT_LIST);
    	break;}
    case 52: { /* <syntax_element> = ID; */
#line 456 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newElement(&pmSeSt[pmStkP+1].id->srcp, WORD_ELEMENT,
					   pmSeSt[pmStkP+1].id,
					   FALSE);
    	break;}
    case 53: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 464 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newElement(&pmSeSt[pmStkP+2].id->srcp, PARAMETER_ELEMENT,
					   pmSeSt[pmStkP+2].id,
					   pmSeSt[pmStkP+4].bits);
    	break;}
    case 54: { /* <optional_indicators> =; */
#line 473 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 55: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 478 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 56: { /* <indicator> = '*'; */
#line 485 "alan.pmk"

	pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 57: { /* <indicator> = '!'; */
#line 490 "alan.pmk"

	pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 58: { /* <syntax_restriction_clauses> = <syntax_restriction>; */
#line 503 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, RESTRICTION_LIST);
    	break;}
    case 59: { /* <syntax_restriction_clauses> = <syntax_restriction_clauses> 'AND' <syntax_restriction>; */
#line 508 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, RESTRICTION_LIST);
    	break;}
    case 60: { /* <syntax_restriction> = ID 'ISA' <restriction_class> 'ELSE' <statements>; */
#line 516 "alan.pmk"

	pmSeSt[pmStkP+1].res = newRestriction(&pmSySt[pmStkP+2].srcp,
			  			  pmSeSt[pmStkP+1].id,
						  pmSeSt[pmStkP+3].restrictionKind,
						  pmSeSt[pmStkP+3].id,
						  pmSeSt[pmStkP+5].stms);
    	break;}
    case 61: { /* <restriction_class> = ID; */
#line 527 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].restrictionKind = ID_RESTRICTION;
    	break;}
    case 62: { /* <restriction_class> = 'CONTAINER'; */
#line 533 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
	pmSeSt[pmStkP+1].restrictionKind = CONTAINER_RESTRICTION;
    	break;}
    case 63: { /* <optional_syntax_restrictions> = '.'; */
#line 541 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 64: { /* <optional_syntax_restrictions> = 'WHERE' <syntax_restriction_clauses>; */
#line 546 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 65: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 556 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newVerb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (compareStrings(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 66: { /* <verb_header> = 'VERB' <id_list>; */
#line 569 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 67: { /* <verb_body> = <simple_verb_body>; */
#line 578 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&pmSeSt[pmStkP+1].srcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   ALTERNATIVE_LIST);
    	break;}
    case 68: { /* <verb_body> = <verb_alternatives>; */
#line 589 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 69: { /* <verb_alternatives> = <verb_alternative>; */
#line 596 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, ALTERNATIVE_LIST);
    	break;}
    case 70: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 601 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				ALTERNATIVE_LIST);
    	break;}
    case 71: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 610 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 72: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 621 "alan.pmk"

	if (pmSeSt[pmStkP+2].stms == NULL) /* No DOES */
	  pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	else
	  pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 73: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */
#line 634 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 74: { /* <optional_checks> =; */
#line 642 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].srcp = nulsrcp;
    	break;}
    case 75: { /* <optional_checks> = 'CHECK' <statements>; */
#line 648 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), CHECK_LIST);
    	break;}
    case 76: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 654 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 77: { /* <check_list> = <check>; */
#line 662 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, CHECK_LIST);
    	break;}
    case 78: { /* <check_list> = <check_list> 'AND' <check>; */
#line 667 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHECK_LIST);
    	break;}
    case 79: { /* <check> = <expression> 'ELSE' <statements>; */
#line 674 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 80: { /* <optional_does> =; */
#line 682 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].srcp = nulsrcp;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 81: { /* <optional_does> = <does>; */
#line 689 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+1].qual;
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 82: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 698 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 83: { /* <class> = 'EVERY' ID <optional_heritage> <properties> <class_tail>; */
#line 709 "alan.pmk"

	pmSeSt[pmStkP+1].cla = newClass(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newProps(pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].whr,
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
#line 742 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 87: { /* <addition> = 'ADD' 'TO' __genSym#2 ID <optional_heritage> <properties> <add_tail>; */
#line 749 "alan.pmk"

	pmSeSt[pmStkP+1].add = newAdd(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+4].id,
				pmSeSt[pmStkP+5].id,
				newProps(pmSeSt[pmStkP+6].nams,
					pmSeSt[pmStkP+6].whr,
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
				newProps(pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].whr,
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
#line 822 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 99: { /* <optional_heritage> =; */
#line 827 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 100: { /* <optional_heritage> = <heritage>; */
#line 832 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 101: { /* <heritage> = 'ISA' ID __genSym#6; */
#line 839 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 104: { /* <properties> =; */
#line 844 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out all fields */
    	break;}
    case 105: { /* <properties> = <properties> <property>; */
#line 849 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+2].nam, NAME_LIST);

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
#line 940 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 106: { /* <property> = <where> __genSym#7; */
#line 946 "alan.pmk"
{
        Where *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 114: { /* <property> = <is> <attributes>; */
#line 956 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 107: { /* <property> = <container_properties>; */
#line 962 "alan.pmk"
{
        Container *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 108: { /* <property> = <description>; */
#line 972 "alan.pmk"
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
    case 111: { /* <property> = <entered>; */
#line 987 "alan.pmk"
{
        List *entered = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].entered = entered;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 112: { /* <property> = <mentioned>; */
#line 997 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].mentioned = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 109: { /* <property> = __genSym#8 <article_or_form>; */
#line 1007 "alan.pmk"
{ List *indefinite = pmSeSt[pmStkP+2].stms;
	Bool isForm = pmSeSt[pmStkP+2].isForm;
        Srcp srcp = pmSeSt[pmStkP+2].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].indefinite = indefinite;
	pmSeSt[pmStkP+1].indefiniteIsForm = isForm;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 113: { /* <property> = 'DEFINITE' <article_or_form>; */
#line 1019 "alan.pmk"
{ List *definite = pmSeSt[pmStkP+2].stms;
	Bool isForm = pmSeSt[pmStkP+2].isForm;
        Srcp srcp = pmSeSt[pmStkP+2].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].definite = definite;
	pmSeSt[pmStkP+1].definiteIsForm = isForm;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 116: { /* <property> = <exit>; */
#line 1031 "alan.pmk"
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 117: { /* <property> = <verb>; */
#line 1037 "alan.pmk"
{ Verb *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 115: { /* <property> = <script>; */
#line 1043 "alan.pmk"
{ Script *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 122: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 1053 "alan.pmk"

	pmSeSt[pmStkP+1].ext = newext(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].idList,
			   pmSeSt[pmStkP+4].id,
			   pmSeSt[pmStkP+5].chks,
			   pmSeSt[pmStkP+5].stms);
	if (pmSeSt[pmStkP+5].id != NULL) { /* END-id given */
	    if (compareStrings(pmSeSt[pmStkP+2].str, pmSeSt[pmStkP+5].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].str);
	}
    	break;}
    case 123: { /* <optional_exit_body> =; */
#line 1068 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 124: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 1075 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 125: { /* <optional_attributes> =; */
#line 1090 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 126: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 1095 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 127: { /* <is> = 'IS'; */
#line 1102 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 128: { /* <is> = 'ARE'; */
#line 1107 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 129: { /* <is> = 'HAS'; */
#line 1112 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 130: { /* <optional_description> =; */
#line 1119 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 131: { /* <optional_description> = <description>; */
#line 1125 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 132: { /* <description> = 'DESCRIPTION' <optional_checks> <optional_does>; */
#line 1133 "alan.pmk"

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
    case 133: { /* <description> = 'DESCRIPTION' <statements>; */
#line 1158 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].srcp2 = nulsrcp;
    	break;}
    case 134: { /* <article_or_form> = <article>; */
#line 1166 "alan.pmk"
 pmSeSt[pmStkP+1].isForm = FALSE; 	break;}
    case 135: { /* <article_or_form> = <form>; */
#line 1167 "alan.pmk"
 pmSeSt[pmStkP+1].isForm = TRUE; 	break;}
    case 136: { /* <article> = 'ARTICLE'; */
#line 1172 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newStatement(&pmSySt[pmStkP+1].srcp,
							NOP_STATEMENT), STATEMENT_LIST);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 137: { /* <article> = 'ARTICLE' <statements>; */
#line 1179 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 138: { /* <form> = 'FORM'; */
#line 1187 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newStatement(&pmSySt[pmStkP+1].srcp,
						 NOP_STATEMENT), STATEMENT_LIST);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 139: { /* <form> = 'FORM' <statements>; */
#line 1194 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 140: { /* <entered> = 'ENTERED' <statements>; */
#line 1203 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 141: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 1212 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 142: { /* <optional_names> =; */
#line 1221 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 143: { /* <optional_names> = <optional_names> <name>; */
#line 1226 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					pmSeSt[pmStkP+2].nam, NAME_LIST);
    	break;}
    case 144: { /* <name> = 'NAME' <ids> <optional_full_stop>; */
#line 1234 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 147: { /* <container_properties> = __genSym#9 <optionally_opaque> 'CONTAINER' <container_body>; */
#line 1247 "alan.pmk"

	pmSeSt[pmStkP+1].cnt = newContainer(newContainerBody(&pmSySt[pmStkP+3].srcp,
					pmSeSt[pmStkP+2].opaque,
					pmSeSt[pmStkP+4].id,
					pmSeSt[pmStkP+4].lims,
					pmSeSt[pmStkP+4].stms,
					pmSeSt[pmStkP+4].stms2,
					pmSeSt[pmStkP+4].chks,
					pmSeSt[pmStkP+4].stms3));
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+3].srcp;
    	break;}
    case 150: { /* <optionally_opaque> =; */
#line 1263 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = FALSE; 	break;}
    case 151: { /* <optionally_opaque> = 'OPAQUE'; */
#line 1265 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = TRUE; 	break;}
    case 152: { /* <container_body> = <optional_taking> <optional_limits> <optional_header> <optional_empty> <optional_extract>; */
#line 1271 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+2].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+3].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+4].stms;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+5].chks;
	pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+5].stms;
    	break;}
    case 154: { /* <optional_taking> =; */
#line 1285 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 155: { /* <optional_taking> = 'TAKING' ID '.'; */
#line 1290 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 156: { /* <optional_limits> =; */
#line 1298 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 157: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1303 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 158: { /* <limits> = <limit>; */
#line 1310 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIMIT_LIST);
    	break;}
    case 159: { /* <limits> = <limits> <limit>; */
#line 1315 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMIT_LIST);
    	break;}
    case 160: { /* <limit> = <limit_attribute> <else_or_then> <statements>; */
#line 1322 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSeSt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 161: { /* <else_or_then> = 'ELSE'; */
#line 1330 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 162: { /* <else_or_then> = 'THEN'; */
#line 1331 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 163: { /* <limit_attribute> = <attribute definition>; */
#line 1336 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 164: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1341 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSySt[pmStkP+1].srcp, newId(&pmSySt[pmStkP+1].srcp, "count"), val(pmSySt[pmStkP+2].chars));
    	break;}
    case 165: { /* <optional_header> =; */
#line 1348 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 166: { /* <optional_header> = 'HEADER' <statements>; */
#line 1353 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 167: { /* <optional_empty> =; */
#line 1360 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 168: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1365 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 169: { /* <optional_extract> =; */
#line 1372 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 170: { /* <optional_extract> = 'EXTRACT' <optional_checks> <optional_does>; */
#line 1378 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
	if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	  lmLog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
    	break;}
    case 171: { /* <optional_extract> = 'EXTRACT' <statements>; */
#line 1386 "alan.pmk"

	pmSeSt[pmStkP+1].chk = NULL;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 172: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1396 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 173: { /* <event_header> = 'EVENT' ID; */
#line 1409 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 174: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1417 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 175: { /* <script> = 'SCRIPT' ID __genSym#10 <optional_description> <step_list>; */
#line 1425 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newScript(&pmSySt[pmStkP+1].srcp,
			          pmSeSt[pmStkP+2].id,
			          pmSeSt[pmStkP+4].stms,
			          pmSeSt[pmStkP+5].stps);
    	break;}
    case 178: { /* <step_list> = <step>; */
#line 1435 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, STEP_LIST);
    	break;}
    case 179: { /* <step_list> = <step_list> <step>; */
#line 1440 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STEP_LIST);
    	break;}
    case 180: { /* <step> = 'STEP' <statements>; */
#line 1447 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 181: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1455 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 182: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1463 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 183: { /* <rule> = 'WHEN' <expression> <then> <statements> <optional_end_when>; */
#line 1474 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 190: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1494 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 191: { /* <optional_statements> =; */
#line 1503 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 192: { /* <optional_statements> = <statements>; */
#line 1508 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 193: { /* <statements> = <statement>; */
#line 1515 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, STATEMENT_LIST);
    	break;}
    case 194: { /* <statements> = <statements> <statement>; */
#line 1520 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STATEMENT_LIST);
    	break;}
    case 195: { /* <statement> = <output_statement>; */
#line 1527 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 197: { /* <statement> = <manipulation_statement>; */
#line 1532 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 200: { /* <statement> = <assignment_statement>; */
#line 1537 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 202: { /* <statement> = <conditional_statement>; */
#line 1542 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 201: { /* <statement> = <repetition_statement>; */
#line 1547 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 198: { /* <statement> = <actor_statement>; */
#line 1552 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 199: { /* <statement> = <event_statement>; */
#line 1557 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 196: { /* <statement> = <special_statement>; */
#line 1562 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 203: { /* <output_statement> = STRING; */
#line 1570 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, PRINT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 204: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1577 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newDescribeStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 205: { /* <output_statement> = 'SAY' <say_form> <expression> '.'; */
#line 1582 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.say.form = pmSeSt[pmStkP+2].sayForm;
    	break;}
    case 206: { /* <output_statement> = 'LIST' <what> '.'; */
#line 1589 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, LIST_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].exp;
    	break;}
    case 207: { /* <output_statement> = 'SHOW' ID '.'; */
#line 1595 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SHOW_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.show.filename = pmSeSt[pmStkP+2].id;
    	break;}
    case 208: { /* <say_form> =; */
#line 1603 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_SIMPLE; 	break;}
    case 209: { /* <say_form> = 'THE'; */
#line 1605 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_DEFINITE; 	break;}
    case 210: { /* <say_form> = 'AN'; */
#line 1607 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_INDEFINITE; 	break;}
    case 211: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */
#line 1614 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newEmptyStatement(pmSySt[pmStkP+1].srcp,
							  pmSeSt[pmStkP+2].exp,
							  pmSeSt[pmStkP+3].whr);
    	break;}
    case 212: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */
#line 1621 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newLocateStatement(pmSySt[pmStkP+1].srcp,
							   pmSeSt[pmStkP+2].exp,
							   pmSeSt[pmStkP+3].whr);
    	break;}
    case 213: { /* <manipulation_statement> = 'INCLUDE' <primary> 'IN' <what> '.'; */
#line 1628 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newIncludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 214: { /* <manipulation_statement> = 'EXCLUDE' <primary> 'FROM' <what> '.'; */
#line 1635 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newExcludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 216: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1646 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SCHEDULE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 215: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1654 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, CANCEL_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 217: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */
#line 1664 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, MAKE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 221: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1672 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SET_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 219: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1679 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, INCREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 220: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1686 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DECREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 218: { /* <assignment_statement> = 'STRIP' <optional_first_or_last> <optional_expression> <optional_word_or_character> 'FROM' <expression> <optional_into> '.'; */
#line 1693 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STRIP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.strip.first = pmSeSt[pmStkP+2].first;
	pmSeSt[pmStkP+1].stm->fields.strip.count = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.wordOrChar = pmSeSt[pmStkP+4].wordOrChar;
	pmSeSt[pmStkP+1].stm->fields.strip.from = pmSeSt[pmStkP+6].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.into = pmSeSt[pmStkP+7].exp;
    	break;}
    case 222: { /* <optional_by_clause> =; */
#line 1705 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 223: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1710 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 224: { /* <optional_first_or_last> =; */
#line 1717 "alan.pmk"
 pmSeSt[pmStkP+1].first = TRUE; 	break;}
    case 225: { /* <optional_first_or_last> = 'FIRST'; */
#line 1719 "alan.pmk"
 pmSeSt[pmStkP+1].first = TRUE; 	break;}
    case 226: { /* <optional_first_or_last> = 'LAST'; */
#line 1721 "alan.pmk"
 pmSeSt[pmStkP+1].first = FALSE; 	break;}
    case 227: { /* <optional_word_or_character> =; */
#line 1726 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 228: { /* <optional_word_or_character> = 'WORD'; */
#line 1728 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 229: { /* <optional_word_or_character> = 'WORDS'; */
#line 1730 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 230: { /* <optional_word_or_character> = 'CHARACTER'; */
#line 1732 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = FALSE; 	break;}
    case 231: { /* <optional_word_or_character> = 'CHARACTERS'; */
#line 1734 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = FALSE; 	break;}
    case 232: { /* <optional_into> =; */
#line 1739 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 233: { /* <optional_into> = 'INTO' <expression>; */
#line 1742 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp; 	break;}
    case 234: { /* <conditional_statement> = <if_statement>; */
#line 1748 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 235: { /* <conditional_statement> = <depending_statement>; */
#line 1753 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 236: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 1761 "alan.pmk"

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
    case 237: { /* <optional_elsif_list> =; */
#line 1779 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 238: { /* <optional_elsif_list> = <elsif_list>; */
#line 1784 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 239: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 1791 "alan.pmk"
{
	Statement *stm;

	stm = newStatement(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, STATEMENT_LIST);
    }	break;}
    case 240: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 1804 "alan.pmk"
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
    case 241: { /* <optional_else_part> =; */
#line 1823 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 242: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 1828 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 243: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' __genSym#12 '.'; */
#line 1835 "alan.pmk"
{ Statement *stm;
	stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPEND_STATEMENT);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 246: { /* <depend_cases> = <depend_case>; */
#line 1846 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, CASE_LIST);
    	break;}
    case 247: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1851 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASE_LIST);
    	break;}
    case 249: { /* <depend_case> = <right_hand_side> 'THEN' <statements>; */
#line 1858 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+2].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 248: { /* <depend_case> = 'ELSE' <statements>; */
#line 1865 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 250: { /* <repetition_statement> = <for_each> ID <optional_loop_restrictions> 'DO' <statements> 'END' <for_each> __genSym#13; */
#line 1875 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSeSt[pmStkP+1].srcp, EACH_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.each.stms = pmSeSt[pmStkP+5].stms;
	pmSeSt[pmStkP+1].stm->fields.each.loopId = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.each.classId = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.each.filters = pmSeSt[pmStkP+3].exps;
    	break;}
    case 253: { /* <optional_loop_restrictions> =; */
#line 1886 "alan.pmk"

	pmSeSt[pmStkP+1].exps = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 254: { /* <optional_loop_restrictions> = <filters>; */
#line 1892 "alan.pmk"

	pmSeSt[pmStkP+1].exps = pmSeSt[pmStkP+1].exps;
	/* While refactoring, for now, use first element */
	if (pmSeSt[pmStkP+1].exps->element.exp->kind == ISA_EXPRESSION)
	    pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].exps->element.exp->fields.isa.class;
    	break;}
    case 255: { /* <for_each> = 'FOR'; */
#line 1902 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 256: { /* <for_each> = 'EACH'; */
#line 1904 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 257: { /* <for_each> = 'FOR' 'EACH'; */
#line 1906 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 259: { /* <actor_statement> = 'USE' 'SCRIPT' ID <optional_for_actor> '.'; */
#line 1912 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, USE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.actorExp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 258: { /* <actor_statement> = 'STOP' <what> '.'; */
#line 1919 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STOP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.stop.actor = pmSeSt[pmStkP+2].exp;
    	break;}
    case 260: { /* <optional_for_actor> =; */
#line 1927 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 261: { /* <optional_for_actor> = 'FOR' <what>; */
#line 1932 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 262: { /* <special_statement> = 'QUIT' '.'; */
#line 1940 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, QUIT_STATEMENT);
    	break;}
    case 263: { /* <special_statement> = 'LOOK' '.'; */
#line 1945 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, LOOK_STATEMENT);
    	break;}
    case 264: { /* <special_statement> = 'SAVE' '.'; */
#line 1950 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAVE_STATEMENT);
    	break;}
    case 265: { /* <special_statement> = 'RESTORE' '.'; */
#line 1955 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTORE_STATEMENT);
    	break;}
    case 266: { /* <special_statement> = 'RESTART' '.'; */
#line 1960 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTART_STATEMENT);
    	break;}
    case 267: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 1965 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SCORE_STATEMENT);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scoreCount;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 268: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 1976 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, VISITS_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 269: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 1982 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SYSTEM_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 270: { /* <optional_expression> =; */
#line 1993 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 271: { /* <optional_expression> = <expression>; */
#line 1996 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp; 	break;}
    case 272: { /* <expression> = <term>; */
#line 2001 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 273: { /* <expression> = <expression> 'OR' <term>; */
#line 2006 "alan.pmk"
 { Expression *exp;
	exp = newExpression(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OR_OPERATOR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 274: { /* <term> = <factor>; */
#line 2018 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 275: { /* <term> = <term> 'AND' <factor>; */
#line 2023 "alan.pmk"
 { Expression *exp;
	exp = newExpression(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not	      = FALSE;
	exp->fields.bin.op    = AND_OPERATOR;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 276: { /* <factor> = <arithmetic>; */
#line 2035 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 278: { /* <factor> = <factor> <optional_not> <relop> <arithmetic>; */
#line 2040 "alan.pmk"
 { Expression *exp;
	exp = newExpression(&pmSeSt[pmStkP+3].srcp, BINARY_EXPRESSION);
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.bin.op = pmSeSt[pmStkP+3].op;
	exp->fields.bin.right = pmSeSt[pmStkP+4].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 277: { /* <factor> = <factor> <optional_not> <where>; */
#line 2051 "alan.pmk"
 { Expression *exp;
	exp = newExpression(&pmSeSt[pmStkP+3].srcp, WHERE_EXPRESSION);
	exp->fields.whr.wht = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = WHERE_EXPRESSION;
    } 	break;}
    case 280: { /* <factor> = <factor> <optional_not> 'BETWEEN' <arithmetic> 'AND' <arithmetic>; */
#line 2061 "alan.pmk"
 { Expression *exp;
	exp = newExpression(&pmSySt[pmStkP+3].srcp, BETWEEN_EXPRESSION);
	exp->fields.btw.val = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.btw.low = pmSeSt[pmStkP+4].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+6].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BETWEEN_EXPRESSION;
    } 	break;}
    case 279: { /* <factor> = <factor> <optional_not> 'CONTAINS' <arithmetic>; */
#line 2072 "alan.pmk"
 { Expression *exp;
	exp = newExpression(&pmSySt[pmStkP+3].srcp, BINARY_EXPRESSION);
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.bin.op = CONTAINS_OPERATOR;
	exp->fields.bin.right = pmSeSt[pmStkP+4].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 281: { /* <arithmetic> = <primary>; */
#line 2086 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 283: { /* <arithmetic> = <primary> 'ISA' ID; */
#line 2091 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].exp, FALSE, pmSeSt[pmStkP+3].id);
        pmSeSt[pmStkP+1].expKd = ISA_EXPRESSION;
    	break;}
    case 284: { /* <arithmetic> = <primary> <is> <something>; */
#line 2097 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id,
					 pmSeSt[pmStkP+3].not, pmSeSt[pmStkP+1].exp);
        pmSeSt[pmStkP+1].expKd = ATTRIBUTE_EXPRESSION;
    	break;}
    case 285: { /* <arithmetic> = <arithmetic> <binop> <primary>; */
#line 2104 "alan.pmk"
 { Expression *exp;
	exp = newExpression(&pmSeSt[pmStkP+2].srcp, BINARY_EXPRESSION);
 	exp->not = FALSE;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 282: { /* <arithmetic> = <aggregate> <filters>; */
#line 2115 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAggregateExpression(pmSeSt[pmStkP+1].srcp,
							pmSeSt[pmStkP+1].agr,
							pmSeSt[pmStkP+1].id,
							pmSeSt[pmStkP+2].exps);
    	break;}
    case 286: { /* <filters> = <filter>; */
#line 2126 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(NULL, pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 287: { /* <filters> = <filters> ',' <filter>; */
#line 2131 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(pmSeSt[pmStkP+1].exps, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 289: { /* <filter expression> = <optional_not> <where>; */
#line 2156 "alan.pmk"
 { Expression *exp;
	exp = newExpression(&pmSeSt[pmStkP+2].srcp, WHERE_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = WHERE_EXPRESSION;
    } 	break;}
    case 290: { /* <filter expression> = <optional_not> 'ISA' ID; */
#line 2165 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, NULL, pmSeSt[pmStkP+1].not, pmSeSt[pmStkP+3].id);
        pmSeSt[pmStkP+1].expKd = ISA_EXPRESSION;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 291: { /* <filter expression> = <is> <something>; */
#line 2172 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+1].srcp,
					pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+2].not, NULL);
        pmSeSt[pmStkP+1].expKd = ATTRIBUTE_EXPRESSION;
    	break;}
    case 293: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 2181 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newExpression(&pmSeSt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 295: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 2191 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BETWEEN_EXPRESSION;
	exp = newExpression(&pmSySt[pmStkP+2].srcp, BETWEEN_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 294: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 2201 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newExpression(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = CONTAINS_OPERATOR;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 300: { /* <primary> = '(' <expression> ')'; */
#line 2216 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 296: { /* <primary> = <optional_minus> Integer; */
#line 2221 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newExpression(&pmSySt[pmStkP+2].srcp, INTEGER_EXPRESSION);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 297: { /* <primary> = STRING; */
#line 2230 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newExpression(&pmSySt[pmStkP+1].srcp, STRING_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 298: { /* <primary> = <what>; */
#line 2237 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 301: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 2242 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newExpression(&pmSySt[pmStkP+1].srcp, RANDOM_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 299: { /* <primary> = 'SCORE'; */
#line 2249 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newExpression(&pmSySt[pmStkP+1].srcp, SCORE_EXPRESSION);
    	break;}
    case 303: { /* <aggregate> = <aggregator> 'OF' ID; */
#line 2257 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 302: { /* <aggregate> = 'COUNT'; */
#line 2264 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = COUNT_AGGREGATE;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 304: { /* <aggregator> = 'MAX'; */
#line 2273 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MAX_AGGREGATE;
    	break;}
    case 305: { /* <aggregator> = 'MIN'; */
#line 2279 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MIN_AGGREGATE;
    	break;}
    case 306: { /* <aggregator> = 'SUM'; */
#line 2285 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = SUM_AGGREGATE;
    	break;}
    case 307: { /* <something> = <optional_not> ID; */
#line 2294 "alan.pmk"

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 308: { /* <what> = <simple_what>; */
#line 2302 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhatExpression(pmSeSt[pmStkP+1].srcp, pmSeSt[pmStkP+1].wht);
    	break;}
    case 309: { /* <what> = <attribute_reference>; */
#line 2307 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 310: { /* <simple_what> = ID; */
#line 2314 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSeSt[pmStkP+1].id->srcp, WHAT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 312: { /* <simple_what> = 'CURRENT' 'ACTOR'; */
#line 2320 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_ACTOR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 313: { /* <simple_what> = 'CURRENT' 'LOCATION'; */
#line 2326 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_LOCATION, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 311: { /* <simple_what> = 'THIS'; */
#line 2332 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_THIS, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 314: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2341 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].id, FALSE, pmSeSt[pmStkP+3].exp);
    	break;}
    case 315: { /* <attribute_reference> = <what> ':' ID; */
#line 2346 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id, FALSE, pmSeSt[pmStkP+1].exp);
    	break;}
    case 316: { /* <optional_where> =; */
#line 2353 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&nulsrcp, WHERE_DEFAULT, NULL);
    	break;}
    case 317: { /* <optional_where> = <where>; */
#line 2358 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 318: { /* <where> = 'HERE'; */
#line 2365 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+1].srcp, WHERE_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 319: { /* <where> = 'NEARBY'; */
#line 2371 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+1].srcp, WHERE_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 320: { /* <where> = 'AT' <what>; */
#line 2377 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+1].srcp, WHERE_AT, pmSeSt[pmStkP+2].exp);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 321: { /* <where> = 'IN' <what>; */
#line 2383 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+1].srcp, WHERE_IN, pmSeSt[pmStkP+2].exp);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 322: { /* <binop> = '+'; */
#line 2391 "alan.pmk"

	pmSeSt[pmStkP+1].op = PLUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 323: { /* <binop> = '-'; */
#line 2397 "alan.pmk"

	pmSeSt[pmStkP+1].op = MINUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 324: { /* <binop> = '*'; */
#line 2403 "alan.pmk"

	pmSeSt[pmStkP+1].op = MULT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 325: { /* <binop> = '/'; */
#line 2409 "alan.pmk"

	pmSeSt[pmStkP+1].op = DIV_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 326: { /* <relop> = '<>'; */
#line 2418 "alan.pmk"

        pmSeSt[pmStkP+1].op   = NE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 327: { /* <relop> = '='; */
#line 2424 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EQ_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 328: { /* <relop> = '=='; */
#line 2430 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EXACT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 329: { /* <relop> = '>='; */
#line 2436 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 330: { /* <relop> = '<='; */
#line 2442 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 331: { /* <relop> = '>'; */
#line 2448 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 332: { /* <relop> = '<'; */
#line 2454 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 333: { /* <optional_qual> =; */
#line 2464 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 334: { /* <optional_qual> = 'BEFORE'; */
#line 2469 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 335: { /* <optional_qual> = 'AFTER'; */
#line 2475 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 336: { /* <optional_qual> = 'ONLY'; */
#line 2481 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 337: { /* <optional_not> =; */
#line 2490 "alan.pmk"

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 338: { /* <optional_not> = 'NOT'; */
#line 2495 "alan.pmk"

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 339: { /* <optional_id> =; */
#line 2502 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 340: { /* <optional_id> = ID; */
#line 2507 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 341: { /* <ids> = ID; */
#line 2514 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 342: { /* <ids> = <ids> ID; */
#line 2519 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, ID_LIST);
    	break;}
    case 343: { /* <id_list> = ID; */
#line 2526 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 344: { /* <id_list> = <id_list> ',' ID; */
#line 2532 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, ID_LIST);
    	break;}
    case 345: { /* <optional_integer> =; */
#line 2540 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 346: { /* <optional_integer> = Integer; */
#line 2545 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 347: { /* <optional_minus> =; */
#line 2552 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 348: { /* <optional_minus> = '-'; */
#line 2557 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 349: { /* ID = Identifier; */
#line 2564 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 350: { /* ID = 'LOCATION'; */
#line 2569 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 351: { /* ID = 'ACTOR'; */
#line 2574 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 352: { /* ID = 'OPAQUE'; */
#line 2579 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 353: { /* ID = 'ON'; */
#line 2584 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 354: { /* ID = 'OF'; */
#line 2589 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 355: { /* ID = 'FIRST'; */
#line 2594 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 356: { /* ID = 'INTO'; */
#line 2599 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 357: { /* ID = 'TAKING'; */
#line 2604 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    default: break; }
}/*pmPaSema()*/

