/*----------------------------------------------------------------------*\

	pmPaSema.c

	ParserMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


#include "util.h"
#include "acode.h"

#include "types.h"
#include "smScan.h"
#include "token.h"

#include "atr_x.h"
#include "cla_x.h"
#include "elm_x.h"
#include "exp_x.h"
#include "ext_x.h"
#include "id_x.h"
#include "ins_x.h"
#include "lst_x.h"
#include "res_x.h"
#include "slt_x.h"
#include "stm_x.h"
#include "stx_x.h"
#include "sym_x.h"
#include "whr_x.h"
#include "vrb_x.h"


#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "alt.h"                /* ALT-nodes */
#include "chk.h"                /* CHK-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "evt.h"                /* EVT-nodes */
#include "lim.h"		/* LIM-nodes */
#include "msg.h"                /* MSG-nodes */
#include "opt.h"		/* OPTIONS */
#include "rul.h"                /* RUL-nodes */
#include "sco.h"                /* SCORES */
#include "scr.h"                /* SCR-nodes */
#include "stp.h"                /* STP-nodes */
#include "str.h"		/* STRINGS */
#include "syn.h"                /* SYN-nodes */
#include "wht.h"                /* WHT-nodes */


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
    WhtNod *wht;
    WhrNod *whr;
    List *vrbs;
    VrbNod *vrb;
    int val;
    List *syns;
    SynNod *syn;
    List *stxs;
    StxNod *stx;
    char *str;
    List *stps;
    StpNod *stp;
    List *stms3;
    List *stms2;
    List *stms;
    StmNod *stm;
    Srcp srcp;
    List *scrs;
    ScrNod *scr;
    List *ruls;
    RulNod *rul;
    List *ress;
    ResNod *res;
    QualKind qual;
    OpKind op;
    Bool not;
    List *nams;
    List *nam;
    List *msgs;
    MsgNod *msg;
    Bool minus;
    List *ment;
    List *lims;
    LimNod *lim;
    List *inss;
    InsNod *ins;
    List *idList2;
    List *idList;
    IdNode *id;
    List *exts;
    ExtNod *ext;
   ExpKind expKd;
    ExpNod *exp;
    List *evts;
    EvtNod *evt;
    List *elms;
    ElmNod *elm;
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
    List *art;
    List *atrs;
    AtrNod *atr;
    List *alts;
    AltNod *alt;
    AgrKind agr;
} pmGrammar;

/* END %%ATTRIBUTES */

extern short pmStkP;
extern Token pmSySt[];
extern pmGrammar pmSeSt[];

/* %%DECLARATIONS - User data and routines */

