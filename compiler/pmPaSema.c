/*----------------------------------------------------------------------*\

	pmPaSema.c

	ParserMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


#include "util.h"

#include "types.h"
#include "smScan.h"
#include "token.h"

#include "id_x.h"
#include "ins_x.h"
#include "cla_x.h"
#include "sym_x.h"
#include "res_x.h"
#include "whr_x.h"

#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "alt.h"                /* ALT-nodes */
#include "atr.h"                /* ATR-nodes */
#include "chk.h"                /* CHK-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "elm.h"                /* ELM-nodes */
#include "evt.h"                /* EVT-nodes */
#include "exp.h"                /* EXP-nodes */
#include "ext.h"                /* EXT-nodes */
#include "lim.h"		/* LIM-nodes */
#include "lst.h"		/* LST-nodes */
#include "msg.h"                /* MSG-nodes */
#include "opt.h"		/* OPTIONS */
#include "rul.h"                /* RUL-nodes */
#include "sco.h"                /* SCORES */
#include "scr.h"                /* SCR-nodes */
#include "slt.h"                /* SLOTS-nodes */
#include "stm.h"		/* STM-nodes */
#include "stp.h"                /* STP-nodes */
#include "str.h"		/* STRINGS */
#include "stx.h"                /* STX-nodes */
#include "syn.h"                /* SYN-nodes */
#include "vrb.h"                /* VRB-nodes */
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
    Bool single;
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

