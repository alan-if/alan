/*----------------------------------------------------------------------*\

	pmPaSema.c

	ParserMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


#include "srcp.h"
#include "smScan.h"
#include "token.h"

#include "lmList.h"

#include "act.h"                /* ACT-nodes */
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
#include "loc.h"                /* LOC-nodes */
#include "lst.h"		/* LST-nodes */
#include "msg.h"                /* MSG-nodes */
#include "nam.h"		/* NAM-nodes */
#include "obj.h"		/* OBJ-nodes */
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
    List *objs;
    ObjNod *obj;
    Bool not;
    List *nams;
    List *nam;
    List *msgs;
    MsgNod *msg;
    Bool minus;
    List *ment;
    List *locs;
    LocNod *loc;
    List *lims;
    LimNod *lim;
    List *inss;
    InsNod *ins;
    List *idList2;
    List *idList;
    NamNod *id;
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
    List *acts;
    ActNod *act;
} pmGrammar;

/* END %%ATTRIBUTES */

extern short pmStkP;
extern Token pmSySt[];
extern pmGrammar pmSeSt[];

/* %%DECLARATIONS - User data and routines */

#line 76 "alan.pmk"


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
#line 175 "alan.pmk"
    switch (rule) {
    case 6: { /* <option> = ID '.'; */
#line 195 "alan.pmk"

	optBool(pmSeSt[pmStkP+1].id->str, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */
#line 200 "alan.pmk"

	optenum(pmSeSt[pmStkP+1].id->str, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->str);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */
#line 205 "alan.pmk"

	optint(pmSeSt[pmStkP+1].id->str, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 11: { /* <unit> = <default>; */
#line 218 "alan.pmk"
 adv.atrs = combine(adv.atrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 12: { /* <unit> = <object_default>; */
#line 221 "alan.pmk"
 adv.oatrs = combine(adv.oatrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 13: { /* <unit> = <location_default>; */
#line 224 "alan.pmk"
 adv.latrs = combine(adv.latrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 14: { /* <unit> = <actor_default>; */
#line 227 "alan.pmk"
 adv.aatrs = combine(adv.aatrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 17: { /* <unit> = <synonyms>; */
#line 230 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns); 	break;}
    case 15: { /* <unit> = <messages>; */
#line 233 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 18: { /* <unit> = <syntax>; */
#line 236 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 19: { /* <unit> = <verb>; */
#line 239 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VRBNOD); 	break;}
    case 20: { /* <unit> = <class>; */
#line 242 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLANOD); 	break;}
    case 21: { /* <unit> = <instance>; */
#line 245 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSNOD); 	break;}
    case 22: { /* <unit> = <location>; */
#line 248 "alan.pmk"
 adv.locs = concat(adv.locs, pmSeSt[pmStkP+1].loc, LOCNOD); 	break;}
    case 23: { /* <unit> = <object>; */
#line 251 "alan.pmk"
 adv.objs = concat(adv.objs, pmSeSt[pmStkP+1].obj, OBJNOD); 	break;}
    case 26: { /* <unit> = <event>; */
#line 254 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVTNOD); 	break;}
    case 24: { /* <unit> = <container>; */
#line 257 "alan.pmk"
 adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, CNTNOD); 	break;}
    case 25: { /* <unit> = <actor>; */
#line 260 "alan.pmk"
 adv.acts = concat(adv.acts, pmSeSt[pmStkP+1].act, ACTNOD); 	break;}
    case 16: { /* <unit> = <rule>; */
#line 263 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULNOD); 	break;}
    case 27: { /* <default> = 'DEFAULT' 'ATTRIBUTES' <attributes>; */
#line 269 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 28: { /* <location_default> = 'LOCATION' 'ATTRIBUTES' <attributes>; */
#line 276 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 29: { /* <object_default> = 'OBJECT' 'ATTRIBUTES' <attributes>; */
#line 283 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 30: { /* <actor_default> = 'ACTOR' 'ATTRIBUTES' <attributes>; */
#line 290 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 31: { /* <attributes> = <attribute definition> '.'; */
#line 298 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, ATRNOD);
    	break;}
    case 32: { /* <attributes> = <attributes> <attribute definition> '.'; */
#line 303 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATRNOD);
    	break;}
    case 33: { /* <attribute definition> = ID; */
#line 310 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						TYPBOOL,
						pmSeSt[pmStkP+1].id,
						TRUE, 0, 0);
    	break;}
    case 34: { /* <attribute definition> = 'NOT' ID; */
#line 318 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+2].id->srcp,
						TYPBOOL,
						pmSeSt[pmStkP+2].id,
						FALSE, 0, 0);
    	break;}
    case 35: { /* <attribute definition> = ID <optional_minus> Integer; */
#line 326 "alan.pmk"

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
    case 36: { /* <attribute definition> = ID STRING; */
#line 340 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						TYPSTR,
						pmSeSt[pmStkP+1].id,
						0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 37: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */
#line 351 "alan.pmk"

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 38: { /* <synonym_list> = <synonym>; */
#line 358 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, SYNNOD);
    	break;}
    case 39: { /* <synonym_list> = <synonym_list> <synonym>; */
#line 363 "alan.pmk"

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNNOD);
    	break;}
    case 40: { /* <synonym> = <id_list> '=' ID '.'; */
#line 370 "alan.pmk"

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].idList,
			      pmSeSt[pmStkP+3].id);
    	break;}
    case 41: { /* <messages> = 'MESSAGE' <message_list>; */
#line 380 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 42: { /* <message_list> = <message>; */
#line 387 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, MSGNOD);
    	break;}
    case 43: { /* <message_list> = <message_list> <message>; */
#line 392 "alan.pmk"

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MSGNOD);
    	break;}
    case 44: { /* <message> = ID ':' <statements>; */
#line 399 "alan.pmk"

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 45: { /* <syntax> = 'SYNTAX' <syntax_list>; */
#line 409 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 46: { /* <syntax_list> = <syntax_item>; */
#line 416 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, STXNOD);
    	break;}
    case 47: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 421 "alan.pmk"

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, STXNOD);
    	break;}
    case 48: { /* <syntax_item> = ID '=' <syntax_elements> <optional_class_restrictions>; */
#line 428 "alan.pmk"

	pmSeSt[pmStkP+1].stx = newstx(&pmSySt[pmStkP+2].srcp,
				  pmSeSt[pmStkP+1].id,
				  concat(pmSeSt[pmStkP+3].elms,
				         newelm(&pmSeSt[pmStkP+1].id->srcp, ELMEOS,
				                NULL,
				                FALSE),
				         ELMNOD),
				  pmSeSt[pmStkP+4].ress);
    	break;}
    case 49: { /* <syntax_elements> = <syntax_element>; */
#line 442 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, ELMNOD);
    	break;}
    case 50: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 447 "alan.pmk"

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                ELMNOD);
    	break;}
    case 51: { /* <syntax_element> = ID; */
#line 456 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+1].id->srcp, ELMWRD,
				     pmSeSt[pmStkP+1].id,
				     FALSE);
    	break;}
    case 52: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 464 "alan.pmk"

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+2].id->srcp, ELMPAR,
				     pmSeSt[pmStkP+2].id,
				     pmSeSt[pmStkP+4].bits);
    	break;}
    case 53: { /* <optional_indicators> =; */
#line 473 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 54: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 478 "alan.pmk"

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 55: { /* <indicator> = '*'; */
#line 485 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0x1;
    	break;}
    case 56: { /* <indicator> = '!'; */
#line 490 "alan.pmk"

	pmSeSt[pmStkP+1].bits = 0x2;
    	break;}
    case 57: { /* <optional_class_restrictions> = '.'; */
#line 497 "alan.pmk"

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 58: { /* <optional_class_restrictions> = 'WHERE' <class_restriction_clauses>; */
#line 502 "alan.pmk"

	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 59: { /* <class_restriction_clauses> = <class_restriction>; */
#line 514 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, RESNOD);
    	break;}
    case 60: { /* <class_restriction_clauses> = <class_restriction_clauses> 'AND' <class_restriction>; */
#line 519 "alan.pmk"

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, RESNOD);
    	break;}
    case 61: { /* <class_restriction> = ID 'ISA' <classes> 'ELSE' <statements>; */
#line 527 "alan.pmk"

	pmSeSt[pmStkP+1].res = newres(&pmSySt[pmStkP+2].srcp,
			  		pmSeSt[pmStkP+1].id,
					pmSeSt[pmStkP+3].single,
					pmSeSt[pmStkP+3].idList,
				    	pmSeSt[pmStkP+5].stms);
    	break;}
    case 62: { /* <classes> = <class_id>; */
#line 538 "alan.pmk"

	pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, IDNOD);
	pmSeSt[pmStkP+1].single = TRUE;
    	break;}
    case 63: { /* <classes> = <classes> 'OR' <class_id>; */
#line 544 "alan.pmk"

	pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, IDNOD);
	pmSeSt[pmStkP+1].single = FALSE;
    	break;}
    case 64: { /* <class_id> = <heritage_id>; */
#line 552 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 67: { /* <class_id> = 'CONTAINER' 'OBJECT'; */
#line 557 "alan.pmk"

	pmSeSt[pmStkP+1].id = newnam(&nulsrcp, "#containerobject");
    	break;}
    case 68: { /* <class_id> = 'CONTAINER' 'ACTOR'; */
#line 562 "alan.pmk"

	pmSeSt[pmStkP+1].id = newnam(&nulsrcp, "#containeractor");
    	break;}
    case 65: { /* <class_id> = 'INTEGER'; */
#line 567 "alan.pmk"

	pmSeSt[pmStkP+1].id = newnam(&nulsrcp, "#integer");
    	break;}
    case 66: { /* <class_id> = 'STRING'; */
#line 572 "alan.pmk"

	pmSeSt[pmStkP+1].id = newnam(&nulsrcp, "#string");
    	break;}
    case 69: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 581 "alan.pmk"

	pmSeSt[pmStkP+1].vrb = newvrb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->str) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 70: { /* <verb_header> = 'VERB' <id_list>; */
#line 594 "alan.pmk"

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 71: { /* <verb_body> = <simple_verb_body>; */
#line 603 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&nulsrcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   ALTNOD);
    	break;}
    case 72: { /* <verb_body> = <verb_alternatives>; */
#line 614 "alan.pmk"

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 73: { /* <verb_alternatives> = <verb_alternative>; */
#line 620 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, ALTNOD);
    	break;}
    case 74: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 625 "alan.pmk"

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				ALTNOD);
    	break;}
    case 75: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */
#line 634 "alan.pmk"

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 76: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 645 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 77: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */
#line 654 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 78: { /* <optional_checks> =; */
#line 662 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
    	break;}
    case 79: { /* <optional_checks> = 'CHECK' <statements>; */
#line 667 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), STMNOD);
    	break;}
    case 80: { /* <optional_checks> = 'CHECK' <check_list>; */
#line 672 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 81: { /* <check_list> = <check>; */
#line 679 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, CHKNOD);
    	break;}
    case 82: { /* <check_list> = <check_list> 'AND' <check>; */
#line 684 "alan.pmk"

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHKNOD);
    	break;}
    case 83: { /* <check> = <expression> 'ELSE' <statements>; */
#line 691 "alan.pmk"

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 84: { /* <optional_does> =; */
#line 699 "alan.pmk"

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 86: { /* <does> = 'DOES' <optional_qual> <statements>; */
#line 710 "alan.pmk"

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 87: { /* <location> = <location_header> <location_body> <location_tail>; */
#line 721 "alan.pmk"

	pmSeSt[pmStkP+1].loc = newloc(&pmSeSt[pmStkP+1].srcp,
			       pmSeSt[pmStkP+1].id,
			       pmSeSt[pmStkP+1].nam,
			       pmSeSt[pmStkP+2].atrs,
			       pmSeSt[pmStkP+2].stms,
			       pmSeSt[pmStkP+2].does,
			       pmSeSt[pmStkP+2].exts,
			       pmSeSt[pmStkP+2].vrbs);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR,
			pmSeSt[pmStkP+1].id->str);
	}
    	break;}
    case 88: { /* <location_header> = 'LOCATION' ID <optional_name>; */
#line 741 "alan.pmk"

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevINF, "THE <id> ISA LOCATION ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 89: { /* <location_body> =; */
#line 751 "alan.pmk"

	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
	pmSeSt[pmStkP+1].does = NULL;
	pmSeSt[pmStkP+1].exts  = NULL;
	pmSeSt[pmStkP+1].vrbs  = NULL;
    	break;}
    case 90: { /* <location_body> = <location_body> <location_body_part>; */
#line 760 "alan.pmk"

      if (pmSeSt[pmStkP+2].atrs != NULL)
	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);
      else if (pmSeSt[pmStkP+2].stms != NULL)
	if (pmSeSt[pmStkP+1].stms != NULL)
          /* DESCRIPTION Already declared */
	  lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "location", NULL);
        else
	  pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
      else if (pmSeSt[pmStkP+2].does != NULL)
	if (pmSeSt[pmStkP+1].does != NULL)
          /* DOES Already declared */
	  lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DOES", "location", NULL);
        else {
	  pmSeSt[pmStkP+1].does = pmSeSt[pmStkP+2].does;
	  if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	    lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	}
      else if (pmSeSt[pmStkP+2].ext != NULL)
	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXTNOD);
      else
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VRBNOD);
    	break;}
    case 93: { /* <location_body_part> = <is> <attributes>; */
#line 788 "alan.pmk"

        pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].does = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
    	break;}
    case 91: { /* <location_body_part> = <description>; */
#line 797 "alan.pmk"

        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].does = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 92: { /* <location_body_part> = <does>; */
#line 807 "alan.pmk"

        pmSeSt[pmStkP+1].does = pmSeSt[pmStkP+1].stms;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+1].qual;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 94: { /* <location_body_part> = <exit>; */
#line 818 "alan.pmk"

        pmSeSt[pmStkP+1].ext = pmSeSt[pmStkP+1].ext;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].does = NULL;
    	break;}
    case 95: { /* <location_body_part> = <verb>; */
#line 827 "alan.pmk"

        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].does = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = pmSeSt[pmStkP+1].vrb;
    	break;}
    case 96: { /* <location_tail> = 'END' 'LOCATION' <optional_id> '.'; */
#line 838 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 97: { /* <class> = 'EVERY' ID <optional_heritage> <slots> <class_tail>; */
#line 846 "alan.pmk"

	pmSeSt[pmStkP+1].cla = newcla(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].idList,
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
	    if (!eqnams(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 201, sevWAR, pmSeSt[pmStkP+2].id->str);
    	break;}
    case 98: { /* <class_tail> = 'END' 'EVERY' <optional_id> '.'; */
#line 870 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 99: { /* <instance> = 'THE' ID <optional_heritage> <slots> <instance tail>; */
#line 877 "alan.pmk"

	pmSeSt[pmStkP+1].ins = newins(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].idList,
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
	    if (!eqnams(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 201, sevWAR, pmSeSt[pmStkP+2].id->str);
    	break;}
    case 100: { /* <instance tail> = 'END' 'THE' <optional_id> '.'; */
#line 901 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 101: { /* <optional_heritage> =; */
#line 906 "alan.pmk"

	pmSeSt[pmStkP+1].idList = NULL;
    	break;}
    case 102: { /* <optional_heritage> = <heritage>; */
#line 911 "alan.pmk"

	pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+1].idList;
    	break;}
    case 103: { /* <heritage> = 'ISA' <heritage_list>; */
#line 918 "alan.pmk"
 pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+2].idList; 	break;}
    case 105: { /* <heritage_list> = <heritage_list> ',' <heritage_id>; */
#line 923 "alan.pmk"
 pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList,
					pmSeSt[pmStkP+3].id, IDNOD); 	break;}
    case 104: { /* <heritage_list> = <heritage_id>; */
#line 927 "alan.pmk"
 pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, IDNOD); 	break;}
    case 106: { /* <heritage_id> = ID; */
#line 932 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id; 	break;}
    case 107: { /* <heritage_id> = 'LOCATION'; */
#line 934 "alan.pmk"
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "location"); 	break;}
    case 108: { /* <heritage_id> = 'OBJECT'; */
#line 936 "alan.pmk"
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "object"); 	break;}
    case 109: { /* <heritage_id> = 'ACTOR'; */
#line 938 "alan.pmk"
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "actor"); 	break;}
    case 110: { /* <heritage_id> = 'CONTAINER'; */
#line 940 "alan.pmk"
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "container"); 	break;}
    case 111: { /* <slots> =; */
#line 946 "alan.pmk"

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
    case 112: { /* <slots> = <slots> <slot>; */
#line 961 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+2].nam, NAMNOD);

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

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXTNOD);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VRBNOD);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, SCRNOD);
    	break;}
    case 118: { /* <slot> = <name>; */
#line 1017 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 113: { /* <slot> = <where>; */
#line 1023 "alan.pmk"
{ WhrNod *whr = pmSeSt[pmStkP+1].whr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
    }	break;}
    case 121: { /* <slot> = <is> <attributes>; */
#line 1029 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 114: { /* <slot> = <container properties>; */
#line 1035 "alan.pmk"
{ CntNod *cnt = pmSeSt[pmStkP+1].cnt;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
    }	break;}
    case 115: { /* <slot> = <surroundings>; */
#line 1041 "alan.pmk"
{ List *surr = pmSeSt[pmStkP+1].surr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].surr = surr;
    }	break;}
    case 116: { /* <slot> = <description>; */
#line 1047 "alan.pmk"
{ List *dscr = pmSeSt[pmStkP+1].stms;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].dscr = dscr;
    }	break;}
    case 119: { /* <slot> = <mentioned>; */
#line 1053 "alan.pmk"
{ List *ment = pmSeSt[pmStkP+1].ment;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = ment;
    }	break;}
    case 117: { /* <slot> = <article>; */
#line 1059 "alan.pmk"
{ List *art = pmSeSt[pmStkP+1].art;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].art = art;
    }	break;}
    case 120: { /* <slot> = <does>; */
#line 1065 "alan.pmk"
{ List *does = pmSeSt[pmStkP+1].does;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].does = does;
    }	break;}
    case 122: { /* <slot> = <exit>; */
#line 1071 "alan.pmk"
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 123: { /* <slot> = <verb>; */
#line 1077 "alan.pmk"
{ VrbNod *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 124: { /* <slot> = <script>; */
#line 1083 "alan.pmk"
{ ScrNod *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 125: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */
#line 1093 "alan.pmk"

	pmSeSt[pmStkP+1].ext = newext(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].idList,
			   pmSeSt[pmStkP+4].id,
			   pmSeSt[pmStkP+5].chks,
			   pmSeSt[pmStkP+5].stms);
	if (pmSeSt[pmStkP+5].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+2].str, pmSeSt[pmStkP+5].id->str) != 0)
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 201, sevWAR, pmSeSt[pmStkP+2].str);
	}
    	break;}
    case 126: { /* <optional_exit_body> =; */
#line 1108 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id	 = NULL;
    	break;}
    case 127: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */
