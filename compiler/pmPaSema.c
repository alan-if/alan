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
    StmNod *stm;
    Srcp srcp2;
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
    List *exps;
    Expression *exp;
    List *evts;
    EvtNod *evt;
    List *elms;
    Element *elm;
    Srcp descriptionSrcp;
    List *descriptionDoes;
    Srcp descriptionCheckSrcp;
    List *descriptionChecks;
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
#line 177 "alan.pmk"
    switch (rule) {
    case 6: { /* <option> = ID '.'; */
#line 197 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */
#line 202 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */
#line 207 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 15: { /* <declaration> = <synonyms>; */
#line 220 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns);	break;}
    case 11: { /* <declaration> = <messages>; */
#line 223 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 16: { /* <declaration> = <syntax>; */
#line 226 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 17: { /* <declaration> = <verb>; */
#line 229 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VERB_LIST); 	break;}
    case 12: { /* <declaration> = <class>; */
#line 232 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLASS_LIST); 	break;}
    case 18: { /* <declaration> = <addition>; */
#line 235 "alan.pmk"
 adv.adds = concat(adv.adds, pmSeSt[pmStkP+1].add, ADD_LIST); 	break;}
    case 13: { /* <declaration> = <instance>; */
#line 238 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSTANCE_LIST); 	break;}
    case 19: { /* <declaration> = <event>; */
#line 241 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVENT_LIST); 	break;}
    case 14: { /* <declaration> = <rule>; */
#line 244 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULE_LIST); 	break;}
    case 20: { /* <attributes> = <attribute definition> '.'; */
#line 250 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, ATTRIBUTE_LIST);
    	break;}
    case 21: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 255 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATTRIBUTE_LIST);
    	break;}
    case 22: { /* <attribute definition> = ID; */
#line 262 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+1].id->srcp,
						BOOLEAN_TYPE,
						pmSeSt[pmStkP+1].id,
						TRUE, 0, 0);
    	break;}
    case 23: { /* <attribute definition> = 'NOT' ID; */
#line 270 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+2].id->srcp,
						BOOLEAN_TYPE,
						pmSeSt[pmStkP+2].id,
						FALSE, 0, 0);
    	break;}
    case 24: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 278 "alan.pmk"

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
#line 292 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+1].id->srcp,
						STRING_TYPE,
						pmSeSt[pmStkP+1].id,
						0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 26: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 303 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 27: { /* <synonym_list> = <synonym_declaration>; */
#line 310 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, SYNONYM_LIST);
    	break;}
    case 28: { /* <synonym_list> = <synonym_list> <synonym_declaration>; */
#line 315 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNONYM_LIST);
    	break;}
    case 29: { /* <synonym_declaration> = <id_list> '=' ID '.'; */
#line 322 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			     			pmSeSt[pmStkP+1].idList,
			      			pmSeSt[pmStkP+3].id);
    	break;}
    case 30: { /* <messages> = 'MESSAGE' <message_list>; */
#line 332 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 31: { /* <message_list> = <message>; */
#line 339 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, MESSAGE_LIST);
    	break;}
    case 32: { /* <message_list> = <message_list> <message>; */
#line 344 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MESSAGE_LIST);
    	break;}
    case 33: { /* <message> = ID ':' <statements>; */
#line 351 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 34: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 361 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 35: { /* <syntax_list> = <syntax_item>; */
#line 368 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, SYNTAX_LIST);
    	break;}
    case 36: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 373 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, SYNTAX_LIST);
    	break;}
    case 37: { /* <syntax_item> = ID '=' <syntax_elements> <optional_syntax_restrictions>; */
#line 380 "alan.pmk"

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
    case 38: { /* <syntax_elements> = <syntax_element>; */
#line 396 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, ELEMENT_LIST);
    	break;}
    case 39: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 401 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                ELEMENT_LIST);
    	break;}
    case 40: { /* <syntax_element> = ID; */
#line 410 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newElement(&pmSeSt[pmStkP+1].id->srcp, WORD_ELEMENT,
					   pmSeSt[pmStkP+1].id,
					   FALSE);
    	break;}
    case 41: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 418 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newElement(&pmSeSt[pmStkP+2].id->srcp, PARAMETER_ELEMENT,
					   pmSeSt[pmStkP+2].id,
					   pmSeSt[pmStkP+4].bits);
    	break;}
    case 42: { /* <optional_indicators> =; */
#line 427 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 43: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 432 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 44: { /* <indicator> = '*'; */
#line 439 "alan.pmk"

	pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 45: { /* <indicator> = '!'; */
#line 444 "alan.pmk"

	pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 46: { /* <syntax_restriction_clauses> = <syntax_restriction>; */
#line 457 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, RESTRICTION_LIST);
    	break;}
    case 47: { /* <syntax_restriction_clauses> = <syntax_restriction_clauses> 'AND' <syntax_restriction>; */
#line 462 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, RESTRICTION_LIST);
    	break;}
    case 48: { /* <syntax_restriction> = ID 'ISA' <restriction_class> 'ELSE' <statements>; */
#line 470 "alan.pmk"

	pmSeSt[pmStkP+1].res = newRestriction(&pmSySt[pmStkP+2].srcp,
			  			  pmSeSt[pmStkP+1].id,
						  pmSeSt[pmStkP+3].restrictionKind,
						  pmSeSt[pmStkP+3].id,
						  pmSeSt[pmStkP+5].stms);
    	break;}
    case 49: { /* <restriction_class> = ID; */
#line 481 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].restrictionKind = ID_RESTRICTION;
    	break;}
    case 50: { /* <restriction_class> = 'CONTAINER'; */
#line 487 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
	pmSeSt[pmStkP+1].restrictionKind = CONTAINER_RESTRICTION;
    	break;}
    case 51: { /* <optional_syntax_restrictions> = '.'; */
#line 495 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 52: { /* <optional_syntax_restrictions> = 'WHERE' <syntax_restriction_clauses>; */
#line 500 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 53: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 510 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newVerb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 54: { /* <verb_header> = 'VERB' <id_list>; */
#line 523 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 55: { /* <verb_body> = <simple_verb_body>; */
#line 532 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&pmSeSt[pmStkP+1].srcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   ALTERNATIVE_LIST);
    	break;}
    case 56: { /* <verb_body> = <verb_alternatives>; */
#line 543 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 57: { /* <verb_alternatives> = <verb_alternative>; */
#line 550 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, ALTERNATIVE_LIST);
    	break;}
    case 58: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 555 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				ALTERNATIVE_LIST);
    	break;}
    case 59: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 564 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 60: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 575 "alan.pmk"

	if (pmSeSt[pmStkP+2].stms == NULL) /* No DOES */
	  pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	else
	  pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 61: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */
#line 588 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 62: { /* <optional_checks> =; */
#line 596 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].srcp = nulsrcp;
    	break;}
    case 63: { /* <optional_checks> = 'CHECK' <statements>; */
#line 602 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), CHECK_LIST);
    	break;}
    case 64: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 608 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 65: { /* <check_list> = <check>; */
#line 616 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, CHECK_LIST);
    	break;}
    case 66: { /* <check_list> = <check_list> 'AND' <check>; */
#line 621 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHECK_LIST);
    	break;}
    case 67: { /* <check> = <expression> 'ELSE' <statements>; */
#line 628 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 68: { /* <optional_does> =; */
#line 636 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 70: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 647 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 71: { /* <class> = 'EVERY' ID <optional_heritage> <properties> <class_tail>; */
#line 658 "alan.pmk"

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
					pmSeSt[pmStkP+4].ment,
					pmSeSt[pmStkP+4].mentSrcp,
					pmSeSt[pmStkP+4].art,
					pmSeSt[pmStkP+4].artSrcp,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 72: { /* <class_tail> = 'END' 'EVERY' <optional_id> __genSym#0; */
#line 685 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 75: { /* <addition> = 'ADD' 'TO' __genSym#1 ID <optional_heritage> <properties> <add_tail>; */
#line 692 "alan.pmk"

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
					pmSeSt[pmStkP+6].ment,
					pmSeSt[pmStkP+6].mentSrcp,
					pmSeSt[pmStkP+6].art,
					pmSeSt[pmStkP+6].artSrcp,
					pmSeSt[pmStkP+6].exts,
					pmSeSt[pmStkP+6].vrbs,
					pmSeSt[pmStkP+6].scrs));
	if (pmSeSt[pmStkP+7].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+4].id, pmSeSt[pmStkP+7].id))
		lmLog(&pmSeSt[pmStkP+7].id->srcp, 200, sevWAR, pmSeSt[pmStkP+4].id->string);
    	break;}
    case 78: { /* <add_tail> = 'END' 'ADD' __genSym#2 <optional_id> __genSym#3; */
#line 719 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+4].id; 	break;}
    case 83: { /* <instance> = 'THE' ID <optional_heritage> <properties> <instance tail>; */
#line 726 "alan.pmk"

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
					pmSeSt[pmStkP+4].ment,
					pmSeSt[pmStkP+4].mentSrcp,
					pmSeSt[pmStkP+4].art,
					pmSeSt[pmStkP+4].artSrcp,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 84: { /* <instance tail> = 'END' 'THE' <optional_id> __genSym#4; */
#line 753 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 87: { /* <optional_heritage> =; */
#line 758 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 88: { /* <optional_heritage> = <heritage>; */
#line 763 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 89: { /* <heritage> = 'ISA' ID __genSym#5; */
#line 770 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 92: { /* <properties> =; */
#line 775 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out all fields */
    	break;}
    case 93: { /* <properties> = <properties> <property>; */
#line 780 "alan.pmk"

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

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXIT_LIST);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VERB_LIST);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, SCRIPT_LIST);
    	break;}
    case 98: { /* <property> = <name>; */
#line 849 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 94: { /* <property> = <where> __genSym#6; */
#line 855 "alan.pmk"
{
        Where *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 100: { /* <property> = <is> <attributes>; */
#line 865 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 95: { /* <property> = <container properties>; */
#line 871 "alan.pmk"
{
        Container *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 96: { /* <property> = <description>; */
#line 881 "alan.pmk"
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
    case 99: { /* <property> = <mentioned>; */
#line 896 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 97: { /* <property> = <article>; */
#line 906 "alan.pmk"
{ List *art = pmSeSt[pmStkP+1].art;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].art = art;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 102: { /* <property> = <exit>; */
#line 916 "alan.pmk"
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 103: { /* <property> = <verb>; */
#line 922 "alan.pmk"
{ Verb *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 101: { /* <property> = <script>; */
#line 928 "alan.pmk"
{ Script *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 106: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 938 "alan.pmk"

	pmSeSt[pmStkP+1].ext = newext(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].idList,
			   pmSeSt[pmStkP+4].id,
			   pmSeSt[pmStkP+5].chks,
			   pmSeSt[pmStkP+5].stms);
	if (pmSeSt[pmStkP+5].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+2].str, pmSeSt[pmStkP+5].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].str);
	}
    	break;}
    case 107: { /* <optional_exit_body> =; */
#line 953 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 108: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 960 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 109: { /* <optional_attributes> =; */
#line 975 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 110: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 980 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 111: { /* <is> = 'IS'; */
#line 987 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 112: { /* <is> = 'ARE'; */
#line 992 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 113: { /* <is> = 'HAS'; */
#line 997 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 114: { /* <optional_description> =; */
#line 1004 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 116: { /* <description> = 'DESCRIPTION' <optional_checks> <optional_does>; */
#line 1015 "alan.pmk"

	if (pmSeSt[pmStkP+2].chks == NULL && pmSeSt[pmStkP+3].stms == NULL) {
	  pmSeSt[pmStkP+1].chks = NULL;
	  pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
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
    case 117: { /* <description> = 'DESCRIPTION' <statements>; */
#line 1040 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].srcp2 = nulsrcp;
    	break;}
    case 118: { /* <article> = 'ARTICLE'; */
#line 1049 "alan.pmk"

	pmSeSt[pmStkP+1].art = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							NOP_STATEMENT), STATEMENT_LIST);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 119: { /* <article> = 'ARTICLE' <statements>; */
#line 1056 "alan.pmk"

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 120: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 1065 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 121: { /* <optional_names> =; */
#line 1074 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 122: { /* <optional_names> = <optional_names> <name>; */
#line 1079 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					pmSeSt[pmStkP+2].nam, NAME_LIST);
    	break;}
    case 123: { /* <name> = 'NAME' <ids> __genSym#7; */
#line 1087 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 126: { /* <container properties> = __genSym#8 'CONTAINER' <container_body>; */
#line 1096 "alan.pmk"

	pmSeSt[pmStkP+1].cnt = newContainer(newContainerBody(&pmSySt[pmStkP+2].srcp,
						pmSeSt[pmStkP+3].lims,
						pmSeSt[pmStkP+3].stms,
						pmSeSt[pmStkP+3].stms2,
						pmSeSt[pmStkP+3].chks,
						pmSeSt[pmStkP+3].stms3));
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 129: { /* <container_body> = <optional_limits> <optional_header> <optional_empty> <optional_extract>; */
#line 1110 "alan.pmk"

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+4].chks;
	pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+4].stms;
    	break;}
    case 130: { /* <optional_limits> =; */
#line 1121 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 131: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1126 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 132: { /* <limits> = <limit>; */
#line 1133 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIMIT_LIST);
    	break;}
    case 133: { /* <limits> = <limits> <limit>; */
#line 1138 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMIT_LIST);
    	break;}
    case 134: { /* <limit> = <limit_attribute> 'THEN' <statements>; */
#line 1145 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 135: { /* <limit_attribute> = <attribute definition>; */
#line 1154 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 136: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1159 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSySt[pmStkP+1].srcp,
				      INTEGER_TYPE,
				      newId(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 137: { /* <optional_header> =; */
#line 1169 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 138: { /* <optional_header> = 'HEADER' <statements>; */
#line 1174 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 139: { /* <optional_empty> =; */
#line 1181 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 140: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1186 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 141: { /* <optional_extract> =; */
#line 1193 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 142: { /* <optional_extract> = 'EXTRACT' <optional_checks> <optional_does>; */
#line 1199 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
	if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	  lmLog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
    	break;}
    case 143: { /* <optional_extract> = 'EXTRACT' <statements>; */
#line 1207 "alan.pmk"

	pmSeSt[pmStkP+1].chk = NULL;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 144: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1217 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 145: { /* <event_header> = 'EVENT' ID; */
#line 1230 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 146: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1238 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 147: { /* <script> = 'SCRIPT' ID __genSym#9 <optional_description> <step_list>; */
#line 1246 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newScript(&pmSySt[pmStkP+1].srcp,
			          pmSeSt[pmStkP+2].id,
			          pmSeSt[pmStkP+4].stms,
			          pmSeSt[pmStkP+5].stps);
    	break;}
    case 150: { /* <step_list> = <step>; */
#line 1256 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, STEP_LIST);
    	break;}
    case 151: { /* <step_list> = <step_list> <step>; */
#line 1261 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STEP_LIST);
    	break;}
    case 152: { /* <step> = 'STEP' <statements>; */
#line 1268 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 153: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1276 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 154: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1284 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 155: { /* <rule> = 'WHEN' <expression> <then> <statements> <optional_end_when>; */
#line 1295 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 162: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1315 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 163: { /* <optional_statements> =; */
#line 1324 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 164: { /* <optional_statements> = <statements>; */
#line 1329 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 165: { /* <statements> = <statement>; */
#line 1336 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, STATEMENT_LIST);
    	break;}
    case 166: { /* <statements> = <statements> <statement>; */
#line 1341 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STATEMENT_LIST);
    	break;}
    case 167: { /* <statement> = <output_statement>; */
#line 1348 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 169: { /* <statement> = <manipulation_statement>; */
#line 1353 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 172: { /* <statement> = <assignment_statement>; */
#line 1358 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 173: { /* <statement> = <conditional_statement>; */
#line 1363 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 174: { /* <statement> = <repetition_statement>; */
#line 1368 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 170: { /* <statement> = <actor_statement>; */
#line 1373 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 171: { /* <statement> = <event_statement>; */
#line 1378 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 168: { /* <statement> = <special_statement>; */
#line 1383 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 175: { /* <output_statement> = STRING; */
#line 1391 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, PRINT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 176: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1398 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DESCRIBE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 177: { /* <output_statement> = 'SAY' <expression> '.'; */
#line 1404 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SAY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 178: { /* <output_statement> = 'LIST' <what> '.'; */
#line 1410 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LIST_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 179: { /* <output_statement> = 'SHOW' ID '.'; */
#line 1416 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SHOW_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.show.filename = pmSeSt[pmStkP+2].id;
    	break;}
    case 180: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */
#line 1425 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, EMPTY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.where = pmSeSt[pmStkP+3].whr;
    	break;}
    case 181: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */
#line 1432 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LOCATE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 183: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1443 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SCHEDULE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 182: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1451 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, CANCEL_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 184: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */
#line 1461 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, MAKE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 187: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1469 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SET_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 185: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1477 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, INCREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 186: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1485 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DECREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 188: { /* <optional_by_clause> =; */
#line 1495 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 189: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1500 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 190: { /* <conditional_statement> = <if_statement>; */
#line 1509 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 191: { /* <conditional_statement> = <depending_statement>; */
#line 1514 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 192: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 1522 "alan.pmk"

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
    case 193: { /* <optional_elsif_list> =; */
#line 1540 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 194: { /* <optional_elsif_list> = <elsif_list>; */
#line 1545 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 195: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 1552 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, STATEMENT_LIST);
    }	break;}
    case 196: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 1565 "alan.pmk"
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
    case 197: { /* <optional_else_part> =; */
#line 1584 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 198: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 1589 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 199: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' __genSym#11 '.'; */
#line 1596 "alan.pmk"
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, DEPEND_STATEMENT);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 202: { /* <depend_cases> = <depend_case>; */
#line 1607 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, CASE_LIST);
    	break;}
    case 203: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1612 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASE_LIST);
    	break;}
    case 205: { /* <depend_case> = <right_hand_side> ':' <statements>; */
#line 1619 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 204: { /* <depend_case> = 'ELSE' <statements>; */
#line 1626 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 206: { /* <repetition_statement> = 'FOR' 'EACH' ID <optional_loop_restriction> 'DO' <statements> 'END' <for_each> __genSym#12; */
#line 1636 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, EACH_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.each.stms = pmSeSt[pmStkP+6].stms;
	pmSeSt[pmStkP+1].stm->fields.each.loopId = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.each.classId = pmSeSt[pmStkP+4].id;
    	break;}
    case 209: { /* <optional_loop_restriction> =; */
#line 1646 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 210: { /* <optional_loop_restriction> = 'ISA' ID; */
#line 1650 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 215: { /* <actor_statement> = 'USE' 'SCRIPT' ID <optional_for_actor> '.'; */
#line 1664 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, USE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 214: { /* <actor_statement> = 'STOP' ID '.'; */
#line 1671 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STOP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.stop.actor = pmSeSt[pmStkP+2].id;
    	break;}
    case 216: { /* <optional_for_actor> =; */
#line 1679 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 217: { /* <optional_for_actor> = 'FOR' ID; */
#line 1684 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 218: { /* <special_statement> = 'QUIT' '.'; */
#line 1692 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, QUIT_STATEMENT);
    	break;}
    case 219: { /* <special_statement> = 'LOOK' '.'; */
#line 1697 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LOOK_STATEMENT);
    	break;}
    case 220: { /* <special_statement> = 'SAVE' '.'; */
#line 1702 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SAVE_STATEMENT);
    	break;}
    case 221: { /* <special_statement> = 'RESTORE' '.'; */
#line 1707 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, RESTORE_STATEMENT);
    	break;}
    case 222: { /* <special_statement> = 'RESTART' '.'; */
#line 1712 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, RESTART_STATEMENT);
    	break;}
    case 223: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 1717 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SCORE_STATEMENT);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 224: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 1728 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, VISITS_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 225: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 1734 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SYSTEM_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 226: { /* <expression> = <term>; */
#line 1745 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 227: { /* <expression> = <expression> 'OR' <term>; */
#line 1750 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OR_OPERATOR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 228: { /* <term> = <factor>; */
#line 1762 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 229: { /* <term> = <term> 'AND' <factor>; */
#line 1767 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not	      = FALSE;
	exp->fields.bin.op    = AND_OPERATOR;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 230: { /* <factor> = <arithmetic>; */
#line 1779 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 232: { /* <factor> = <factor> <optional_not> <relop> <arithmetic>; */
#line 1784 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+3].srcp, BINARY_EXPRESSION);
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.bin.op = pmSeSt[pmStkP+3].op;
	exp->fields.bin.right = pmSeSt[pmStkP+4].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 231: { /* <factor> = <factor> <optional_not> <where>; */
#line 1795 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+3].srcp, WHERE_EXPRESSION);
	exp->fields.whr.wht = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = WHERE_EXPRESSION;
    } 	break;}
    case 234: { /* <factor> = <factor> <optional_not> 'BETWEEN' <arithmetic> 'AND' <arithmetic>; */
#line 1805 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+3].srcp, BETWEEN_EXPRESSION);
	exp->fields.btw.val = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.btw.low = pmSeSt[pmStkP+4].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+6].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BETWEEN_EXPRESSION;
    } 	break;}
    case 233: { /* <factor> = <factor> <optional_not> 'CONTAINS' <arithmetic>; */
#line 1816 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+3].srcp, BINARY_EXPRESSION);
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.bin.op = CONTAINS_OPERATOR;
	exp->fields.bin.right = pmSeSt[pmStkP+4].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 235: { /* <arithmetic> = <primary>; */
#line 1830 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 237: { /* <arithmetic> = <primary> 'ISA' ID; */
#line 1835 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, ISA_EXPRESSION);
	exp->fields.isa.wht = pmSeSt[pmStkP+1].exp;
	exp->not = FALSE;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = ISA_EXPRESSION;
    } 	break;}
    case 238: { /* <arithmetic> = <primary> <is> <something>; */
#line 1845 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, ATTRIBUTE_EXPRESSION);
	exp->fields.atr.wht = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+3].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = ATTRIBUTE_EXPRESSION;
    } 	break;}
    case 239: { /* <arithmetic> = <arithmetic> <binop> <primary>; */
#line 1855 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, BINARY_EXPRESSION);
 	exp->not = FALSE;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 236: { /* <arithmetic> = <aggregate> <aggregation_filters>; */
#line 1866 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, AGGREGATE_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.agr.kind = pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.attribute = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.filters = pmSeSt[pmStkP+2].exps;
    	break;}
    case 240: { /* <aggregation_filters> = <aggregation_filter>; */
#line 1877 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(NULL, pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 241: { /* <aggregation_filters> = <aggregation_filters> ',' <aggregation_filter>; */
#line 1882 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(pmSeSt[pmStkP+1].exps, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 242: { /* <aggregation_filter> = <optional_not> <where>; */
#line 1889 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, WHERE_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = WHERE_EXPRESSION;
    } 	break;}
    case 243: { /* <aggregation_filter> = <optional_not> 'ISA' ID; */
#line 1898 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, ISA_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = ISA_EXPRESSION;
    } 	break;}
    case 244: { /* <aggregation_filter> = <is> <something>; */
#line 1907 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, ATTRIBUTE_EXPRESSION);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = ATTRIBUTE_EXPRESSION;
    } 	break;}
    case 246: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 1918 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 248: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 1928 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BETWEEN_EXPRESSION;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BETWEEN_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 247: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 1938 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = CONTAINS_OPERATOR;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 253: { /* <primary> = '(' <expression> ')'; */
#line 1953 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 249: { /* <primary> = <optional_minus> Integer; */
#line 1958 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, INTEGER_EXPRESSION);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 250: { /* <primary> = STRING; */
#line 1967 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, STRING_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 251: { /* <primary> = <what>; */
#line 1974 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, WHAT_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 254: { /* <primary> = <attribute_reference>; */
#line 1980 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, ATTRIBUTE_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, WHAT_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 255: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 1988 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, RANDOM_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 252: { /* <primary> = 'SCORE'; */
#line 1995 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, SCORE_EXPRESSION);
    	break;}
    case 257: { /* <aggregate> = <aggregator> 'OF' ID; */
#line 2003 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 256: { /* <aggregate> = 'COUNT'; */
#line 2010 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = COUNT_AGGREGATE;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 258: { /* <aggregator> = 'MAX'; */
#line 2019 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MAX_AGGREGATE;
    	break;}
    case 259: { /* <aggregator> = 'MIN'; */
#line 2025 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MIN_AGGREGATE;
    	break;}
    case 260: { /* <aggregator> = 'SUM'; */
#line 2031 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = SUM_AGGREGATE;
    	break;}
    case 261: { /* <something> = <optional_not> ID; */
#line 2040 "alan.pmk"

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 262: { /* <what> = ID; */
#line 2048 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSeSt[pmStkP+1].id->srcp, WHAT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 264: { /* <what> = 'CURRENT' 'ACTOR'; */
#line 2054 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_ACTOR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 265: { /* <what> = 'CURRENT' 'LOCATION'; */
#line 2060 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_LOCATION, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 263: { /* <what> = 'THIS'; */
#line 2066 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_THIS, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 266: { /* <optional_where> =; */
#line 2075 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 267: { /* <optional_where> = <where>; */
#line 2080 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 268: { /* <where> = 'HERE'; */
#line 2087 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 269: { /* <where> = 'NEARBY'; */
#line 2093 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 270: { /* <where> = 'AT' <what>; */
#line 2099 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHERE_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 271: { /* <where> = 'IN' <what>; */
#line 2105 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 272: { /* <binop> = '+'; */
#line 2113 "alan.pmk"

	pmSeSt[pmStkP+1].op = PLUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 273: { /* <binop> = '-'; */
#line 2119 "alan.pmk"

	pmSeSt[pmStkP+1].op = MINUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 274: { /* <binop> = '*'; */
#line 2125 "alan.pmk"

	pmSeSt[pmStkP+1].op = MULT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 275: { /* <binop> = '/'; */
#line 2131 "alan.pmk"

	pmSeSt[pmStkP+1].op = DIV_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 276: { /* <relop> = '<>'; */
#line 2140 "alan.pmk"

        pmSeSt[pmStkP+1].op   = NE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 277: { /* <relop> = '='; */
#line 2146 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EQ_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 278: { /* <relop> = '=='; */
#line 2152 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EXACT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 279: { /* <relop> = '>='; */
#line 2158 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 280: { /* <relop> = '<='; */
#line 2164 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 281: { /* <relop> = '>'; */
#line 2170 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 282: { /* <relop> = '<'; */
#line 2176 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 283: { /* <optional_qual> =; */
#line 2186 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 284: { /* <optional_qual> = 'BEFORE'; */
#line 2191 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 285: { /* <optional_qual> = 'AFTER'; */
#line 2197 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 286: { /* <optional_qual> = 'ONLY'; */
#line 2203 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 287: { /* <optional_not> =; */
#line 2212 "alan.pmk"

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 288: { /* <optional_not> = 'NOT'; */
#line 2217 "alan.pmk"

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 289: { /* <optional_id> =; */
#line 2224 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 290: { /* <optional_id> = ID; */
#line 2229 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 291: { /* <ids> = ID; */
#line 2236 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 292: { /* <ids> = <ids> ID; */
#line 2241 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, ID_LIST);
    	break;}
    case 293: { /* <id_list> = ID; */
#line 2248 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 294: { /* <id_list> = <id_list> ',' ID; */
#line 2254 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, ID_LIST);
    	break;}
    case 295: { /* <optional_integer> =; */
#line 2262 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 296: { /* <optional_integer> = Integer; */
#line 2267 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 297: { /* <optional_minus> =; */
#line 2274 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 298: { /* <optional_minus> = '-'; */
#line 2279 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 299: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2286 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 300: { /* ID = Identifier; */
#line 2295 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 301: { /* ID = 'LOCATION'; */
#line 2300 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "location");
    	break;}
    case 302: { /* ID = 'ACTOR'; */
#line 2305 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "actor");
    	break;}
    default: break; }
}/*pmPaSema()*/

