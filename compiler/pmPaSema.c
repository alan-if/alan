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
#include "cnt.h"		/* CNT-nodes */
#include "elm.h"                /* ELM-nodes */
#include "evt.h"                /* EVT-nodes */
#include "exp.h"                /* EXP-nodes */
#include "ext.h"                /* EXT-nodes */
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
    int bits;
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
    List *nams2;
    List *nams;
    NamNod *nam;
    List *msgs;
    MsgNod *msg;
    Bool minus;
    List *locs;
    LocNod *loc;
    List *lims;
    LimNod *lim;
    List *exts;
    ExtNod *ext;
   ExpKind expkd;
    ExpNod *exp;
    List *evts;
    EvtNod *evt;
    List *elms;
    ElmNod *elm;
    List *cnts;
    CntNod *cnt;
    NamKind classkd;
    List *chks;
    ChkNod *chk;
    List *cases;
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

	optBool(pmSeSt[pmStkP+1].nam->str, &pmSeSt[pmStkP+1].nam->srcp, TRUE);
    	break;}
    case 7: { /* <option> = ID ID '.'; */

	optenum(pmSeSt[pmStkP+1].nam->str, &pmSeSt[pmStkP+1].nam->srcp, pmSeSt[pmStkP+2].nam->str);
    	break;}
    case 8: { /* <option> = ID Integer '.'; */

	optint(pmSeSt[pmStkP+1].nam->str, &pmSeSt[pmStkP+1].nam->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 11: { /* <unit> = <default>; */

	adv.atrs = combine(adv.atrs, pmSeSt[pmStkP+1].atrs);
    	break;}
    case 12: { /* <unit> = <object_default>; */

	adv.oatrs = combine(adv.oatrs, pmSeSt[pmStkP+1].atrs);
    	break;}
    case 13: { /* <unit> = <location_default>; */

	adv.latrs = combine(adv.latrs, pmSeSt[pmStkP+1].atrs);
    	break;}
    case 14: { /* <unit> = <actor_default>; */

	adv.aatrs = combine(adv.aatrs, pmSeSt[pmStkP+1].atrs);
    	break;}
    case 17: { /* <unit> = <synonyms>; */

	adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns);
    	break;}
    case 15: { /* <unit> = <messages>; */

	adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs);
    	break;}
    case 18: { /* <unit> = <syntax>; */

	adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs);
    	break;}
    case 19: { /* <unit> = <verb>; */

	adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VRBNOD);
    	break;}
    case 20: { /* <unit> = <location>; */

	adv.locs = concat(adv.locs, pmSeSt[pmStkP+1].loc, LOCNOD);
    	break;}
    case 21: { /* <unit> = <object>; */

	adv.objs = concat(adv.objs, pmSeSt[pmStkP+1].obj, OBJNOD);
    	break;}
    case 24: { /* <unit> = <event>; */

	adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVTNOD);
    	break;}
    case 22: { /* <unit> = <container>; */

	adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, CNTNOD);
    	break;}
    case 23: { /* <unit> = <actor>; */

	adv.acts = concat(adv.acts, pmSeSt[pmStkP+1].act, ACTNOD);
    	break;}
    case 16: { /* <unit> = <rule>; */

	adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULNOD);
    	break;}
    case 25: { /* <default> = 'DEFAULT' 'ATTRIBUTES' <attributes>; */

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 26: { /* <location_default> = 'LOCATION' 'ATTRIBUTES' <attributes>; */

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 27: { /* <object_default> = 'OBJECT' 'ATTRIBUTES' <attributes>; */

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 28: { /* <actor_default> = 'ACTOR' 'ATTRIBUTES' <attributes>; */

	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+3].atrs;
    	break;}
    case 29: { /* <attributes> = <attribute> '.'; */

	pmSeSt[pmStkP+1].atrs = concat(NULL, pmSeSt[pmStkP+1].atr, ATRNOD);
    	break;}
    case 30: { /* <attributes> = <attributes> <attribute> '.'; */

	pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATRNOD);
    	break;}
    case 31: { /* <attribute> = ID; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].nam->srcp,
				TYPBOOL,
				pmSeSt[pmStkP+1].nam,
				TRUE, 0, 0);
    	break;}
    case 32: { /* <attribute> = 'NOT' ID; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+2].nam->srcp,
				TYPBOOL,
				pmSeSt[pmStkP+2].nam,
				FALSE, 0, 0);
    	break;}
    case 33: { /* <attribute> = ID <optional_minus> Integer; */

	if (pmSeSt[pmStkP+2].minus)
		pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].nam->srcp,
					TYPINT,
					pmSeSt[pmStkP+1].nam,
					-val(pmSySt[pmStkP+3].chars), 0, 0);
	else
		pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].nam->srcp,
					TYPINT,
					pmSeSt[pmStkP+1].nam,
					val(pmSySt[pmStkP+3].chars), 0, 0);
    	break;}
    case 34: { /* <attribute> = ID STRING; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSeSt[pmStkP+1].nam->srcp,
				TYPSTR,
				pmSeSt[pmStkP+1].nam,
				0, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 35: { /* <synonyms> = 'SYNONYMS' <synonym_list>; */

	pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 36: { /* <synonym_list> = <synonym>; */

	pmSeSt[pmStkP+1].syns = concat(NULL, pmSeSt[pmStkP+1].syn, SYNNOD);
    	break;}
    case 37: { /* <synonym_list> = <synonym_list> <synonym>; */

	pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNNOD);
    	break;}
    case 38: { /* <synonym> = <id_list> '=' ID '.'; */

	pmSeSt[pmStkP+1].syn = newsyn(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].nams,
			      pmSeSt[pmStkP+3].nam);
    	break;}
    case 39: { /* <messages> = 'MESSAGE' <message_list>; */

	pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 40: { /* <message_list> = <message>; */

	pmSeSt[pmStkP+1].msgs = concat(NULL, pmSeSt[pmStkP+1].msg, MSGNOD);
    	break;}
    case 41: { /* <message_list> = <message_list> <message>; */

	pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MSGNOD);
    	break;}
    case 42: { /* <message> = ID ':' <statements>; */

	pmSeSt[pmStkP+1].msg = newmsg(&pmSySt[pmStkP+2].srcp,
			      pmSeSt[pmStkP+1].nam,
			      pmSeSt[pmStkP+3].stms);
    	break;}
    case 43: { /* <syntax> = 'SYNTAX' <syntax_list>; */

	pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 44: { /* <syntax_list> = <syntax_item>; */

	pmSeSt[pmStkP+1].stxs = concat(NULL, pmSeSt[pmStkP+1].stx, STXNOD);
    	break;}
    case 45: { /* <syntax_list> = <syntax_list> <syntax_item>; */

	pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, STXNOD);
    	break;}
    case 46: { /* <syntax_item> = ID '=' <syntax_elements> <optional_class_restrictions>; */

	pmSeSt[pmStkP+1].stx = newstx(&pmSySt[pmStkP+2].srcp,
				  pmSeSt[pmStkP+1].nam,
				  concat(pmSeSt[pmStkP+3].elms,
				         newelm(&pmSeSt[pmStkP+1].nam->srcp, ELMEOS,
				                NULL,
				                FALSE),
				         ELMNOD),
				  pmSeSt[pmStkP+4].ress);
    	break;}
    case 47: { /* <syntax_elements> = <syntax_element>; */

	pmSeSt[pmStkP+1].elms = concat(NULL, pmSeSt[pmStkP+1].elm, ELMNOD);
    	break;}
    case 48: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */

	pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
				       pmSeSt[pmStkP+2].elm,
	                                ELMNOD);
    	break;}
    case 49: { /* <syntax_element> = ID; */

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+1].nam->srcp, ELMWRD,
				     pmSeSt[pmStkP+1].nam,
				     FALSE);
    	break;}
    case 50: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */

	pmSeSt[pmStkP+1].elm = newelm(&pmSeSt[pmStkP+2].nam->srcp, ELMPAR,
				     pmSeSt[pmStkP+2].nam,
				     pmSeSt[pmStkP+4].bits);
    	break;}
    case 51: { /* <optional_indicators> =; */

	pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 52: { /* <optional_indicators> = <optional_indicators> <indicator>; */

	pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 53: { /* <indicator> = '*'; */

	pmSeSt[pmStkP+1].bits = 0x1;
    	break;}
    case 54: { /* <indicator> = '!'; */

	pmSeSt[pmStkP+1].bits = 0x2;
    	break;}
    case 55: { /* <optional_class_restrictions> = '.'; */

	pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 56: { /* <optional_class_restrictions> = 'WHERE' <class_restrictions>; */

	pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 57: { /* <class_restrictions> = <class_restriction>; */

	pmSeSt[pmStkP+1].ress = concat(NULL, pmSeSt[pmStkP+1].res, RESNOD);
    	break;}
    case 58: { /* <class_restrictions> = <class_restrictions> 'AND' <class_restriction>; */

	pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
				       pmSeSt[pmStkP+3].res, RESNOD);
    	break;}
    case 59: { /* <class_restriction> = ID 'ISA' <classes> 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].res = newres(&pmSySt[pmStkP+2].srcp,
				    pmSeSt[pmStkP+1].nam,
				    pmSeSt[pmStkP+3].single,
				    pmSeSt[pmStkP+3].classkd,
				    pmSeSt[pmStkP+5].stms);
    	break;}
    case 60: { /* <classes> = <class_identifier>; */

	pmSeSt[pmStkP+1].classkd = pmSeSt[pmStkP+1].classkd;
        pmSeSt[pmStkP+1].single = TRUE;
    	break;}
    case 61: { /* <classes> = <classes> 'OR' <class_identifier>; */

	pmSeSt[pmStkP+1].classkd = pmSeSt[pmStkP+1].classkd | pmSeSt[pmStkP+3].classkd;
        pmSeSt[pmStkP+1].single = FALSE;
    	break;}
    case 62: { /* <class_identifier> = 'OBJECT'; */

	pmSeSt[pmStkP+1].classkd = NAMOBJ;
    	break;}
    case 63: { /* <class_identifier> = 'ACTOR'; */

	pmSeSt[pmStkP+1].classkd = NAMACT;
    	break;}
    case 64: { /* <class_identifier> = 'CONTAINER'; */

	pmSeSt[pmStkP+1].classkd = NAMCNT;
    	break;}
    case 67: { /* <class_identifier> = 'CONTAINER' 'OBJECT'; */

	pmSeSt[pmStkP+1].classkd = NAMCOBJ;
    	break;}
    case 68: { /* <class_identifier> = 'CONTAINER' 'ACTOR'; */

	pmSeSt[pmStkP+1].classkd = NAMCACT;
    	break;}
    case 65: { /* <class_identifier> = 'INTEGER'; */

	pmSeSt[pmStkP+1].classkd = NAMNUM;
    	break;}
    case 66: { /* <class_identifier> = 'STRING'; */

	pmSeSt[pmStkP+1].classkd = NAMSTR;
    	break;}
    case 69: { /* <optional_verbs> =; */

	pmSeSt[pmStkP+1].vrbs = NULL;
    	break;}
    case 70: { /* <optional_verbs> = <optional_verbs> <verb>; */

	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VRBNOD);
    	break;}
    case 71: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */

	pmSeSt[pmStkP+1].vrb = newvrb(&pmSeSt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+1].nams,
			   pmSeSt[pmStkP+2].alts);
	if (pmSeSt[pmStkP+3].nam != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].nam->str) != 0)
		lmLog(&pmSeSt[pmStkP+3].nam->srcp, 201, sevWAR, pmSeSt[pmStkP+1].str);
	}
    	break;}
    case 72: { /* <verb_header> = 'VERB' <id_list>; */

	pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+2].str;
	pmSeSt[pmStkP+1].nams   = pmSeSt[pmStkP+2].nams;
    	break;}
    case 73: { /* <verb_body> = <simple_verb_body>; */

	pmSeSt[pmStkP+1].alts = concat(NULL,
				 newalt(&nulsrcp,
					NULL,
					pmSeSt[pmStkP+1].chks,
					pmSeSt[pmStkP+1].qual,
					pmSeSt[pmStkP+1].stms),
				   ALTNOD);
    	break;}
    case 74: { /* <verb_body> = <verb_alternatives>; */

	pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 75: { /* <verb_alternatives> = <verb_alternative>; */

	pmSeSt[pmStkP+1].alts = concat(NULL, pmSeSt[pmStkP+1].alt, ALTNOD);
    	break;}
    case 76: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */

	pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
					 pmSeSt[pmStkP+2].alt,
	     				ALTNOD);
    	break;}
    case 77: { /* <verb_alternative> = 'WHEN' ID <simple_verb_body>; */

	pmSeSt[pmStkP+1].alt = newalt(&pmSySt[pmStkP+1].srcp,
				       pmSeSt[pmStkP+2].nam,
				       pmSeSt[pmStkP+3].chks,
				       pmSeSt[pmStkP+3].qual,
				       pmSeSt[pmStkP+3].stms);
    	break;}
    case 78: { /* <simple_verb_body> = <optional_checks> <optional_does>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 79: { /* <verb_tail> = 'END' 'VERB' <optional_id> '.'; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 80: { /* <optional_checks> =; */

	pmSeSt[pmStkP+1].chks = NULL;
    	break;}
    case 81: { /* <optional_checks> = 'CHECK' <statements>; */

	pmSeSt[pmStkP+1].chks = concat(NULL, newchk(NULL, pmSeSt[pmStkP+2].stms), STMNOD);
    	break;}
    case 82: { /* <optional_checks> = 'CHECK' <check_list>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 83: { /* <check_list> = <check>; */

	pmSeSt[pmStkP+1].chks = concat(NULL, pmSeSt[pmStkP+1].chk, CHKNOD);
    	break;}
    case 84: { /* <check_list> = <check_list> 'AND' <check>; */

	pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHKNOD);
    	break;}
    case 85: { /* <check> = <expression> 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].chk = newchk(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 86: { /* <optional_does> =; */

	pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 88: { /* <does> = 'DOES' <optional_qual> <statements>; */

	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 89: { /* <location> = <location_header> <location_body> <location_tail>; */

	pmSeSt[pmStkP+1].loc = newloc(&pmSeSt[pmStkP+1].srcp,
			       pmSeSt[pmStkP+1].nam,
			       pmSeSt[pmStkP+1].nams,
			       pmSeSt[pmStkP+2].atrs,
			       pmSeSt[pmStkP+2].stms,
			       pmSeSt[pmStkP+2].stms2,
			       pmSeSt[pmStkP+2].exts,
			       pmSeSt[pmStkP+2].vrbs);
	if (pmSeSt[pmStkP+3].nam != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].nam, pmSeSt[pmStkP+3].nam))
		lmLog(&pmSeSt[pmStkP+3].nam->srcp, 201, sevWAR,
			pmSeSt[pmStkP+1].nam->str);
	}
    	break;}
    case 90: { /* <location_header> = 'LOCATION' ID <optional_name>; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].nam  = pmSeSt[pmStkP+2].nam;
	pmSeSt[pmStkP+1].nams = pmSeSt[pmStkP+3].nams;
    	break;}
    case 91: { /* <location_body> =; */

	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
	pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].exts  = NULL;
	pmSeSt[pmStkP+1].vrbs  = NULL;
    	break;}
    case 92: { /* <location_body> = <location_body> <location_body_part>; */

      if (pmSeSt[pmStkP+2].atrs != NULL)
	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);
      else if (pmSeSt[pmStkP+2].stms != NULL)
	if (pmSeSt[pmStkP+1].stms != NULL)
          /* DESCRIPTION Already declared */
	  lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "location", NULL);
        else
	  pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
      else if (pmSeSt[pmStkP+2].stms2 != NULL)
	if (pmSeSt[pmStkP+1].stms2 != NULL)
          /* DOES Already declared */
	  lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DOES", "location", NULL);
        else {
	  pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+2].stms2;
	  if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	    lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	}
      else if (pmSeSt[pmStkP+2].ext != NULL)
	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXTNOD);
      else
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VRBNOD);
    	break;}
    case 95: { /* <location_body_part> = <is> <attributes>; */

        pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].stms2 = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
    	break;}
    case 93: { /* <location_body_part> = <description>; */

        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms2 = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 94: { /* <location_body_part> = <does>; */

        pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+1].stms;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+1].qual;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 96: { /* <location_body_part> = <exit>; */

        pmSeSt[pmStkP+1].ext = pmSeSt[pmStkP+1].ext;
        pmSeSt[pmStkP+1].vrb = NULL;
        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].stms2 = NULL;
    	break;}
    case 97: { /* <location_body_part> = <verb>; */

        pmSeSt[pmStkP+1].atrs = NULL;
        pmSeSt[pmStkP+1].stms = NULL;
        pmSeSt[pmStkP+1].stms2 = NULL;
        pmSeSt[pmStkP+1].ext = NULL;
        pmSeSt[pmStkP+1].vrb = pmSeSt[pmStkP+1].vrb;
    	break;}
    case 98: { /* <location_tail> = 'END' 'LOCATION' <optional_id> '.'; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 99: { /* <optional_exits> =; */

	pmSeSt[pmStkP+1].exts = NULL;
    	break;}
    case 100: { /* <optional_exits> = <optional_exits> <exit>; */

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXTNOD);
    	break;}
    case 101: { /* <exit> = 'EXIT' <id_list> 'TO' ID <optional_exit_body> '.'; */

	pmSeSt[pmStkP+1].ext = newext(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].nams,
			   pmSeSt[pmStkP+4].nam,
			   pmSeSt[pmStkP+5].chks,
			   pmSeSt[pmStkP+5].stms);
	if (pmSeSt[pmStkP+5].nam != NULL) { /* END-id given */
	    if (strcmp(pmSeSt[pmStkP+2].str, pmSeSt[pmStkP+5].nam->str) != 0)
		lmLog(&pmSeSt[pmStkP+5].nam->srcp, 201, sevWAR, pmSeSt[pmStkP+2].str);
	}
    	break;}
    case 102: { /* <optional_exit_body> =; */

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].nam	 = NULL;
    	break;}
    case 103: { /* <optional_exit_body> = <optional_checks> <optional_does> 'END' 'EXIT' <optional_id>; */

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+5].nam;
    	break;}
    case 104: { /* <object> = <object_header> <object_body> <object_tail>; */

	pmSeSt[pmStkP+1].obj = newobj(&pmSeSt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+1].nam,
			     pmSeSt[pmStkP+1].nams2,
			     pmSeSt[pmStkP+1].whr,
			     pmSeSt[pmStkP+2].cnt,
			     pmSeSt[pmStkP+2].atrs,
			     pmSeSt[pmStkP+2].stms,
			     pmSeSt[pmStkP+2].stms3,
			     pmSeSt[pmStkP+2].stms2,
			     pmSeSt[pmStkP+2].vrbs);
	if (pmSeSt[pmStkP+3].nam != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].nam, pmSeSt[pmStkP+3].nam))
		lmLog(&pmSeSt[pmStkP+3].nam->srcp, 201, sevWAR, pmSeSt[pmStkP+1].nam->str);
	}
    	break;}
    case 105: { /* <object_header> = 'OBJECT' ID <optional_where> <optional_names> <optional_where>; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].nam  = pmSeSt[pmStkP+2].nam;
	pmSeSt[pmStkP+1].nams2 = pmSeSt[pmStkP+4].nams2;
        if (pmSeSt[pmStkP+3].whr->whr != WHR_DEFAULT) {
          pmSeSt[pmStkP+1].whr  = pmSeSt[pmStkP+3].whr;
          if (pmSeSt[pmStkP+5].whr->whr != WHR_DEFAULT)
            lmLog(&pmSeSt[pmStkP+5].whr->srcp, 340, sevWAR, pmSeSt[pmStkP+2].nam->str);
        } else
          pmSeSt[pmStkP+1].whr  = pmSeSt[pmStkP+5].whr;
    	break;}
    case 106: { /* <object_body> =; */

	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL; /* Description */
        pmSeSt[pmStkP+1].stms2 = NULL; /* Mentioned */
	pmSeSt[pmStkP+1].stms3 = NULL; /* The article */
	pmSeSt[pmStkP+1].vrbs  = NULL;
    	break;}
    case 107: { /* <object_body> = <object_body> <object_body_part>; */

	if (pmSeSt[pmStkP+2].cnt != NULL)
          if (pmSeSt[pmStkP+1].cnt != NULL)
            /* CONTAINER Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "CONTAINER", "object", NULL);
          else
	    pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+2].cnt;
        else if (pmSeSt[pmStkP+2].atrs != NULL)
          pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);
	else if (pmSeSt[pmStkP+2].stms != NULL)
          if (pmSeSt[pmStkP+1].stms != NULL)
            /* DESCRIPTION Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "object", NULL);
	  else
	    pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
        else if (pmSeSt[pmStkP+2].stms2 != NULL)
          if (pmSeSt[pmStkP+1].stms2 != NULL)
            /* MENTIONED Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "MENTIONED", "object", NULL);
          else
	    pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+2].stms2;
        else if (pmSeSt[pmStkP+2].stms3 != NULL)
          if (pmSeSt[pmStkP+1].stms3 != NULL)
            /* ARTICLE Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "ARTICLE", "object", NULL);
          else
	    pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+2].stms3;
        else
 	  pmSeSt[pmStkP+1].vrbs  = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VRBNOD);
    	break;}
    case 108: { /* <object_body_part> = <properties>; */

	pmSeSt[pmStkP+1].cnt   = pmSeSt[pmStkP+1].cnt;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
        pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].stms3 = NULL;
        pmSeSt[pmStkP+1].vrb   = NULL;
    	break;}
    case 112: { /* <object_body_part> = <is> <attributes>; */

	pmSeSt[pmStkP+1].atrs  = pmSeSt[pmStkP+2].atrs;
	pmSeSt[pmStkP+1].stms  = NULL;
        pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].stms3 = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
    	break;}
    case 109: { /* <object_body_part> = <description>; */

	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].stms3 = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
    	break;}
    case 111: { /* <object_body_part> = <mentioned>; */

        pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].stms3 = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
    	break;}
    case 110: { /* <object_body_part> = <article>; */

	pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
        pmSeSt[pmStkP+1].stms2 = NULL;
    	break;}
    case 113: { /* <object_body_part> = <verb>; */

	pmSeSt[pmStkP+1].vrb   = pmSeSt[pmStkP+1].vrb;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
        pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].stms3 = NULL;
    	break;}
    case 114: { /* <object_tail> = 'END' 'OBJECT' <optional_id> '.'; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 115: { /* <optional_attributes> =; */

	pmSeSt[pmStkP+1].atrs = NULL;
    	break;}
    case 116: { /* <optional_attributes> = <optional_attributes> <is> <attributes>; */

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+3].atrs);
    	break;}
    case 117: { /* <is> = 'IS'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 118: { /* <is> = 'ARE'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 119: { /* <is> = 'HAS'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 120: { /* <optional_description> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 122: { /* <description> = 'DESCRIPTION'; */

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), STMNOD);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 123: { /* <description> = 'DESCRIPTION' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 124: { /* <article> = 'ARTICLE'; */

	pmSeSt[pmStkP+1].stms = concat(NULL, newstm(&pmSySt[pmStkP+1].srcp,
							STM_NOP), STMNOD);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 125: { /* <article> = 'ARTICLE' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 126: { /* <mentioned> = 'MENTIONED' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 127: { /* <optional_name> =; */

	pmSeSt[pmStkP+1].nams = NULL;
    	break;}
    case 128: { /* <optional_name> = <name>; */

	pmSeSt[pmStkP+1].nams = pmSeSt[pmStkP+1].nams;
    	break;}
    case 129: { /* <optional_names> =; */

	pmSeSt[pmStkP+1].nams2 = NULL;
    	break;}
    case 130: { /* <optional_names> = <optional_names> <name>; */

	pmSeSt[pmStkP+1].nams2 = concat(pmSeSt[pmStkP+1].nams2,
					     pmSeSt[pmStkP+2].nams, NAMNOD);
    	break;}
    case 131: { /* <name> = 'NAME' <ids>; */

	pmSeSt[pmStkP+1].nams = pmSeSt[pmStkP+2].nams;
    	break;}
    case 132: { /* <properties> = 'CONTAINER' <container_body>; */

	pmSeSt[pmStkP+1].cnt = newcnt(&pmSySt[pmStkP+1].srcp,
					  NULL,
					  pmSeSt[pmStkP+2].lims,
					  pmSeSt[pmStkP+2].stms,
					  pmSeSt[pmStkP+2].stms2);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	adv.cnts = concat(adv.cnts, pmSeSt[pmStkP+1].cnt, CNTNOD);
    	break;}
    case 133: { /* <container> = <container_header> <container_body> <container_tail>; */

	pmSeSt[pmStkP+1].cnt = newcnt(&pmSeSt[pmStkP+1].srcp,
				pmSeSt[pmStkP+1].nam,
				pmSeSt[pmStkP+2].lims,
				pmSeSt[pmStkP+2].stms,
				pmSeSt[pmStkP+2].stms2);
	if (pmSeSt[pmStkP+3].nam != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].nam, pmSeSt[pmStkP+3].nam))
		lmLog(&pmSeSt[pmStkP+3].nam->srcp, 201, sevWAR, pmSeSt[pmStkP+1].nam->str);
	}
    	break;}
    case 134: { /* <container_header> = 'CONTAINER' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].nam  = pmSeSt[pmStkP+2].nam;
    	break;}
    case 135: { /* <container_body> = <optional_limits> <optional_header> <optional_empty>; */

	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+1].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+3].stms;
    	break;}
    case 136: { /* <container_tail> = 'END' 'CONTAINER' <optional_id> '.'; */

	pmSeSt[pmStkP+1].nam  = pmSeSt[pmStkP+3].nam;
    	break;}
    case 137: { /* <optional_limits> =; */

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 138: { /* <optional_limits> = 'LIMITS' <limits>; */

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 139: { /* <limits> = <limit>; */

	pmSeSt[pmStkP+1].lims = concat(NULL, pmSeSt[pmStkP+1].lim, LIMNOD);
    	break;}
    case 140: { /* <limits> = <limits> <limit>; */

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMNOD);
    	break;}
    case 141: { /* <limit> = <limit_attribute> 'THEN' <statements>; */

	pmSeSt[pmStkP+1].lim = newlim(&pmSySt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 142: { /* <limit_attribute> = <attribute>; */

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 143: { /* <limit_attribute> = 'COUNT' Integer; */

	pmSeSt[pmStkP+1].atr = newatr(&pmSySt[pmStkP+1].srcp,
				      TYPINT,
				      newnam(&pmSySt[pmStkP+1].srcp, "count"),
				      val(pmSySt[pmStkP+2].chars), 0, 0);
    	break;}
    case 144: { /* <optional_header> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 145: { /* <optional_header> = 'HEADER' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 146: { /* <optional_empty> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 147: { /* <optional_empty> = 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 148: { /* <event> = <event_header> <statements> <event_tail>; */

	pmSeSt[pmStkP+1].evt = newevt(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].nam,
			    pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].nam != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].nam, pmSeSt[pmStkP+3].nam))
		lmLog(&pmSeSt[pmStkP+3].nam->srcp, 201, sevWAR, pmSeSt[pmStkP+1].nam->str);
	}
    	break;}
    case 149: { /* <event_header> = 'EVENT' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].nam  = pmSeSt[pmStkP+2].nam;
    	break;}
    case 150: { /* <event_tail> = 'END' 'EVENT' <optional_id> '.'; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 151: { /* <actor> = <actor_header> <actor_body> <actor_tail>; */

	pmSeSt[pmStkP+1].act = newact(&pmSeSt[pmStkP+1].srcp,
			    pmSeSt[pmStkP+1].nam,
			    pmSeSt[pmStkP+1].nams2,
			    pmSeSt[pmStkP+1].whr,
			    pmSeSt[pmStkP+2].cnt,
			    pmSeSt[pmStkP+2].atrs,
			    pmSeSt[pmStkP+2].stms,
			    pmSeSt[pmStkP+2].vrbs,
			    pmSeSt[pmStkP+2].scrs);
	if (pmSeSt[pmStkP+3].nam != NULL) { /* END-id given */
	    if (!eqnams(pmSeSt[pmStkP+1].nam, pmSeSt[pmStkP+3].nam))
		lmLog(&pmSeSt[pmStkP+3].nam->srcp, 201, sevWAR, pmSeSt[pmStkP+1].nam->str);
	}
    	break;}
    case 152: { /* <actor_header> = 'ACTOR' ID <optional_where> <optional_names> <optional_where>; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].nam  = pmSeSt[pmStkP+2].nam;
	pmSeSt[pmStkP+1].nams2 = pmSeSt[pmStkP+4].nams2;
        if (pmSeSt[pmStkP+3].whr->whr != WHR_DEFAULT) {
          pmSeSt[pmStkP+1].whr  = pmSeSt[pmStkP+3].whr;
          if (pmSeSt[pmStkP+5].whr->whr != WHR_DEFAULT)
            lmLog(&pmSeSt[pmStkP+5].whr->srcp, 340, sevWAR, pmSeSt[pmStkP+2].nam->str);
        } else
          pmSeSt[pmStkP+1].whr  = pmSeSt[pmStkP+5].whr;
    	break;}
    case 153: { /* <actor_body> =; */

	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL; /* Description */
	pmSeSt[pmStkP+1].vrbs  = NULL;
        pmSeSt[pmStkP+1].scrs  = NULL; /* The scripts */
    	break;}
    case 154: { /* <actor_body> = <actor_body> <actor_body_part>; */

	if (pmSeSt[pmStkP+2].cnt != NULL)
          if (pmSeSt[pmStkP+1].cnt != NULL)
            /* CONTAINER Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "CONTAINER", "actor", NULL);
          else
	    pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+2].cnt;
        else if (pmSeSt[pmStkP+2].atrs != NULL)
          pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);
	else if (pmSeSt[pmStkP+2].stms != NULL)
          if (pmSeSt[pmStkP+1].stms != NULL)
            /* DESCRIPTION Already declared */
	    lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "DESCRIPTION", "actor", NULL);
	  else
	    pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
        else if (pmSeSt[pmStkP+2].vrb != NULL)
 	  pmSeSt[pmStkP+1].vrbs  = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VRBNOD);
        else
 	  pmSeSt[pmStkP+1].scrs  = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, VRBNOD);
    	break;}
    case 155: { /* <actor_body_part> = <properties>; */

	pmSeSt[pmStkP+1].cnt   = pmSeSt[pmStkP+1].cnt;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
        pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
    	break;}
    case 157: { /* <actor_body_part> = <is> <attributes>; */

	pmSeSt[pmStkP+1].atrs  = pmSeSt[pmStkP+2].atrs;
	pmSeSt[pmStkP+1].stms  = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
    	break;}
    case 156: { /* <actor_body_part> = <description>; */

	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+1].stms;
	pmSeSt[pmStkP+1].srcp  = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].stms3 = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
    	break;}
    case 158: { /* <actor_body_part> = <verb>; */

	pmSeSt[pmStkP+1].vrb   = pmSeSt[pmStkP+1].vrb;
	pmSeSt[pmStkP+1].scr   = NULL;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
    	break;}
    case 159: { /* <actor_body_part> = <script>; */

	pmSeSt[pmStkP+1].scr   = pmSeSt[pmStkP+1].scr;
	pmSeSt[pmStkP+1].cnt   = NULL;
	pmSeSt[pmStkP+1].atrs  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
	pmSeSt[pmStkP+1].vrb   = NULL;
    	break;}
    case 160: { /* <actor_tail> = 'END' 'ACTOR' <optional_id> '.'; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 161: { /* <optional_actor_script> =; */

	pmSeSt[pmStkP+1].scrs = NULL;
    	break;}
    case 162: { /* <optional_actor_script> = <optional_actor_script> <script>; */

	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs,
					     pmSeSt[pmStkP+2].scr, SCRNOD);
    	break;}
    case 163: { /* <script> = 'SCRIPT' <integer_or_id> '.' <optional_description> <step_list>; */

	pmSeSt[pmStkP+1].scr = newscr(&pmSySt[pmStkP+1].srcp,
			     pmSeSt[pmStkP+2].nam,
			     pmSeSt[pmStkP+2].val,
			     pmSeSt[pmStkP+4].stms,
			     pmSeSt[pmStkP+5].stps);
    	break;}
    case 164: { /* <step_list> = <step>; */

	pmSeSt[pmStkP+1].stps = concat(NULL, pmSeSt[pmStkP+1].stp, STPNOD);
    	break;}
    case 165: { /* <step_list> = <step_list> <step>; */

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STPNOD);
    	break;}
    case 166: { /* <step> = 'STEP' <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 167: { /* <step> = 'STEP' 'AFTER' Integer <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   val(pmSySt[pmStkP+3].chars),
			   NULL,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 168: { /* <step> = 'STEP' 'WAIT' 'UNTIL' <expression> <statements>; */

	pmSeSt[pmStkP+1].stp = newstp(&pmSySt[pmStkP+1].srcp,
			   0,
			   pmSeSt[pmStkP+4].exp,
			   pmSeSt[pmStkP+5].stms);
    	break;}
    case 169: { /* <rule> = 'WHEN' <expression> '=>' <statements>; */

	pmSeSt[pmStkP+1].rul = newrul(&pmSySt[pmStkP+1].srcp,
			   pmSeSt[pmStkP+2].exp,
			   pmSeSt[pmStkP+4].stms);
    	break;}
    case 170: { /* <start> = 'START' <where> '.' <optional_statements>; */

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 171: { /* <optional_statements> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 172: { /* <optional_statements> = <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 173: { /* <statements> = <statement>; */

	pmSeSt[pmStkP+1].stms = concat(NULL, pmSeSt[pmStkP+1].stm, STMNOD);
    	break;}
    case 174: { /* <statements> = <statements> <statement>; */

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STMNOD);
    	break;}
    case 175: { /* <statement> = <output_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 177: { /* <statement> = <manipulation_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 179: { /* <statement> = <assignment_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 181: { /* <statement> = <comparison_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 180: { /* <statement> = <actor_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 178: { /* <statement> = <event_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 176: { /* <statement> = <special_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 182: { /* <output_statement> = STRING; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_PRINT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 183: { /* <output_statement> = 'DESCRIBE' <what> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DESCRIBE);
	pmSeSt[pmStkP+1].stm->fields.describe.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 184: { /* <output_statement> = 'SAY' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAY);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 185: { /* <output_statement> = 'LIST' <what> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LIST);
	pmSeSt[pmStkP+1].stm->fields.list.wht = pmSeSt[pmStkP+2].wht;
    	break;}
    case 186: { /* <special_statement> = 'QUIT' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_QUIT);
    	break;}
    case 187: { /* <special_statement> = 'LOOK' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOOK);
    	break;}
    case 188: { /* <special_statement> = 'SAVE' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SAVE);
    	break;}
    case 189: { /* <special_statement> = 'RESTORE' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTORE);
    	break;}
    case 190: { /* <special_statement> = 'RESTART' '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_RESTART);
    	break;}
    case 191: { /* <special_statement> = 'SCORE' <optional_integer> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCORE);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scocnt;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 192: { /* <special_statement> = 'VISITS' Integer '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_VISITS);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 193: { /* <special_statement> = 'SYSTEM' STRING '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SYSTEM);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 194: { /* <manipulation_statement> = 'EMPTY' <what> <optional_where> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_EMPTY);
	pmSeSt[pmStkP+1].stm->fields.empty.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.empty.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 195: { /* <manipulation_statement> = 'LOCATE' <what> <where> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_LOCATE);
	pmSeSt[pmStkP+1].stm->fields.locate.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.locate.whr = pmSeSt[pmStkP+3].whr;
    	break;}
    case 197: { /* <event_statement> = 'SCHEDULE' ID <optional_where> 'AFTER' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SCHEDULE);
	pmSeSt[pmStkP+1].stm->fields.schedule.nam  = pmSeSt[pmStkP+2].nam;
	pmSeSt[pmStkP+1].stm->fields.schedule.whr  = pmSeSt[pmStkP+3].whr;
	pmSeSt[pmStkP+1].stm->fields.schedule.when = pmSeSt[pmStkP+5].exp;
    	break;}
    case 196: { /* <event_statement> = 'CANCEL' ID '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_CANCEL);
	pmSeSt[pmStkP+1].stm->fields.cancel.nam = pmSeSt[pmStkP+2].nam;
    	break;}
    case 198: { /* <assignment_statement> = 'MAKE' <what> <something> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_MAKE);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].nam;
    	break;}
    case 201: { /* <assignment_statement> = 'SET' <attribute_reference> 'TO' <expression> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_SET);
	pmSeSt[pmStkP+1].stm->fields.set.atr = pmSeSt[pmStkP+2].nam;
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 199: { /* <assignment_statement> = 'INCREASE' <attribute_reference> <optional_by_clause> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_INCR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].nam;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 200: { /* <assignment_statement> = 'DECREASE' <attribute_reference> <optional_by_clause> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DECR);
	pmSeSt[pmStkP+1].stm->fields.incr.atr = pmSeSt[pmStkP+2].nam;
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].wht;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 202: { /* <optional_by_clause> =; */

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 203: { /* <optional_by_clause> = 'BY' <expression>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 204: { /* <comparison_statement> = <if_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 205: { /* <comparison_statement> = <depending_statement>; */

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 206: { /* <if_statement> = 'IF' <expression> 'THEN' <statements> <optional_elsif_list> <optional_else_part> 'END' 'IF' '.'; */

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
    case 207: { /* <optional_elsif_list> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 208: { /* <optional_elsif_list> = <elsif_list>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 209: { /* <elsif_list> = 'ELSIF' <expression> 'THEN' <statements>; */
{
	StmNod *stm;

	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_IF);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = concat(NULL, stm, STMNOD);
    }	break;}
    case 210: { /* <elsif_list> = <elsif_list> 'ELSIF' <expression> 'THEN' <statements>; */
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
    case 211: { /* <optional_else_part> =; */

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 212: { /* <optional_else_part> = 'ELSE' <statements>; */

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 213: { /* <depending_statement> = 'DEPENDING' 'ON' <primary> <depend_cases> 'END' 'DEPEND' '.'; */
{ StmNod *stm;
	stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPEND);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 214: { /* <depend_cases> = <depend_case>; */

	pmSeSt[pmStkP+1].cases = concat(NULL, pmSeSt[pmStkP+1].stm, CASENOD);
    	break;}
    case 215: { /* <depend_cases> = <depend_cases> <depend_case>; */

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASENOD);
    	break;}
    case 217: { /* <depend_case> = <right_hand_side> ':' <statements>; */

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+2].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 216: { /* <depend_case> = 'ELSE' <statements>; */

        pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_DEPCASE);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 218: { /* <actor_statement> = 'USE' 'SCRIPT' <integer_or_id> <optional_for_actor> '.'; */

	pmSeSt[pmStkP+1].stm = newstm(&pmSySt[pmStkP+1].srcp, STM_USE);
	pmSeSt[pmStkP+1].stm->fields.use.script = pmSeSt[pmStkP+3].nam;
	pmSeSt[pmStkP+1].stm->fields.use.scriptno = pmSeSt[pmStkP+3].val;
	pmSeSt[pmStkP+1].stm->fields.use.actor = pmSeSt[pmStkP+4].nam;
    	break;}
    case 219: { /* <optional_for_actor> =; */

	pmSeSt[pmStkP+1].nam = NULL;
    	break;}
    case 220: { /* <optional_for_actor> = 'FOR' ID; */

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].nam;
    	break;}
    case 221: { /* <expression> = <term>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 222: { /* <expression> = <expression> 'OR' <term>; */
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_OR;;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 223: { /* <term> = <factor>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 224: { /* <term> = <term> 'AND' <factor>; */
 { ExpNod *exp;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not	      = FALSE;
	exp->fields.bin.op    = OP_AND;
	exp->fields.bin.left  = pmSeSt[pmStkP+1].exp;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 225: { /* <factor> = <primary>; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 226: { /* <factor> = <primary> <right_hand_side>; */

        /* <right_hand_side> contains the top expr, hang in the <primary> and
           propagate that node */
        switch (pmSeSt[pmStkP+2].expkd) {
	case EXPBIN: pmSeSt[pmStkP+2].exp->fields.bin.left = pmSeSt[pmStkP+1].exp; break;
	case EXPWHR: pmSeSt[pmStkP+2].exp->fields.whr.wht = pmSeSt[pmStkP+1].exp; break;
	case EXPATR: pmSeSt[pmStkP+2].exp->fields.atr.wht = pmSeSt[pmStkP+1].exp; break;
	case EXPBTW: pmSeSt[pmStkP+2].exp->fields.btw.val = pmSeSt[pmStkP+1].exp; break;
	default: syserr("Unrecognized switch in <right_hand_side> semantic rule.");
	}
        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 228: { /* <right_hand_side> = <binop> <primary>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expkd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPBIN);
	exp->fields.bin.op = pmSeSt[pmStkP+1].op;
	exp->fields.bin.right = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 229: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expkd = EXPBIN;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = pmSeSt[pmStkP+2].op;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 227: { /* <right_hand_side> = <optional_not> <where>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expkd = EXPWHR;
	exp = newexp(&pmSeSt[pmStkP+2].srcp, EXPWHR);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.whr.whr = pmSeSt[pmStkP+2].whr;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 230: { /* <right_hand_side> = <is> <something>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expkd = EXPATR;
	exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	exp->not = pmSeSt[pmStkP+2].not;
	exp->fields.atr.atr = pmSeSt[pmStkP+2].nam;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 232: { /* <right_hand_side> = <optional_not> 'BETWEEN' <factor> 'AND' <factor>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expkd = EXPBTW;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBTW);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.btw.low = pmSeSt[pmStkP+3].exp;
	exp->fields.btw.high = pmSeSt[pmStkP+5].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 231: { /* <right_hand_side> = <optional_not> 'CONTAINS' <factor>; */
 { ExpNod *exp;
        pmSeSt[pmStkP+1].expkd = EXPBIN;
	exp = newexp(&pmSySt[pmStkP+2].srcp, EXPBIN);
	exp->not = pmSeSt[pmStkP+1].not;
	exp->fields.bin.op = OP_CONTAINS;
	exp->fields.bin.right = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].exp = exp;
    } 	break;}
    case 238: { /* <primary> = '(' <expression> ')'; */

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 233: { /* <primary> = <optional_minus> Integer; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+2].srcp, EXPINT);
	if (pmSeSt[pmStkP+1].minus)
	  pmSeSt[pmStkP+1].exp->fields.val.val = -val(pmSySt[pmStkP+2].chars);
	else
	  pmSeSt[pmStkP+1].exp->fields.val.val = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 234: { /* <primary> = STRING; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSTR);
	pmSeSt[pmStkP+1].exp->fields.str.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].exp->fields.str.len = pmSySt[pmStkP+1].len;
    	break;}
    case 235: { /* <primary> = <what>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 239: { /* <primary> = <attribute_reference>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPATR);
	pmSeSt[pmStkP+1].exp->fields.atr.atr = pmSeSt[pmStkP+1].nam;
	pmSeSt[pmStkP+1].exp->fields.atr.wht = newexp(&pmSeSt[pmStkP+1].srcp, EXPWHT);
	pmSeSt[pmStkP+1].exp->fields.atr.wht->fields.wht.wht = pmSeSt[pmStkP+1].wht;
    	break;}
    case 237: { /* <primary> = <aggregate> <where>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSeSt[pmStkP+1].srcp, EXPAGR);
	pmSeSt[pmStkP+1].exp->fields.agr.agr	= pmSeSt[pmStkP+1].agr;
	pmSeSt[pmStkP+1].exp->fields.agr.atrnam = pmSeSt[pmStkP+1].nam;
	pmSeSt[pmStkP+1].exp->fields.agr.whr	= pmSeSt[pmStkP+2].whr;
    	break;}
    case 240: { /* <primary> = 'RANDOM' <primary> 'TO' <primary>; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPRND);
	pmSeSt[pmStkP+1].exp->fields.rnd.from	= pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].exp->fields.rnd.to	= pmSeSt[pmStkP+4].exp;
    	break;}
    case 236: { /* <primary> = 'SCORE'; */

	pmSeSt[pmStkP+1].exp = newexp(&pmSySt[pmStkP+1].srcp, EXPSCORE);
    	break;}
    case 242: { /* <aggregate> = 'SUM' 'OF' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_SUM;
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 243: { /* <aggregate> = 'MAX' 'OF' ID; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_MAX;
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+3].nam;
    	break;}
    case 241: { /* <aggregate> = 'COUNT'; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].agr = AGR_COUNT;
	pmSeSt[pmStkP+1].nam = NULL;
    	break;}
    case 244: { /* <something> = <optional_not> ID; */

	pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].not;
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].nam;
    	break;}
    case 245: { /* <what> = 'OBJECT'; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_OBJ, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 246: { /* <what> = 'LOCATION'; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_LOC, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 247: { /* <what> = 'ACTOR'; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSySt[pmStkP+1].srcp, WHT_ACT, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 248: { /* <what> = ID; */

	pmSeSt[pmStkP+1].wht = newwht(&pmSeSt[pmStkP+1].nam->srcp, WHT_ID, pmSeSt[pmStkP+1].nam);
	pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].nam->srcp;
    	break;}
    case 249: { /* <optional_where> =; */

        pmSeSt[pmStkP+1].whr = newwhr(&nulsrcp, WHR_DEFAULT, NULL);
    	break;}
    case 250: { /* <optional_where> = <where>; */

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 251: { /* <where> = 'HERE'; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_HERE, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 252: { /* <where> = 'NEARBY'; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_NEAR, NULL);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 253: { /* <where> = 'AT' <what>; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_AT, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 254: { /* <where> = 'IN' <what>; */

        pmSeSt[pmStkP+1].whr = newwhr(&pmSySt[pmStkP+1].srcp, WHR_IN, pmSeSt[pmStkP+2].wht);
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 255: { /* <binop> = '+'; */

	pmSeSt[pmStkP+1].op = OP_PLUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 256: { /* <binop> = '-'; */

	pmSeSt[pmStkP+1].op = OP_MINUS;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 257: { /* <binop> = '*'; */

	pmSeSt[pmStkP+1].op = OP_MULT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 258: { /* <binop> = '/'; */

	pmSeSt[pmStkP+1].op = OP_DIV;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 259: { /* <relop> = '<>'; */

        pmSeSt[pmStkP+1].op   = OP_NE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 260: { /* <relop> = '='; */

        pmSeSt[pmStkP+1].op   = OP_EQ;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 261: { /* <relop> = '=='; */

        pmSeSt[pmStkP+1].op   = OP_EXACT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 262: { /* <relop> = '>='; */

        pmSeSt[pmStkP+1].op   = OP_GE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 263: { /* <relop> = '<='; */

        pmSeSt[pmStkP+1].op   = OP_LE;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 264: { /* <relop> = '>'; */

        pmSeSt[pmStkP+1].op   = OP_GT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 265: { /* <relop> = '<'; */

        pmSeSt[pmStkP+1].op   = OP_LT;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 266: { /* <optional_qual> =; */

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 267: { /* <optional_qual> = 'BEFORE'; */

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 268: { /* <optional_qual> = 'AFTER'; */

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 269: { /* <optional_qual> = 'ONLY'; */

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 270: { /* <optional_not> =; */

        pmSeSt[pmStkP+1].not = FALSE;
    	break;}
    case 271: { /* <optional_not> = 'NOT'; */

        pmSeSt[pmStkP+1].not = TRUE;
    	break;}
    case 272: { /* <optional_id> =; */

        pmSeSt[pmStkP+1].nam = NULL;
    	break;}
    case 273: { /* <optional_id> = ID; */

        pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+1].nam;
    	break;}
    case 274: { /* <ids> = ID; */

        pmSeSt[pmStkP+1].nams = concat(NULL, pmSeSt[pmStkP+1].nam, NAMNOD);
    	break;}
    case 275: { /* <ids> = <ids> ID; */

        pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+2].nam, NAMNOD);
    	break;}
    case 276: { /* <id_list> = ID; */

        pmSeSt[pmStkP+1].str = newstr(pmSeSt[pmStkP+1].nam->str);
        pmSeSt[pmStkP+1].nams = concat(NULL, pmSeSt[pmStkP+1].nam, NAMNOD);
    	break;}
    case 277: { /* <id_list> = <id_list> ',' ID; */

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+3].nam, NAMNOD);
    	break;}
    case 278: { /* <optional_integer> =; */

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 279: { /* <optional_integer> = Integer; */

        pmSeSt[pmStkP+1].str = newstr(pmSySt[pmStkP+1].chars);
    	break;}
    case 280: { /* <optional_minus> =; */

	pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 281: { /* <optional_minus> = '-'; */

	pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 282: { /* <attribute_reference> = ID 'OF' <what>; */

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+1].nam;
	pmSeSt[pmStkP+1].wht = pmSeSt[pmStkP+3].wht;
    	break;}
    case 283: { /* <integer_or_id> = Integer; */

	pmSeSt[pmStkP+1].val = val(pmSySt[pmStkP+1].chars);
	pmSeSt[pmStkP+1].nam = NULL;
    	break;}
    case 284: { /* <integer_or_id> = ID; */

	pmSeSt[pmStkP+1].val = 0;
	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+1].nam;
    	break;}
    case 285: { /* ID = IDENT; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 286: { /* ID = 'DEFAULT'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "default");
    	break;}
    case 287: { /* ID = 'ARTICLE'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "article");
    	break;}
    case 288: { /* ID = 'MESSAGE'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "message");
    	break;}
    case 289: { /* ID = 'QUIT'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "quit");
    	break;}
    case 290: { /* ID = 'SAVE'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "save");
    	break;}
    case 291: { /* ID = 'RESTORE'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "restore");
    	break;}
    case 292: { /* ID = 'RESTART'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "restart");
    	break;}
    case 293: { /* ID = 'WAIT'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "wait");
    	break;}
    case 294: { /* ID = 'BETWEEN'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "between");
    	break;}
    case 295: { /* ID = 'CONTAINS'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "contains");
    	break;}
    case 296: { /* ID = 'ON'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "on");
    	break;}
    case 297: { /* ID = 'IN'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "in");
    	break;}
    case 298: { /* ID = 'AFTER'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "after");
    	break;}
    case 299: { /* ID = 'BEFORE'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "before");
    	break;}
    case 300: { /* ID = 'CHECK'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "check");
    	break;}
    case 301: { /* ID = 'DEPEND'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "depend");
    	break;}
    case 302: { /* ID = 'EXIT'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "exit");
    	break;}
    case 303: { /* ID = 'FOR'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "for");
    	break;}
    case 304: { /* ID = 'INTEGER'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "integer");
    	break;}
    case 305: { /* ID = 'ISA'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "isa");
    	break;}
    case 306: { /* ID = 'LIMITS'; */

        pmSeSt[pmStkP+1].nam = newnam(&pmSySt[pmStkP+1].srcp, "limits");
    	break;}
    default: break; }
}/*pmPaSema()*/