#line 1115 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 128: { /* <object> = <object_header> <object_body> <object_tail>; */
#line 1127 "alan.pmk"

	pmSeSt[pmStkP+1].obj = newobj(&pmSeSt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+1].id,
			     pmSeSt[pmStkP+1].nams,
			     pmSeSt[pmStkP+1].whr,
			     pmSeSt[pmStkP+2].cnt,
			     pmSeSt[pmStkP+2].atrs,
			     pmSeSt[pmStkP+2].dscr,
			     pmSeSt[pmStkP+2].art,
			     pmSeSt[pmStkP+2].ment,
			     pmSeSt[pmStkP+2].vrbs);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->str);
	}
    	break;}
    case 129: { /* <object_header> = 'OBJECT' ID <optional_where> <optional_names> <optional_where>; */
#line 1148 "alan.pmk"

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevINF, "THE <id> ISA OBJECT ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].nams = pmSeSt[pmStkP+4].nams;
        if (pmSeSt[pmStkP+3].whr->whr != WHR_DEFAULT) {
          pmSeSt[pmStkP+1].whr  = pmSeSt[pmStkP+3].whr;
          if (pmSeSt[pmStkP+5].whr->whr != WHR_DEFAULT)
            lmLog(&pmSeSt[pmStkP+5].whr->srcp, 340, sevWAR, pmSeSt[pmStkP+2].id->str);
        } else
          pmSeSt[pmStkP+1].whr  = pmSeSt[pmStkP+5].whr;
    	break;}
    case 130: { /* <object_body> =; */
#line 1164 "alan.pmk"

	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL; /* Description */
        pmSeSt[pmStkP+1].ment = NULL; /* Mentioned */
	pmSeSt[pmStkP+1].art = NULL; /* The article */
	pmSeSt[pmStkP+1].vrbs  = NULL;
    	break;}
    case 131: { /* <object_body> = <object_body> <object_body_part>; */
#line 1174 "alan.pmk"

	if (pmSeSt[pmStkP+2].cnt != NULL)
          if (pmSeSt[pmStkP+1].cnt != NULL)
            /* CONTAINER Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "CONTAINER", "object", NULL);
          else
	    pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+2].cnt;
        else if (pmSeSt[pmStkP+2].atrs != NULL)
          pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);
	else if (pmSeSt[pmStkP+2].dscr != NULL)
          if (pmSeSt[pmStkP+1].dscr != NULL)
            /* DESCRIPTION Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "object", NULL);
	  else
	    pmSeSt[pmStkP+1].dscr = pmSeSt[pmStkP+2].dscr;
        else if (pmSeSt[pmStkP+2].ment != NULL)
          if (pmSeSt[pmStkP+1].ment != NULL)
            /* MENTIONED Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "MENTIONED", "object", NULL);
          else
	    pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+2].ment;
        else if (pmSeSt[pmStkP+2].art != NULL)
          if (pmSeSt[pmStkP+1].art != NULL)
            /* ARTICLE Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "ARTICLE", "object", NULL);
          else
	    pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].art;
        else
 	  pmSeSt[pmStkP+1].vrbs  = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VRBNOD);
    	break;}
    case 132: { /* <object_body_part> = <container properties>; */
#line 1208 "alan.pmk"

	pmSeSt[pmStkP+1].cnt   = pmSeSt[pmStkP+1].cnt;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].art = NULL;
        pmSeSt[pmStkP+1].vrb   = NULL;
    	break;}
    case 136: { /* <object_body_part> = <is> <attributes>; */
#line 1219 "alan.pmk"

	pmSeSt[pmStkP+1].atrs  = pmSeSt[pmStkP+2].atrs;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].art = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
    	break;}
    case 133: { /* <object_body_part> = <description>; */
#line 1229 "alan.pmk"

	pmSeSt[pmStkP+1].dscr  = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].art = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
    	break;}
    case 135: { /* <object_body_part> = <mentioned>; */
#line 1240 "alan.pmk"

        pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].art = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
    	break;}
    case 134: { /* <object_body_part> = <article>; */
#line 1251 "alan.pmk"

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+1].art;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].ment = NULL;
    	break;}
    case 137: { /* <object_body_part> = <verb>; */
#line 1262 "alan.pmk"

	pmSeSt[pmStkP+1].vrb   = pmSeSt[pmStkP+1].vrb;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].art = NULL;
    	break;}
    case 138: { /* <object_tail> = 'END' 'OBJECT' <optional_id> '.'; */
#line 1274 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 139: { /* <optional_attributes> =; */
#line 1286 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 140: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */
#line 1291 "alan.pmk"

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 141: { /* <is> = 'IS'; */
#line 1298 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 142: { /* <is> = 'ARE'; */
#line 1303 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 143: { /* <is> = 'HAS'; */
#line 1308 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 144: { /* <surroundings> = 'SURROUNDINGS'; */
#line 1316 "alan.pmk"

	pmSeSt[pmStkP+1].surr = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
						STM_NOP), STMNOD);
    	break;}
    case 145: { /* <surroundings> = 'SURROUNDINGS' <statements>; */
#line 1322 "alan.pmk"

	pmSeSt[pmStkP+1].surr = pmSeSt[pmStkP+2].stms;
    	break;}
    case 146: { /* <optional_description> =; */
#line 1330 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 148: { /* <description> = 'DESCRIPTION'; */
#line 1340 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), STMNOD);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 149: { /* <description> = 'DESCRIPTION' <statements>; */
#line 1347 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 150: { /* <article> = 'ARTICLE'; */
#line 1355 "alan.pmk"

	pmSeSt[pmStkP+1].art = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), STMNOD);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 151: { /* <article> = 'ARTICLE' <statements>; */
#line 1362 "alan.pmk"

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 152: { /* <mentioned> = 'MENTIONED' <statements>; */
#line 1371 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 153: { /* <optional_name> =; */
#line 1380 "alan.pmk"

	pmSeSt[pmStkP+1].nam = NULL;
    	break;}
    case 154: { /* <optional_name> = <name>; */
#line 1385 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+1].nam;
    	break;}
    case 155: { /* <optional_names> =; */
#line 1392 "alan.pmk"

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 156: { /* <optional_names> = <optional_names> <name>; */
#line 1397 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					     pmSeSt[pmStkP+2].nam, NAMNOD);
    	break;}
    case 157: { /* <name> = 'NAME' <ids>; */
#line 1405 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 158: { /* <container properties> = 'CONTAINER' <container_body>; */
#line 1414 "alan.pmk"

	pmSeSt[pmStkP+1].cnt = newcnt(&pmSySt[pmStkP+1].srcp,
					  NULL,
					  pmSeSt[pmStkP+2].lims,
					  pmSeSt[pmStkP+2].stms,
					  pmSeSt[pmStkP+2].stms2);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, CNTNOD);
    	break;}
    case 159: { /* <container> = <container_header> <container_body> <container_tail>; */
#line 1428 "alan.pmk"

	pmSeSt[pmStkP+1].cnt = newcnt(&pmSeSt[pmStkP+1].srcp,
				pmSeSt[pmStkP+1].id,
				pmSeSt[pmStkP+2].lims,
				pmSeSt[pmStkP+2].stms,
				pmSeSt[pmStkP+2].stms2);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->str);
	}
    	break;}
    case 160: { /* <container_header> = 'CONTAINER' ID; */
#line 1443 "alan.pmk"

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevINF, "THE <id> ISA CONTAINER ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 161: { /* <container_body> = <optional_limits> <optional_header> <optional_empty>; */
#line 1452 "alan.pmk"

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
    	break;}
    case 162: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */
#line 1461 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+3].id;
    	break;}
    case 163: { /* <optional_limits> =; */
#line 1468 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 164: { /* <optional_limits> = 'LIMITS' <limits>; */
#line 1473 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 165: { /* <limits> = <limit>; */
#line 1480 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIMNOD);
    	break;}
    case 166: { /* <limits> = <limits> <limit>; */
#line 1485 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMNOD);
    	break;}
    case 167: { /* <limit> = <limit_attribute> 'THEN' <statements>; */
#line 1492 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 168: { /* <limit_attribute> = <attribute definition>; */
#line 1501 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 169: { /* <limit_attribute> = 'COUNT' Integer; */
#line 1506 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newatr(&pmSySt[pmStkP+1].srcp,
				      TYPINT,
				      newnam(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 170: { /* <optional_header> =; */
#line 1516 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 171: { /* <optional_header> = 'HEADER' <statements>; */
#line 1521 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 172: { /* <optional_empty> =; */
#line 1528 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 173: { /* <optional_empty> = 'ELSE' <statements>; */
#line 1533 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 174: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1541 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->str);
	}
    	break;}
    case 175: { /* <event_header> = 'EVENT' ID; */
#line 1554 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 176: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */
#line 1562 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 177: { /* <actor> = <actor_header> <actor_body> <actor_tail>; */
#line 1570 "alan.pmk"

	pmSeSt[pmStkP+1].act = newact(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+1].nams,
			    pmSeSt[pmStkP+1].whr,
			    pmSeSt[pmStkP+2].cnt,
			    pmSeSt[pmStkP+2].atrs,
			    pmSeSt[pmStkP+2].dscr,
			    pmSeSt[pmStkP+2].vrbs,
			    pmSeSt[pmStkP+2].scrs);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->str);
	}
    	break;}
    case 178: { /* <actor_header> = 'ACTOR' ID <optional_where> <optional_names> <optional_where>; */
#line 1589 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].nams = pmSeSt[pmStkP+4].nams;
        if (pmSeSt[pmStkP+3].whr->whr != WHR_DEFAULT) {
          pmSeSt[pmStkP+1].whr  = pmSeSt[pmStkP+3].whr;
          if (pmSeSt[pmStkP+5].whr->whr != WHR_DEFAULT)
            lmLog(&pmSeSt[pmStkP+5].whr->srcp, 340, sevWAR, pmSeSt[pmStkP+2].id->str);
        } else
          pmSeSt[pmStkP+1].whr  = pmSeSt[pmStkP+5].whr;
    	break;}
    case 179: { /* <actor_body> =; */
#line 1604 "alan.pmk"

	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL; /* Description */
	pmSeSt[pmStkP+1].vrbs  = NULL;
        pmSeSt[pmStkP+1].scrs  = NULL; /* The scripts */
    	break;}
    case 180: { /* <actor_body> = <actor_body> <actor_body_part>; */
#line 1613 "alan.pmk"

	if (pmSeSt[pmStkP+2].cnt != NULL)
          if (pmSeSt[pmStkP+1].cnt != NULL)
            /* CONTAINER Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "CONTAINER", "actor", NULL);
          else
	    pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+2].cnt;
        else if (pmSeSt[pmStkP+2].atrs != NULL)
          pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);
	else if (pmSeSt[pmStkP+2].dscr != NULL)
          if (pmSeSt[pmStkP+1].dscr != NULL)
            /* DESCRIPTION Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "actor", NULL);
	  else
	    pmSeSt[pmStkP+1].dscr = pmSeSt[pmStkP+2].dscr;
        else if (pmSeSt[pmStkP+2].vrb != NULL)
 	  pmSeSt[pmStkP+1].vrbs  = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VRBNOD);
        else
 	  pmSeSt[pmStkP+1].scrs  = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, VRBNOD);
    	break;}
    case 181: { /* <actor_body_part> = <container properties>; */
#line 1637 "alan.pmk"

	pmSeSt[pmStkP+1].cnt   = pmSeSt[pmStkP+1].cnt;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
    	break;}
    case 183: { /* <actor_body_part> = <is> <attributes>; */
#line 1647 "alan.pmk"

	pmSeSt[pmStkP+1].atrs  = pmSeSt[pmStkP+2].atrs;
	pmSeSt[pmStkP+1].dscr  = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
    	break;}
    case 182: { /* <actor_body_part> = <description>; */
#line 1656 "alan.pmk"

	pmSeSt[pmStkP+1].dscr  = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
    	break;}
    case 184: { /* <actor_body_part> = <verb>; */
#line 1666 "alan.pmk"

	pmSeSt[pmStkP+1].vrb   = pmSeSt[pmStkP+1].vrb;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
    	break;}
    case 185: { /* <actor_body_part> = <script>; */
#line 1675 "alan.pmk"

	pmSeSt[pmStkP+1].scr   = pmSeSt[pmStkP+1].scr;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
    	break;}
    case 186: { /* <actor_tail> = 'END' 'ACTOR' <optional_id> '.'; */
#line 1686 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 187: { /* <script> = 'SCRIPT' <integer_or_id> '.' <optional_description> <step_list>; */
#line 1694 "alan.pmk"

	pmSeSt[pmStkP+1].scr = newscr(&pmSySt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+2].id,
			     pmSeSt[pmStkP+2].val,
			     pmSeSt[pmStkP+4].stms,
			     pmSeSt[pmStkP+5].stps);
    	break;}
    case 188: { /* <step_list> = <step>; */
#line 1705 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, STPNOD);
    	break;}
    case 189: { /* <step_list> = <step_list> <step>; */
#line 1710 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STPNOD);
    	break;}
    case 190: { /* <step> = 'STEP' <statements>; */
#line 1717 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 191: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */
#line 1725 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 192: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */
#line 1733 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 193: { /* <rule> = 'WHEN' <expression> '=>' <statements>; */
#line 1744 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 194: { /* <start> = 'START' <where> '.' <optional_statements>; */
#line 1754 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 195: { /* <optional_statements> =; */
#line 1763 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 196: { /* <optional_statements> = <statements>; */
#line 1768 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 197: { /* <statements> = <statement>; */
#line 1775 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, STMNOD);
    	break;}
    case 198: { /* <statements> = <statements> <statement>; */
#line 1780 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STMNOD);
    	break;}
    case 199: { /* <statement> = <output_statement>; */
#line 1787 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 201: { /* <statement> = <manipulation_statement>; */
#line 1792 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 203: { /* <statement> = <assignment_statement>; */
#line 1797 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 205: { /* <statement> = <comparison_statement>; */
#line 1802 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 204: { /* <statement> = <actor_statement>; */
#line 1807 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 202: { /* <statement> = <event_statement>; */
#line 1812 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 200: { /* <statement> = <special_statement>; */
#line 1817 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 206: { /* <output_statement> = STRING; */
#line 1825 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_PRINT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 207: { /* <output_statement> = 'DESCRIBE' <what> '.'; */
#line 1832 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DESCRIBE);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 208: { /* <output_statement> = 'SAY' <expression> '.'; */
#line 1838 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAY);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 209: { /* <output_statement> = 'LIST' <what> '.'; */
#line 1844 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LIST);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 210: { /* <special_statement> = 'QUIT' '.'; */
#line 1852 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_QUIT);
    	break;}
    case 211: { /* <special_statement> = 'LOOK' '.'; */
#line 1857 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOOK);
    	break;}
    case 212: { /* <special_statement> = 'SAVE' '.'; */
#line 1862 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAVE);
    	break;}
    case 213: { /* <special_statement> = 'RESTORE' '.'; */
#line 1867 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTORE);
    	break;}
    case 214: { /* <special_statement> = 'RESTART' '.'; */
#line 1872 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTART);
    	break;}
    case 215: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */
