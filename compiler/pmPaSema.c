/*----------------------------------------------------------------------*\

	pmPaSema.c

	ParserMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


#include "util.h"

#include "types.h"
#include "smScan.h"
#include "token.h"

#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "alt.h"                /* ALT-nodes */
#include "atr.h"                /* ATR-nodes */
#include "chk.h"                /* CHK-nodes */
#include "cla.h"		/* CLASS-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "elm.h"                /* ELM-nodes */
#include "evt.h"                /* EVT-nodes */
#include "exp.h"                /* EXP-nodes */
#include "ext.h"                /* EXT-nodes */
#include "ins.h"		/* INS-nodes */
#include "lim.h"		/* LIM-nodes */
#include "lst.h"		/* LST-nodes */
#include "msg.h"                /* MSG-nodes */
#include "opt.h"		/* OPTIONS */
#include "res.h"                /* RES-nodes */
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
#include "whr.h"                /* WHR-nodes */
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
    List *surr;
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

#line 74 "alan.pmk"


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
#line 167 "alan.pmk"
    switch (rule) {
    case 6: { /* <option> = ID '.'; */
#line 187 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */
#line 192 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */
#line 197 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 11: { /* <unit> = <default>; */
#line 210 "alan.pmk"
 adv.atrs = combine(adv.atrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 12: { /* <unit> = <object_default>; */
#line 213 "alan.pmk"
 adv.oatrs = combine(adv.oatrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 13: { /* <unit> = <location_default>; */
#line 216 "alan.pmk"
 adv.latrs = combine(adv.latrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 14: { /* <unit> = <actor_default>; */
#line 219 "alan.pmk"
 adv.aatrs = combine(adv.aatrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 17: { /* <unit> = <synonyms>; */
#line 222 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns); 	break;}
    case 15: { /* <unit> = <messages>; */
#line 225 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 18: { /* <unit> = <syntax>; */
#line 228 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 19: { /* <unit> = <verb>; */
#line 231 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, LIST_VRB); 	break;}
    case 20: { /* <unit> = <class>; */
#line 234 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, LIST_CLA); 	break;}
    case 21: { /* <unit> = <instance>; */
#line 237 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, LIST_INS); 	break;}
    case 23: { /* <unit> = <event>; */
#line 240 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, LIST_EVT); 	break;}
    case 22: { /* <unit> = <container>; */
#line 243 "alan.pmk"
 adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, LIST_CNT); 	break;}
    case 16: { /* <unit> = <rule>; */
#line 246 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, LIST_RUL); 	break;}
    case 24: { /* <default> = 'DEFAULT' 'ATTRIBUTES' <attributes>; */
#line 252 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 25: { /* <location_default> = 'LOCATION' 'ATTRIBUTES' <attributes>; */
#line 259 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 26: { /* <object_default> = 'OBJECT' 'ATTRIBUTES' <attributes>; */
#line 266 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 27: { /* <actor_default> = 'ACTOR' 'ATTRIBUTES' <attributes>; */
#line 273 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 28: { /* <attributes> = <attribute definition> '.'; */
#line 281 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, LIST_ATR);
    	break;}
    case 29: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 286 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, LIST_ATR);
    	break;}
    case 30: { /* <attribute definition> = ID; */
#line 293 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						TYPBOOL,
						pmSeSt[pmStkP+1].id,
						TRUE, 0, 0);
    	break;}
    case 31: { /* <attribute definition> = 'NOT' ID; */
#line 301 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+2].id->srcp,
						TYPBOOL,
						pmSeSt[pmStkP+2].id,
						FALSE, 0, 0);
    	break;}
    case 32: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 309 "alan.pmk"

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
    case 33: { /* <attribute definition> = ID STRING; */
#line 323 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						TYPSTR,
						pmSeSt[pmStkP+1].id,
						0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 34: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 334 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 35: { /* <synonym_list> = <synonym>; */
#line 341 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, LIST_SYN);
    	break;}
    case 36: { /* <synonym_list> = <synonym_list> <synonym>; */
#line 346 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, LIST_SYN);
    	break;}
    case 37: { /* <synonym> = <id_list> '=' ID '.'; */
#line 353 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].idList,
			      pmSeSt[pmStkP+3].id);
    	break;}
    case 38: { /* <messages> = 'MESSAGE' <message_list>; */
#line 363 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 39: { /* <message_list> = <message>; */
#line 370 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, LIST_MSG);
    	break;}
    case 40: { /* <message_list> = <message_list> <message>; */
#line 375 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, LIST_MSG);
    	break;}
    case 41: { /* <message> = ID ':' <statements>; */
#line 382 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 42: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 392 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 43: { /* <syntax_list> = <syntax_item>; */
#line 399 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, LIST_STX);
    	break;}
    case 44: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 404 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, LIST_STX);
    	break;}
    case 45: { /* <syntax_item> = ID '=' <syntax_elements> <optional_class_restrictions>; */
#line 411 "alan.pmk"

	pmSeSt[pmStkP+1].stx = newstx(&pmSySt[pmStkP+2].srcp,
				  pmSeSt[pmStkP+1].id,
				  concat(pmSeSt[pmStkP+3].elms,
				         newelm(&pmSeSt[pmStkP+1].id->srcp, ELMEOS,
				                NULL,
				                FALSE),
				         LIST_ELM),
				  pmSeSt[pmStkP+4].ress);
    	break;}
    case 46: { /* <syntax_elements> = <syntax_element>; */
#line 425 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, LIST_ELM);
    	break;}
    case 47: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 430 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                LIST_ELM);
    	break;}
    case 48: { /* <syntax_element> = ID; */
#line 439 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+1].id->srcp, ELMWRD,
				     pmSeSt[pmStkP+1].id,
				     FALSE);
    	break;}
    case 49: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 447 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+2].id->srcp, ELMPAR,
				     pmSeSt[pmStkP+2].id,
				     pmSeSt[pmStkP+4].bits);
    	break;}
    case 50: { /* <optional_indicators> =; */
#line 456 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 51: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 461 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 52: { /* <indicator> = '*'; */
#line 468 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0x1;
    	break;}
    case 53: { /* <indicator> = '!'; */
#line 473 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0x2;
    	break;}
    case 54: { /* <optional_class_restrictions> = '.'; */
#line 480 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 55: { /* <optional_class_restrictions> = 'WHERE' <class_restriction_clauses>; */
#line 485 "alan.pmk"

	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 56: { /* <class_restriction_clauses> = <class_restriction>; */
#line 498 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, LIST_RES);
    	break;}
    case 57: { /* <class_restriction_clauses> = <class_restriction_clauses> 'AND' <class_restriction>; */
#line 503 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, LIST_RES);
    	break;}
    case 58: { /* <class_restriction> = ID 'ISA' <classes> 'ELSE' <statements>; */
#line 511 "alan.pmk"

	pmSeSt[pmStkP+1].res = newres(&pmSySt[pmStkP+2].srcp,
			  		pmSeSt[pmStkP+1].id,
					pmSeSt[pmStkP+3].single,
					pmSeSt[pmStkP+3].idList,
				    	pmSeSt[pmStkP+5].stms);
    	break;}
    case 59: { /* <classes> = <class_id>; */
#line 522 "alan.pmk"

	pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
	pmSeSt[pmStkP+1].single = TRUE;
    	break;}
    case 60: { /* <classes> = <classes> 'OR' <class_id>; */
#line 528 "alan.pmk"

	pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, LIST_ID);
	pmSeSt[pmStkP+1].single = FALSE;
    	break;}
    case 61: { /* <class_id> = <heritage_id>; */
#line 536 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 62: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 545 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newvrb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 63: { /* <verb_header> = 'VERB' <id_list>; */
#line 558 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 64: { /* <verb_body> = <simple_verb_body>; */
#line 567 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&nulsrcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   LIST_ALT);
    	break;}
    case 65: { /* <verb_body> = <verb_alternatives>; */
#line 578 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 66: { /* <verb_alternatives> = <verb_alternative>; */
#line 584 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, LIST_ALT);
    	break;}
    case 67: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 589 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				LIST_ALT);
    	break;}
    case 68: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 598 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 69: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 609 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 70: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */
#line 618 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 71: { /* <optional_checks> =; */
#line 626 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
    	break;}
    case 72: { /* <optional_checks> = 'CHECK' <statements>; */
#line 631 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), LIST_STM);
    	break;}
    case 73: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 636 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 74: { /* <check_list> = <check>; */
#line 643 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, LIST_CHK);
    	break;}
    case 75: { /* <check_list> = <check_list> 'AND' <check>; */
#line 648 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, LIST_CHK);
    	break;}
    case 76: { /* <check> = <expression> 'ELSE' <statements>; */
#line 655 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 77: { /* <optional_does> =; */
#line 663 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 79: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 674 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 80: { /* <class> = 'EVERY' ID <optional_heritage> <slots> <class_tail>; */
#line 685 "alan.pmk"

	pmSeSt[pmStkP+1].cla = newcla(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newSlots(pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].surr,
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
    case 81: { /* <class_tail> = 'END' 'EVERY' <optional_id> '.'; */
#line 709 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 82: { /* <instance> = 'THE' ID <optional_heritage> <slots> <instance tail>; */
#line 716 "alan.pmk"

	pmSeSt[pmStkP+1].ins = newins(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newSlots(pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].surr,
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
    case 83: { /* <instance tail> = 'END' 'THE' <optional_id> '.'; */
#line 740 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 84: { /* <optional_heritage> =; */
#line 745 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 85: { /* <optional_heritage> = <heritage>; */
#line 750 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 86: { /* <heritage> = 'ISA' <heritage_id>; */
#line 757 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 87: { /* <heritage_id> = ID; */
#line 762 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id; 	break;}
    case 88: { /* <slots> =; */
#line 768 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
	pmSeSt[pmStkP+1].whr = NULL;
	pmSeSt[pmStkP+1].atrs = NULL;
	pmSeSt[pmStkP+1].cnt = NULL;
	pmSeSt[pmStkP+1].surr = NULL;
	pmSeSt[pmStkP+1].dscr = NULL;
	pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].does = NULL;
	pmSeSt[pmStkP+1].exts = NULL;
	pmSeSt[pmStkP+1].vrbs = NULL;
	pmSeSt[pmStkP+1].scrs = NULL;
    	break;}
    case 89: { /* <slots> = <slots> <slot>; */
#line 783 "alan.pmk"

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

	if (pmSeSt[pmStkP+1].surr != NULL)
            /* SURROUNDINGS already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "SURROUNDINGS", "class/instance", NULL);
	else 
	    pmSeSt[pmStkP+1].surr = pmSeSt[pmStkP+2].surr;

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
    case 95: { /* <slot> = <name>; */
#line 839 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 90: { /* <slot> = <where>; */
#line 845 "alan.pmk"
{ WhrNod *whr = pmSeSt[pmStkP+1].whr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
    }	break;}
    case 98: { /* <slot> = <is> <attributes>; */
#line 851 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 91: { /* <slot> = <container properties>; */
#line 857 "alan.pmk"
{ CntNod *cnt = pmSeSt[pmStkP+1].cnt;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
    }	break;}
    case 92: { /* <slot> = <surroundings>; */
#line 863 "alan.pmk"
{ List *surr = pmSeSt[pmStkP+1].surr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].surr = surr;
    }	break;}
    case 93: { /* <slot> = <description>; */
#line 869 "alan.pmk"
{ List *dscr = pmSeSt[pmStkP+1].stms;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].dscr = dscr;
    }	break;}
    case 96: { /* <slot> = <mentioned>; */
#line 875 "alan.pmk"
{ List *ment = pmSeSt[pmStkP+1].ment;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = ment;
    }	break;}
    case 94: { /* <slot> = <article>; */
#line 881 "alan.pmk"
{ List *art = pmSeSt[pmStkP+1].art;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].art = art;
    }	break;}
    case 97: { /* <slot> = <does>; */
#line 887 "alan.pmk"
{ List *does = pmSeSt[pmStkP+1].does;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].does = does;
    }	break;}
    case 99: { /* <slot> = <exit>; */
#line 893 "alan.pmk"
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 100: { /* <slot> = <verb>; */
#line 899 "alan.pmk"
{ VrbNod *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 101: { /* <slot> = <script>; */
#line 905 "alan.pmk"
{ ScrNod *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 102: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 915 "alan.pmk"

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
    case 103: { /* <optional_exit_body> =; */
#line 930 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 104: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 937 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 105: { /* <optional_attributes> =; */
#line 952 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 106: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 957 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 107: { /* <is> = 'IS'; */
#line 964 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 108: { /* <is> = 'ARE'; */
#line 969 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 109: { /* <is> = 'HAS'; */
#line 974 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 110: { /* <surroundings> = 'SURROUNDINGS'; */
#line 982 "alan.pmk"

	pmSeSt[pmStkP+1].surr = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
						STM_NOP), LIST_STM);
    	break;}
    case 111: { /* <surroundings> = 'SURROUNDINGS' <statements>; */
#line 988 "alan.pmk"

	pmSeSt[pmStkP+1].surr = pmSeSt[pmStkP+2].stms;
    	break;}
    case 112: { /* <optional_description> =; */
#line 996 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 114: { /* <description> = 'DESCRIPTION'; */
#line 1006 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), LIST_STM);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 115: { /* <description> = 'DESCRIPTION' <statements>; */
#line 1013 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 116: { /* <article> = 'ARTICLE'; */
#line 1021 "alan.pmk"

	pmSeSt[pmStkP+1].art = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), LIST_STM);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 117: { /* <article> = 'ARTICLE' <statements>; */
#line 1028 "alan.pmk"

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 118: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 1037 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 119: { /* <optional_names> =; */
#line 1046 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 120: { /* <optional_names> = <optional_names> <name>; */
#line 1051 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					     pmSeSt[pmStkP+2].nam, LIST_NAM);
    	break;}
    case 121: { /* <name> = 'NAME' <ids>; */
#line 1059 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 122: { /* <container properties> = 'CONTAINER' <container_body>; */
#line 1068 "alan.pmk"

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
    case 123: { /* <container> = <container_header> <container_body> <container_tail>; */
#line 1086 "alan.pmk"

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
    case 124: { /* <container_header> = 'CONTAINER' ID; */
#line 1105 "alan.pmk"

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevINF, "THE <id> ISA CONTAINER ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 125: { /* <container_body> = <optional_limits> <optional_header> <optional_empty>; */
#line 1114 "alan.pmk"

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
    	break;}
    case 126: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */
#line 1123 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+3].id;
    	break;}
    case 127: { /* <optional_limits> =; */
#line 1130 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 128: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1135 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 129: { /* <limits> = <limit>; */
#line 1142 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIST_LIM);
    	break;}
    case 130: { /* <limits> = <limits> <limit>; */
#line 1147 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIST_LIM);
    	break;}
    case 131: { /* <limit> = <limit_attribute> 'THEN' <statements>; */
#line 1154 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 132: { /* <limit_attribute> = <attribute definition>; */
#line 1163 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 133: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1168 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSySt[pmStkP+1].srcp,
				      TYPINT,
				      newId(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 134: { /* <optional_header> =; */
#line 1178 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 135: { /* <optional_header> = 'HEADER' <statements>; */
#line 1183 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 136: { /* <optional_empty> =; */
#line 1190 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 137: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1195 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 138: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1203 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 139: { /* <event_header> = 'EVENT' ID; */
#line 1216 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 140: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1224 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 141: { /* <script> = 'SCRIPT' <integer_or_id> '.' <optional_description> <step_list>; */
#line 1232 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newscr(&pmSySt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+2].id,
			     pmSeSt[pmStkP+2].val,
			     pmSeSt[pmStkP+4].stms,
			     pmSeSt[pmStkP+5].stps);
    	break;}
    case 142: { /* <step_list> = <step>; */
#line 1243 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, LIST_STP);
    	break;}
    case 143: { /* <step_list> = <step_list> <step>; */
#line 1248 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, LIST_STP);
    	break;}
    case 144: { /* <step> = 'STEP' <statements>; */
#line 1255 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 145: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1263 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 146: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1271 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 147: { /* <rule> = 'WHEN' <expression> '=>' <statements>; */
#line 1282 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 148: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1292 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 149: { /* <optional_statements> =; */
#line 1301 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 150: { /* <optional_statements> = <statements>; */
#line 1306 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 151: { /* <statements> = <statement>; */
#line 1313 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, LIST_STM);
    	break;}
    case 152: { /* <statements> = <statements> <statement>; */
#line 1318 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, LIST_STM);
    	break;}
    case 153: { /* <statement> = <output_statement>; */
#line 1325 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 155: { /* <statement> = <manipulation_statement>; */
#line 1330 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 157: { /* <statement> = <assignment_statement>; */
#line 1335 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 159: { /* <statement> = <comparison_statement>; */
#line 1340 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 158: { /* <statement> = <actor_statement>; */
#line 1345 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 156: { /* <statement> = <event_statement>; */
#line 1350 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 154: { /* <statement> = <special_statement>; */
#line 1355 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 160: { /* <output_statement> = STRING; */
#line 1363 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_PRINT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 161: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1370 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DESCRIBE);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 162: { /* <output_statement> = 'SAY' <expression> '.'; */
#line 1376 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAY);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 163: { /* <output_statement> = 'LIST' <what> '.'; */
#line 1382 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LIST);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 164: { /* <special_statement> = 'QUIT' '.'; */
#line 1390 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_QUIT);
    	break;}
    case 165: { /* <special_statement> = 'LOOK' '.'; */
#line 1395 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOOK);
    	break;}
    case 166: { /* <special_statement> = 'SAVE' '.'; */
#line 1400 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAVE);
    	break;}
    case 167: { /* <special_statement> = 'RESTORE' '.'; */
#line 1405 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTORE);
    	break;}
    case 168: { /* <special_statement> = 'RESTART' '.'; */
#line 1410 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTART);
    	break;}
    case 169: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 1415 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCORE);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 170: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 1426 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_VISITS);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 171: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 1432 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SYSTEM);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 172: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */
#line 1443 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_EMPTY);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 173: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */
#line 1450 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOCATE);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 175: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1461 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCHEDULE);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 174: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1469 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_CANCEL);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 176: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */
#line 1479 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_MAKE);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 179: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1487 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SET);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 177: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1495 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_INCR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 178: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1503 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DECR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 180: { /* <optional_by_clause> =; */
#line 1513 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 181: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1518 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 182: { /* <comparison_statement> = <if_statement>; */
#line 1527 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 183: { /* <comparison_statement> = <depending_statement>; */
#line 1532 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 184: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 1540 "alan.pmk"

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
    case 185: { /* <optional_elsif_list> =; */
#line 1558 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 186: { /* <optional_elsif_list> = <elsif_list>; */
#line 1563 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 187: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 1570 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_IF);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, LIST_STM);
    }	break;}
    case 188: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 1583 "alan.pmk"
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
    case 189: { /* <optional_else_part> =; */
#line 1602 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 190: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 1607 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 191: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' 'DEPEND' '.'; */
#line 1614 "alan.pmk"
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPEND);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 192: { /* <depend_cases> = <depend_case>; */
#line 1625 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, LIST_CASE);
    	break;}
    case 193: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1630 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, LIST_CASE);
    	break;}
    case 195: { /* <depend_case> = <right_hand_side> ':' <statements>; */
#line 1637 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 194: { /* <depend_case> = 'ELSE' <statements>; */
#line 1644 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 196: { /* <actor_statement> = 'USE' 'SCRIPT' <integer_or_id> <optional_for_actor> '.'; */
#line 1654 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_USE);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.scriptno = pmSeSt[pmStkP+3].val;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 197: { /* <optional_for_actor> =; */
#line 1664 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 198: { /* <optional_for_actor> = 'FOR' ID; */
#line 1669 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 199: { /* <expression> = <term>; */
#line 1678 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 200: { /* <expression> = <expression> 'OR' <term>; */
#line 1683 "alan.pmk"
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_OR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 201: { /* <term> = <factor>; */
#line 1695 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 202: { /* <term> = <term> 'AND' <factor>; */
#line 1700 "alan.pmk"
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_AND;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 203: { /* <factor> = <primary>; */
#line 1712 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 204: { /* <factor> = <primary> <right_hand_side>; */
#line 1717 "alan.pmk"

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
    case 206: { /* <right_hand_side> = <binop> <primary>; */
#line 1734 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPBIN);
	exp->fields.bin.op = pmSeSt[pmStkP+1].op;
	exp->fields.bin.right = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 207: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 1743 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 205: { /* <right_hand_side> = <optional_not> <where>; */
#line 1753 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPWHR;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPWHR);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 208: { /* <right_hand_side> = <optional_not> 'ISA' <heritage_id>; */
#line 1762 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPISA;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPISA);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 209: { /* <right_hand_side> = <is> <something>; */
#line 1771 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPATR;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 211: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 1780 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBTW;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBTW);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 210: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 1790 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = OP_CONTAINS;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 217: { /* <primary> = '(' <expression> ')'; */
#line 1803 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 212: { /* <primary> = <optional_minus> Integer; */
#line 1808 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, EXPINT);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 213: { /* <primary> = STRING; */
#line 1817 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSTR);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 214: { /* <primary> = <what>; */
#line 1824 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 218: { /* <primary> = <attribute_reference>; */
#line 1830 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 216: { /* <primary> = <aggregate> <where>; */
#line 1838 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPAGR);
	pmSeSt[pmStkP+1].exp->fields.agr.agr	= pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.whr	= pmSeSt[pmStkP+2].whr;
    	break;}
    case 219: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 1846 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPRND);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 215: { /* <primary> = 'SCORE'; */
#line 1853 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSCORE);
    	break;}
    case 221: { /* <aggregate> = 'SUM' 'OF' ID; */
#line 1861 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_SUM;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 222: { /* <aggregate> = 'MAX' 'OF' ID; */
#line 1868 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_MAX;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 220: { /* <aggregate> = 'COUNT'; */
#line 1875 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_COUNT;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 223: { /* <something> = <optional_not> ID; */
#line 1885 "alan.pmk"

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 224: { /* <what> = 'OBJECT'; */
#line 1893 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_OBJ, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 225: { /* <what> = 'LOCATION'; */
#line 1899 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_LOC, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 226: { /* <what> = ID; */
#line 1905 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSeSt[pmStkP+1].id->srcp, WHT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 227: { /* <optional_where> =; */
#line 1914 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 228: { /* <optional_where> = <where>; */
#line 1919 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 229: { /* <where> = 'HERE'; */
#line 1926 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 230: { /* <where> = 'NEARBY'; */
#line 1932 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 231: { /* <where> = 'AT' <what>; */
#line 1938 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 232: { /* <where> = 'IN' <what>; */
#line 1944 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 233: { /* <binop> = '+'; */
#line 1952 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_PLUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 234: { /* <binop> = '-'; */
#line 1958 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_MINUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 235: { /* <binop> = '*'; */
#line 1964 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_MULT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 236: { /* <binop> = '/'; */
#line 1970 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_DIV;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 237: { /* <relop> = '<>'; */
#line 1979 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_NE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 238: { /* <relop> = '='; */
#line 1985 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_EQ;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 239: { /* <relop> = '=='; */
#line 1991 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_EXACT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 240: { /* <relop> = '>='; */
#line 1997 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_GE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 241: { /* <relop> = '<='; */
#line 2003 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_LE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 242: { /* <relop> = '>'; */
#line 2009 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_GT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 243: { /* <relop> = '<'; */
#line 2015 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_LT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 244: { /* <optional_qual> =; */
#line 2025 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 245: { /* <optional_qual> = 'BEFORE'; */
#line 2030 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 246: { /* <optional_qual> = 'AFTER'; */
#line 2036 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 247: { /* <optional_qual> = 'ONLY'; */
#line 2042 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 248: { /* <optional_not> =; */
#line 2051 "alan.pmk"

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 249: { /* <optional_not> = 'NOT'; */
#line 2056 "alan.pmk"

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 250: { /* <optional_id> =; */
#line 2063 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 251: { /* <optional_id> = ID; */
#line 2068 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 252: { /* <ids> = ID; */
#line 2075 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
    	break;}
    case 253: { /* <ids> = <ids> ID; */
#line 2080 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, LIST_ID);
    	break;}
    case 254: { /* <id_list> = ID; */
#line 2087 "alan.pmk"

        pmSeSt[pmStkP+1].str = newstr(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
    	break;}
    case 255: { /* <id_list> = <id_list> ',' ID; */
#line 2093 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, LIST_ID);
    	break;}
    case 256: { /* <optional_integer> =; */
#line 2101 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 257: { /* <optional_integer> = Integer; */
#line 2106 "alan.pmk"

        pmSeSt[pmStkP+1].str = newstr(pmSySt[pmStkP+1].chars);
    	break;}
    case 258: { /* <optional_minus> =; */
#line 2113 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 259: { /* <optional_minus> = '-'; */
#line 2118 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 260: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2125 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 261: { /* <integer_or_id> = Integer; */
#line 2134 "alan.pmk"

	pmSeSt[pmStkP+1].val = val(pmSySt[pmStkP+1].chars);
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 262: { /* <integer_or_id> = ID; */
#line 2140 "alan.pmk"

	pmSeSt[pmStkP+1].val = 0;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 263: { /* ID = Identifier; */
#line 2148 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 264: { /* ID = 'DEFAULT'; */
#line 2153 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "default");
    	break;}
    case 265: { /* ID = 'THE'; */
#line 2158 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "the");
    	break;}
    case 266: { /* ID = 'ARTICLE'; */
#line 2163 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "article");
    	break;}
    case 267: { /* ID = 'MESSAGE'; */
#line 2168 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "message");
    	break;}
    case 268: { /* ID = 'QUIT'; */
#line 2173 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "quit");
    	break;}
    case 269: { /* ID = 'SAVE'; */
#line 2178 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "save");
    	break;}
    case 270: { /* ID = 'RESTORE'; */
#line 2183 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "restore");
    	break;}
    case 271: { /* ID = 'RESTART'; */
#line 2188 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "restart");
    	break;}
    case 272: { /* ID = 'WAIT'; */
#line 2193 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "wait");
    	break;}
    case 273: { /* ID = 'BETWEEN'; */
#line 2198 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "between");
    	break;}
    case 274: { /* ID = 'CONTAINS'; */
#line 2203 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "contains");
    	break;}
    case 275: { /* ID = 'ON'; */
#line 2208 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "on");
    	break;}
    case 276: { /* ID = 'IN'; */
#line 2213 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "in");
    	break;}
    case 277: { /* ID = 'AFTER'; */
#line 2218 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "after");
    	break;}
    case 278: { /* ID = 'BEFORE'; */
#line 2223 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "before");
    	break;}
    case 279: { /* ID = 'CHECK'; */
#line 2228 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "check");
    	break;}
    case 280: { /* ID = 'DEPEND'; */
#line 2233 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "depend");
    	break;}
    case 281: { /* ID = 'EXIT'; */
#line 2238 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "exit");
    	break;}
    case 282: { /* ID = 'FOR'; */
#line 2243 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "for");
    	break;}
    case 283: { /* ID = 'ISA'; */
#line 2248 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "isa");
    	break;}
    case 284: { /* ID = 'LIMITS'; */
#line 2253 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "limits");
    	break;}
    case 285: { /* ID = 'THING'; */
#line 2258 "alan.pmk"

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, "thing");
    	break;}
    default: break; }
}/*pmPaSema()*/