#line 79 "alan.pmk"


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
#line 170 "alan.pmk"
    switch (rule) {
    case 6: { /* <option> = ID '.'; */
#line 190 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */
#line 195 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */
#line 200 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 15: { /* <unit> = <synonyms>; */
#line 213 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns); 	break;}
    case 11: { /* <unit> = <messages>; */
#line 216 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 16: { /* <unit> = <syntax>; */
#line 219 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 17: { /* <unit> = <verb>; */
#line 222 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, LIST_VRB); 	break;}
    case 12: { /* <unit> = <class>; */
#line 225 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, LIST_CLA); 	break;}
    case 13: { /* <unit> = <instance>; */
#line 228 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, LIST_INS); 	break;}
    case 19: { /* <unit> = <event>; */
#line 231 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, LIST_EVT); 	break;}
    case 18: { /* <unit> = <container>; */
#line 234 "alan.pmk"
 adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, LIST_CNT); 	break;}
    case 14: { /* <unit> = <rule>; */
#line 237 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, LIST_RUL); 	break;}
    case 20: { /* <attributes> = <attribute definition> '.'; */
#line 243 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, LIST_ATR);
    	break;}
    case 21: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 248 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, LIST_ATR);
    	break;}
    case 22: { /* <attribute definition> = ID; */
#line 255 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						BOOLEAN_TYPE,
						pmSeSt[pmStkP+1].id,
						TRUE, 0, 0);
    	break;}
    case 23: { /* <attribute definition> = 'NOT' ID; */
#line 263 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+2].id->srcp,
						BOOLEAN_TYPE,
						pmSeSt[pmStkP+2].id,
						FALSE, 0, 0);
    	break;}
    case 24: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 271 "alan.pmk"

	if (pmSeSt[pmStkP+2].minus)
		pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
							INTEGER_TYPE,
							pmSeSt[pmStkP+1].id,
							-val(pmSySt[pmStkP+3].chars), 0, 0);
	else
		pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
							INTEGER_TYPE,
							pmSeSt[pmStkP+1].id,
							val(pmSySt[pmStkP+3].chars), 0, 0);
    	break;}
    case 25: { /* <attribute definition> = ID STRING; */
#line 285 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						STRING_TYPE,
						pmSeSt[pmStkP+1].id,
						0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 26: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 296 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 27: { /* <synonym_list> = <synonym>; */
#line 303 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, LIST_SYN);
    	break;}
    case 28: { /* <synonym_list> = <synonym_list> <synonym>; */
#line 308 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, LIST_SYN);
    	break;}
    case 29: { /* <synonym> = <id_list> '=' ID '.'; */
#line 315 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].idList,
			      pmSeSt[pmStkP+3].id);
    	break;}
    case 30: { /* <messages> = 'MESSAGE' <message_list>; */
#line 325 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 31: { /* <message_list> = <message>; */
#line 332 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, LIST_MSG);
    	break;}
    case 32: { /* <message_list> = <message_list> <message>; */
#line 337 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, LIST_MSG);
    	break;}
    case 33: { /* <message> = ID ':' <statements>; */
#line 344 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 34: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 354 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 35: { /* <syntax_list> = <syntax_item>; */
#line 361 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, LIST_STX);
    	break;}
    case 36: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 366 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, LIST_STX);
    	break;}
    case 37: { /* <syntax_item> = ID '=' <syntax_elements> <optional_class_restrictions>; */
#line 373 "alan.pmk"

	pmSeSt[pmStkP+1].stx = newstx(&pmSySt[pmStkP+2].srcp,
				  pmSeSt[pmStkP+1].id,
				  concat(pmSeSt[pmStkP+3].elms,
				         newelm(&pmSeSt[pmStkP+1].id->srcp, END_OF_SYNTAX,
				                NULL,
				                FALSE),
				         LIST_ELM),
				  pmSeSt[pmStkP+4].ress);
    	break;}
    case 38: { /* <syntax_elements> = <syntax_element>; */
#line 387 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, LIST_ELM);
    	break;}
    case 39: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 392 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                LIST_ELM);
    	break;}
    case 40: { /* <syntax_element> = ID; */
#line 401 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+1].id->srcp, WORD_ELEMENT,
				     pmSeSt[pmStkP+1].id,
				     FALSE);
    	break;}
    case 41: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 409 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+2].id->srcp, PARAMETER_ELEMENT,
				     pmSeSt[pmStkP+2].id,
				     pmSeSt[pmStkP+4].bits);
    	break;}
    case 42: { /* <optional_indicators> =; */
#line 418 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 43: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 423 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 44: { /* <indicator> = '*'; */
#line 430 "alan.pmk"

	pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 45: { /* <indicator> = '!'; */
#line 435 "alan.pmk"

	pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 46: { /* <optional_class_restrictions> = '.'; */
#line 442 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 47: { /* <optional_class_restrictions> = 'WHERE' <class_restriction_clauses>; */
#line 447 "alan.pmk"

	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 48: { /* <class_restriction_clauses> = <class_restriction>; */
#line 461 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, LIST_RES);
    	break;}
    case 49: { /* <class_restriction_clauses> = <class_restriction_clauses> 'AND' <class_restriction>; */
#line 466 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, LIST_RES);
    	break;}
    case 50: { /* <class_restriction> = ID 'ISA' <class_id> 'ELSE' <statements>; */
#line 474 "alan.pmk"

	pmSeSt[pmStkP+1].res = newres(&pmSySt[pmStkP+2].srcp,
			  		pmSeSt[pmStkP+1].id,
					pmSeSt[pmStkP+3].id,
				    	pmSeSt[pmStkP+5].stms);
    	break;}
    case 51: { /* <class_id> = ID; */
#line 484 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 52: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 493 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newvrb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 53: { /* <verb_header> = 'VERB' <id_list>; */
#line 506 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 54: { /* <verb_body> = <simple_verb_body>; */
#line 515 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&nulsrcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   LIST_ALT);
    	break;}
    case 55: { /* <verb_body> = <verb_alternatives>; */
#line 526 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 56: { /* <verb_alternatives> = <verb_alternative>; */
#line 532 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, LIST_ALT);
    	break;}
    case 57: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 537 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				LIST_ALT);
    	break;}
    case 58: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 546 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 59: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 557 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 60: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */
#line 566 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 61: { /* <optional_checks> =; */
#line 574 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
    	break;}
    case 62: { /* <optional_checks> = 'CHECK' <statements>; */
#line 579 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), LIST_STM);
    	break;}
    case 63: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 584 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 64: { /* <check_list> = <check>; */
#line 591 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, LIST_CHK);
    	break;}
    case 65: { /* <check_list> = <check_list> 'AND' <check>; */
#line 596 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, LIST_CHK);
    	break;}
    case 66: { /* <check> = <expression> 'ELSE' <statements>; */
#line 603 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 67: { /* <optional_does> =; */
#line 611 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 69: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 622 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 70: { /* <class> = 'EVERY' ID <optional_heritage> <slots> <class_tail>; */
#line 633 "alan.pmk"

	pmSeSt[pmStkP+1].cla = newClass(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newSlots(pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].dscr,
					pmSeSt[pmStkP+4].ment,
					pmSeSt[pmStkP+4].art,
					pmSeSt[pmStkP+4].does,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 201, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 71: { /* <class_tail> = 'END' 'EVERY' <optional_id> __genSym#0; */
#line 656 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 74: { /* <instance> = 'THE' ID <optional_heritage> <slots> <instance tail>; */
#line 663 "alan.pmk"

	pmSeSt[pmStkP+1].ins = newInstance(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newSlots(pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].dscr,
					pmSeSt[pmStkP+4].ment,
					pmSeSt[pmStkP+4].art,
					pmSeSt[pmStkP+4].does,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 201, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 75: { /* <instance tail> = 'END' 'THE' <optional_id> __genSym#1; */
#line 686 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 78: { /* <optional_heritage> =; */
#line 691 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 79: { /* <optional_heritage> = <heritage>; */
#line 696 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 80: { /* <heritage> = 'ISA' ID __genSym#2; */
#line 703 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 83: { /* <slots> =; */
#line 708 "alan.pmk"

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
    case 84: { /* <slots> = <slots> __genSym#3 <slot>; */
#line 722 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+3].nam, LIST_NAM);

        if (pmSeSt[pmStkP+3].whr != NULL) {
            if (pmSeSt[pmStkP+1].whr != NULL) 
                /* WHERE clause already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "WHERE", "class/instance", NULL);
	    else
	        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+3].whr;
        }

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);

	if (pmSeSt[pmStkP+3].cnt != NULL) {
            if (pmSeSt[pmStkP+1].cnt != NULL)
                /* CONTAINER properties already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "CONTAINER properties", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+3].cnt;
        }

	if (pmSeSt[pmStkP+3].dscr != NULL) {
            if(pmSeSt[pmStkP+1].dscr != NULL)
                /* DESCRIPTION already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "DESCRIPTION", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].dscr = pmSeSt[pmStkP+3].dscr;
        }

	if (pmSeSt[pmStkP+3].ment != NULL) {
            if (pmSeSt[pmStkP+1].ment != NULL)
                /* MENTIONED already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "MENTIONED", "class/instance", NULL);
	    else
                pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+3].ment;
        }

	if (pmSeSt[pmStkP+3].art != NULL) {
            if (pmSeSt[pmStkP+1].art != NULL)
                /* ARTICLE already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "ARTICLE", "class/instance", NULL);
            else 
	        pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+3].art;
        }

	if (pmSeSt[pmStkP+3].does != NULL) {
            if (pmSeSt[pmStkP+1].does != NULL)
                /* DOES/ENTERED already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "DOES/ENTERED", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].does = pmSeSt[pmStkP+3].does;
        }

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+3].ext, LIST_EXT);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+3].vrb, LIST_VRB);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+3].scr, LIST_SCR);
    	break;}
    case 91: { /* <slot> = <name>; */
#line 784 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 87: { /* <slot> = <where> __genSym#4; */
#line 790 "alan.pmk"
{
        WhrNod *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 94: { /* <slot> = <is> <attributes>; */
#line 800 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 88: { /* <slot> = <container properties>; */
#line 806 "alan.pmk"
{
        CntNod *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 89: { /* <slot> = <description>; */
#line 816 "alan.pmk"
{
        List *dscr = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].dscr = dscr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 92: { /* <slot> = <mentioned>; */
#line 826 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 90: { /* <slot> = <article>; */
#line 836 "alan.pmk"
{ List *art = pmSeSt[pmStkP+1].art;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].art = art;
    }	break;}
    case 93: { /* <slot> = <does>; */
#line 842 "alan.pmk"
{
        List *does = pmSeSt[pmStkP+1].does;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].does = does;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 95: { /* <slot> = <exit>; */
#line 852 "alan.pmk"
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 96: { /* <slot> = <verb>; */
#line 858 "alan.pmk"
{ VrbNod *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 97: { /* <slot> = <script>; */
#line 864 "alan.pmk"
{ ScrNod *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 100: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 874 "alan.pmk"

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
    case 101: { /* <optional_exit_body> =; */
#line 889 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 102: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 896 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 103: { /* <optional_attributes> =; */
#line 911 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 104: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 916 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 105: { /* <is> = 'IS'; */
#line 923 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 106: { /* <is> = 'ARE'; */
#line 928 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 107: { /* <is> = 'HAS'; */
#line 933 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 108: { /* <optional_description> =; */
#line 940 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 110: { /* <description> = 'DESCRIPTION'; */
#line 950 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), LIST_STM);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 111: { /* <description> = 'DESCRIPTION' <statements>; */
#line 957 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 112: { /* <article> = 'ARTICLE'; */
#line 965 "alan.pmk"

	pmSeSt[pmStkP+1].art = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), LIST_STM);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 113: { /* <article> = 'ARTICLE' <statements>; */
#line 972 "alan.pmk"

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 114: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 981 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 115: { /* <optional_names> =; */
#line 990 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 116: { /* <optional_names> = <optional_names> <name>; */
#line 995 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					pmSeSt[pmStkP+2].nam, LIST_NAM);
    	break;}
    case 117: { /* <name> = 'NAME' <ids>; */
#line 1003 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 118: { /* <container properties> = 'CONTAINER' <container_body>; */
#line 1012 "alan.pmk"

#ifndef FIXME
	syserr("UNIMPL: parser - newcnt");
#else
	pmSeSt[pmStkP+1].cnt = newcnt(&pmSySt[pmStkP+1].srcp,
					  NULL,
					  pmSeSt[pmStkP+2].lims,
					  pmSeSt[pmStkP+2].stms,
					  pmSeSt[pmStkP+2].stms2);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, LIST_CNT);
#endif
    	break;}
    case 119: { /* <container> = <container_header> <container_body> <container_tail>; */
#line 1030 "alan.pmk"

#ifndef FIXME
	syserr("UNIMPL: parser - newcnt");
#else
	pmSeSt[pmStkP+1].cnt = newcnt(&pmSeSt[pmStkP+1].srcp,
				pmSeSt[pmStkP+1].id,
				pmSeSt[pmStkP+2].lims,
				pmSeSt[pmStkP+2].stms,
				pmSeSt[pmStkP+2].stms2);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
#endif
    	break;}
    case 120: { /* <container_header> = 'CONTAINER' ID; */
#line 1049 "alan.pmk"

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevINF, "THE <id> ISA CONTAINER ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 121: { /* <container_body> = <optional_limits> <optional_header> <optional_empty>; */
#line 1058 "alan.pmk"

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
    	break;}
    case 122: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */
#line 1067 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+3].id;
    	break;}
    case 123: { /* <optional_limits> =; */
#line 1074 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 124: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1079 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 125: { /* <limits> = <limit>; */
#line 1086 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIST_LIM);
    	break;}
    case 126: { /* <limits> = <limits> <limit>; */
#line 1091 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIST_LIM);
    	break;}
    case 127: { /* <limit> = <limit_attribute> 'THEN' <statements>; */
#line 1098 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 128: { /* <limit_attribute> = <attribute definition>; */
#line 1107 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 129: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1112 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSySt[pmStkP+1].srcp,
				      INTEGER_TYPE,
				      newId(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 130: { /* <optional_header> =; */
#line 1122 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 131: { /* <optional_header> = 'HEADER' <statements>; */
#line 1127 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 132: { /* <optional_empty> =; */
#line 1134 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 133: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1139 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 134: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1147 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 135: { /* <event_header> = 'EVENT' ID; */
#line 1160 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 136: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1168 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 137: { /* <script> = 'SCRIPT' <integer_or_id> '.' <optional_description> <step_list>; */
#line 1176 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newscr(&pmSySt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+2].id,
			     pmSeSt[pmStkP+2].val,
			     pmSeSt[pmStkP+4].stms,
			     pmSeSt[pmStkP+5].stps);
    	break;}
    case 138: { /* <step_list> = <step>; */
#line 1187 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, LIST_STP);
    	break;}
    case 139: { /* <step_list> = <step_list> <step>; */
#line 1192 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, LIST_STP);
    	break;}
    case 140: { /* <step> = 'STEP' <statements>; */
#line 1199 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 141: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1207 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 142: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1215 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 143: { /* <rule> = 'WHEN' <expression> '=>' <statements>; */
#line 1226 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 144: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1236 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 145: { /* <optional_statements> =; */
#line 1245 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 146: { /* <optional_statements> = <statements>; */
#line 1250 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 147: { /* <statements> = <statement>; */
#line 1257 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, LIST_STM);
    	break;}
    case 148: { /* <statements> = <statements> <statement>; */
#line 1262 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, LIST_STM);
    	break;}
    case 149: { /* <statement> = <output_statement>; */
#line 1269 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 151: { /* <statement> = <manipulation_statement>; */
#line 1274 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 153: { /* <statement> = <assignment_statement>; */
#line 1279 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 155: { /* <statement> = <comparison_statement>; */
#line 1284 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 154: { /* <statement> = <actor_statement>; */
#line 1289 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 152: { /* <statement> = <event_statement>; */
#line 1294 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 150: { /* <statement> = <special_statement>; */
#line 1299 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 156: { /* <output_statement> = STRING; */
#line 1307 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_PRINT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 157: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1314 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DESCRIBE);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 158: { /* <output_statement> = 'SAY' <expression> '.'; */
#line 1320 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAY);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 159: { /* <output_statement> = 'LIST' <what> '.'; */
#line 1326 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LIST);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 160: { /* <special_statement> = 'QUIT' '.'; */
#line 1334 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_QUIT);
    	break;}
    case 161: { /* <special_statement> = 'LOOK' '.'; */
#line 1339 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOOK);
    	break;}
    case 162: { /* <special_statement> = 'SAVE' '.'; */
#line 1344 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAVE);
    	break;}
    case 163: { /* <special_statement> = 'RESTORE' '.'; */
#line 1349 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTORE);
    	break;}
    case 164: { /* <special_statement> = 'RESTART' '.'; */
#line 1354 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTART);
    	break;}
    case 165: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 1359 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCORE);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 166: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 1370 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_VISITS);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 167: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 1376 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SYSTEM);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 168: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */
#line 1387 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_EMPTY);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 169: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */
#line 1394 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOCATE);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 171: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1405 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCHEDULE);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 170: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1413 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_CANCEL);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 172: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */
#line 1423 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_MAKE);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 175: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1431 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SET);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 173: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1439 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_INCR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 174: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1447 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DECR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 176: { /* <optional_by_clause> =; */
#line 1457 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 177: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1462 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 178: { /* <comparison_statement> = <if_statement>; */
#line 1471 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 179: { /* <comparison_statement> = <depending_statement>; */
#line 1476 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 180: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 1484 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_IF);
	pmSeSt[pmStkP+1].stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	if (pmSeSt[pmStkP+5].stms == NULL)
	    pmSeSt[pmStkP+1].stm->fields.iff.els = pmSeSt[pmStkP+6].stms;
	else {
	    /* Connect the else_part to the elsif_list */
	    pmSeSt[pmStkP+5].stms->tail->element.stm->fields.iff.els =
			pmSeSt[pmStkP+6].stms;
	    pmSeSt[pmStkP+5].stms->next = NULL;
	    pmSeSt[pmStkP+1].stm->fields.iff.els = pmSeSt[pmStkP+5].stms;
	}
    	break;}
    case 181: { /* <optional_elsif_list> =; */
#line 1502 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 182: { /* <optional_elsif_list> = <elsif_list>; */
#line 1507 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 183: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 1514 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_IF);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, LIST_STM);
    }	break;}
    case 184: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 1527 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+2].srcp, STM_IF);
	stm->fields.iff.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+5].stms;
	stm->fields.iff.els = NULL;

	/* Now connect it to the previous ELSIFs, I.e connect the */
	/* else-part of the last element in the elsif_list to this */
	/* IF-statement and also concat this element to the list so */
	/* we'll know were the end is next time */
	pmSeSt[pmStkP+1].stms->tail->element.stm->fields.iff.els = concat(NULL, stm, LIST_STM);
	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, stm, LIST_STM);
    }	break;}
    case 185: { /* <optional_else_part> =; */
#line 1546 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 186: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 1551 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 187: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' 'DEPEND' '.'; */
#line 1558 "alan.pmk"
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPEND);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 188: { /* <depend_cases> = <depend_case>; */
#line 1569 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, LIST_CASE);
    	break;}
    case 189: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1574 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, LIST_CASE);
    	break;}
    case 191: { /* <depend_case> = <right_hand_side> ':' <statements>; */
#line 1581 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 190: { /* <depend_case> = 'ELSE' <statements>; */
#line 1588 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 192: { /* <actor_statement> = 'USE' 'SCRIPT' <integer_or_id> <optional_for_actor> '.'; */
#line 1598 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_USE);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.scriptno = pmSeSt[pmStkP+3].val;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 193: { /* <optional_for_actor> =; */
#line 1608 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 194: { /* <optional_for_actor> = 'FOR' ID; */
#line 1613 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 195: { /* <expression> = <term>; */
#line 1622 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 196: { /* <expression> = <expression> 'OR' <term>; */
#line 1627 "alan.pmk"
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_OR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 197: { /* <term> = <factor>; */
#line 1639 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 198: { /* <term> = <term> 'AND' <factor>; */
#line 1644 "alan.pmk"
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_AND;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 199: { /* <factor> = <primary>; */
#line 1656 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 200: { /* <factor> = <primary> <right_hand_side>; */
#line 1661 "alan.pmk"

        /* <right_hand_side> contains the top expr, hang in the <primary> and
           propagate that node */
        switch (pmSeSt[pmStkP+2].expKd) {
	case EXPBIN: pmSeSt[pmStkP+2].exp->fields.bin.left = pmSeSt[pmStkP+1].exp; break;
	case EXPWHR: pmSeSt[pmStkP+2].exp->fields.whr.wht = pmSeSt[pmStkP+1].exp; break;
	case EXPATR: pmSeSt[pmStkP+2].exp->fields.atr.wht = pmSeSt[pmStkP+1].exp; break;
	case EXPBTW: pmSeSt[pmStkP+2].exp->fields.btw.val = pmSeSt[pmStkP+1].exp; break;
	case EXPISA: pmSeSt[pmStkP+2].exp->fields.isa.id = pmSeSt[pmStkP+1].id; break;
	default: syserr("Unrecognized switch in <right_hand_side> semantic rule.");
	}
        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 202: { /* <right_hand_side> = <binop> <primary>; */
#line 1678 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPBIN);
	exp->fields.bin.op = pmSeSt[pmStkP+1].op;
	exp->fields.bin.right = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 203: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 1687 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 201: { /* <right_hand_side> = <optional_not> <where>; */
#line 1697 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPWHR;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPWHR);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 204: { /* <right_hand_side> = <optional_not> 'ISA' ID; */
#line 1706 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPISA;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPISA);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 205: { /* <right_hand_side> = <is> <something>; */
#line 1715 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPATR;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 207: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 1724 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBTW;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBTW);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 206: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 1734 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = OP_CONTAINS;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 213: { /* <primary> = '(' <expression> ')'; */
#line 1747 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 208: { /* <primary> = <optional_minus> Integer; */
#line 1752 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, EXPINT);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 209: { /* <primary> = STRING; */
#line 1761 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSTR);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 210: { /* <primary> = <what>; */
#line 1768 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 214: { /* <primary> = <attribute_reference>; */
#line 1774 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 212: { /* <primary> = <aggregate> <where>; */
#line 1782 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPAGR);
	pmSeSt[pmStkP+1].exp->fields.agr.agr	= pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.whr	= pmSeSt[pmStkP+2].whr;
    	break;}
    case 215: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 1790 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPRND);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 211: { /* <primary> = 'SCORE'; */
#line 1797 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSCORE);
    	break;}
    case 217: { /* <aggregate> = 'SUM' 'OF' ID; */
#line 1805 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_SUM;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 218: { /* <aggregate> = 'MAX' 'OF' ID; */
#line 1812 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_MAX;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 216: { /* <aggregate> = 'COUNT'; */
#line 1819 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_COUNT;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 219: { /* <something> = <optional_not> ID; */
#line 1829 "alan.pmk"

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 220: { /* <what> = ID; */
#line 1837 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSeSt[pmStkP+1].id->srcp, WHT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 221: { /* <optional_where> =; */
#line 1846 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 222: { /* <optional_where> = <where>; */
#line 1851 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 223: { /* <where> = 'HERE'; */
#line 1858 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 224: { /* <where> = 'NEARBY'; */
#line 1864 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 225: { /* <where> = 'AT' <what>; */
#line 1870 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 226: { /* <where> = 'IN' <what>; */
#line 1876 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 227: { /* <binop> = '+'; */
#line 1884 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_PLUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 228: { /* <binop> = '-'; */
#line 1890 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_MINUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 229: { /* <binop> = '*'; */
#line 1896 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_MULT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 230: { /* <binop> = '/'; */
#line 1902 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_DIV;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 231: { /* <relop> = '<>'; */
#line 1911 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_NE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 232: { /* <relop> = '='; */
#line 1917 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_EQ;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 233: { /* <relop> = '=='; */
#line 1923 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_EXACT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 234: { /* <relop> = '>='; */
#line 1929 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_GE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 235: { /* <relop> = '<='; */
#line 1935 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_LE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 236: { /* <relop> = '>'; */
#line 1941 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_GT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 237: { /* <relop> = '<'; */
#line 1947 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_LT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 238: { /* <optional_qual> =; */
#line 1957 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 239: { /* <optional_qual> = 'BEFORE'; */
#line 1962 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 240: { /* <optional_qual> = 'AFTER'; */
#line 1968 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 241: { /* <optional_qual> = 'ONLY'; */
#line 1974 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 242: { /* <optional_not> =; */
#line 1983 "alan.pmk"

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 243: { /* <optional_not> = 'NOT'; */
#line 1988 "alan.pmk"

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 244: { /* <optional_id> =; */
#line 1995 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 245: { /* <optional_id> = ID; */
#line 2000 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 246: { /* <ids> = ID; */
#line 2007 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
    	break;}
    case 247: { /* <ids> = <ids> ID; */
#line 2012 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, LIST_ID);
    	break;}
    case 248: { /* <id_list> = ID; */
#line 2019 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
    	break;}
    case 249: { /* <id_list> = <id_list> ',' ID; */
#line 2025 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, LIST_ID);
    	break;}
    case 250: { /* <optional_integer> =; */
#line 2033 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 251: { /* <optional_integer> = Integer; */
#line 2038 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 252: { /* <optional_minus> =; */
#line 2045 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 253: { /* <optional_minus> = '-'; */
#line 2050 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 254: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2057 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 255: { /* <integer_or_id> = Integer; */
#line 2066 "alan.pmk"

	pmSeSt[pmStkP+1].val = val(pmSySt[pmStkP+1].chars);
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 256: { /* <integer_or_id> = ID; */
#line 2072 "alan.pmk"

	pmSeSt[pmStkP+1].val = 0;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 257: { /* ID = Identifier; */
#line 2080 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    default: break; }
}/*pmPaSema()*/

