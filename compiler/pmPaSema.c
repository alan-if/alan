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
#include "slt_x.h"

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
    switch (rule) {
          case 6: { /* <option> = ID '.'; */

	optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */

	optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */

	optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 13: { /* <unit> = <synonyms>; */
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns); 	break;}
    case 11: { /* <unit> = <messages>; */
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 14: { /* <unit> = <syntax>; */
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 15: { /* <unit> = <verb>; */
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, LIST_VRB); 	break;}
    case 16: { /* <unit> = <class>; */
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, LIST_CLA); 	break;}
    case 17: { /* <unit> = <instance>; */
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, LIST_INS); 	break;}
    case 19: { /* <unit> = <event>; */
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, LIST_EVT); 	break;}
    case 18: { /* <unit> = <container>; */
 adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, LIST_CNT); 	break;}
    case 12: { /* <unit> = <rule>; */
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, LIST_RUL); 	break;}
    case 20: { /* <attributes> = <attribute definition> '.'; */

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, LIST_ATR);
    	break;}
    case 21: { /* <attributes> = <attributes> <attribute definition> '.'; */

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, LIST_ATR);
    	break;}
    case 22: { /* <attribute definition> = ID; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						TYPBOOL,
						pmSeSt[pmStkP+1].id,
						TRUE, 0, 0);
    	break;}
    case 23: { /* <attribute definition> = 'NOT' ID; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+2].id->srcp,
						TYPBOOL,
						pmSeSt[pmStkP+2].id,
						FALSE, 0, 0);
    	break;}
    case 24: { /* <attribute definition> = ID <optional_minus> Integer; */

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

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
						TYPSTR,
						pmSeSt[pmStkP+1].id,
						0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 26: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 27: { /* <synonym_list> = <synonym>; */

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, LIST_SYN);
    	break;}
    case 28: { /* <synonym_list> = <synonym_list> <synonym>; */

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, LIST_SYN);
    	break;}
    case 29: { /* <synonym> = <id_list> '=' ID '.'; */

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].idList,
			      pmSeSt[pmStkP+3].id);
    	break;}
    case 30: { /* <messages> = 'MESSAGE' <message_list>; */

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 31: { /* <message_list> = <message>; */

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, LIST_MSG);
    	break;}
    case 32: { /* <message_list> = <message_list> <message>; */

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, LIST_MSG);
    	break;}
    case 33: { /* <message> = ID ':' <statements>; */

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 34: { /* <syntax> = 'SYNTAX' <syntax_list>; */

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 35: { /* <syntax_list> = <syntax_item>; */

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, LIST_STX);
    	break;}
    case 36: { /* <syntax_list> = <syntax_list> <syntax_item>; */

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, LIST_STX);
    	break;}
    case 37: { /* <syntax_item> = ID '=' <syntax_elements> <optional_class_restrictions>; */

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

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, LIST_ELM);
    	break;}
    case 39: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                LIST_ELM);
    	break;}
    case 40: { /* <syntax_element> = ID; */

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+1].id->srcp, ELMWRD,
				     pmSeSt[pmStkP+1].id,
				     FALSE);
    	break;}
    case 41: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+2].id->srcp, ELMPAR,
				     pmSeSt[pmStkP+2].id,
				     pmSeSt[pmStkP+4].bits);
    	break;}
    case 42: { /* <optional_indicators> =; */

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 43: { /* <optional_indicators> = <optional_indicators> <indicator>; */

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 44: { /* <indicator> = '*'; */

	pmSeSt[pmStkP+1].bits = 0x1;
    	break;}
    case 45: { /* <indicator> = '!'; */

	pmSeSt[pmStkP+1].bits = 0x2;
    	break;}
    case 46: { /* <optional_class_restrictions> = '.'; */

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 47: { /* <optional_class_restrictions> = 'WHERE' <class_restriction_clauses>; */

	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 48: { /* <class_restriction_clauses> = <class_restriction>; */

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, LIST_RES);
    	break;}
    case 49: { /* <class_restriction_clauses> = <class_restriction_clauses> 'AND' <class_restriction>; */

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, LIST_RES);
    	break;}
    case 50: { /* <class_restriction> = ID 'ISA' <classes> 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].res = newres(&pmSySt[pmStkP+2].srcp,
			  		pmSeSt[pmStkP+1].id,
					pmSeSt[pmStkP+3].single,
					pmSeSt[pmStkP+3].idList,
				    	pmSeSt[pmStkP+5].stms);
    	break;}
    case 51: { /* <classes> = <class_id>; */

	pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
	pmSeSt[pmStkP+1].single = TRUE;
    	break;}
    case 52: { /* <classes> = <classes> 'OR' <class_id>; */

	pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, LIST_ID);
	pmSeSt[pmStkP+1].single = FALSE;
    	break;}
    case 53: { /* <class_id> = <heritage_id>; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 54: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */

	pmSeSt[pmStkP+1].vrb = newvrb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 55: { /* <verb_header> = 'VERB' <id_list>; */

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 56: { /* <verb_body> = <simple_verb_body>; */

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&nulsrcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   LIST_ALT);
    	break;}
    case 57: { /* <verb_body> = <verb_alternatives>; */

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 58: { /* <verb_alternatives> = <verb_alternative>; */

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, LIST_ALT);
    	break;}
    case 59: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				LIST_ALT);
    	break;}
    case 60: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 61: { /* <simple_verb_body> = <optional_checks> <optional_does>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 62: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 63: { /* <optional_checks> =; */

	pmSeSt[pmStkP+1].chks = NULL;
    	break;}
    case 64: { /* <optional_checks> = 'CHECK' <statements>; */

	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), LIST_STM);
    	break;}
    case 65: { /* <optional_checks> = 'CHECK' <check_list>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 66: { /* <check_list> = <check>; */

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, LIST_CHK);
    	break;}
    case 67: { /* <check_list> = <check_list> 'AND' <check>; */

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, LIST_CHK);
    	break;}
    case 68: { /* <check> = <expression> 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 69: { /* <optional_does> =; */

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 71: { /* <does> = 'DOES' <optional_qual> <statements>; */

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 72: { /* <class> = 'EVERY' ID <optional_heritage> <slots> <class_tail>; */

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
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 74: { /* <instance> = 'THE' ID <optional_heritage> <slots> <instance tail>; */

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
    case 75: { /* <instance tail> = 'END' 'THE' <optional_id> '.'; */
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 76: { /* <optional_heritage> =; */

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 77: { /* <optional_heritage> = <heritage>; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 78: { /* <heritage> = 'ISA' <heritage_id> __genSym#0; */
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 81: { /* <heritage_id> = ID; */
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id; 	break;}
    case 82: { /* <slots> =; */

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
    case 83: { /* <slots> = <slots> __genSym#1 <slot>; */

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+3].nam, LIST_NAM);

        if (pmSeSt[pmStkP+3].whr != NULL)
            if (pmSeSt[pmStkP+1].whr != NULL) 
                /* WHERE clause already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "WHERE", "class/instance", NULL);
	    else
	        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+3].whr;

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);

	if (pmSeSt[pmStkP+3].cnt != NULL)
            if (pmSeSt[pmStkP+1].cnt != NULL)
                /* CONTAINER properties already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "CONTAINER properties", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+3].cnt;

	if (pmSeSt[pmStkP+3].dscr != NULL)
            if(pmSeSt[pmStkP+1].dscr != NULL)
                /* DESCRIPTION already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "DESCRIPTION", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].dscr = pmSeSt[pmStkP+3].dscr;

	if (pmSeSt[pmStkP+3].ment != NULL)
            if (pmSeSt[pmStkP+1].ment != NULL)
                /* MENTIONED already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "MENTIONED", "class/instance", NULL);
	    else
                pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+3].ment;

	if (pmSeSt[pmStkP+3].art != NULL)
            if (pmSeSt[pmStkP+1].art != NULL)
                /* ARTICLE already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "ARTICLE", "class/instance", NULL);
            else 
	        pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+3].art;

	if (pmSeSt[pmStkP+3].does != NULL)
            if (pmSeSt[pmStkP+1].does != NULL)
                /* DOES/ENTERED already declared */
	        lmLogv(&pmSeSt[pmStkP+3].srcp, 204, sevERR, "DOES/ENTERED", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].does = pmSeSt[pmStkP+3].does;

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+3].ext, LIST_EXT);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+3].vrb, LIST_VRB);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+3].scr, LIST_SCR);
    	break;}
    case 90: { /* <slot> = <name>; */
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 86: { /* <slot> = <where> __genSym#2; */
{ WhrNod *whr = pmSeSt[pmStkP+1].whr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
    }	break;}
    case 93: { /* <slot> = <is> <attributes>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 87: { /* <slot> = <container properties>; */
{ CntNod *cnt = pmSeSt[pmStkP+1].cnt;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
    }	break;}
    case 88: { /* <slot> = <description>; */
{ List *dscr = pmSeSt[pmStkP+1].stms;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].dscr = dscr;
    }	break;}
    case 91: { /* <slot> = <mentioned>; */
{ List *ment = pmSeSt[pmStkP+1].stms;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = ment;
    }	break;}
    case 89: { /* <slot> = <article>; */
{ List *art = pmSeSt[pmStkP+1].art;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].art = art;
    }	break;}
    case 92: { /* <slot> = <does>; */
{ List *does = pmSeSt[pmStkP+1].does;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].does = does;
    }	break;}
    case 94: { /* <slot> = <exit>; */
{ ExtNod *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 95: { /* <slot> = <verb>; */
{ VrbNod *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 96: { /* <slot> = <script>; */
{ ScrNod *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 99: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */

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
    case 100: { /* <optional_exit_body> =; */

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 101: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 102: { /* <optional_attributes> =; */

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 103: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 104: { /* <is> = 'IS'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 105: { /* <is> = 'ARE'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 106: { /* <is> = 'HAS'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 107: { /* <optional_description> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 109: { /* <description> = 'DESCRIPTION'; */

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), LIST_STM);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 110: { /* <description> = 'DESCRIPTION' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 111: { /* <article> = 'ARTICLE'; */

	pmSeSt[pmStkP+1].art = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), LIST_STM);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 112: { /* <article> = 'ARTICLE' <statements>; */

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 113: { /* <mentioned> = 'MENTIONED' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 114: { /* <optional_names> =; */

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 115: { /* <optional_names> = <optional_names> <name>; */

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					     pmSeSt[pmStkP+2].nam, LIST_NAM);
    	break;}
    case 116: { /* <name> = 'NAME' <ids>; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 117: { /* <container properties> = 'CONTAINER' <container_body>; */

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
    case 118: { /* <container> = <container_header> <container_body> <container_tail>; */

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
    case 119: { /* <container_header> = 'CONTAINER' ID; */

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevINF, "THE <id> ISA CONTAINER ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 120: { /* <container_body> = <optional_limits> <optional_header> <optional_empty>; */

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
    	break;}
    case 121: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+3].id;
    	break;}
    case 122: { /* <optional_limits> =; */

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 123: { /* <optional_limits> = 'LIMITS' <limits>; */

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 124: { /* <limits> = <limit>; */

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIST_LIM);
    	break;}
    case 125: { /* <limits> = <limits> <limit>; */

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIST_LIM);
    	break;}
    case 126: { /* <limit> = <limit_attribute> 'THEN' <statements>; */

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 127: { /* <limit_attribute> = <attribute definition>; */

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 128: { /* <limit_attribute> = 'COUNT' Integer; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSySt[pmStkP+1].srcp,
				      TYPINT,
				      newId(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 129: { /* <optional_header> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 130: { /* <optional_header> = 'HEADER' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 131: { /* <optional_empty> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 132: { /* <optional_empty> = 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 133: { /* <event> = <event_header> <statements> <event_tail>; */

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 134: { /* <event_header> = 'EVENT' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 135: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 136: { /* <script> = 'SCRIPT' <integer_or_id> '.' <optional_description> <step_list>; */

	pmSeSt[pmStkP+1].scr = newscr(&pmSySt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+2].id,
			     pmSeSt[pmStkP+2].val,
			     pmSeSt[pmStkP+4].stms,
			     pmSeSt[pmStkP+5].stps);
    	break;}
    case 137: { /* <step_list> = <step>; */

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, LIST_STP);
    	break;}
    case 138: { /* <step_list> = <step_list> <step>; */

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, LIST_STP);
    	break;}
    case 139: { /* <step> = 'STEP' <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 140: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 141: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 142: { /* <rule> = 'WHEN' <expression> '=>' <statements>; */

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 143: { /* <start> = 'START' <where> '.' <optional_statements>; */

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 144: { /* <optional_statements> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 145: { /* <optional_statements> = <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 146: { /* <statements> = <statement>; */

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, LIST_STM);
    	break;}
    case 147: { /* <statements> = <statements> <statement>; */

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, LIST_STM);
    	break;}
    case 148: { /* <statement> = <output_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 150: { /* <statement> = <manipulation_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 152: { /* <statement> = <assignment_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 154: { /* <statement> = <comparison_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 153: { /* <statement> = <actor_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 151: { /* <statement> = <event_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 149: { /* <statement> = <special_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 155: { /* <output_statement> = STRING; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_PRINT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 156: { /* <output_statement> = 'DESCRIBE' <what> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DESCRIBE);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 157: { /* <output_statement> = 'SAY' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAY);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 158: { /* <output_statement> = 'LIST' <what> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LIST);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 159: { /* <special_statement> = 'QUIT' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_QUIT);
    	break;}
    case 160: { /* <special_statement> = 'LOOK' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOOK);
    	break;}
    case 161: { /* <special_statement> = 'SAVE' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAVE);
    	break;}
    case 162: { /* <special_statement> = 'RESTORE' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTORE);
    	break;}
    case 163: { /* <special_statement> = 'RESTART' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTART);
    	break;}
    case 164: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCORE);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 165: { /* <special_statement> = 'VISITS' Integer '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_VISITS);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 166: { /* <special_statement> = 'SYSTEM' STRING '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SYSTEM);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 167: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_EMPTY);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 168: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOCATE);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 170: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCHEDULE);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 169: { /* <event_statement> = 'CANCEL' ID '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_CANCEL);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 171: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_MAKE);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 174: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SET);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 172: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_INCR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 173: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DECR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 175: { /* <optional_by_clause> =; */

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 176: { /* <optional_by_clause> = 'BY' <expression>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 177: { /* <comparison_statement> = <if_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 178: { /* <comparison_statement> = <depending_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 179: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */

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
    case 180: { /* <optional_elsif_list> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 181: { /* <optional_elsif_list> = <elsif_list>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 182: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_IF);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, LIST_STM);
    }	break;}
    case 183: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
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
    case 184: { /* <optional_else_part> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 185: { /* <optional_else_part> = 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 186: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' 'DEPEND' '.'; */
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPEND);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 187: { /* <depend_cases> = <depend_case>; */

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, LIST_CASE);
    	break;}
    case 188: { /* <depend_cases> = <depend_cases> <depend_case>; */

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, LIST_CASE);
    	break;}
    case 190: { /* <depend_case> = <right_hand_side> ':' <statements>; */

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 189: { /* <depend_case> = 'ELSE' <statements>; */

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 191: { /* <actor_statement> = 'USE' 'SCRIPT' <integer_or_id> <optional_for_actor> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_USE);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.scriptno = pmSeSt[pmStkP+3].val;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 192: { /* <optional_for_actor> =; */

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 193: { /* <optional_for_actor> = 'FOR' ID; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 194: { /* <expression> = <term>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 195: { /* <expression> = <expression> 'OR' <term>; */
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_OR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 196: { /* <term> = <factor>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 197: { /* <term> = <term> 'AND' <factor>; */
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_AND;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 198: { /* <factor> = <primary>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 199: { /* <factor> = <primary> <right_hand_side>; */

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
    case 201: { /* <right_hand_side> = <binop> <primary>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPBIN);
	exp->fields.bin.op = pmSeSt[pmStkP+1].op;
	exp->fields.bin.right = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 202: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 200: { /* <right_hand_side> = <optional_not> <where>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPWHR;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPWHR);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 203: { /* <right_hand_side> = <optional_not> 'ISA' <heritage_id>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPISA;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPISA);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 204: { /* <right_hand_side> = <is> <something>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPATR;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 206: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBTW;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBTW);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 205: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = OP_CONTAINS;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 212: { /* <primary> = '(' <expression> ')'; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 207: { /* <primary> = <optional_minus> Integer; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, EXPINT);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 208: { /* <primary> = STRING; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSTR);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 209: { /* <primary> = <what>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 213: { /* <primary> = <attribute_reference>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 211: { /* <primary> = <aggregate> <where>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPAGR);
	pmSeSt[pmStkP+1].exp->fields.agr.agr	= pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.whr	= pmSeSt[pmStkP+2].whr;
    	break;}
    case 214: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPRND);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 210: { /* <primary> = 'SCORE'; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSCORE);
    	break;}
    case 216: { /* <aggregate> = 'SUM' 'OF' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_SUM;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 217: { /* <aggregate> = 'MAX' 'OF' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_MAX;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 215: { /* <aggregate> = 'COUNT'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_COUNT;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 218: { /* <something> = <optional_not> ID; */

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 219: { /* <what> = ID; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSeSt[pmStkP+1].id->srcp, WHT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 220: { /* <optional_where> =; */

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 221: { /* <optional_where> = <where>; */

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 222: { /* <where> = 'HERE'; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 223: { /* <where> = 'NEARBY'; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 224: { /* <where> = 'AT' <what>; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 225: { /* <where> = 'IN' <what>; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 226: { /* <binop> = '+'; */

	pmSeSt[pmStkP+1].op = OP_PLUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 227: { /* <binop> = '-'; */

	pmSeSt[pmStkP+1].op = OP_MINUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 228: { /* <binop> = '*'; */

	pmSeSt[pmStkP+1].op = OP_MULT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 229: { /* <binop> = '/'; */

	pmSeSt[pmStkP+1].op = OP_DIV;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 230: { /* <relop> = '<>'; */

        pmSeSt[pmStkP+1].op   = OP_NE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 231: { /* <relop> = '='; */

        pmSeSt[pmStkP+1].op   = OP_EQ;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 232: { /* <relop> = '=='; */

        pmSeSt[pmStkP+1].op   = OP_EXACT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 233: { /* <relop> = '>='; */

        pmSeSt[pmStkP+1].op   = OP_GE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 234: { /* <relop> = '<='; */

        pmSeSt[pmStkP+1].op   = OP_LE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 235: { /* <relop> = '>'; */

        pmSeSt[pmStkP+1].op   = OP_GT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 236: { /* <relop> = '<'; */

        pmSeSt[pmStkP+1].op   = OP_LT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 237: { /* <optional_qual> =; */

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 238: { /* <optional_qual> = 'BEFORE'; */

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 239: { /* <optional_qual> = 'AFTER'; */

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 240: { /* <optional_qual> = 'ONLY'; */

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 241: { /* <optional_not> =; */

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 242: { /* <optional_not> = 'NOT'; */

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 243: { /* <optional_id> =; */

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 244: { /* <optional_id> = ID; */

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 245: { /* <ids> = ID; */

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
    	break;}
    case 246: { /* <ids> = <ids> ID; */

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, LIST_ID);
    	break;}
    case 247: { /* <id_list> = ID; */

        pmSeSt[pmStkP+1].str = newstr(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, LIST_ID);
    	break;}
    case 248: { /* <id_list> = <id_list> ',' ID; */

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, LIST_ID);
    	break;}
    case 249: { /* <optional_integer> =; */

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 250: { /* <optional_integer> = Integer; */

        pmSeSt[pmStkP+1].str = newstr(pmSySt[pmStkP+1].chars);
    	break;}
    case 251: { /* <optional_minus> =; */

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 252: { /* <optional_minus> = '-'; */

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 253: { /* <attribute_reference> = ID 'OF' <what>; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 254: { /* <integer_or_id> = Integer; */

	pmSeSt[pmStkP+1].val = val(pmSySt[pmStkP+1].chars);
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 255: { /* <integer_or_id> = ID; */

	pmSeSt[pmStkP+1].val = 0;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 256: { /* ID = Identifier; */

	pmSeSt[pmStkP+1].id = newId(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    default: break; }
}/*pmPaSema()*/

