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

#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "alt.h"                /* ALT-nodes */
#include "evt.h"                /* EVT-nodes */
#include "lim.h"		/* LIM-nodes */
#include "msg.h"                /* MSG-nodes */
#include "opt.h"		/* OPTIONS */
#include "rul.h"                /* RUL-nodes */
#include "sco.h"                /* SCORES */
#include "str.h"		/* STRINGS */
#include "syn.h"                /* SYN-nodes */


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
    List *vrbs;
    Verb *vrb;
    int val;
    List *syns;
    SynNod *syn;
    List *stxs;
    StxNod *stx;
    char *str;
    List *stps;
    Step *stp;
    List *stms3;
    List *stms2;
    List *stms;
    StmNod *stm;
    Srcp srcp;
    List *scrs;
    Script *scr;
    List *ruls;
    RulNod *rul;
    RestrictionKind restrictionKind;
    List *ress;
    ResNod *res;
    QualKind qual;
    OperatorKind op;
    Bool not;
    List *nams;
    List *nam;
    List *msgs;
    MsgNod *msg;
    Bool minus;
    Srcp mentSrcp;
    List *ment;
    List *lims;
    LimNod *lim;
    List *inss;
    Instance *ins;
    List *idList2;
    List *idList;
    IdNode *id;
    List *exts;
    ExtNod *ext;
    ExpressionKind expKd;
    Expression *exp;
    List *evts;
    EvtNod *evt;
    List *elms;
    ElmNod *elm;
    Srcp dscrSrcp;
    List *dscr;
    List *does;
    List *cnts;
    CntNod *cnt;
    List *clas;
    ClaNod *cla;
    List *chks;
    ChkNod *chk;
    List *cases;
    int bits;
    List *atrs;
    Attribute *atr;
    Srcp artSrcp;
    List *art;
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
#line 174 "alan.pmk"
    switch (rule) {
    case 6: { /* <option> = ID '.'; */
#line 194 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */
#line 199 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */
#line 204 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 15: { /* <unit> = <synonyms>; */
#line 217 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns); 	break;}
    case 11: { /* <unit> = <messages>; */
#line 220 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 16: { /* <unit> = <syntax>; */
#line 223 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 17: { /* <unit> = <verb>; */
#line 226 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VERB_LIST); 	break;}
    case 12: { /* <unit> = <class>; */
#line 229 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLASS_LIST); 	break;}
    case 18: { /* <unit> = <add>; */
#line 232 "alan.pmk"
 adv.adds = concat(adv.adds, pmSeSt[pmStkP+1].add, ADD_LIST); 	break;}
    case 13: { /* <unit> = <instance>; */
#line 235 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSTANCE_LIST); 	break;}
    case 19: { /* <unit> = <event>; */
#line 238 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVENT_LIST); 	break;}
    case 14: { /* <unit> = <rule>; */
#line 241 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULE_LIST); 	break;}
    case 20: { /* <attributes> = <attribute definition> '.'; */
#line 247 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, ATTRIBUTE_LIST);
    	break;}
    case 21: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 252 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATTRIBUTE_LIST);
    	break;}
    case 22: { /* <attribute definition> = ID; */
#line 259 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+1].id->srcp,
						BOOLEAN_TYPE,
						pmSeSt[pmStkP+1].id,
						TRUE, 0, 0);
    	break;}
    case 23: { /* <attribute definition> = 'NOT' ID; */
#line 267 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+2].id->srcp,
						BOOLEAN_TYPE,
						pmSeSt[pmStkP+2].id,
						FALSE, 0, 0);
    	break;}
    case 24: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 275 "alan.pmk"

	if (pmSeSt[pmStkP+2].minus)
		pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+1].id->srcp,
							INTEGER_TYPE,
							pmSeSt[pmStkP+1].id,
							-val(pmSySt[pmStkP+3].chars), 0, 0);
	else
		pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+1].id->srcp,
							INTEGER_TYPE,
							pmSeSt[pmStkP+1].id,
							val(pmSySt[pmStkP+3].chars), 0, 0);
    	break;}
    case 25: { /* <attribute definition> = ID STRING; */
#line 289 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+1].id->srcp,
						STRING_TYPE,
						pmSeSt[pmStkP+1].id,
						0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 26: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 300 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 27: { /* <synonym_list> = <synonym>; */
#line 307 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, SYNONYM_LIST);
    	break;}
    case 28: { /* <synonym_list> = <synonym_list> <synonym>; */
#line 312 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNONYM_LIST);
    	break;}
    case 29: { /* <synonym> = <id_list> '=' ID '.'; */
#line 319 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].idList,
			      pmSeSt[pmStkP+3].id);
    	break;}
    case 30: { /* <messages> = 'MESSAGE' <message_list>; */
#line 329 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 31: { /* <message_list> = <message>; */
#line 336 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, MESSAGE_LIST);
    	break;}
    case 32: { /* <message_list> = <message_list> <message>; */
#line 341 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MESSAGE_LIST);
    	break;}
    case 33: { /* <message> = ID ':' <statements>; */
#line 348 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 34: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 358 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 35: { /* <syntax_list> = <syntax_item>; */
#line 365 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, SYNTAX_LIST);
    	break;}
    case 36: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 370 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, SYNTAX_LIST);
    	break;}
    case 37: { /* <syntax_item> = ID '=' <syntax_elements> <optional_class_restrictions>; */
#line 377 "alan.pmk"

	pmSeSt[pmStkP+1].stx = newstx(&pmSySt[pmStkP+2].srcp,
				  pmSeSt[pmStkP+1].id,
				  concat(pmSeSt[pmStkP+3].elms,
				         newelm(&pmSeSt[pmStkP+1].id->srcp, END_OF_SYNTAX,
				                NULL,
				                FALSE),
				         ELEMENT_LIST),
				  pmSeSt[pmStkP+4].ress);
    	break;}
    case 38: { /* <syntax_elements> = <syntax_element>; */
#line 391 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, ELEMENT_LIST);
    	break;}
    case 39: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 396 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                ELEMENT_LIST);
    	break;}
    case 40: { /* <syntax_element> = ID; */
#line 405 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+1].id->srcp, WORD_ELEMENT,
				     pmSeSt[pmStkP+1].id,
				     FALSE);
    	break;}
    case 41: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 413 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+2].id->srcp, PARAMETER_ELEMENT,
				     pmSeSt[pmStkP+2].id,
				     pmSeSt[pmStkP+4].bits);
    	break;}
    case 42: { /* <optional_indicators> =; */
#line 422 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 43: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 427 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 44: { /* <indicator> = '*'; */
#line 434 "alan.pmk"

	pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 45: { /* <indicator> = '!'; */
#line 439 "alan.pmk"

	pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 46: { /* <optional_class_restrictions> = '.'; */
#line 446 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 47: { /* <optional_class_restrictions> = 'WHERE' <class_restriction_clauses>; */
#line 451 "alan.pmk"

	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 48: { /* <class_restriction_clauses> = <class_restriction>; */
#line 465 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, RESTRICTION_LIST);
    	break;}
    case 49: { /* <class_restriction_clauses> = <class_restriction_clauses> 'AND' <class_restriction>; */
#line 470 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, RESTRICTION_LIST);
    	break;}
    case 50: { /* <class_restriction> = ID 'ISA' <restriction_class> 'ELSE' <statements>; */
#line 478 "alan.pmk"

	pmSeSt[pmStkP+1].res = newRestriction(&pmSySt[pmStkP+2].srcp,
			  			  pmSeSt[pmStkP+1].id,
						  pmSeSt[pmStkP+3].restrictionKind,
						  pmSeSt[pmStkP+3].id,
						  pmSeSt[pmStkP+5].stms);
    	break;}
    case 51: { /* <restriction_class> = ID; */
#line 489 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].restrictionKind = ID_RESTRICTION;
    	break;}
    case 52: { /* <restriction_class> = 'CONTAINER'; */
#line 495 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
	pmSeSt[pmStkP+1].restrictionKind = CONTAINER_RESTRICTION;
    	break;}
    case 53: { /* <restriction_class> = 'INTEGER'; */
#line 501 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
	pmSeSt[pmStkP+1].restrictionKind = INTEGER_RESTRICTION;
    	break;}
    case 54: { /* <restriction_class> = 'STRING'; */
#line 507 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
	pmSeSt[pmStkP+1].restrictionKind = STRING_RESTRICTION;
    	break;}
    case 55: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 518 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newVerb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 56: { /* <verb_header> = 'VERB' <id_list>; */
#line 531 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 57: { /* <verb_body> = <simple_verb_body>; */
#line 540 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&nulsrcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   ALTERNATIVE_LIST);
    	break;}
    case 58: { /* <verb_body> = <verb_alternatives>; */
#line 551 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 59: { /* <verb_alternatives> = <verb_alternative>; */
#line 558 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, ALTERNATIVE_LIST);
    	break;}
    case 60: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 563 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				ALTERNATIVE_LIST);
    	break;}
    case 61: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 572 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 62: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 583 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 63: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */
#line 592 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 64: { /* <optional_checks> =; */
#line 600 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
    	break;}
    case 65: { /* <optional_checks> = 'CHECK' <statements>; */
#line 605 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), STATEMENT_LIST);
    	break;}
    case 66: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 610 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 67: { /* <check_list> = <check>; */
#line 617 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, CHECK_LIST);
    	break;}
    case 68: { /* <check_list> = <check_list> 'AND' <check>; */
#line 622 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHECK_LIST);
    	break;}
    case 69: { /* <check> = <expression> 'ELSE' <statements>; */
#line 629 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 70: { /* <optional_does> =; */
#line 637 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 72: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 648 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 73: { /* <class> = 'EVERY' ID <optional_heritage> <properties> <class_tail>; */
#line 659 "alan.pmk"

	pmSeSt[pmStkP+1].cla = newClass(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newProps(pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].dscr,
					&pmSeSt[pmStkP+4].dscrSrcp,
					pmSeSt[pmStkP+4].ment,
					&pmSeSt[pmStkP+4].mentSrcp,
					pmSeSt[pmStkP+4].art,
					&pmSeSt[pmStkP+4].artSrcp,
					pmSeSt[pmStkP+4].does,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 201, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 74: { /* <class_tail> = 'END' 'EVERY' <optional_id> __genSym#0; */
#line 685 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 77: { /* <add> = 'ADD' 'TO' 'EVERY' ID <optional_heritage> <properties> <add_tail>; */
#line 692 "alan.pmk"

	pmSeSt[pmStkP+1].add = newAdd(&pmSySt[pmStkP+3].srcp,
				pmSeSt[pmStkP+4].id,
				pmSeSt[pmStkP+5].id,
				newProps(pmSeSt[pmStkP+6].nams,
					pmSeSt[pmStkP+6].whr,
					pmSeSt[pmStkP+6].atrs,
					pmSeSt[pmStkP+6].cnt,
					pmSeSt[pmStkP+6].dscr,
					&pmSeSt[pmStkP+6].dscrSrcp,
					pmSeSt[pmStkP+6].ment,
					&pmSeSt[pmStkP+6].mentSrcp,
					pmSeSt[pmStkP+6].art,
					&pmSeSt[pmStkP+6].artSrcp,
					pmSeSt[pmStkP+6].does,
					pmSeSt[pmStkP+6].exts,
					pmSeSt[pmStkP+6].vrbs,
					pmSeSt[pmStkP+6].scrs));
	if (pmSeSt[pmStkP+7].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+4].id, pmSeSt[pmStkP+7].id))
		lmLog(&pmSeSt[pmStkP+7].id->srcp, 201, sevWAR, pmSeSt[pmStkP+4].id->string);
    	break;}
    case 78: { /* <add_tail> = 'END' 'ADD' __genSym#1 <optional_id> __genSym#2; */
#line 718 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+4].id; 	break;}
    case 83: { /* <instance> = 'THE' ID <optional_heritage> <properties> <instance tail>; */
#line 725 "alan.pmk"

	pmSeSt[pmStkP+1].ins = newInstance(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newProps(pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].dscr,
					&pmSeSt[pmStkP+4].dscrSrcp,
					pmSeSt[pmStkP+4].ment,
					&pmSeSt[pmStkP+4].mentSrcp,
					pmSeSt[pmStkP+4].art,
					&pmSeSt[pmStkP+4].artSrcp,
					pmSeSt[pmStkP+4].does,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 201, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 84: { /* <instance tail> = 'END' 'THE' <optional_id> __genSym#3; */
#line 751 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 87: { /* <optional_heritage> =; */
#line 756 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 88: { /* <optional_heritage> = <heritage>; */
#line 761 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 89: { /* <heritage> = 'ISA' ID __genSym#4; */
#line 768 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 92: { /* <properties> =; */
#line 773 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
	pmSeSt[pmStkP+1].whr = NULL;
	pmSeSt[pmStkP+1].atrs = NULL;
	pmSeSt[pmStkP+1].cnt = NULL;
	pmSeSt[pmStkP+1].dscr = NULL;
	pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].does = NULL;
	pmSeSt[pmStkP+1].exts = NULL;
	pmSeSt[pmStkP+1].vrbs = NULL;
	pmSeSt[pmStkP+1].scrs = NULL;
    	break;}
    case 93: { /* <properties> = <properties> <property>; */
#line 787 "alan.pmk"

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

	if (pmSeSt[pmStkP+2].dscr != NULL) {
            if(pmSeSt[pmStkP+1].dscr != NULL)
                /* DESCRIPTION already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "class/instance", NULL);
	    else {
	        pmSeSt[pmStkP+1].dscr = pmSeSt[pmStkP+2].dscr;
		pmSeSt[pmStkP+1].dscrSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].ment != NULL) {
            if (pmSeSt[pmStkP+1].ment != NULL)
                /* MENTIONED already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "MENTIONED", "class/instance", NULL);
	    else {
                pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+2].ment;
		pmSeSt[pmStkP+1].mentSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].art != NULL) {
            if (pmSeSt[pmStkP+1].art != NULL)
                /* ARTICLE already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "ARTICLE", "class/instance", NULL);
            else { 
	        pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].art;
		pmSeSt[pmStkP+1].artSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].does != NULL) {
            if (pmSeSt[pmStkP+1].does != NULL)
                /* DOES/ENTERED already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DOES/ENTERED", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].does = pmSeSt[pmStkP+2].does;
        }

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXIT_LIST);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VERB_LIST);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, SCRIPT_LIST);
    	break;}
    case 98: { /* <property> = <name>; */
#line 855 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 94: { /* <property> = <where> __genSym#5; */
#line 861 "alan.pmk"
{
        Where *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 101: { /* <property> = <is> <attributes>; */
#line 871 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 95: { /* <property> = <container properties>; */
#line 877 "alan.pmk"
{
        CntNod *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 96: { /* <property> = <description>; */
#line 887 "alan.pmk"
{
        List *dscr = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].dscr = dscr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 99: { /* <property> = <mentioned>; */
#line 897 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 97: { /* <property> = <article>; */
#line 907 "alan.pmk"
{ List *art = pmSeSt[pmStkP+1].art;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].art = art;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 100: { /* <property> = <does>; */
#line 916 "alan.pmk"
{
        List *does = pmSeSt[pmStkP+1].does;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].does = does;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 102: { /* <property> = <exit>; */
#line 926 "alan.pmk"
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 103: { /* <property> = <verb>; */
#line 932 "alan.pmk"
{ Verb *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 104: { /* <property> = <script>; */
#line 938 "alan.pmk"
{ Script *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 107: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 948 "alan.pmk"

	pmSeSt[pmStkP+1].ext = newext(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].idList,
			   pmSeSt[pmStkP+4].id,
			   pmSeSt[pmStkP+5].chks,
			   pmSeSt[pmStkP+5].stms);
	if (pmSeSt[pmStkP+5].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+2].str, pmSeSt[pmStkP+5].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 201, sevWAR, pmSeSt[pmStkP+2].str);
	}
    	break;}
    case 108: { /* <optional_exit_body> =; */
#line 963 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 109: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 970 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 110: { /* <optional_attributes> =; */
#line 985 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 111: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 990 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 112: { /* <is> = 'IS'; */
#line 997 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 113: { /* <is> = 'ARE'; */
#line 1002 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 114: { /* <is> = 'HAS'; */
#line 1007 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 115: { /* <optional_description> =; */
#line 1014 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 117: { /* <description> = 'DESCRIPTION'; */
#line 1024 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
						NOP_STATEMENT), STATEMENT_LIST);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 118: { /* <description> = 'DESCRIPTION' <statements>; */
#line 1031 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 119: { /* <article> = 'ARTICLE'; */
#line 1039 "alan.pmk"

	pmSeSt[pmStkP+1].art = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							NOP_STATEMENT), STATEMENT_LIST);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 120: { /* <article> = 'ARTICLE' <statements>; */
#line 1046 "alan.pmk"

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 121: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 1055 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 122: { /* <optional_names> =; */
#line 1064 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 123: { /* <optional_names> = <optional_names> <name>; */
#line 1069 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					pmSeSt[pmStkP+2].nam, NAME_LIST);
    	break;}
    case 124: { /* <name> = 'NAME' <ids> __genSym#6; */
#line 1077 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 127: { /* <container properties> = __genSym#7 'CONTAINER' <container_body>; */
#line 1086 "alan.pmk"

	pmSeSt[pmStkP+1].cnt = newContainer(&pmSySt[pmStkP+2].srcp,
						   pmSeSt[pmStkP+3].lims,
						   pmSeSt[pmStkP+3].stms,
						   pmSeSt[pmStkP+3].stms2);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, CONTAINER_LIST);
    	break;}
    case 130: { /* <container_body> = <optional_limits> <optional_header> <optional_empty>; */
#line 1099 "alan.pmk"

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
    	break;}
    case 131: { /* <optional_limits> =; */
#line 1108 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 132: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1113 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 133: { /* <limits> = <limit>; */
#line 1120 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIMIT_LIST);
    	break;}
    case 134: { /* <limits> = <limits> <limit>; */
#line 1125 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMIT_LIST);
    	break;}
    case 135: { /* <limit> = <limit_attribute> 'THEN' <statements>; */
#line 1132 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 136: { /* <limit_attribute> = <attribute definition>; */
#line 1141 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 137: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1146 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSySt[pmStkP+1].srcp,
				      INTEGER_TYPE,
				      newId(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 138: { /* <optional_header> =; */
#line 1156 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 139: { /* <optional_header> = 'HEADER' <statements>; */
#line 1161 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 140: { /* <optional_empty> =; */
#line 1168 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 141: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1173 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 142: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1181 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 143: { /* <event_header> = 'EVENT' ID; */
#line 1194 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 144: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1202 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 145: { /* <script> = 'SCRIPT' ID '.' <optional_description> <step_list>; */
#line 1210 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newScript(&pmSySt[pmStkP+1].srcp,
			          pmSeSt[pmStkP+2].id,
			          pmSeSt[pmStkP+4].stms,
			          pmSeSt[pmStkP+5].stps);
    	break;}
    case 146: { /* <step_list> = <step>; */
#line 1220 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, STEP_LIST);
    	break;}
    case 147: { /* <step_list> = <step_list> <step>; */
#line 1225 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STEP_LIST);
    	break;}
    case 148: { /* <step> = 'STEP' <statements>; */
#line 1232 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 149: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1240 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 150: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1248 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 151: { /* <rule> = 'WHEN' <expression> '=>' <statements>; */
#line 1259 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 152: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1269 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 153: { /* <optional_statements> =; */
#line 1278 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 154: { /* <optional_statements> = <statements>; */
#line 1283 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 155: { /* <statements> = <statement>; */
#line 1290 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, STATEMENT_LIST);
    	break;}
    case 156: { /* <statements> = <statements> <statement>; */
#line 1295 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STATEMENT_LIST);
    	break;}
    case 157: { /* <statement> = <output_statement>; */
#line 1302 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 159: { /* <statement> = <manipulation_statement>; */
#line 1307 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 161: { /* <statement> = <assignment_statement>; */
#line 1312 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 163: { /* <statement> = <comparison_statement>; */
#line 1317 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 162: { /* <statement> = <actor_statement>; */
#line 1322 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 160: { /* <statement> = <event_statement>; */
#line 1327 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 158: { /* <statement> = <special_statement>; */
#line 1332 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 164: { /* <output_statement> = STRING; */
#line 1340 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, PRINT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 165: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1347 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DESCRIBE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 166: { /* <output_statement> = 'SAY' <expression> '.'; */
#line 1353 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SAY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 167: { /* <output_statement> = 'LIST' <what> '.'; */
#line 1359 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LIST_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 168: { /* <special_statement> = 'QUIT' '.'; */
#line 1367 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, QUIT_STATEMENT);
    	break;}
    case 169: { /* <special_statement> = 'LOOK' '.'; */
#line 1372 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LOOK_STATEMENT);
    	break;}
    case 170: { /* <special_statement> = 'SAVE' '.'; */
#line 1377 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SAVE_STATEMENT);
    	break;}
    case 171: { /* <special_statement> = 'RESTORE' '.'; */
#line 1382 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, RESTORE_STATEMENT);
    	break;}
    case 172: { /* <special_statement> = 'RESTART' '.'; */
#line 1387 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, RESTART_STATEMENT);
    	break;}
    case 173: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 1392 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SCORE_STATEMENT);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 174: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 1403 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, VISITS_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 175: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 1409 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SYSTEM_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 176: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */
#line 1420 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, EMPTY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 177: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */
#line 1427 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LOCATE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 179: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1438 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SCHEDULE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 178: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1446 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, CANCEL_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 180: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */
#line 1456 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, MAKE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 183: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1464 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SET_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 181: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1472 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, INCREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 182: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1480 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DECREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 184: { /* <optional_by_clause> =; */
#line 1490 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 185: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1495 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 186: { /* <comparison_statement> = <if_statement>; */
#line 1504 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 187: { /* <comparison_statement> = <depending_statement>; */
#line 1509 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 188: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 1517 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
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
    case 189: { /* <optional_elsif_list> =; */
#line 1535 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 190: { /* <optional_elsif_list> = <elsif_list>; */
#line 1540 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 191: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 1547 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, STATEMENT_LIST);
    }	break;}
    case 192: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 1560 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+2].srcp, IF_STATEMENT);
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
    case 193: { /* <optional_else_part> =; */
#line 1579 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 194: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 1584 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 195: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' 'DEPEND' '.'; */
#line 1591 "alan.pmk"
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, DEPEND_STATEMENT);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 196: { /* <depend_cases> = <depend_case>; */
#line 1602 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, CASE_LIST);
    	break;}
    case 197: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1607 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASE_LIST);
    	break;}
    case 199: { /* <depend_case> = <right_hand_side> ':' <statements>; */
#line 1614 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 198: { /* <depend_case> = 'ELSE' <statements>; */
#line 1621 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 200: { /* <actor_statement> = 'USE' 'SCRIPT' ID <optional_for_actor> '.'; */
#line 1631 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, USE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 201: { /* <optional_for_actor> =; */
#line 1640 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 202: { /* <optional_for_actor> = 'FOR' ID; */
#line 1645 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 203: { /* <expression> = <term>; */
#line 1654 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 204: { /* <expression> = <expression> 'OR' <term>; */
#line 1659 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OR_OPERATOR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 205: { /* <term> = <factor>; */
#line 1671 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 206: { /* <term> = <term> 'AND' <factor>; */
#line 1676 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not	      = FALSE;
	exp->fields.bin.op    = AND_OPERATOR;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 207: { /* <factor> = <primary>; */
#line 1688 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 208: { /* <factor> = <primary> <right_hand_side>; */
#line 1693 "alan.pmk"

        /* <right_hand_side> contains the top expr, hang in the <primary> and
           propagate that node */
        switch (pmSeSt[pmStkP+2].expKd) {
	case BINARY_EXPRESSION: pmSeSt[pmStkP+2].exp->fields.bin.left = pmSeSt[pmStkP+1].exp; break;
	case WHERE_EXPRESSION: pmSeSt[pmStkP+2].exp->fields.whr.wht = pmSeSt[pmStkP+1].exp; break;
	case ATTRIBUTE_EXPRESSION: pmSeSt[pmStkP+2].exp->fields.atr.wht = pmSeSt[pmStkP+1].exp; break;
	case BETWEEN_EXPRESSION: pmSeSt[pmStkP+2].exp->fields.btw.val = pmSeSt[pmStkP+1].exp; break;
	case ISA_EXPRESSION: pmSeSt[pmStkP+2].exp->fields.isa.wht = pmSeSt[pmStkP+1].exp; break;
	default: syserr("Unrecognized switch in <right_hand_side> semantic rule.");
	}
        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 210: { /* <right_hand_side> = <binop> <primary>; */
#line 1710 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, BINARY_EXPRESSION);
	exp->fields.bin.op = pmSeSt[pmStkP+1].op;
	exp->fields.bin.right = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 211: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 1719 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 209: { /* <right_hand_side> = <optional_not> <where>; */
#line 1729 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = WHERE_EXPRESSION;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, WHERE_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 212: { /* <right_hand_side> = <optional_not> 'ISA' ID; */
#line 1738 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = ISA_EXPRESSION;
	exp = newexp(&pmSySt[pmStkP+2].srcp, ISA_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 213: { /* <right_hand_side> = <is> <something>; */
#line 1747 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = ATTRIBUTE_EXPRESSION;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, ATTRIBUTE_EXPRESSION);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 215: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 1756 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BETWEEN_EXPRESSION;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BETWEEN_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 214: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 1766 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = CONTAINS_OPERATOR;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 221: { /* <primary> = '(' <expression> ')'; */
#line 1779 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 216: { /* <primary> = <optional_minus> Integer; */
#line 1784 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, INTEGER_EXPRESSION);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 217: { /* <primary> = STRING; */
#line 1793 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, STRING_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 218: { /* <primary> = <what>; */
#line 1800 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, WHAT_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 222: { /* <primary> = <attribute_reference>; */
#line 1806 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, ATTRIBUTE_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, WHAT_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 220: { /* <primary> = <aggregate> <where>; */
#line 1814 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, AGGREGATE_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.agr.kind	= pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.whr	= pmSeSt[pmStkP+2].whr;
    	break;}
    case 223: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 1822 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, RANDOM_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 219: { /* <primary> = 'SCORE'; */
#line 1829 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, SCORE_EXPRESSION);
    	break;}
    case 225: { /* <aggregate> = 'SUM' 'OF' ID; */
#line 1837 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = SUM_AGGREGATE;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 226: { /* <aggregate> = 'MAX' 'OF' ID; */
#line 1844 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MAX_AGGREGATE;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 224: { /* <aggregate> = 'COUNT'; */
#line 1851 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = COUNT_AGGREGATE;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 227: { /* <something> = <optional_not> ID; */
#line 1861 "alan.pmk"

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 228: { /* <what> = ID; */
#line 1869 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSeSt[pmStkP+1].id->srcp, WHAT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 230: { /* <what> = 'CURRENT' 'ACTOR'; */
#line 1875 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_ACTOR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 231: { /* <what> = 'CURRENT' 'LOCATION'; */
#line 1881 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_LOCATION, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 229: { /* <what> = 'THIS'; */
#line 1887 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_THIS, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 232: { /* <optional_where> =; */
#line 1896 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 233: { /* <optional_where> = <where>; */
#line 1901 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 234: { /* <where> = 'HERE'; */
#line 1908 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 235: { /* <where> = 'NEARBY'; */
#line 1914 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 236: { /* <where> = 'AT' <what>; */
#line 1920 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 237: { /* <where> = 'IN' <what>; */
#line 1926 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 238: { /* <binop> = '+'; */
#line 1934 "alan.pmk"

	pmSeSt[pmStkP+1].op = PLUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 239: { /* <binop> = '-'; */
#line 1940 "alan.pmk"

	pmSeSt[pmStkP+1].op = MINUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 240: { /* <binop> = '*'; */
#line 1946 "alan.pmk"

	pmSeSt[pmStkP+1].op = MULT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 241: { /* <binop> = '/'; */
#line 1952 "alan.pmk"

	pmSeSt[pmStkP+1].op = DIV_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 242: { /* <relop> = '<>'; */
#line 1961 "alan.pmk"

        pmSeSt[pmStkP+1].op   = NE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 243: { /* <relop> = '='; */
#line 1967 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EQ_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 244: { /* <relop> = '=='; */
#line 1973 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EXACT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 245: { /* <relop> = '>='; */
#line 1979 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 246: { /* <relop> = '<='; */
#line 1985 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 247: { /* <relop> = '>'; */
#line 1991 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 248: { /* <relop> = '<'; */
#line 1997 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 249: { /* <optional_qual> =; */
#line 2007 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 250: { /* <optional_qual> = 'BEFORE'; */
#line 2012 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 251: { /* <optional_qual> = 'AFTER'; */
#line 2018 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 252: { /* <optional_qual> = 'ONLY'; */
#line 2024 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 253: { /* <optional_not> =; */
#line 2033 "alan.pmk"

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 254: { /* <optional_not> = 'NOT'; */
#line 2038 "alan.pmk"

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 255: { /* <optional_id> =; */
#line 2045 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 256: { /* <optional_id> = ID; */
#line 2050 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 257: { /* <ids> = ID; */
#line 2057 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 258: { /* <ids> = <ids> ID; */
#line 2062 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, ID_LIST);
    	break;}
    case 259: { /* <id_list> = ID; */
#line 2069 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 260: { /* <id_list> = <id_list> ',' ID; */
#line 2075 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, ID_LIST);
    	break;}
    case 261: { /* <optional_integer> =; */
#line 2083 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 262: { /* <optional_integer> = Integer; */
#line 2088 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 263: { /* <optional_minus> =; */
#line 2095 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 264: { /* <optional_minus> = '-'; */
#line 2100 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 265: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2107 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 266: { /* ID = Identifier; */
#line 2116 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 267: { /* ID = 'LOCATION'; */
#line 2121 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "location");
    	break;}
    case 268: { /* ID = 'ACTOR'; */
#line 2126 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "actor");
    	break;}
    default: break; }
}/*pmPaSema()*/

