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
    ResKind resKd;
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

	optBool(pmSeSt[pmStkP+1].id->str, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */

	optenum(pmSeSt[pmStkP+1].id->str, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->str);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */

	optint(pmSeSt[pmStkP+1].id->str, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 11: { /* <unit> = <default>; */
 adv.atrs = combine(adv.atrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 12: { /* <unit> = <object_default>; */
 adv.oatrs = combine(adv.oatrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 13: { /* <unit> = <location_default>; */
 adv.latrs = combine(adv.latrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 14: { /* <unit> = <actor_default>; */
 adv.aatrs = combine(adv.aatrs, pmSeSt[pmStkP+1].atrs); 	break;}
    case 17: { /* <unit> = <synonyms>; */
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns); 	break;}
    case 15: { /* <unit> = <messages>; */
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 18: { /* <unit> = <syntax>; */
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 19: { /* <unit> = <verb>; */
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VRBNOD); 	break;}
    case 20: { /* <unit> = <class>; */
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLANOD); 	break;}
    case 21: { /* <unit> = <instance>; */
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSNOD); 	break;}
    case 22: { /* <unit> = <location>; */
 adv.locs = concat(adv.locs, pmSeSt[pmStkP+1].loc, LOCNOD); 	break;}
    case 23: { /* <unit> = <object>; */
 adv.objs = concat(adv.objs, pmSeSt[pmStkP+1].obj, OBJNOD); 	break;}
    case 26: { /* <unit> = <event>; */
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVTNOD); 	break;}
    case 24: { /* <unit> = <container>; */
 adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, CNTNOD); 	break;}
    case 25: { /* <unit> = <actor>; */
 adv.acts = concat(adv.acts, pmSeSt[pmStkP+1].act, ACTNOD); 	break;}
    case 16: { /* <unit> = <rule>; */
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULNOD); 	break;}
    case 27: { /* <default> = 'DEFAULT' 'ATTRIBUTES' <attributes>; */

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 28: { /* <location_default> = 'LOCATION' 'ATTRIBUTES' <attributes>; */

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 29: { /* <object_default> = 'OBJECT' 'ATTRIBUTES' <attributes>; */

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 30: { /* <actor_default> = 'ACTOR' 'ATTRIBUTES' <attributes>; */

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 31: { /* <attributes> = <attribute> '.'; */

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, ATRNOD);
    	break;}
    case 32: { /* <attributes> = <attributes> <attribute> '.'; */

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATRNOD);
    	break;}
    case 33: { /* <attribute> = ID; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
				TYPBOOL,
				pmSeSt[pmStkP+1].id,
				TRUE, 0, 0);
    	break;}
    case 34: { /* <attribute> = 'NOT' ID; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+2].id->srcp,
				TYPBOOL,
				pmSeSt[pmStkP+2].id,
				FALSE, 0, 0);
    	break;}
    case 35: { /* <attribute> = ID <optional_minus> Integer; */

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
    case 36: { /* <attribute> = ID STRING; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].id->srcp,
				TYPSTR,
				pmSeSt[pmStkP+1].id,
				0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 37: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 38: { /* <synonym_list> = <synonym>; */

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, SYNNOD);
    	break;}
    case 39: { /* <synonym_list> = <synonym_list> <synonym>; */

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNNOD);
    	break;}
    case 40: { /* <synonym> = <id_list> '=' ID '.'; */

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].idList,
			      pmSeSt[pmStkP+3].id);
    	break;}
    case 41: { /* <messages> = 'MESSAGE' <message_list>; */

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 42: { /* <message_list> = <message>; */

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, MSGNOD);
    	break;}
    case 43: { /* <message_list> = <message_list> <message>; */

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MSGNOD);
    	break;}
    case 44: { /* <message> = ID ':' <statements>; */

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].id,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 45: { /* <syntax> = 'SYNTAX' <syntax_list>; */

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 46: { /* <syntax_list> = <syntax_item>; */

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, STXNOD);
    	break;}
    case 47: { /* <syntax_list> = <syntax_list> <syntax_item>; */

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, STXNOD);
    	break;}
    case 48: { /* <syntax_item> = ID '=' <syntax_elements> <optional_class_restrictions>; */

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

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, ELMNOD);
    	break;}
    case 50: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                ELMNOD);
    	break;}
    case 51: { /* <syntax_element> = ID; */

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+1].id->srcp, ELMWRD,
				     pmSeSt[pmStkP+1].id,
				     FALSE);
    	break;}
    case 52: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+2].id->srcp, ELMPAR,
				     pmSeSt[pmStkP+2].id,
				     pmSeSt[pmStkP+4].bits);
    	break;}
    case 53: { /* <optional_indicators> =; */

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 54: { /* <optional_indicators> = <optional_indicators> <indicator>; */

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 55: { /* <indicator> = '*'; */

	pmSeSt[pmStkP+1].bits = 0x1;
    	break;}
    case 56: { /* <indicator> = '!'; */

	pmSeSt[pmStkP+1].bits = 0x2;
    	break;}
    case 57: { /* <optional_class_restrictions> = '.'; */

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 58: { /* <optional_class_restrictions> = 'WHERE' <class_restriction_clauses>; */

	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 59: { /* <class_restriction_clauses> = <class_restriction>; */

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, RESNOD);
    	break;}
    case 60: { /* <class_restriction_clauses> = <class_restriction_clauses> 'AND' <class_restriction>; */

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, RESNOD);
    	break;}
    case 61: { /* <class_restriction> = ID 'ISA' <classes> 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].res = newres(&pmSySt[pmStkP+2].srcp,
			  		pmSeSt[pmStkP+1].id,
				    	pmSeSt[pmStkP+3].single,
				    	pmSeSt[pmStkP+3].resKd,
					pmSeSt[pmStkP+3].idList,
				    	pmSeSt[pmStkP+5].stms);
    	break;}
    case 62: { /* <classes> = <class_id>; */

	pmSeSt[pmStkP+1].resKd = pmSeSt[pmStkP+1].resKd;
        pmSeSt[pmStkP+1].single = TRUE;
	pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, IDNOD);
    	break;}
    case 63: { /* <classes> = <classes> 'OR' <class_id>; */

    	unimpl(&pmSySt[pmStkP+2].srcp, "Parsing");
        pmSeSt[pmStkP+1].single = FALSE;
    	break;}
    case 64: { /* <class_id> = <heritage_id>; */

	if (strcmp(pmSeSt[pmStkP+1].id->str, "actor") == 0)
	    pmSeSt[pmStkP+1].resKd = ACTOR_RESTRICTION;
	else if (strcmp(pmSeSt[pmStkP+1].id->str, "object") == 0)
	    pmSeSt[pmStkP+1].resKd = OBJECT_RESTRICTION;
	else
	    pmSeSt[pmStkP+1].resKd = ID_RESTRICTION;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 67: { /* <class_id> = 'CONTAINER' 'OBJECT'; */

	pmSeSt[pmStkP+1].resKd = CONTAINEROBJECT_RESTRICTION;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 68: { /* <class_id> = 'CONTAINER' 'ACTOR'; */

	pmSeSt[pmStkP+1].resKd = CONTAINERACTOR_RESTRICTION;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 65: { /* <class_id> = 'INTEGER'; */

	pmSeSt[pmStkP+1].resKd = INTEGER_RESTRICTION;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 66: { /* <class_id> = 'STRING'; */

	pmSeSt[pmStkP+1].resKd = STRING_RESTRICTION;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 69: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */

	pmSeSt[pmStkP+1].vrb = newvrb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].idList,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->str) != 0)
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 70: { /* <verb_header> = 'VERB' <id_list>; */

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+2].idList;
    	break;}
    case 71: { /* <verb_body> = <simple_verb_body>; */

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&nulsrcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   ALTNOD);
    	break;}
    case 72: { /* <verb_body> = <verb_alternatives>; */

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 73: { /* <verb_alternatives> = <verb_alternative>; */

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, ALTNOD);
    	break;}
    case 74: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				ALTNOD);
    	break;}
    case 75: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].id,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 76: { /* <simple_verb_body> = <optional_checks> <optional_does>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 77: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 78: { /* <optional_checks> =; */

	pmSeSt[pmStkP+1].chks = NULL;
    	break;}
    case 79: { /* <optional_checks> = 'CHECK' <statements>; */

	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), STMNOD);
    	break;}
    case 80: { /* <optional_checks> = 'CHECK' <check_list>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 81: { /* <check_list> = <check>; */

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, CHKNOD);
    	break;}
    case 82: { /* <check_list> = <check_list> 'AND' <check>; */

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHKNOD);
    	break;}
    case 83: { /* <check> = <expression> 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 84: { /* <optional_does> =; */

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 86: { /* <does> = 'DOES' <optional_qual> <statements>; */

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 87: { /* <location> = <location_header> <location_body> <location_tail>; */

	pmSeSt[pmStkP+1].loc = newloc(&pmSeSt[pmStkP+1].srcp,
			       pmSeSt[pmStkP+1].id,
			       pmSeSt[pmStkP+1].nam,
			       pmSeSt[pmStkP+2].atrs,
			       pmSeSt[pmStkP+2].stms,
			       pmSeSt[pmStkP+2].stms2,
			       pmSeSt[pmStkP+2].exts,
			       pmSeSt[pmStkP+2].vrbs);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR,
			pmSeSt[pmStkP+1].id->str);
	}
    	break;}
    case 88: { /* <location_header> = 'LOCATION' ID <optional_name>; */

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevWAR, "THE <id> ISA LOCATION ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 89: { /* <location_body> =; */

	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
	pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].exts  = NULL;
	pmSeSt[pmStkP+1].vrbs  = NULL;
    	break;}
    case 90: { /* <location_body> = <location_body> <location_body_part>; */

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

        pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].does = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
    	break;}
    case 91: { /* <location_body_part> = <description>; */

        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].does = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 92: { /* <location_body_part> = <does>; */

        pmSeSt[pmStkP+1].does = pmSeSt[pmStkP+1].stms;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+1].qual;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 94: { /* <location_body_part> = <exit>; */

        pmSeSt[pmStkP+1].ext = pmSeSt[pmStkP+1].ext;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].does = NULL;
    	break;}
    case 95: { /* <location_body_part> = <verb>; */

        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].does = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = pmSeSt[pmStkP+1].vrb;
    	break;}
    case 96: { /* <location_tail> = 'END' 'LOCATION' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 97: { /* <class> = 'EVERY' ID <optional_heritage> <slots> <class_tail>; */

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
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 99: { /* <instance> = 'THE' ID <optional_heritage> <slots> <instance tail>; */

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
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 101: { /* <optional_heritage> =; */

	pmSeSt[pmStkP+1].idList = NULL;
    	break;}
    case 102: { /* <optional_heritage> = <heritage>; */

	pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+1].idList;
    	break;}
    case 103: { /* <heritage> = 'ISA' <heritage_list>; */
 pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+2].idList; 	break;}
    case 105: { /* <heritage_list> = <heritage_list> ',' <heritage_id>; */
 pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList,
					pmSeSt[pmStkP+3].id, IDNOD); 	break;}
    case 104: { /* <heritage_list> = <heritage_id>; */
 pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, IDNOD); 	break;}
    case 106: { /* <heritage_id> = ID; */
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id; 	break;}
    case 107: { /* <heritage_id> = 'THING'; */
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "thing"); 	break;}
    case 108: { /* <heritage_id> = 'LOCATION'; */
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "location"); 	break;}
    case 109: { /* <heritage_id> = 'OBJECT'; */
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "object"); 	break;}
    case 110: { /* <heritage_id> = 'ACTOR'; */
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "actor"); 	break;}
    case 111: { /* <heritage_id> = 'CONTAINER'; */
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, "container"); 	break;}
    case 112: { /* <slots> =; */

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
    case 113: { /* <slots> = <slots> <slot>; */

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

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+1].nam;
    	break;}
    case 114: { /* <slot> = <where>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 121: { /* <slot> = <is> <attributes>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 122: { /* <slot> = <container>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+1].cnt;
    	break;}
    case 115: { /* <slot> = <surroundings>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].surr = pmSeSt[pmStkP+1].surr;
    	break;}
    case 116: { /* <slot> = <description>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].dscr = pmSeSt[pmStkP+1].dscr;
    	break;}
    case 119: { /* <slot> = <mentioned>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+1].ment;
    	break;}
    case 117: { /* <slot> = <article>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+1].art;
    	break;}
    case 120: { /* <slot> = <does>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].does = pmSeSt[pmStkP+1].does;
    	break;}
    case 123: { /* <slot> = <exit>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = pmSeSt[pmStkP+1].ext;
    	break;}
    case 124: { /* <slot> = <verb>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = pmSeSt[pmStkP+1].vrb;
    	break;}
    case 125: { /* <slot> = <script>; */

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = pmSeSt[pmStkP+1].scr;
    	break;}
    case 126: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */

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
    case 127: { /* <optional_exit_body> =; */

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id	 = NULL;
    	break;}
    case 128: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 129: { /* <object> = <object_header> <object_body> <object_tail>; */

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
    case 130: { /* <object_header> = 'OBJECT' ID <optional_where> <optional_names> <optional_where>; */

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevWAR, "THE <id> ISA OBJECT ---");
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
    case 131: { /* <object_body> =; */

	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL; /* Description */
        pmSeSt[pmStkP+1].ment = NULL; /* Mentioned */
	pmSeSt[pmStkP+1].art = NULL; /* The article */
	pmSeSt[pmStkP+1].vrbs  = NULL;
    	break;}
    case 132: { /* <object_body> = <object_body> <object_body_part>; */

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
    case 133: { /* <object_body_part> = <properties>; */

	pmSeSt[pmStkP+1].cnt   = pmSeSt[pmStkP+1].cnt;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].art = NULL;
        pmSeSt[pmStkP+1].vrb   = NULL;
    	break;}
    case 137: { /* <object_body_part> = <is> <attributes>; */

	pmSeSt[pmStkP+1].atrs  = pmSeSt[pmStkP+2].atrs;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].art = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
    	break;}
    case 134: { /* <object_body_part> = <description>; */

	pmSeSt[pmStkP+1].dscr  = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].art = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
    	break;}
    case 136: { /* <object_body_part> = <mentioned>; */

        pmSeSt[pmStkP+1].ment = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].art = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
    	break;}
    case 135: { /* <object_body_part> = <article>; */

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+1].art;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].ment = NULL;
    	break;}
    case 138: { /* <object_body_part> = <verb>; */

	pmSeSt[pmStkP+1].vrb   = pmSeSt[pmStkP+1].vrb;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].ment = NULL;
	pmSeSt[pmStkP+1].art = NULL;
    	break;}
    case 139: { /* <object_tail> = 'END' 'OBJECT' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 140: { /* <optional_attributes> =; */

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 141: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 142: { /* <is> = 'IS'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 143: { /* <is> = 'ARE'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 144: { /* <is> = 'HAS'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 145: { /* <surroundings> = 'SURROUNDINGS'; */

	pmSeSt[pmStkP+1].surr = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
						STM_NOP), STMNOD);
    	break;}
    case 146: { /* <surroundings> = 'SURROUNDINGS' <statements>; */

	pmSeSt[pmStkP+1].surr = pmSeSt[pmStkP+2].stms;
    	break;}
    case 147: { /* <optional_description> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 149: { /* <description> = 'DESCRIPTION'; */

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), STMNOD);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 150: { /* <description> = 'DESCRIPTION' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 151: { /* <article> = 'ARTICLE'; */

	pmSeSt[pmStkP+1].art = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), STMNOD);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 152: { /* <article> = 'ARTICLE' <statements>; */

	pmSeSt[pmStkP+1].art = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 153: { /* <mentioned> = 'MENTIONED' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 154: { /* <optional_name> =; */

	pmSeSt[pmStkP+1].nam = NULL;
    	break;}
    case 155: { /* <optional_name> = <name>; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+1].nam;
    	break;}
    case 156: { /* <optional_names> =; */

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 157: { /* <optional_names> = <optional_names> <name>; */

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams,
					     pmSeSt[pmStkP+2].nam, NAMNOD);
    	break;}
    case 158: { /* <name> = 'NAME' <ids>; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 159: { /* <properties> = 'CONTAINER' <container_body>; */

	pmSeSt[pmStkP+1].cnt = newcnt(&pmSySt[pmStkP+1].srcp,
					  NULL,
					  pmSeSt[pmStkP+2].lims,
					  pmSeSt[pmStkP+2].stms,
					  pmSeSt[pmStkP+2].stms2);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, CNTNOD);
    	break;}
    case 160: { /* <container> = <container_header> <container_body> <container_tail>; */

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
    case 161: { /* <container_header> = 'CONTAINER' ID; */

	lmLog(&pmSySt[pmStkP+1].srcp, 800, sevWAR, "THE <id> ISA CONTAINER ---");
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 162: { /* <container_body> = <optional_limits> <optional_header> <optional_empty>; */

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
    	break;}
    case 163: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+3].id;
    	break;}
    case 164: { /* <optional_limits> =; */

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 165: { /* <optional_limits> = 'LIMITS' <limits>; */

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 166: { /* <limits> = <limit>; */

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIMNOD);
    	break;}
    case 167: { /* <limits> = <limits> <limit>; */

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMNOD);
    	break;}
    case 168: { /* <limit> = <limit_attribute> 'THEN' <statements>; */

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 169: { /* <limit_attribute> = <attribute>; */

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 170: { /* <limit_attribute> = 'COUNT' Integer; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSySt[pmStkP+1].srcp,
				      TYPINT,
				      newnam(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 171: { /* <optional_header> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 172: { /* <optional_header> = 'HEADER' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 173: { /* <optional_empty> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 174: { /* <optional_empty> = 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 175: { /* <event> = <event_header> <statements> <event_tail>; */

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].id,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 201, sevWAR, pmSeSt[pmStkP+1].id->str);
	}
    	break;}
    case 176: { /* <event_header> = 'EVENT' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 177: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 178: { /* <actor> = <actor_header> <actor_body> <actor_tail>; */

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
    case 179: { /* <actor_header> = 'ACTOR' ID <optional_where> <optional_names> <optional_where>; */

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
    case 180: { /* <actor_body> =; */

	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL; /* Description */
	pmSeSt[pmStkP+1].vrbs  = NULL;
        pmSeSt[pmStkP+1].scrs  = NULL; /* The scripts */
    	break;}
    case 181: { /* <actor_body> = <actor_body> <actor_body_part>; */

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
    case 182: { /* <actor_body_part> = <properties>; */

	pmSeSt[pmStkP+1].cnt   = pmSeSt[pmStkP+1].cnt;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
        pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
    	break;}
    case 184: { /* <actor_body_part> = <is> <attributes>; */

	pmSeSt[pmStkP+1].atrs  = pmSeSt[pmStkP+2].atrs;
	pmSeSt[pmStkP+1].dscr  = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
    	break;}
    case 183: { /* <actor_body_part> = <description>; */

	pmSeSt[pmStkP+1].dscr  = pmSeSt[pmStkP+1].dscr;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
    	break;}
    case 185: { /* <actor_body_part> = <verb>; */

	pmSeSt[pmStkP+1].vrb   = pmSeSt[pmStkP+1].vrb;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
    	break;}
    case 186: { /* <actor_body_part> = <script>; */

	pmSeSt[pmStkP+1].scr   = pmSeSt[pmStkP+1].scr;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].dscr  = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
    	break;}
    case 187: { /* <actor_tail> = 'END' 'ACTOR' <optional_id> '.'; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 188: { /* <script> = 'SCRIPT' <integer_or_id> '.' <optional_description> <step_list>; */

	pmSeSt[pmStkP+1].scr = newscr(&pmSySt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+2].id,
			     pmSeSt[pmStkP+2].val,
			     pmSeSt[pmStkP+4].stms,
			     pmSeSt[pmStkP+5].stps);
    	break;}
    case 189: { /* <step_list> = <step>; */

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, STPNOD);
    	break;}
    case 190: { /* <step_list> = <step_list> <step>; */

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STPNOD);
    	break;}
    case 191: { /* <step> = 'STEP' <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 192: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 193: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 194: { /* <rule> = 'WHEN' <expression> '=>' <statements>; */

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 195: { /* <start> = 'START' <where> '.' <optional_statements>; */

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 196: { /* <optional_statements> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 197: { /* <optional_statements> = <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 198: { /* <statements> = <statement>; */

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, STMNOD);
    	break;}
    case 199: { /* <statements> = <statements> <statement>; */

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STMNOD);
    	break;}
    case 200: { /* <statement> = <output_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 202: { /* <statement> = <manipulation_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 204: { /* <statement> = <assignment_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 206: { /* <statement> = <comparison_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 205: { /* <statement> = <actor_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 203: { /* <statement> = <event_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 201: { /* <statement> = <special_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 207: { /* <output_statement> = STRING; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_PRINT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 208: { /* <output_statement> = 'DESCRIBE' <what> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DESCRIBE);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 209: { /* <output_statement> = 'SAY' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAY);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 210: { /* <output_statement> = 'LIST' <what> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LIST);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 211: { /* <special_statement> = 'QUIT' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_QUIT);
    	break;}
    case 212: { /* <special_statement> = 'LOOK' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOOK);
    	break;}
    case 213: { /* <special_statement> = 'SAVE' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAVE);
    	break;}
    case 214: { /* <special_statement> = 'RESTORE' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTORE);
    	break;}
    case 215: { /* <special_statement> = 'RESTART' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTART);
    	break;}
    case 216: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCORE);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 217: { /* <special_statement> = 'VISITS' Integer '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_VISITS);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 218: { /* <special_statement> = 'SYSTEM' STRING '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SYSTEM);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 219: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_EMPTY);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 220: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOCATE);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 222: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCHEDULE);
	pmSeSt[pmStkP+1].stm->fields.schedule.id  = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 221: { /* <event_statement> = 'CANCEL' ID '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_CANCEL);
	pmSeSt[pmStkP+1].stm->fields.cancel.id = pmSeSt[pmStkP+2].id;
    	break;}
    case 223: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_MAKE);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 226: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SET);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 224: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_INCR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 225: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DECR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 227: { /* <optional_by_clause> =; */

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 228: { /* <optional_by_clause> = 'BY' <expression>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 229: { /* <comparison_statement> = <if_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 230: { /* <comparison_statement> = <depending_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 231: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */

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
    case 232: { /* <optional_elsif_list> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 233: { /* <optional_elsif_list> = <elsif_list>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 234: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_IF);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, STMNOD);
    }	break;}
    case 235: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
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
    case 236: { /* <optional_else_part> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 237: { /* <optional_else_part> = 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 238: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' 'DEPEND' '.'; */
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPEND);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 239: { /* <depend_cases> = <depend_case>; */

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, CASENOD);
    	break;}
    case 240: { /* <depend_cases> = <depend_cases> <depend_case>; */

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASENOD);
    	break;}
    case 242: { /* <depend_case> = <right_hand_side> ':' <statements>; */

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 241: { /* <depend_case> = 'ELSE' <statements>; */

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 243: { /* <actor_statement> = 'USE' 'SCRIPT' <integer_or_id> <optional_for_actor> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_USE);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].stm->fields.use.scriptno = pmSeSt[pmStkP+3].val;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].id;
    	break;}
    case 244: { /* <optional_for_actor> =; */

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 245: { /* <optional_for_actor> = 'FOR' ID; */

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 246: { /* <expression> = <term>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 247: { /* <expression> = <expression> 'OR' <term>; */
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_OR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 248: { /* <term> = <factor>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 249: { /* <term> = <term> 'AND' <factor>; */
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_AND;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 250: { /* <factor> = <primary>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 251: { /* <factor> = <primary> <right_hand_side>; */

        /* <right_hand_side> contains the top expr, hang in the <primary> and
           propagate that node */
        switch (pmSeSt[pmStkP+2].expKd) {
	case EXPBIN: pmSeSt[pmStkP+2].exp->fields.bin.left = pmSeSt[pmStkP+1].exp; break;
	case EXPWHR: pmSeSt[pmStkP+2].exp->fields.whr.wht = pmSeSt[pmStkP+1].exp; break;
	case EXPATR: pmSeSt[pmStkP+2].exp->fields.atr.wht = pmSeSt[pmStkP+1].exp; break;
	case EXPBTW: pmSeSt[pmStkP+2].exp->fields.btw.val = pmSeSt[pmStkP+1].exp; break;
	default: syserr("Unrecognized switch in <right_hand_side> semantic rule.");
	}
        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 253: { /* <right_hand_side> = <binop> <primary>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPBIN);
	exp->fields.bin.op = pmSeSt[pmStkP+1].op;
	exp->fields.bin.right = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 254: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 252: { /* <right_hand_side> = <optional_not> <where>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPWHR;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPWHR);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 255: { /* <right_hand_side> = <optional_not> 'ISA' ID; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPISA;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPISA);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.isa.id = pmSeSt[pmStkP+3].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 256: { /* <right_hand_side> = <is> <something>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPATR;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].id;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 258: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBTW;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBTW);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 257: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expKd = EXPBIN;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = OP_CONTAINS;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 264: { /* <primary> = '(' <expression> ')'; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 259: { /* <primary> = <optional_minus> Integer; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, EXPINT);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 260: { /* <primary> = STRING; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSTR);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 261: { /* <primary> = <what>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 265: { /* <primary> = <attribute_reference>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 263: { /* <primary> = <aggregate> <where>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPAGR);
	pmSeSt[pmStkP+1].exp->fields.agr.agr	= pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.atrnam = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].exp->fields.agr.whr	= pmSeSt[pmStkP+2].whr;
    	break;}
    case 266: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPRND);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 262: { /* <primary> = 'SCORE'; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSCORE);
    	break;}
    case 268: { /* <aggregate> = 'SUM' 'OF' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_SUM;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 269: { /* <aggregate> = 'MAX' 'OF' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_MAX;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 267: { /* <aggregate> = 'COUNT'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_COUNT;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 270: { /* <something> = <optional_not> ID; */

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 271: { /* <what> = 'OBJECT'; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_OBJ, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 272: { /* <what> = 'LOCATION'; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_LOC, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 273: { /* <what> = 'ACTOR'; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_ACT, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 274: { /* <what> = ID; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSeSt[pmStkP+1].id->srcp, WHT_ID, pmSeSt[pmStkP+1].id);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 275: { /* <optional_where> =; */

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 276: { /* <optional_where> = <where>; */

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 277: { /* <where> = 'HERE'; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 278: { /* <where> = 'NEARBY'; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 279: { /* <where> = 'AT' <what>; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 280: { /* <where> = 'IN' <what>; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 281: { /* <binop> = '+'; */

	pmSeSt[pmStkP+1].op = OP_PLUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 282: { /* <binop> = '-'; */

	pmSeSt[pmStkP+1].op = OP_MINUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 283: { /* <binop> = '*'; */

	pmSeSt[pmStkP+1].op = OP_MULT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 284: { /* <binop> = '/'; */

	pmSeSt[pmStkP+1].op = OP_DIV;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 285: { /* <relop> = '<>'; */

        pmSeSt[pmStkP+1].op   = OP_NE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 286: { /* <relop> = '='; */

        pmSeSt[pmStkP+1].op   = OP_EQ;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 287: { /* <relop> = '=='; */

        pmSeSt[pmStkP+1].op   = OP_EXACT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 288: { /* <relop> = '>='; */

        pmSeSt[pmStkP+1].op   = OP_GE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 289: { /* <relop> = '<='; */

        pmSeSt[pmStkP+1].op   = OP_LE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 290: { /* <relop> = '>'; */

        pmSeSt[pmStkP+1].op   = OP_GT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 291: { /* <relop> = '<'; */

        pmSeSt[pmStkP+1].op   = OP_LT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 292: { /* <optional_qual> =; */

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 293: { /* <optional_qual> = 'BEFORE'; */

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 294: { /* <optional_qual> = 'AFTER'; */

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 295: { /* <optional_qual> = 'ONLY'; */

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 296: { /* <optional_not> =; */

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 297: { /* <optional_not> = 'NOT'; */

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 298: { /* <optional_id> =; */

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 299: { /* <optional_id> = ID; */

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 300: { /* <ids> = ID; */

        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, IDNOD);
    	break;}
    case 301: { /* <ids> = <ids> ID; */

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, IDNOD);
    	break;}
    case 302: { /* <id_list> = ID; */

        pmSeSt[pmStkP+1].str = newstr(pmSeSt[pmStkP+1].id->str);
        pmSeSt[pmStkP+1].idList = concat(NULL, pmSeSt[pmStkP+1].id, IDNOD);
    	break;}
    case 303: { /* <id_list> = <id_list> ',' ID; */

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, IDNOD);
    	break;}
    case 304: { /* <optional_integer> =; */

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 305: { /* <optional_integer> = Integer; */

        pmSeSt[pmStkP+1].str = newstr(pmSySt[pmStkP+1].chars);
    	break;}
    case 306: { /* <optional_minus> =; */

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 307: { /* <optional_minus> = '-'; */

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 308: { /* <attribute_reference> = ID 'OF' <what>; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 309: { /* <integer_or_id> = Integer; */

	pmSeSt[pmStkP+1].val = val(pmSySt[pmStkP+1].chars);
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 310: { /* <integer_or_id> = ID; */

	pmSeSt[pmStkP+1].val = 0;
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 311: { /* ID = Identifier; */
 pmSeSt[pmStkP+1].id = newnam(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars); 	break;}
    default: break; }
}/*pmPaSema()*/