#line 1877 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCORE);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 216: { /* <special_statement> = 'VISITS' Integer '.'; */
#line 1888 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_VISITS);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 217: { /* <special_statement> = 'SYSTEM' STRING '.'; */
#line 1894 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SYSTEM);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 218: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */
#line 1905 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_EMPTY);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 219: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */
#line 1912 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOCATE);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 221: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */
#line 1923 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCHEDULE);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 220: { /* <event_statement> = 'CANCEL' ID '.'; */
#line 1931 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_CANCEL);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 222: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */
#line 1941 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_MAKE);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 225: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */
#line 1949 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SET);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 223: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1957 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_INCR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 224: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */
#line 1965 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DECR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 226: { /* <optional_by_clause> =; */
#line 1975 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 227: { /* <optional_by_clause> = 'BY' <expression>; */
#line 1980 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 228: { /* <comparison_statement> = <if_statement>; */
#line 1989 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 229: { /* <comparison_statement> = <depending_statement>; */
#line 1994 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 230: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */
#line 2002 "alan.pmk"

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
    case 231: { /* <optional_elsif_list> =; */
#line 2020 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 232: { /* <optional_elsif_list> = <elsif_list>; */
#line 2025 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 233: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
#line 2032 "alan.pmk"
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_IF);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, STMNOD);
    }	break;}
    case 234: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
