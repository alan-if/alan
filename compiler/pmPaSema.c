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
    Bool opaque;
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
    FormType form;
    List *exts;
    ExtNod *ext;
    ExpressionKind expKd;
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
    Srcp indefiniteSrcp;
    List *indefinite;
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
#line 181 "alan.pmk"
    switch (rule) {
    case 8: { /* <option> = ID '.'; */
#line 201 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 9: { /* <option> = ID ID '.'; */
#line 206 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 10: { /* <option> = ID Integer '.'; */
#line 211 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 17: { /* <declaration> = <synonyms>; */
#line 224 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns);	break;}
    case 13: { /* <declaration> = <messages>; */
#line 227 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 18: { /* <declaration> = <syntax>; */
#line 230 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 19: { /* <declaration> = <verb>; */
#line 233 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VERB_LIST); 	break;}
    case 14: { /* <declaration> = <class>; */
#line 236 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLASS_LIST); 	break;}
    case 20: { /* <declaration> = <addition>; */
#line 239 "alan.pmk"
 adv.adds = concat(adv.adds, pmSeSt[pmStkP+1].add, ADD_LIST); 	break;}
    case 15: { /* <declaration> = <instance>; */
#line 242 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSTANCE_LIST); 	break;}
    case 21: { /* <declaration> = <event>; */
#line 245 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVENT_LIST); 	break;}
    case 16: { /* <declaration> = <rule>; */
#line 248 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULE_LIST); 	break;}
    case 22: { /* <attributes> = <attribute definition> '.'; */
#line 254 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, ATTRIBUTE_LIST);
    	break;}
    case 23: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 259 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATTRIBUTE_LIST);
    	break;}
    case 24: { /* <attribute definition> = ID; */
#line 266 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+1].id->srcp,
						BOOLEAN_TYPE,
						pmSeSt[pmStkP+1].id,
						TRUE, 0, 0);
    	break;}
    case 25: { /* <attribute definition> = 'NOT' ID; */
#line 274 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+2].id->srcp,
						BOOLEAN_TYPE,
						pmSeSt[pmStkP+2].id,
						FALSE, 0, 0);
    	break;}
    case 26: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 282 "alan.pmk"

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
    case 27: { /* <attribute definition> = ID STRING; */
#line 296 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSeSt[pmStkP+1].id->srcp,
						STRING_TYPE,
						pmSeSt[pmStkP+1].id,
						0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 28: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 307 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 29: { /* <synonym_list> = <synonym_declaration>; */
#line 314 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, SYNONYM_LIST);
    	break;}
    case 30: { /* <synonym_list> = <synonym_list> <synonym_declaration>; */
#line 319 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNONYM_LIST);
    	break;}
    case 31: { /* <synonym_declaration> = <id_list> '=' ID '.'; */
#line 326 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			     			pmSeSt[pmStkP+1].idList,
			      			pmSeSt[pmStkP+3].id);
    	break;}
    case 32: { /* <messages> = 'MESSAGE' <message_list>; */
#line 336 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 33: { /* <message_list> = <message>; */
#line 343 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, MESSAGE_LIST);
    	break;}
    case 34: { /* <message_list> = <message_list> <message>; */
#line 348 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MESSAGE_LIST);
    	break;}
    case 35: { /* <message> = ID ':' <statements>; */
#line 355 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 36: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 365 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 37: { /* <syntax_list> = <syntax_item>; */
#line 372 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, SYNTAX_LIST);
    	break;}
    case 38: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 377 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, SYNTAX_LIST);
    	break;}
    case 39: { /* <syntax_item> = ID '=' <syntax_elements> <optional_syntax_restrictions>; */
#line 384 "alan.pmk"

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
    case 40: { /* <syntax_elements> = <syntax_element>; */
#line 400 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, ELEMENT_LIST);
    	break;}
    case 41: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 405 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                ELEMENT_LIST);
    	break;}
    case 42: { /* <syntax_element> = ID; */
#line 414 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newElement(&pmSeSt[pmStkP+1].id->srcp, WORD_ELEMENT,
					   pmSeSt[pmStkP+1].id,
					   FALSE);
    	break;}
    case 43: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 422 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newElement(&pmSeSt[pmStkP+2].id->srcp, PARAMETER_ELEMENT,
					   pmSeSt[pmStkP+2].id,
					   pmSeSt[pmStkP+4].bits);
    	break;}
    case 44: { /* <optional_indicators> =; */
#line 431 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 45: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 436 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 46: { /* <indicator> = '*'; */
#line 443 "alan.pmk"

	pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 47: { /* <indicator> = '!'; */
#line 448 "alan.pmk"

	pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 48: { /* <syntax_restriction_clauses> = <syntax_restriction>; */
#line 461 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, RESTRICTION_LIST);
    	break;}
    case 49: { /* <syntax_restriction_clauses> = <syntax_restriction_clauses> 'AND' <syntax_restriction>; */
#line 466 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, RESTRICTION_LIST);
    	break;}
    case 50: { /* <syntax_restriction> = ID 'ISA' <restriction_class> 'ELSE' <statements>; */
#line 474 "alan.pmk"

	pmSeSt[pmStkP+1].res = newRestriction(&pmSySt[pmStkP+2].srcp,
			  			  pmSeSt[pmStkP+1].id,
						  pmSeSt[pmStkP+3].restrictionKind,
						  pmSeSt[pmStkP+3].id,
						  pmSeSt[pmStkP+5].stms);
    	break;}
    case 51: { /* <restriction_class> = ID; */
#line 485 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].restrictionKind = ID_RESTRICTION;
    	break;}
    case 52: { /* <restriction_class> = 'CONTAINER'; */
#line 491 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
	pmSeSt[pmStkP+1].restrictionKind = CONTAINER_RESTRICTION;
    	break;}
    case 53: { /* <optional_syntax_restrictions> = '.'; */
#line 499 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 54: { /* <optional_syntax_restrictions> = 'WHERE' <syntax_restriction_clauses>; */
#line 504 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 55: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 514 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newVerb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 56: { /* <verb_header> = 'VERB' <id_list>; */
#line 527 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 57: { /* <verb_body> = <simple_verb_body>; */
#line 536 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&pmSeSt[pmStkP+1].srcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   ALTERNATIVE_LIST);
    	break;}
    case 58: { /* <verb_body> = <verb_alternatives>; */
#line 547 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 59: { /* <verb_alternatives> = <verb_alternative>; */
#line 554 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, ALTERNATIVE_LIST);
    	break;}
    case 60: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 559 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				ALTERNATIVE_LIST);
    	break;}
    case 61: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 568 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 62: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 579 "alan.pmk"

	if (pmSeSt[pmStkP+2].stms == NULL) /* No DOES */
	  pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	else
	  pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+2].srcp;
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
	pmSeSt[pmStkP+1].srcp = nulsrcp;
    	break;}
    case 65: { /* <optional_checks> = 'CHECK' <statements>; */
#line 606 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), CHECK_LIST);
    	break;}
    case 66: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 612 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 67: { /* <check_list> = <check>; */
#line 620 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, CHECK_LIST);
    	break;}
    case 68: { /* <check_list> = <check_list> 'AND' <check>; */
#line 625 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHECK_LIST);
    	break;}
    case 69: { /* <check> = <expression> 'ELSE' <statements>; */
#line 632 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 70: { /* <optional_does> =; */
#line 640 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 72: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 651 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 73: { /* <class> = 'EVERY' ID <optional_heritage> <properties> <class_tail>; */
#line 662 "alan.pmk"

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
					pmSeSt[pmStkP+4].entered,
					pmSeSt[pmStkP+4].enteredSrcp,
					pmSeSt[pmStkP+4].ment,
					pmSeSt[pmStkP+4].mentSrcp,
					pmSeSt[pmStkP+4].indefinite,
					pmSeSt[pmStkP+4].indefiniteSrcp,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 74: { /* <class_tail> = 'END' 'EVERY' <optional_id> __genSym#1; */
#line 691 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 77: { /* <addition> = 'ADD' 'TO' __genSym#2 ID <optional_heritage> <properties> <add_tail>; */
#line 698 "alan.pmk"

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
					pmSeSt[pmStkP+6].entered,
					pmSeSt[pmStkP+6].enteredSrcp,
					pmSeSt[pmStkP+6].ment,
					pmSeSt[pmStkP+6].mentSrcp,
					pmSeSt[pmStkP+6].indefinite,
					pmSeSt[pmStkP+6].indefiniteSrcp,
					pmSeSt[pmStkP+6].exts,
					pmSeSt[pmStkP+6].vrbs,
					pmSeSt[pmStkP+6].scrs));
	if (pmSeSt[pmStkP+7].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+4].id, pmSeSt[pmStkP+7].id))
		lmLog(&pmSeSt[pmStkP+7].id->srcp, 200, sevWAR, pmSeSt[pmStkP+4].id->string);
    	break;}
    case 80: { /* <add_tail> = 'END' 'ADD' __genSym#3 <optional_id> __genSym#4; */
#line 727 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+4].id; 	break;}
    case 85: { /* <instance> = 'THE' ID <optional_heritage> <properties> <instance tail>; */
#line 734 "alan.pmk"

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
					pmSeSt[pmStkP+4].entered,
					pmSeSt[pmStkP+4].enteredSrcp,
					pmSeSt[pmStkP+4].ment,
					pmSeSt[pmStkP+4].mentSrcp,
					pmSeSt[pmStkP+4].indefinite,
					pmSeSt[pmStkP+4].indefiniteSrcp,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 86: { /* <instance tail> = 'END' 'THE' <optional_id> __genSym#5; */
#line 763 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 89: { /* <optional_heritage> =; */
#line 768 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 90: { /* <optional_heritage> = <heritage>; */
#line 773 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 91: { /* <heritage> = 'ISA' ID __genSym#6; */
#line 780 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 94: { /* <properties> =; */
#line 785 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out all fields */
    	break;}
    case 95: { /* <properties> = <properties> <property>; */
#line 790 "alan.pmk"

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

	if (pmSeSt[pmStkP+2].ment != NULL) {
            if (pmSeSt[pmStkP+1].ment != NULL)
                /* MENTIONED already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "MENTIONED", "class/instance", NULL);
	    else {
                pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+2].ment;
		pmSeSt[pmStkP+1].mentSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].indefinite != NULL) {
            if (pmSeSt[pmStkP+1].indefinite != NULL)
                /* Indefinite Article/Form already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "[Indefinite] Article/Form", "class/instance", NULL);
            else { 
	        pmSeSt[pmStkP+1].indefinite = pmSeSt[pmStkP+2].indefinite;
		pmSeSt[pmStkP+1].indefiniteSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXIT_LIST);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VERB_LIST);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, SCRIPT_LIST);
    	break;}
    case 100: { /* <property> = <name>; */
#line 869 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 96: { /* <property> = <where> __genSym#7; */
#line 875 "alan.pmk"
{
        Where *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 103: { /* <property> = <is> <attributes>; */
#line 885 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 97: { /* <property> = <container properties>; */
#line 891 "alan.pmk"
{
        Container *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 98: { /* <property> = <description>; */
#line 901 "alan.pmk"
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
    case 101: { /* <property> = <entered>; */
#line 916 "alan.pmk"
{
        List *entered = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].entered = entered;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 102: { /* <property> = <mentioned>; */
#line 926 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 99: { /* <property> = __genSym#8 <article>; */
#line 936 "alan.pmk"
{ List *indefinite = pmSeSt[pmStkP+2].indefinite;
        Srcp srcp = pmSeSt[pmStkP+2].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].indefinite = indefinite;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 105: { /* <property> = <exit>; */
#line 946 "alan.pmk"
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 106: { /* <property> = <verb>; */
#line 952 "alan.pmk"
{ Verb *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 104: { /* <property> = <script>; */
#line 958 "alan.pmk"
{ Script *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 111: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 968 "alan.pmk"

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
    case 112: { /* <optional_exit_body> =; */
#line 983 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 113: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 990 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 114: { /* <optional_attributes> =; */
#line 1005 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 115: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 1010 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 116: { /* <is> = 'IS'; */
#line 1017 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 117: { /* <is> = 'ARE'; */
#line 1022 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 118: { /* <is> = 'HAS'; */
#line 1027 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 119: { /* <optional_description> =; */
#line 1034 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 121: { /* <description> = 'DESCRIPTION' <optional_checks> <optional_does>; */
#line 1045 "alan.pmk"

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
    case 122: { /* <description> = 'DESCRIPTION' <statements>; */
#line 1070 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].srcp2 = nulsrcp;
    	break;}
    case 123: { /* <article> = 'ARTICLE'; */
#line 1079 "alan.pmk"

	pmSeSt[pmStkP+1].indefinite = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							NOP_STATEMENT), STATEMENT_LIST);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 124: { /* <article> = 'ARTICLE' <statements>; */
#line 1086 "alan.pmk"

	pmSeSt[pmStkP+1].indefinite = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 125: { /* <entered> = 'ENTERED' <statements>; */
#line 1095 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 126: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 1104 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 127: { /* <optional_names> =; */
#line 1113 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 128: { /* <optional_names> = <optional_names> <name>; */
#line 1118 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					pmSeSt[pmStkP+2].nam, NAME_LIST);
    	break;}
    case 129: { /* <name> = 'NAME' <ids> <optional_full_stop>; */
#line 1126 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 132: { /* <container properties> = __genSym#9 <optionally_opaque> 'CONTAINER' <container_body>; */
#line 1139 "alan.pmk"

	pmSeSt[pmStkP+1].cnt = newContainer(newContainerBody(&pmSySt[pmStkP+3].srcp,
					pmSeSt[pmStkP+2].opaque,
					pmSeSt[pmStkP+4].lims,
					pmSeSt[pmStkP+4].stms,
					pmSeSt[pmStkP+4].stms2,
					pmSeSt[pmStkP+4].chks,
					pmSeSt[pmStkP+4].stms3));
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+3].srcp;
    	break;}
    case 135: { /* <optionally_opaque> =; */
#line 1154 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = FALSE; 	break;}
    case 136: { /* <optionally_opaque> = 'OPAQUE'; */
#line 1156 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = TRUE; 	break;}
    case 137: { /* <container_body> = <optional_limits> <optional_header> <optional_empty> <optional_extract>; */
#line 1162 "alan.pmk"

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+4].chks;
	pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+4].stms;
    	break;}
    case 138: { /* <optional_limits> =; */
#line 1173 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 139: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1178 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 140: { /* <limits> = <limit>; */
#line 1185 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIMIT_LIST);
    	break;}
    case 141: { /* <limits> = <limits> <limit>; */
#line 1190 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMIT_LIST);
    	break;}
    case 142: { /* <limit> = <limit_attribute> 'THEN' <statements>; */
#line 1197 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 143: { /* <limit_attribute> = <attribute definition>; */
#line 1206 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 144: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1211 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newAttribute(&pmSySt[pmStkP+1].srcp,
				      INTEGER_TYPE,
				      newId(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 145: { /* <optional_header> =; */
#line 1221 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 146: { /* <optional_header> = 'HEADER' <statements>; */
#line 1226 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 147: { /* <optional_empty> =; */
#line 1233 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 148: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1238 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 149: { /* <optional_extract> =; */
#line 1245 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 150: { /* <optional_extract> = 'EXTRACT' <optional_checks> <optional_does>; */
#line 1251 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
	if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	  lmLog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
    	break;}
    case 151: { /* <optional_extract> = 'EXTRACT' <statements>; */
#line 1259 "alan.pmk"

	pmSeSt[pmStkP+1].chk = NULL;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 152: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1269 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 153: { /* <event_header> = 'EVENT' ID; */
#line 1282 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 154: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1290 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 155: { /* <script> = 'SCRIPT' ID __genSym#10 <optional_description> <step_list>; */
#line 1298 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newScript(&pmSySt[pmStkP+1].srcp,
			          pmSeSt[pmStkP+2].id,
			          pmSeSt[pmStkP+4].stms,
			          pmSeSt[pmStkP+5].stps);
    	break;}
    case 158: { /* <step_list> = <step>; */
#line 1308 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, STEP_LIST);
    	break;}
    case 159: { /* <step_list> = <step_list> <step>; */
#line 1313 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STEP_LIST);
    	break;}
    case 160: { /* <step> = 'STEP' <statements>; */
#line 1320 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 161: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1328 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 162: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1336 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 163: { /* <rule> = 'WHEN' <expression> <then> <statements> <optional_end_when>; */
#line 1347 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 170: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1367 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 171: { /* <optional_statements> =; */
#line 1376 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 172: { /* <optional_statements> = <statements>; */
#line 1381 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 173: { /* <statements> = <statement>; */
#line 1388 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, STATEMENT_LIST);
    	break;}
    case 174: { /* <statements> = <statements> <statement>; */
#line 1393 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STATEMENT_LIST);
    	break;}
    case 175: { /* <statement> = <output_statement>; */
#line 1400 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 177: { /* <statement> = <manipulation_statement>; */
#line 1405 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 180: { /* <statement> = <assignment_statement>; */
#line 1410 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 181: { /* <statement> = <conditional_statement>; */
#line 1415 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 182: { /* <statement> = <repetition_statement>; */
#line 1420 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 178: { /* <statement> = <actor_statement>; */
#line 1425 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 179: { /* <statement> = <event_statement>; */
#line 1430 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 176: { /* <statement> = <special_statement>; */
#line 1435 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 183: { /* <output_statement> = STRING; */
#line 1443 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, PRINT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 184: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1450 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DESCRIBE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 185: { /* <output_statement> = 'SAY' <form> <expression> '.'; */
#line 1456 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SAY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.say.form = pmSeSt[pmStkP+2].form;
    	break;}
    case 186: { /* <output_statement> = 'LIST' <what> '.'; */
#line 1463 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LIST_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 187: { /* <output_statement> = 'SHOW' ID '.'; */
#line 1469 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SHOW_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.show.filename = pmSeSt[pmStkP+2].id;
    	break;}
    case 188: { /* <form> =; */
#line 1477 "alan.pmk"
 pmSeSt[pmStkP+1].form = SIMPLE; 	break;}
    case 189: { /* <form> = 'THE'; */
#line 1479 "alan.pmk"
 pmSeSt[pmStkP+1].form = DEFINITE; 	break;}
    case 190: { /* <form> = 'AN'; */
#line 1481 "alan.pmk"
 pmSeSt[pmStkP+1].form = INDEFINITE; 	break;}
    case 191: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */
#line 1488 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, EMPTY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.where = pmSeSt[pmStkP+3].whr;
    	break;}
    case 192: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */
#line 1495 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LOCATE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 194: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1506 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SCHEDULE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 193: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1514 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, CANCEL_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 195: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */
#line 1524 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, MAKE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 198: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1532 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SET_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 196: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1540 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, INCREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 197: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1548 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DECREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 199: { /* <optional_by_clause> =; */
#line 1558 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 200: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1563 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 201: { /* <conditional_statement> = <if_statement>; */
#line 1572 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 202: { /* <conditional_statement> = <depending_statement>; */
#line 1577 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 203: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 1585 "alan.pmk"

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
    case 204: { /* <optional_elsif_list> =; */
#line 1603 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 205: { /* <optional_elsif_list> = <elsif_list>; */
#line 1608 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 206: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 1615 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, STATEMENT_LIST);
    }	break;}
    case 207: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 1628 "alan.pmk"
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
    case 208: { /* <optional_else_part> =; */
#line 1647 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 209: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 1652 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 210: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' __genSym#12 '.'; */
#line 1659 "alan.pmk"
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, DEPEND_STATEMENT);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 213: { /* <depend_cases> = <depend_case>; */
#line 1670 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, CASE_LIST);
    	break;}
    case 214: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1675 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASE_LIST);
    	break;}
    case 216: { /* <depend_case> = <right_hand_side> ':' <statements>; */
#line 1682 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 215: { /* <depend_case> = 'ELSE' <statements>; */
#line 1689 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 217: { /* <repetition_statement> = 'FOR' 'EACH' ID <optional_loop_restriction> 'DO' <statements> 'END' <for_each> __genSym#13; */
#line 1699 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, EACH_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.each.stms = pmSeSt[pmStkP+6].stms;
	pmSeSt[pmStkP+1].stm->fields.each.loopId = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.each.classId = pmSeSt[pmStkP+4].id;
    	break;}
    case 220: { /* <optional_loop_restriction> =; */
#line 1709 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 221: { /* <optional_loop_restriction> = 'ISA' ID; */
#line 1713 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 226: { /* <actor_statement> = 'USE' 'SCRIPT' ID <optional_for_actor> '.'; */
#line 1727 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, USE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 225: { /* <actor_statement> = 'STOP' ID '.'; */
#line 1734 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STOP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.stop.actor = pmSeSt[pmStkP+2].id;
    	break;}
    case 227: { /* <optional_for_actor> =; */
#line 1742 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 228: { /* <optional_for_actor> = 'FOR' ID; */
#line 1747 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 229: { /* <special_statement> = 'QUIT' '.'; */
#line 1755 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, QUIT_STATEMENT);
    	break;}
    case 230: { /* <special_statement> = 'LOOK' '.'; */
#line 1760 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, LOOK_STATEMENT);
    	break;}
    case 231: { /* <special_statement> = 'SAVE' '.'; */
#line 1765 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SAVE_STATEMENT);
    	break;}
    case 232: { /* <special_statement> = 'RESTORE' '.'; */
#line 1770 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, RESTORE_STATEMENT);
    	break;}
    case 233: { /* <special_statement> = 'RESTART' '.'; */
#line 1775 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, RESTART_STATEMENT);
    	break;}
    case 234: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 1780 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SCORE_STATEMENT);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scoreCount;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 235: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 1791 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, VISITS_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 236: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 1797 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, SYSTEM_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 237: { /* <expression> = <term>; */
#line 1808 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 238: { /* <expression> = <expression> 'OR' <term>; */
#line 1813 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OR_OPERATOR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 239: { /* <term> = <factor>; */
#line 1825 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 240: { /* <term> = <term> 'AND' <factor>; */
#line 1830 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not	      = FALSE;
	exp->fields.bin.op    = AND_OPERATOR;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 241: { /* <factor> = <arithmetic>; */
#line 1842 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 243: { /* <factor> = <factor> <optional_not> <relop> <arithmetic>; */
#line 1847 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+3].srcp, BINARY_EXPRESSION);
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.bin.op = pmSeSt[pmStkP+3].op;
	exp->fields.bin.right = pmSeSt[pmStkP+4].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 242: { /* <factor> = <factor> <optional_not> <where>; */
#line 1858 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+3].srcp, WHERE_EXPRESSION);
	exp->fields.whr.wht = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = WHERE_EXPRESSION;
    } 	break;}
    case 245: { /* <factor> = <factor> <optional_not> 'BETWEEN' <arithmetic> 'AND' <arithmetic>; */
#line 1868 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+3].srcp, BETWEEN_EXPRESSION);
	exp->fields.btw.val = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.btw.low = pmSeSt[pmStkP+4].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+6].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BETWEEN_EXPRESSION;
    } 	break;}
    case 244: { /* <factor> = <factor> <optional_not> 'CONTAINS' <arithmetic>; */
#line 1879 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+3].srcp, BINARY_EXPRESSION);
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.bin.op = CONTAINS_OPERATOR;
	exp->fields.bin.right = pmSeSt[pmStkP+4].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 246: { /* <arithmetic> = <primary>; */
#line 1893 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 248: { /* <arithmetic> = <primary> 'ISA' ID; */
#line 1898 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, ISA_EXPRESSION);
	exp->fields.isa.wht = pmSeSt[pmStkP+1].exp;
	exp->not = FALSE;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = ISA_EXPRESSION;
    } 	break;}
    case 249: { /* <arithmetic> = <primary> <is> <something>; */
#line 1908 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, ATTRIBUTE_EXPRESSION);
	exp->fields.atr.wht = pmSeSt[pmStkP+1].exp;
	exp->not = pmSeSt[pmStkP+3].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = ATTRIBUTE_EXPRESSION;
    } 	break;}
    case 250: { /* <arithmetic> = <arithmetic> <binop> <primary>; */
#line 1918 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, BINARY_EXPRESSION);
 	exp->not = FALSE;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.left = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
    } 	break;}
    case 247: { /* <arithmetic> = <aggregate> <aggregation_filters>; */
#line 1929 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, AGGREGATE_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.agr.kind = pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.attribute = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.filters = pmSeSt[pmStkP+2].exps;
    	break;}
    case 251: { /* <aggregation_filters> = <aggregation_filter>; */
#line 1940 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(NULL, pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 252: { /* <aggregation_filters> = <aggregation_filters> ',' <aggregation_filter>; */
#line 1945 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(pmSeSt[pmStkP+1].exps, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 253: { /* <aggregation_filter> = <optional_not> <where>; */
#line 1952 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, WHERE_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = WHERE_EXPRESSION;
    } 	break;}
    case 254: { /* <aggregation_filter> = <optional_not> 'ISA' ID; */
#line 1961 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, ISA_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = ISA_EXPRESSION;
    } 	break;}
    case 255: { /* <aggregation_filter> = <is> <something>; */
#line 1970 "alan.pmk"
 { Expression *exp;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, ATTRIBUTE_EXPRESSION);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
        pmSeSt[pmStkP+1].expKd = ATTRIBUTE_EXPRESSION;
    } 	break;}
    case 257: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 1981 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 259: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 1991 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BETWEEN_EXPRESSION;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BETWEEN_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 258: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 2001 "alan.pmk"
 { Expression *exp;
        pmSeSt[pmStkP+1].expKd = BINARY_EXPRESSION;
	exp = newexp(&pmSySt[pmStkP+2].srcp, BINARY_EXPRESSION);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = CONTAINS_OPERATOR;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 264: { /* <primary> = '(' <expression> ')'; */
#line 2016 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 260: { /* <primary> = <optional_minus> Integer; */
#line 2021 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, INTEGER_EXPRESSION);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 261: { /* <primary> = STRING; */
#line 2030 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, STRING_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 262: { /* <primary> = <what>; */
#line 2037 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, WHAT_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 265: { /* <primary> = <attribute_reference>; */
#line 2043 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, ATTRIBUTE_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, WHAT_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 266: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 2051 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, RANDOM_EXPRESSION);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 263: { /* <primary> = 'SCORE'; */
#line 2058 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, SCORE_EXPRESSION);
    	break;}
    case 268: { /* <aggregate> = <aggregator> 'OF' ID; */
#line 2066 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 267: { /* <aggregate> = 'COUNT'; */
#line 2073 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = COUNT_AGGREGATE;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 269: { /* <aggregator> = 'MAX'; */
#line 2082 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MAX_AGGREGATE;
    	break;}
    case 270: { /* <aggregator> = 'MIN'; */
#line 2088 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = MIN_AGGREGATE;
    	break;}
    case 271: { /* <aggregator> = 'SUM'; */
#line 2094 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = SUM_AGGREGATE;
    	break;}
    case 272: { /* <something> = <optional_not> ID; */
#line 2103 "alan.pmk"

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 273: { /* <what> = ID; */
#line 2111 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSeSt[pmStkP+1].id->srcp, WHAT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 275: { /* <what> = 'CURRENT' 'ACTOR'; */
#line 2117 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_ACTOR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 276: { /* <what> = 'CURRENT' 'LOCATION'; */
#line 2123 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_LOCATION, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 274: { /* <what> = 'THIS'; */
#line 2129 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newWhat(&pmSySt[pmStkP+1].srcp, WHAT_THIS, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 277: { /* <optional_where> =; */
#line 2138 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 278: { /* <optional_where> = <where>; */
#line 2143 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 279: { /* <where> = 'HERE'; */
#line 2150 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 280: { /* <where> = 'NEARBY'; */
#line 2156 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 281: { /* <where> = 'AT' <what>; */
#line 2162 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHERE_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 282: { /* <where> = 'IN' <what>; */
#line 2168 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 283: { /* <binop> = '+'; */
#line 2176 "alan.pmk"

	pmSeSt[pmStkP+1].op = PLUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 284: { /* <binop> = '-'; */
#line 2182 "alan.pmk"

	pmSeSt[pmStkP+1].op = MINUS_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 285: { /* <binop> = '*'; */
#line 2188 "alan.pmk"

	pmSeSt[pmStkP+1].op = MULT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 286: { /* <binop> = '/'; */
#line 2194 "alan.pmk"

	pmSeSt[pmStkP+1].op = DIV_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 287: { /* <relop> = '<>'; */
#line 2203 "alan.pmk"

        pmSeSt[pmStkP+1].op   = NE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 288: { /* <relop> = '='; */
#line 2209 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EQ_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 289: { /* <relop> = '=='; */
#line 2215 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EXACT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 290: { /* <relop> = '>='; */
#line 2221 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 291: { /* <relop> = '<='; */
#line 2227 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LE_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 292: { /* <relop> = '>'; */
#line 2233 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 293: { /* <relop> = '<'; */
#line 2239 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LT_OPERATOR;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 294: { /* <optional_qual> =; */
#line 2249 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 295: { /* <optional_qual> = 'BEFORE'; */
#line 2254 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 296: { /* <optional_qual> = 'AFTER'; */
#line 2260 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 297: { /* <optional_qual> = 'ONLY'; */
#line 2266 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 298: { /* <optional_not> =; */
#line 2275 "alan.pmk"

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 299: { /* <optional_not> = 'NOT'; */
#line 2280 "alan.pmk"

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 300: { /* <optional_id> =; */
#line 2287 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 301: { /* <optional_id> = ID; */
#line 2292 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 302: { /* <ids> = ID; */
#line 2299 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 303: { /* <ids> = <ids> ID; */
#line 2304 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, ID_LIST);
    	break;}
    case 304: { /* <id_list> = ID; */
#line 2311 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 305: { /* <id_list> = <id_list> ',' ID; */
#line 2317 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, ID_LIST);
    	break;}
    case 306: { /* <optional_integer> =; */
#line 2325 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 307: { /* <optional_integer> = Integer; */
#line 2330 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 308: { /* <optional_minus> =; */
#line 2337 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 309: { /* <optional_minus> = '-'; */
#line 2342 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 310: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2349 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 311: { /* ID = Identifier; */
#line 2358 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 312: { /* ID = 'LOCATION'; */
#line 2363 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "location");
    	break;}
    case 313: { /* ID = 'ACTOR'; */
#line 2368 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "actor");
    	break;}
    case 314: { /* ID = 'OPAQUE'; */
#line 2373 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "opaque");
    	break;}
    case 315: { /* ID = 'ON'; */
#line 2378 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "on");
    	break;}
    case 316: { /* ID = 'OF'; */
#line 2383 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "of");
    	break;}
    default: break; }
}/*pmPaSema()*/