#line 77 "alan.pmk"


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
#line 169 "alan.pmk"
    switch (rule) {
    case 6: { /* <option> = ID '.'; */
#line 189 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */
#line 194 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */
#line 199 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 13: { /* <unit> = <synonyms>; */
#line 212 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns); 	break;}
    case 11: { /* <unit> = <messages>; */
#line 215 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 14: { /* <unit> = <syntax>; */
#line 218 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 15: { /* <unit> = <verb>; */
#line 221 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, LIST_VRB); 	break;}
    case 16: { /* <unit> = <class>; */
#line 224 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, LIST_CLA); 	break;}
    case 17: { /* <unit> = <instance>; */
#line 227 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, LIST_INS); 	break;}
    case 19: { /* <unit> = <event>; */
#line 230 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, LIST_EVT); 	break;}
    case 18: { /* <unit> = <container>; */
#line 233 "alan.pmk"
 adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, LIST_CNT); 	break;}
    case 12: { /* <unit> = <rule>; */
#line 236 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, LIST_RUL); 	break;}
    case 20: { /* <attributes> = <attribute definition> '.'; */
#line 242 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, LIST_ATR);
    	break;}
    case 21: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 247 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, LIST_ATR);
    	break;}
    case 22: { /* <attribute definition> = ID; */
#line 254 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						TYPBOOL,
						pmSeSt[pmStkP+1].id,
						TRUE, 0, 0);
    	break;}
    case 23: { /* <attribute definition> = 'NOT' ID; */
#line 262 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+2].id->srcp,
						TYPBOOL,
						pmSeSt[pmStkP+2].id,
						FALSE, 0, 0);
    	break;}
    case 24: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 270 "alan.pmk"

	if (pmSeSt[pmStkP+2].minus)
		pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
							TYPINT,
							pmSeSt[pmStkP+1].id,
							-val(pmSySt[pmStkP+3].chars), 0, 0);
	else
		pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
							TYPINT,
							pmSeSt[pmStkP+1].id,
							val(pmSySt[pmStkP+3].chars), 0, 0);
    	break;}
    case 25: { /* <attribute definition> = ID STRING; */
#line 284 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						TYPSTR,
						pmSeSt[pmStkP+1].id,
						0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 26: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 295 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 27: { /* <synonym_list> = <synonym>; */
#line 302 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, LIST_SYN);
    	break;}
    case 28: { /* <synonym_list> = <synonym_list> <synonym>; */
#line 307 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, LIST_SYN);
    	break;}
    case 29: { /* <synonym> = <id_list> '=' ID '.'; */
#line 314 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].idList,
			      pmSeSt[pmStkP+3].id);
    	break;}
    case 30: { /* <messages> = 'MESSAGE' <message_list>; */
#line 324 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 31: { /* <message_list> = <message>; */
#line 331 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, LIST_MSG);
    	break;}
    case 32: { /* <message_list> = <message_list> <message>; */
#line 336 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, LIST_MSG);
    	break;}
    case 33: { /* <message> = ID ':' <statements>; */
#line 343 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 34: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 353 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 35: { /* <syntax_list> = <syntax_item>; */
#line 360 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, LIST_STX);
    	break;}
    case 36: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 365 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, LIST_STX);
    	break;}
    case 37: { /* <syntax_item> = ID '=' <syntax_elements> <optional_class_restrictions>; */
#line 372 "alan.pmk"

	pmSeSt[pmStkP+1].stx = newstx(&pmSySt[pmStkP+2].srcp,
				  pmSeSt[pmStkP+1].id,
				  concat(pmSeSt[pmStkP+3].elms,
				         newelm(&pmSeSt[pmStkP+1].id->srcp, ELMEOS,
				                NULL,
				                FALSE),
				         LIST_ELM),
				  pmSeSt[pmStkP+4].ress);
    	break;}
    case 38: { /* <syntax_elements> = <syntax_element>; */
#line 386 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, LIST_ELM);
    	break;}
    case 39: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 391 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                LIST_ELM);
    	break;}
    case 40: { /* <syntax_element> = ID; */
#line 400 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+1].id->srcp, ELMWRD,
				     pmSeSt[pmStkP+1].id,
				     FALSE);
    	break;}
    case 41: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 408 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+2].id->srcp, ELMPAR,
				     pmSeSt[pmStkP+2].id,
				     pmSeSt[pmStkP+4].bits);
    	break;}
    case 42: { /* <optional_indicators> =; */
#line 417 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 43: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 422 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 44: { /* <indicator> = '*'; */
#line 429 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0x1;
    	break;}
    case 45: { /* <indicator> = '!'; */
#line 434 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0x2;
    	break;}
    case 46: { /* <optional_class_restrictions> = '.'; */
#line 441 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 47: { /* <optional_class_restrictions> = 'WHERE' <class_restriction_clauses>; */
#line 446 "alan.pmk"

	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 48: { /* <class_restriction_clauses> = <class_restriction>; */
#line 459 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, LIST_RES);
    	break;}
    case 49: { /* <class_restriction_clauses> = <class_restriction_clauses> 'AND' <class_restriction>; */
#line 464 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, LIST_RES);
    	break;}
    case 50: { /* <class_restriction> = ID 'ISA' <classes> 'ELSE' <statements>; */
#line 472 "alan.pmk"

	pmSeSt[pmStkP+1].res = newres(&pmSySt[pmStkP+2].srcp,
			  		pmSeSt[pmStkP+1].id,
					pmSeSt[pmStkP+3].single,
					pmSeSt[pmStkP+3].idList,
				    	pmSeSt[pmStkP+5].stms);
    	break;}
    case 51: { /* <classes> = <class_id>; */
#line 483 "alan.pmk"

	pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
	pmSeSt[pmStkP+1].single = TRUE;
    	break;}
    case 52: { /* <classes> = <classes> 'OR' <class_id>; */
#line 489 "alan.pmk"

	pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, LIST_ID);
	pmSeSt[pmStkP+1].single = FALSE;
    	break;}
    case 53: { /* <class_id> = <heritage_id>; */
#line 497 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 54: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 506 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newvrb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 55: { /* <verb_header> = 'VERB' <id_list>; */
#line 519 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 56: { /* <verb_body> = <simple_verb_body>; */
#line 528 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&nulsrcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   LIST_ALT);
    	break;}
    case 57: { /* <verb_body> = <verb_alternatives>; */
#line 539 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 58: { /* <verb_alternatives> = <verb_alternative>; */
#line 545 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, LIST_ALT);
    	break;}
    case 59: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 550 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				LIST_ALT);
    	break;}
    case 60: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 559 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 61: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 570 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 62: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */
#line 579 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 63: { /* <optional_checks> =; */
#line 587 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
    	break;}
    case 64: { /* <optional_checks> = 'CHECK' <statements>; */
#line 592 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), LIST_STM);
    	break;}
    case 65: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 597 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 66: { /* <check_list> = <check>; */
#line 604 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, LIST_CHK);
    	break;}
    case 67: { /* <check_list> = <check_list> 'AND' <check>; */
#line 609 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, LIST_CHK);
    	break;}
    case 68: { /* <check> = <expression> 'ELSE' <statements>; */
#line 616 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 69: { /* <optional_does> =; */
#line 624 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 71: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 635 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 72: { /* <class> = 'EVERY' ID <optional_heritage> <slots> <class_tail>; */
#line 646 "alan.pmk"

	pmSeSt[pmStkP+1].cla = newcla(&pmSySt[pmStkP+1].srcp,
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
    case 73: { /* <class_tail> = 'END' 'EVERY' <optional_id> '.'; */
#line 669 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 74: { /* <instance> = 'THE' ID <optional_heritage> <slots> <instance tail>; */
#line 676 "alan.pmk"

	pmSeSt[pmStkP+1].ins = newins(&pmSySt[pmStkP+1].srcp,
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
    case 75: { /* <instance tail> = 'END' 'THE' <optional_id> '.'; */
#line 699 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 76: { /* <optional_heritage> =; */
#line 704 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 77: { /* <optional_heritage> = <heritage>; */
#line 709 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 78: { /* <heritage> = 'ISA' <heritage_id> __genSym#0; */
#line 716 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 81: { /* <heritage_id> = ID; */
#line 721 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id; 	break;}
    case 82: { /* <slots> =; */
#line 727 "alan.pmk"

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
    case 83: { /* <slots> = <slots> <slot>; */
#line 741 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+2].nam, LIST_NAM);

	if (pmSeSt[pmStkP+1].whr != NULL)
            /* WHERE clause already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "WHERE", "class/instance", NULL);
	else 
	    pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+2].whr;

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);

	if (pmSeSt[pmStkP+1].cnt != NULL)
            /* CONTAINER properties already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "CONTAINER properties", "class/instance", NULL);
	else 
	    pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+2].cnt;

	if (pmSeSt[pmStkP+1].dscr != NULL)
            /* DESCRIPTION already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "class/instance", NULL);
	else 
	    pmSeSt[pmStkP+1].dscr = pmSeSt[pmStkP+2].dscr;

	if (pmSeSt[pmStkP+1].ment != NULL)
            /* MENTIONED already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "MENTIONED", "class/instance", NULL);
	else 
	    pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+2].ment;

	if (pmSeSt[pmStkP+1].art != NULL)
            /* ARTICLE already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "ARTICLE", "class/instance", NULL);
	else 
	    pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].art;

	if (pmSeSt[pmStkP+1].does != NULL)
            /* DOES/ENTERED already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DOES/ENTERED", "class/instance", NULL);
	else 
	    pmSeSt[pmStkP+1].does = pmSeSt[pmStkP+2].does;

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, LIST_EXT);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, LIST_VRB);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, LIST_SCR);
    	break;}
    case 88: { /* <slot> = <name>; */
#line 791 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 84: { /* <slot> = <where>; */
#line 797 "alan.pmk"
{ WhrNod *whr = pmSeSt[pmStkP+1].whr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
    }	break;}
    case 91: { /* <slot> = <is> <attributes>; */
#line 803 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 85: { /* <slot> = <container properties>; */
#line 809 "alan.pmk"
{ CntNod *cnt = pmSeSt[pmStkP+1].cnt;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
    }	break;}
    case 86: { /* <slot> = <description>; */
#line 815 "alan.pmk"
{ List *dscr = pmSeSt[pmStkP+1].stms;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].dscr = dscr;
    }	break;}
    case 89: { /* <slot> = <mentioned>; */
#line 821 "alan.pmk"
{ List *ment = pmSeSt[pmStkP+1].ment;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = ment;
    }	break;}
    case 87: { /* <slot> = <article>; */
#line 827 "alan.pmk"
{ List *art = pmSeSt[pmStkP+1].art;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].art = art;
    }	break;}
    case 90: { /* <slot> = <does>; */
#line 833 "alan.pmk"
{ List *does = pmSeSt[pmStkP+1].does;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].does = does;
    }	break;}
    case 92: { /* <slot> = <exit>; */
#line 839 "alan.pmk"
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 93: { /* <slot> = <verb>; */
#line 845 "alan.pmk"
{ VrbNod *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 94: { /* <slot> = <script>; */
#line 851 "alan.pmk"
{ ScrNod *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 95: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 861 "alan.pmk"

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
    case 96: { /* <optional_exit_body> =; */
#line 876 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 97: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 883 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 98: { /* <optional_attributes> =; */
#line 898 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 99: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 903 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 100: { /* <is> = 'IS'; */
#line 910 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 101: { /* <is> = 'ARE'; */
#line 915 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 102: { /* <is> = 'HAS'; */
#line 920 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 103: { /* <optional_description> =; */
#line 927 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 105: { /* <description> = 'DESCRIPTION'; */
#line 937 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), LIST_STM);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 106: { /* <description> = 'DESCRIPTION' <statements>; */
#line 944 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 107: { /* <article> = 'ARTICLE'; */
#line 952 "alan.pmk"

	pmSeSt[pmStkP+1].art = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), LIST_STM);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 108: { /* <article> = 'ARTICLE' <statements>; */
#line 959 "alan.pmk"

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 109: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 968 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 110: { /* <optional_names> =; */
#line 977 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 111: { /* <optional_names> = <optional_names> <name>; */
#line 982 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					     pmSeSt[pmStkP+2].nam, LIST_NAM);
    	break;}
    case 112: { /* <name> = 'NAME' <ids>; */
#line 990 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 113: { /* <container properties> = 'CONTAINER' <container_body>; */
#line 999 "alan.pmk"

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
    case 114: { /* <container> = <container_header> <container_body> <container_tail>; */
#line 1017 "alan.pmk"

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
    case 115: { /* <container_header> = 'CONTAINER' ID; */
#line 1036 "alan.pmk"

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevINF, "THE <id> ISA CONTAINER ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 116: { /* <container_body> = <optional_limits> <optional_header> <optional_empty>; */
#line 1045 "alan.pmk"

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
    	break;}
    case 117: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */
#line 1054 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+3].id;
    	break;}
    case 118: { /* <optional_limits> =; */
#line 1061 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 119: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1066 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 120: { /* <limits> = <limit>; */
#line 1073 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIST_LIM);
    	break;}
    case 121: { /* <limits> = <limits> <limit>; */
#line 1078 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIST_LIM);
    	break;}
    case 122: { /* <limit> = <limit_attribute> 'THEN' <statements>; */
#line 1085 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 123: { /* <limit_attribute> = <attribute definition>; */
#line 1094 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 124: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1099 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSySt[pmStkP+1].srcp,
				      TYPINT,
				      newId(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 125: { /* <optional_header> =; */
#line 1109 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 126: { /* <optional_header> = 'HEADER' <statements>; */
#line 1114 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 127: { /* <optional_empty> =; */
#line 1121 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 128: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1126 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 129: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1134 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 130: { /* <event_header> = 'EVENT' ID; */
#line 1147 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 131: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1155 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 132: { /* <script> = 'SCRIPT' <integer_or_id> '.' <optional_description> <step_list>; */
#line 1163 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newscr(&pmSySt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+2].id,
			     pmSeSt[pmStkP+2].val,
			     pmSeSt[pmStkP+4].stms,
			     pmSeSt[pmStkP+5].stps);
    	break;}
    case 133: { /* <step_list> = <step>; */
#line 1174 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, LIST_STP);
    	break;}
    case 134: { /* <step_list> = <step_list> <step>; */
#line 1179 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, LIST_STP);
    	break;}
    case 135: { /* <step> = 'STEP' <statements>; */
#line 1186 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 136: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1194 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 137: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1202 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 138: { /* <rule> = 'WHEN' <expression> '=>' <statements>; */
#line 1213 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 139: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1223 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 140: { /* <optional_statements> =; */
#line 1232 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 141: { /* <optional_statements> = <statements>; */
#line 1237 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 142: { /* <statements> = <statement>; */
#line 1244 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, LIST_STM);
    	break;}
    case 143: { /* <statements> = <statements> <statement>; */
#line 1249 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, LIST_STM);
    	break;}
    case 144: { /* <statement> = <output_statement>; */
#line 1256 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 146: { /* <statement> = <manipulation_statement>; */
#line 1261 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 148: { /* <statement> = <assignment_statement>; */
#line 1266 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 150: { /* <statement> = <comparison_statement>; */
#line 1271 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 149: { /* <statement> = <actor_statement>; */
#line 1276 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 147: { /* <statement> = <event_statement>; */
#line 1281 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 145: { /* <statement> = <special_statement>; */
#line 1286 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 151: { /* <output_statement> = STRING; */
#line 1294 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_PRINT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 152: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1301 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DESCRIBE);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 153: { /* <output_statement> = 'SAY' <expression> '.'; */
#line 1307 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAY);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 154: { /* <output_statement> = 'LIST' <what> '.'; */
#line 1313 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LIST);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 155: { /* <special_statement> = 'QUIT' '.'; */
#line 1321 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_QUIT);
    	break;}
    case 156: { /* <special_statement> = 'LOOK' '.'; */
#line 1326 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOOK);
    	break;}
    case 157: { /* <special_statement> = 'SAVE' '.'; */
#line 1331 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAVE);
    	break;}
    case 158: { /* <special_statement> = 'RESTORE' '.'; */
#line 1336 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTORE);
    	break;}
    case 159: { /* <special_statement> = 'RESTART' '.'; */
#line 1341 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTART);
    	break;}
    case 160: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 1346 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCORE);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 161: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 1357 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_VISITS);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 162: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 1363 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SYSTEM);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 163: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */
#line 1374 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_EMPTY);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 164: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */
#line 1381 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOCATE);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 166: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1392 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCHEDULE);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 165: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1400 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_CANCEL);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 167: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */
#line 1410 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_MAKE);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 170: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1418 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SET);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 168: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1426 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_INCR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 169: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1434 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DECR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 171: { /* <optional_by_clause> =; */
#line 1444 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 172: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1449 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 173: { /* <comparison_statement> = <if_statement>; */
#line 1458 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 174: { /* <comparison_statement> = <depending_statement>; */
#line 1463 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 175: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 1471 "alan.pmk"

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
    case 176: { /* <optional_elsif_list> =; */
#line 1489 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 177: { /* <optional_elsif_list> = <elsif_list>; */
#line 1494 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 178: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 1501 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_IF);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, LIST_STM);
    }	break;}
    case 179: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 1514 "alan.pmk"
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
    case 180: { /* <optional_else_part> =; */
#line 1533 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 181: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 1538 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 182: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' 'DEPEND' '.'; */
#line 1545 "alan.pmk"
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPEND);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 183: { /* <depend_cases> = <depend_case>; */
#line 1556 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, LIST_CASE);
    	break;}
    case 184: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1561 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, LIST_CASE);
    	break;}
    case 186: { /* <depend_case> = <right_hand_side> ':' <statements>; */
#line 1568 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 185: { /* <depend_case> = 'ELSE' <statements>; */
#line 1575 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 187: { /* <actor_statement> = 'USE' 'SCRIPT' <integer_or_id> <optional_for_actor> '.'; */
#line 1585 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_USE);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.scriptno = pmSeSt[pmStkP+3].val;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 188: { /* <optional_for_actor> =; */
#line 1595 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 189: { /* <optional_for_actor> = 'FOR' ID; */
#line 1600 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 190: { /* <expression> = <term>; */
#line 1609 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 191: { /* <expression> = <expression> 'OR' <term>; */
#line 1614 "alan.pmk"
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_OR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 192: { /* <term> = <factor>; */
#line 1626 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 193: { /* <term> = <term> 'AND' <factor>; */
#line 1631 "alan.pmk"
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_AND;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 194: { /* <factor> = <primary>; */
#line 1643 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 195: { /* <factor> = <primary> <right_hand_side>; */
#line 1648 "alan.pmk"

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
    case 197: { /* <right_hand_side> = <binop> <primary>; */
#line 1665 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPBIN);
	exp->fields.bin.op = pmSeSt[pmStkP+1].op;
	exp->fields.bin.right = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 198: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 1674 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 196: { /* <right_hand_side> = <optional_not> <where>; */
#line 1684 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPWHR;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPWHR);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 199: { /* <right_hand_side> = <optional_not> 'ISA' <heritage_id>; */
#line 1693 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPISA;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPISA);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 200: { /* <right_hand_side> = <is> <something>; */
#line 1702 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPATR;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 202: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 1711 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBTW;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBTW);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 201: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 1721 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = OP_CONTAINS;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 208: { /* <primary> = '(' <expression> ')'; */
#line 1734 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 203: { /* <primary> = <optional_minus> Integer; */
#line 1739 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, EXPINT);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 204: { /* <primary> = STRING; */
#line 1748 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSTR);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 205: { /* <primary> = <what>; */
#line 1755 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 209: { /* <primary> = <attribute_reference>; */
#line 1761 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 207: { /* <primary> = <aggregate> <where>; */
#line 1769 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPAGR);
	pmSeSt[pmStkP+1].exp->fields.agr.agr	= pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.whr	= pmSeSt[pmStkP+2].whr;
    	break;}
    case 210: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 1777 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPRND);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 206: { /* <primary> = 'SCORE'; */
#line 1784 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSCORE);
    	break;}
    case 212: { /* <aggregate> = 'SUM' 'OF' ID; */
#line 1792 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_SUM;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 213: { /* <aggregate> = 'MAX' 'OF' ID; */
#line 1799 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_MAX;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 211: { /* <aggregate> = 'COUNT'; */
#line 1806 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_COUNT;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 214: { /* <something> = <optional_not> ID; */
#line 1816 "alan.pmk"

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 215: { /* <what> = ID; */
#line 1824 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSeSt[pmStkP+1].id->srcp, WHT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 216: { /* <optional_where> =; */
#line 1833 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 217: { /* <optional_where> = <where>; */
#line 1838 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 218: { /* <where> = 'HERE'; */
#line 1845 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 219: { /* <where> = 'NEARBY'; */
#line 1851 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 220: { /* <where> = 'AT' <what>; */
#line 1857 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 221: { /* <where> = 'IN' <what>; */
#line 1863 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 222: { /* <binop> = '+'; */
#line 1871 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_PLUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 223: { /* <binop> = '-'; */
#line 1877 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_MINUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 224: { /* <binop> = '*'; */
#line 1883 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_MULT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 225: { /* <binop> = '/'; */
#line 1889 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_DIV;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 226: { /* <relop> = '<>'; */
#line 1898 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_NE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 227: { /* <relop> = '='; */
#line 1904 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_EQ;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 228: { /* <relop> = '=='; */
#line 1910 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_EXACT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 229: { /* <relop> = '>='; */
#line 1916 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_GE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 230: { /* <relop> = '<='; */
#line 1922 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_LE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 231: { /* <relop> = '>'; */
#line 1928 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_GT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 232: { /* <relop> = '<'; */
#line 1934 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_LT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 233: { /* <optional_qual> =; */
#line 1944 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 234: { /* <optional_qual> = 'BEFORE'; */
#line 1949 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 235: { /* <optional_qual> = 'AFTER'; */
#line 1955 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 236: { /* <optional_qual> = 'ONLY'; */
#line 1961 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 237: { /* <optional_not> =; */
#line 1970 "alan.pmk"

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 238: { /* <optional_not> = 'NOT'; */
#line 1975 "alan.pmk"

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 239: { /* <optional_id> =; */
#line 1982 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 240: { /* <optional_id> = ID; */
#line 1987 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 241: { /* <ids> = ID; */
#line 1994 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
    	break;}
    case 242: { /* <ids> = <ids> ID; */
#line 1999 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, LIST_ID);
    	break;}
    case 243: { /* <id_list> = ID; */
#line 2006 "alan.pmk"

        pmSeSt[pmStkP+1].str = newstr(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
    	break;}
    case 244: { /* <id_list> = <id_list> ',' ID; */
#line 2012 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, LIST_ID);
    	break;}
    case 245: { /* <optional_integer> =; */
#line 2020 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 246: { /* <optional_integer> = Integer; */
#line 2025 "alan.pmk"

        pmSeSt[pmStkP+1].str = newstr(pmSySt[pmStkP+1].chars);
    	break;}
    case 247: { /* <optional_minus> =; */
#line 2032 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 248: { /* <optional_minus> = '-'; */
#line 2037 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 249: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2044 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 250: { /* <integer_or_id> = Integer; */
#line 2053 "alan.pmk"

	pmSeSt[pmStkP+1].val = val(pmSySt[pmStkP+1].chars);
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 251: { /* <integer_or_id> = ID; */
#line 2059 "alan.pmk"

	pmSeSt[pmStkP+1].val = 0;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 252: { /* ID = Identifier; */
#line 2067 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    default: break; }
}/*pmPaSema()*/