#line 2045 "alan.pmk"
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
	pmSeSt[pmStkP+1].stms->tail->element.stm->fields.iff.els = concat(NULL, stm, STMNOD);
	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, stm, STMNOD);
    }	break;}
    case 235: { /* <optional_else_part> =; */
#line 2064 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 236: { /* <optional_else_part> = 'ELSE' <statements>; */
#line 2069 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 237: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' 'DEPEND' '.'; */
#line 2076 "alan.pmk"
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPEND);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 238: { /* <depend_cases> = <depend_case>; */
#line 2087 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, CASENOD);
    	break;}
    case 239: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 2092 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASENOD);
    	break;}
    case 241: { /* <depend_case> = <right_hand_side> ':' <statements>; */
#line 2099 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 240: { /* <depend_case> = 'ELSE' <statements>; */
#line 2106 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 242: { /* <actor_statement> = 'USE' 'SCRIPT' <integer_or_id> <optional_for_actor> '.'; */
#line 2116 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_USE);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.scriptno = pmSeSt[pmStkP+3].val;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 243: { /* <optional_for_actor> =; */
#line 2126 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 244: { /* <optional_for_actor> = 'FOR' ID; */
#line 2131 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 245: { /* <expression> = <term>; */
#line 2140 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 246: { /* <expression> = <expression> 'OR' <term>; */
#line 2145 "alan.pmk"
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_OR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 247: { /* <term> = <factor>; */
#line 2157 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 248: { /* <term> = <term> 'AND' <factor>; */
#line 2162 "alan.pmk"
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_AND;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 249: { /* <factor> = <primary>; */
#line 2174 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 250: { /* <factor> = <primary> <right_hand_side>; */
#line 2179 "alan.pmk"

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
    case 252: { /* <right_hand_side> = <binop> <primary>; */
#line 2196 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPBIN);
	exp->fields.bin.op = pmSeSt[pmStkP+1].op;
	exp->fields.bin.right = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 253: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 2205 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 251: { /* <right_hand_side> = <optional_not> <where>; */
#line 2215 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPWHR;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPWHR);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 254: { /* <right_hand_side> = <optional_not> 'ISA' <heritage_id>; */
#line 2224 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPISA;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPISA);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 255: { /* <right_hand_side> = <is> <something>; */
#line 2233 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPATR;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 257: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
#line 2242 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBTW;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBTW);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 256: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
#line 2252 "alan.pmk"
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = OP_CONTAINS;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 263: { /* <primary> = '(' <expression> ')'; */
#line 2265 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 258: { /* <primary> = <optional_minus> Integer; */
#line 2270 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, EXPINT);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 259: { /* <primary> = STRING; */
#line 2279 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSTR);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 260: { /* <primary> = <what>; */
#line 2286 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 264: { /* <primary> = <attribute_reference>; */
#line 2292 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 262: { /* <primary> = <aggregate> <where>; */
#line 2300 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPAGR);
	pmSeSt[pmStkP+1].exp->fields.agr.agr	= pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.atrnam = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.whr	= pmSeSt[pmStkP+2].whr;
    	break;}
    case 265: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */
#line 2308 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPRND);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 261: { /* <primary> = 'SCORE'; */
#line 2315 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSCORE);
    	break;}
    case 267: { /* <aggregate> = 'SUM' 'OF' ID; */
#line 2323 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_SUM;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 268: { /* <aggregate> = 'MAX' 'OF' ID; */
#line 2330 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_MAX;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 266: { /* <aggregate> = 'COUNT'; */
#line 2337 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_COUNT;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 269: { /* <something> = <optional_not> ID; */
#line 2347 "alan.pmk"

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 270: { /* <what> = 'OBJECT'; */
#line 2355 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_OBJ, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 271: { /* <what> = 'LOCATION'; */
#line 2361 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_LOC, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 272: { /* <what> = 'ACTOR'; */
#line 2367 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_ACT, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 273: { /* <what> = ID; */
#line 2373 "alan.pmk"

	pmSeSt[pmStkP+1].wht = newwht(&pmSeSt[pmStkP+1].id->srcp, WHT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 274: { /* <optional_where> =; */
#line 2382 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 275: { /* <optional_where> = <where>; */
#line 2387 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 276: { /* <where> = 'HERE'; */
#line 2394 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 277: { /* <where> = 'NEARBY'; */
#line 2400 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 278: { /* <where> = 'AT' <what>; */
#line 2406 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 279: { /* <where> = 'IN' <what>; */
#line 2412 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 280: { /* <binop> = '+'; */
#line 2420 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_PLUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 281: { /* <binop> = '-'; */
#line 2426 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_MINUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 282: { /* <binop> = '*'; */
#line 2432 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_MULT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 283: { /* <binop> = '/'; */
#line 2438 "alan.pmk"

	pmSeSt[pmStkP+1].op = OP_DIV;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 284: { /* <relop> = '<>'; */
#line 2447 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_NE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 285: { /* <relop> = '='; */
#line 2453 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_EQ;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 286: { /* <relop> = '=='; */
#line 2459 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_EXACT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 287: { /* <relop> = '>='; */
#line 2465 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_GE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 288: { /* <relop> = '<='; */
#line 2471 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_LE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 289: { /* <relop> = '>'; */
#line 2477 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_GT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 290: { /* <relop> = '<'; */
#line 2483 "alan.pmk"

        pmSeSt[pmStkP+1].op   = OP_LT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 291: { /* <optional_qual> =; */
#line 2493 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 292: { /* <optional_qual> = 'BEFORE'; */
#line 2498 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 293: { /* <optional_qual> = 'AFTER'; */
#line 2504 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 294: { /* <optional_qual> = 'ONLY'; */
#line 2510 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 295: { /* <optional_not> =; */
#line 2519 "alan.pmk"

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 296: { /* <optional_not> = 'NOT'; */
#line 2524 "alan.pmk"

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 297: { /* <optional_id> =; */
#line 2531 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 298: { /* <optional_id> = ID; */
#line 2536 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 299: { /* <ids> = ID; */
#line 2543 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, IDNOD);
    	break;}
    case 300: { /* <ids> = <ids> ID; */
#line 2548 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, IDNOD);
    	break;}
    case 301: { /* <id_list> = ID; */
#line 2555 "alan.pmk"

        pmSeSt[pmStkP+1].str = newstr(pmSeSt[pmStkP+1].id->str);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, IDNOD);
    	break;}
    case 302: { /* <id_list> = <id_list> ',' ID; */
#line 2561 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, IDNOD);
    	break;}
    case 303: { /* <optional_integer> =; */
#line 2569 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 304: { /* <optional_integer> = Integer; */
#line 2574 "alan.pmk"

        pmSeSt[pmStkP+1].str = newstr(pmSySt[pmStkP+1].chars);
    	break;}
    case 305: { /* <optional_minus> =; */
#line 2581 "alan.pmk"

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 306: { /* <optional_minus> = '-'; */
#line 2586 "alan.pmk"

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 307: { /* <attribute_reference> = ID 'OF' <what>; */
#line 2593 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 308: { /* <integer_or_id> = Integer; */
#line 2602 "alan.pmk"

	pmSeSt[pmStkP+1].val = val(pmSySt[pmStkP+1].chars);
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 309: { /* <integer_or_id> = ID; */
#line 2608 "alan.pmk"

	pmSeSt[pmStkP+1].val = 0;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 310: { /* ID = Identifier; */
#line 2616 "alan.pmk"

	pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 311: { /* ID = 'DEFAULT'; */
#line 2621 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "default");
    	break;}
    case 312: { /* ID = 'THE'; */
#line 2626 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "the");
    	break;}
    case 313: { /* ID = 'ARTICLE'; */
#line 2631 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "article");
    	break;}
    case 314: { /* ID = 'MESSAGE'; */
#line 2636 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "message");
    	break;}
    case 315: { /* ID = 'QUIT'; */
#line 2641 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "quit");
    	break;}
    case 316: { /* ID = 'SAVE'; */
#line 2646 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "save");
    	break;}
    case 317: { /* ID = 'RESTORE'; */
#line 2651 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "restore");
    	break;}
    case 318: { /* ID = 'RESTART'; */
#line 2656 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "restart");
    	break;}
    case 319: { /* ID = 'WAIT'; */
#line 2661 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "wait");
    	break;}
    case 320: { /* ID = 'BETWEEN'; */
#line 2666 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "between");
    	break;}
    case 321: { /* ID = 'CONTAINS'; */
#line 2671 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "contains");
    	break;}
    case 322: { /* ID = 'ON'; */
#line 2676 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "on");
    	break;}
    case 323: { /* ID = 'IN'; */
#line 2681 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "in");
    	break;}
    case 324: { /* ID = 'AFTER'; */
#line 2686 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "after");
    	break;}
    case 325: { /* ID = 'BEFORE'; */
#line 2691 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "before");
    	break;}
    case 326: { /* ID = 'CHECK'; */
#line 2696 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "check");
    	break;}
    case 327: { /* ID = 'DEPEND'; */
#line 2701 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "depend");
    	break;}
    case 328: { /* ID = 'EXIT'; */
#line 2706 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "exit");
    	break;}
    case 329: { /* ID = 'FOR'; */
#line 2711 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "for");
    	break;}
    case 330: { /* ID = 'ISA'; */
#line 2716 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "isa");
    	break;}
    case 331: { /* ID = 'LIMITS'; */
#line 2721 "alan.pmk"

        pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "limits");
    	break;}
    case 332: { /* ID = 'THING'; */
#line 2726 "alan.pmk"

	pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "thing");
    	break;}
    default: break; }
}/*pmPaSema()*/

