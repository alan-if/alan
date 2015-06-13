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

#include "lmList.h"

#include "alt.h"
#include "evt.h"
#include "lim.h"
#include "msg.h"
#include "opt.h"
#include "sco.h"
#include "str.h"
#include "syn.h"
#include "adv_x.h"
#include "add_x.h"
#include "article_x.h"
#include "atr_x.h"
#include "cla_x.h"
#include "chk_x.h"
#include "cnt_x.h"
#include "elm_x.h"
#include "exp_x.h"
#include "ext_x.h"
#include "id_x.h"
#include "ins_x.h"
#include "description_x.h"
#include "initialize_x.h"
#include "resource_x.h"
#include "rul_x.h"
#include "lst_x.h"
#include "prop_x.h"
#include "res_x.h"
#include "scr_x.h"
#include "stm_x.h"
#include "stp_x.h"
#include "stx_x.h"
#include "sym_x.h"
#include "srcp_x.h"
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
    Bool wordOrChar;
    What *wht;
    Where *whr;
    List *vrbs;
    Verb *vrb;
    int val;
    Transitivity transitivity;
    List *syns;
    Synonym *syn;
    List *stxs;
    Syntax *stx;
    char *str;
    List *stps;
    Step *stp;
    List *stms3;
    List *stms2;
    List *stms;
    Statement *stm;
    Srcp srcp2;
    Srcp srcp;
    List *set;
    List *scrs;
    Script *scr;
    SayForm sayForm;
    List *ruls;
    RulNod *rul;
    RestrictionKind restrictionKind;
    List *ress;
    Restriction *res;
    QualKind qual;
    Srcp pronounsSrcp;
    List *pronouns;
    List *prompt;
    Bool present;
    Bool opaque;
    OperatorKind op;
    Bool on_or_off;
    Article *negative;
    List *nams;
    List *nam;
    List *msgs;
    Message *msg;
    Bool minus;
    Srcp mentionedSrcp;
    List *mentioned;
    List *lims;
    LimNod *lim;
    Bool isForm;
    List *inss;
    Instance *ins;
    Article *indefinite;
    List *idList2;
    List *idList;
    Id *id;
    List *form;
    Bool first;
    List *exts;
    Exit *ext;
    List *exps;
    Expression *exp;
    List *evts;
    Event *evt;
    Initialize *initialize;
    Srcp enteredSrcp;
    List *entered;
    List *elms;
    Element *elm;
    Srcp descriptionSrcp;
    List *descriptionDoes;
    List *descriptionChecks;
    Srcp descriptionCheckSrcp;
    Description *description;
    Srcp definiteSrcp;
    Bool definiteIsForm;
    Article *definite;
    List *cnts;
    Container *cnt;
    List *clas;
    Class *cla;
    List *chks;
    CheckNode *chk;
    List *cases;
    int bits;
    List *atrs;
    Attribute *atr;
    Article *article;
    List *alts;
    Alternative *alt;
    AggregateKind agr;
    AddNode *add;
} pmGrammar;

/* END %%ATTRIBUTES */

extern short pmStkP;
extern Token pmSySt[];
extern pmGrammar pmSeSt[];

/* %%DECLARATIONS - User data and routines */

#line 84 "alan.pmk"


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
#line 203 "alan.pmk"
    switch (rule) {
    case 8: { /* <option> = ID '.'; */
#line 223 "alan.pmk"

        optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, TRUE);
    	break;}
    case 9: { /* <option> = ID ID '.'; */
#line 228 "alan.pmk"

        optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 10: { /* <option> = ID Integer '.'; */
#line 233 "alan.pmk"

        optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 19: { /* <declaration> = <synonyms>; */
#line 246 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns);	break;}
    case 15: { /* <declaration> = <messages>; */
#line 249 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 20: { /* <declaration> = <syntax>; */
#line 252 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 21: { /* <declaration> = <verb>; */
#line 255 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VERB_LIST); 	break;}
    case 16: { /* <declaration> = <class>; */
#line 258 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLASS_LIST); 	break;}
    case 22: { /* <declaration> = <addition>; */
#line 261 "alan.pmk"
 adv.adds = concat(adv.adds, pmSeSt[pmStkP+1].add, ADD_LIST); 	break;}
    case 17: { /* <declaration> = <instance>; */
#line 264 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSTANCE_LIST); 	break;}
    case 23: { /* <declaration> = <event>; */
#line 267 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVENT_LIST); 	break;}
    case 18: { /* <declaration> = <rule>; */
#line 270 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULE_LIST); 	break;}
    case 14: { /* <declaration> = <prompt>; */
#line 273 "alan.pmk"

      if (adv.prompt != NULL)
        lmLog(&pmSeSt[pmStkP+1].srcp, 308, sevWAR, "PROMPT");
      adv.prompt = pmSeSt[pmStkP+1].prompt;
    	break;}
    case 24: { /* <prompt> = 'prompt' <statements>; */
#line 284 "alan.pmk"

      pmSeSt[pmStkP+1].prompt = pmSeSt[pmStkP+2].stms;
      pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 25: { /* <attributes> = <attribute_definition> '.'; */
#line 293 "alan.pmk"

        pmSeSt[pmStkP+1].atrs = newList(pmSeSt[pmStkP+1].atr, ATTRIBUTE_LIST);
    	break;}
    case 26: { /* <attributes> = <attributes> <attribute_definition> '.'; */
#line 298 "alan.pmk"

        pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATTRIBUTE_LIST);
    	break;}
    case 27: { /* <attribute_definition> = ID; */
#line 305 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, TRUE);
    	break;}
    case 28: { /* <attribute_definition> = 'not' ID; */
#line 310 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id, FALSE);
    	break;}
    case 31: { /* <attribute_definition> = ID <optional_minus> Integer; */
#line 315 "alan.pmk"

        if (pmSeSt[pmStkP+2].minus)
           pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, -val(pmSySt[pmStkP+3].chars));
        else
           pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, val(pmSySt[pmStkP+3].chars));
    	break;}
    case 29: { /* <attribute_definition> = ID STRING; */
#line 323 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newStringAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 30: { /* <attribute_definition> = ID ID; */
#line 327 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newReferenceAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+2].id);
    	break;}
    case 32: { /* <attribute_definition> = ID '{' <optional_members> '}'; */
#line 332 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newSetAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id,
                                                      newSetExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+3].set));
    	break;}
    case 33: { /* <optional_members> =; */
#line 340 "alan.pmk"
 pmSeSt[pmStkP+1].set = NULL; 	break;}
    case 34: { /* <optional_members> = <set_members>; */
#line 343 "alan.pmk"
 pmSeSt[pmStkP+1].set = pmSeSt[pmStkP+1].set; 	break;}
    case 35: { /* <set_members> = <set_member>; */
#line 348 "alan.pmk"

        pmSeSt[pmStkP+1].set = newList(pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 36: { /* <set_members> = <set_members> ',' <set_member>; */
#line 353 "alan.pmk"

        pmSeSt[pmStkP+1].set = concat(pmSeSt[pmStkP+1].set, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 37: { /* <set_member> = <what>; */
#line 360 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 39: { /* <set_member> = <optional_minus> Integer; */
#line 365 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].minus?-val(pmSySt[pmStkP+2].chars):val(pmSySt[pmStkP+2].chars));
    	break;}
    case 38: { /* <set_member> = STRING; */
#line 370 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].fpos, pmSySt[pmStkP+1].len);
    	break;}
    case 40: { /* <synonyms> = 'synonyms' <synonym_list>; */
#line 378 "alan.pmk"

        pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 41: { /* <synonym_list> = <synonym_declaration>; */
#line 385 "alan.pmk"

        pmSeSt[pmStkP+1].syns = newList(pmSeSt[pmStkP+1].syn, SYNONYM_LIST);
    	break;}
    case 42: { /* <synonym_list> = <synonym_list> <synonym_declaration>; */
#line 390 "alan.pmk"

        pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNONYM_LIST);
    	break;}
    case 43: { /* <synonym_declaration> = <id_list> '=' ID '.'; */
#line 397 "alan.pmk"

        pmSeSt[pmStkP+1].syn = newSynonym(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id);
    	break;}
    case 44: { /* <messages> = 'message' <message_list>; */
#line 405 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 45: { /* <message_list> = <message>; */
#line 412 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = newList(pmSeSt[pmStkP+1].msg, MESSAGE_LIST);
    	break;}
    case 46: { /* <message_list> = <message_list> <message>; */
#line 417 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MESSAGE_LIST);
    	break;}
    case 47: { /* <message> = ID ':' <statements>; */
#line 424 "alan.pmk"

        pmSeSt[pmStkP+1].msg = newMessage(&pmSySt[pmStkP+2].srcp,
                                    pmSeSt[pmStkP+1].id,
                                    pmSeSt[pmStkP+3].stms);
    	break;}
    case 48: { /* <syntax> = 'syntax' <syntax_list>; */
#line 435 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 49: { /* <syntax_list> = <syntax_item>; */
#line 442 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = newList(pmSeSt[pmStkP+1].stx, SYNTAX_LIST);
    	break;}
    case 50: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 447 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, SYNTAX_LIST);
    	break;}
    case 51: { /* <syntax_item> = ID '=' <syntax_elements> <optional_syntax_restrictions>; */
#line 454 "alan.pmk"

        pmSeSt[pmStkP+1].stx = newSyntax(pmSySt[pmStkP+2].srcp,
                                       pmSeSt[pmStkP+1].id,
                                       concat(pmSeSt[pmStkP+3].elms,
                                              newEndOfSyntax(),
                                              ELEMENT_LIST),
                                       pmSeSt[pmStkP+4].ress,
	                                   pmSeSt[pmStkP+4].srcp);
    	break;}
    case 52: { /* <syntax_elements> = <syntax_element>; */
#line 467 "alan.pmk"

        pmSeSt[pmStkP+1].elms = newList(pmSeSt[pmStkP+1].elm, ELEMENT_LIST);
    	break;}
    case 53: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 472 "alan.pmk"

        pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
                                         pmSeSt[pmStkP+2].elm,
                                         ELEMENT_LIST);
    	break;}
    case 54: { /* <syntax_element> = ID; */
#line 481 "alan.pmk"

        pmSeSt[pmStkP+1].elm = newWordElement(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id);
    	break;}
    case 55: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 487 "alan.pmk"

        pmSeSt[pmStkP+1].elm = newParameterElement(pmSeSt[pmStkP+2].id->srcp,
                                                    pmSeSt[pmStkP+2].id,
                                                    pmSeSt[pmStkP+4].bits);
    	break;}
    case 56: { /* <optional_indicators> =; */
#line 496 "alan.pmk"

        pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 57: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 501 "alan.pmk"

        pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 58: { /* <indicator> = '*'; */
#line 508 "alan.pmk"

        pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 59: { /* <indicator> = '!'; */
#line 513 "alan.pmk"

        pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 60: { /* <syntax_restriction_clauses> = <syntax_restriction>; */
#line 526 "alan.pmk"

        pmSeSt[pmStkP+1].ress = newList(pmSeSt[pmStkP+1].res, RESTRICTION_LIST);
    	break;}
    case 61: { /* <syntax_restriction_clauses> = <syntax_restriction_clauses> 'and' <syntax_restriction>; */
#line 531 "alan.pmk"

        pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
                                                    pmSeSt[pmStkP+3].res, RESTRICTION_LIST);
    	break;}
    case 62: { /* <syntax_restriction> = ID 'isa' <restriction_class> 'else' <statements>; */
#line 539 "alan.pmk"

        pmSeSt[pmStkP+1].res = newRestriction(pmSySt[pmStkP+2].srcp,
                                                   pmSeSt[pmStkP+1].id,
                                                   pmSeSt[pmStkP+3].restrictionKind,
                                                   pmSeSt[pmStkP+3].id,
                                                   pmSeSt[pmStkP+5].stms);
    	break;}
    case 63: { /* <restriction_class> = ID; */
#line 550 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
        pmSeSt[pmStkP+1].restrictionKind = ID_RESTRICTION;
    	break;}
    case 64: { /* <restriction_class> = 'container'; */
#line 556 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
        pmSeSt[pmStkP+1].restrictionKind = CONTAINER_RESTRICTION;
    	break;}
    case 65: { /* <optional_syntax_restrictions> = '.'; */
#line 564 "alan.pmk"

        pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 66: { /* <optional_syntax_restrictions> = 'where' <syntax_restriction_clauses>; */
#line 569 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 67: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 579 "alan.pmk"

        pmSeSt[pmStkP+1].vrb = newVerb(&pmSeSt[pmStkP+1].srcp,
                              pmSeSt[pmStkP+1].idList,
                              pmSeSt[pmStkP+2].alts);
        if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
            if (compareStrings(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
                lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].str);
        }
    	break;}
    case 68: { /* <verb_header> = __genSym#1 'verb' <id_list>; */
#line 592 "alan.pmk"

        pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+2].srcp;
        pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+3].str;
        pmSeSt[pmStkP+1].idList   = pmSeSt[pmStkP+3].idList;
    	break;}
    case 71: { /* <verb_body> = <simple_verb_body>; */
#line 601 "alan.pmk"

        pmSeSt[pmStkP+1].alts = newList(newAlternative(pmSeSt[pmStkP+1].srcp,
                                                   NULL,
                                                   pmSeSt[pmStkP+1].chks,
                                                   pmSeSt[pmStkP+1].qual,
                                                   pmSeSt[pmStkP+1].stms),
                                    ALTERNATIVE_LIST);
    	break;}
    case 72: { /* <verb_body> = <verb_alternatives>; */
#line 611 "alan.pmk"

        pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 73: { /* <verb_alternatives> = <verb_alternative>; */
#line 618 "alan.pmk"

        pmSeSt[pmStkP+1].alts = newList(pmSeSt[pmStkP+1].alt, ALTERNATIVE_LIST);
    	break;}
    case 74: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 623 "alan.pmk"

        pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
                                           pmSeSt[pmStkP+2].alt,
                                           ALTERNATIVE_LIST);
    	break;}
    case 75: { /* <verb_alternative> = 'when' ID <simple_verb_body>; */
#line 632 "alan.pmk"

        pmSeSt[pmStkP+1].alt = newAlternative(pmSySt[pmStkP+1].srcp,
                                                 pmSeSt[pmStkP+2].id,
                                                 pmSeSt[pmStkP+3].chks,
                                                 pmSeSt[pmStkP+3].qual,
                                                 pmSeSt[pmStkP+3].stms);
    	break;}
    case 76: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 643 "alan.pmk"

        if (pmSeSt[pmStkP+2].stms == NULL) /* No DOES */
            pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        else
            pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+2].srcp;
        pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 77: { /* <verb_tail> = 'end' 'verb' <optional_id> '.'; */
#line 656 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 78: { /* <optional_checks> =; */
#line 664 "alan.pmk"

        pmSeSt[pmStkP+1].chks = NULL;
        pmSeSt[pmStkP+1].srcp = nulsrcp;
    	break;}
    case 79: { /* <optional_checks> = 'check' <statements>; */
#line 670 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].chks = newList(newCheck(NULL, pmSeSt[pmStkP+2].stms), CHECK_LIST);
    	break;}
    case 80: { /* <optional_checks> = 'check' <check_list>; */
#line 676 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 81: { /* <check_list> = <check>; */
#line 684 "alan.pmk"

        pmSeSt[pmStkP+1].chks = newList(pmSeSt[pmStkP+1].chk, CHECK_LIST);
    	break;}
    case 82: { /* <check_list> = <check_list> 'and' <check>; */
#line 689 "alan.pmk"

        pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHECK_LIST);
    	break;}
    case 83: { /* <check> = <expression> 'else' <statements>; */
#line 696 "alan.pmk"

        pmSeSt[pmStkP+1].chk = newCheck(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 84: { /* <optional_does> =; */
#line 704 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
        pmSeSt[pmStkP+1].srcp = nulsrcp;
        pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 85: { /* <optional_does> = <does>; */
#line 711 "alan.pmk"

        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+1].qual;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 86: { /* <does> = 'does' <optional_qual> <statements>; */
#line 720 "alan.pmk"

        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 87: { /* <class> = 'every' ID <optional_heritage> <properties> <class_tail>; */
#line 731 "alan.pmk"

        pmSeSt[pmStkP+1].cla = newClass(&pmSySt[pmStkP+1].srcp,
                                pmSeSt[pmStkP+2].id,
                                pmSeSt[pmStkP+3].id,
                                newProps(pmSeSt[pmStkP+4].whr,
                                         pmSeSt[pmStkP+4].nams,
                                         pmSeSt[pmStkP+4].pronounsSrcp,
                                         pmSeSt[pmStkP+4].pronouns,
                                         pmSeSt[pmStkP+4].atrs,
                                         pmSeSt[pmStkP+4].initialize,
                                         pmSeSt[pmStkP+4].description,
                                         pmSeSt[pmStkP+4].mentionedSrcp,
                                         pmSeSt[pmStkP+4].mentioned,
                                         pmSeSt[pmStkP+4].definite,
                                         pmSeSt[pmStkP+4].indefinite,
                                         pmSeSt[pmStkP+4].negative,
                                         pmSeSt[pmStkP+4].cnt,
                                         pmSeSt[pmStkP+4].vrbs,
                                         pmSeSt[pmStkP+4].enteredSrcp,
                                         pmSeSt[pmStkP+4].entered,
                                         pmSeSt[pmStkP+4].exts,
                                         pmSeSt[pmStkP+4].scrs));
        if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
            if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
                lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 88: { /* <class_tail> = 'end' 'every' <optional_id> __genSym#2; */
#line 761 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 91: { /* <addition> = 'add' 'to' __genSym#3 ID <optional_heritage> <properties> <add_tail>; */
#line 768 "alan.pmk"

	pmSeSt[pmStkP+1].add = newAdd(pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+4].id,
				pmSeSt[pmStkP+5].id,
				newProps(pmSeSt[pmStkP+6].whr,
					pmSeSt[pmStkP+6].nams,
					pmSeSt[pmStkP+6].pronounsSrcp,
					pmSeSt[pmStkP+6].pronouns,
					pmSeSt[pmStkP+6].atrs,
					pmSeSt[pmStkP+6].initialize,
					pmSeSt[pmStkP+6].description,
					pmSeSt[pmStkP+6].mentionedSrcp,
					pmSeSt[pmStkP+6].mentioned,
					pmSeSt[pmStkP+6].definite,
					pmSeSt[pmStkP+6].indefinite,
					pmSeSt[pmStkP+6].negative,
					pmSeSt[pmStkP+6].cnt,
					pmSeSt[pmStkP+6].vrbs,
					pmSeSt[pmStkP+6].enteredSrcp,
					pmSeSt[pmStkP+6].entered,
					pmSeSt[pmStkP+6].exts,
					pmSeSt[pmStkP+6].scrs));
	if (pmSeSt[pmStkP+7].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+4].id, pmSeSt[pmStkP+7].id))
		lmLog(&pmSeSt[pmStkP+7].id->srcp, 200, sevWAR, pmSeSt[pmStkP+4].id->string);
    	break;}
    case 94: { /* <add_tail> = 'end' 'add' __genSym#4 <optional_id> __genSym#5; */
#line 798 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+4].id; 	break;}
    case 99: { /* <instance> = 'the' ID <optional_heritage> <properties> <instance_tail>; */
#line 805 "alan.pmk"

	pmSeSt[pmStkP+1].ins = newInstance(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].id,
				pmSeSt[pmStkP+3].id,
				newProps(pmSeSt[pmStkP+4].whr,
					pmSeSt[pmStkP+4].nams,
					pmSeSt[pmStkP+4].pronounsSrcp,
					pmSeSt[pmStkP+4].pronouns,
					pmSeSt[pmStkP+4].atrs,
					pmSeSt[pmStkP+4].initialize,
					pmSeSt[pmStkP+4].description,
					pmSeSt[pmStkP+4].mentionedSrcp,
					pmSeSt[pmStkP+4].mentioned,
					pmSeSt[pmStkP+4].definite,
					pmSeSt[pmStkP+4].indefinite,
					pmSeSt[pmStkP+4].negative,
					pmSeSt[pmStkP+4].cnt,
					pmSeSt[pmStkP+4].vrbs,
					pmSeSt[pmStkP+4].enteredSrcp,
					pmSeSt[pmStkP+4].entered,
					pmSeSt[pmStkP+4].exts,
					pmSeSt[pmStkP+4].scrs));
	if (pmSeSt[pmStkP+5].id != NULL)  /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+2].id, pmSeSt[pmStkP+5].id))
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 100: { /* <instance_tail> = 'end' 'the' <optional_id> __genSym#6; */
#line 835 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 103: { /* <optional_heritage> =; */
#line 840 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 104: { /* <optional_heritage> = <heritage>; */
#line 845 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 105: { /* <heritage> = 'isa' ID __genSym#7; */
#line 852 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 108: { /* <properties> =; */
#line 857 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out all fields */
    	break;}
    case 109: { /* <properties> = <properties> <property>; */
#line 862 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+2].nam, NAME_LIST);

        if (pmSeSt[pmStkP+2].pronouns != NULL) {
            if (pmSeSt[pmStkP+1].pronouns != NULL) 
                /* WHERE clause already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "PRONOUN", "class/instance", NULL);
	    else {
		pmSeSt[pmStkP+1].pronouns = combine(pmSeSt[pmStkP+1].pronouns, pmSeSt[pmStkP+2].pronouns);
		pmSeSt[pmStkP+1].pronounsSrcp = pmSeSt[pmStkP+2].pronounsSrcp;
	    }
        }

        if (pmSeSt[pmStkP+2].whr != NULL) {
            if (pmSeSt[pmStkP+1].whr != NULL) 
                /* WHERE clause already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "WHERE", "class/instance", NULL);
	    else
	        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+2].whr;
        }

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);

	if (pmSeSt[pmStkP+2].initialize != NULL) {
            if (pmSeSt[pmStkP+1].initialize != NULL)
                /* INITIALIZE properties already declared */
	        lmLogv(&pmSeSt[pmStkP+2].initialize->srcp, 204, sevERR, "INITIALIZE property", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].initialize = pmSeSt[pmStkP+2].initialize;
        }

	if (pmSeSt[pmStkP+2].cnt != NULL) {
            if (pmSeSt[pmStkP+1].cnt != NULL)
                /* CONTAINER properties already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "CONTAINER properties", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+2].cnt;
        }

	if (pmSeSt[pmStkP+2].description != NULL) {
	  if (pmSeSt[pmStkP+2].description->checks != NULL) {
            if(pmSeSt[pmStkP+1].description != NULL && pmSeSt[pmStkP+1].description->checks != NULL)
                /* DESCRIPTION CHECK already declared */
	        lmLogv(&pmSeSt[pmStkP+2].description->checkSrcp, 204, sevERR, "DESCRIPTION CHECK", "class/instance", NULL);
	    else {
		if (pmSeSt[pmStkP+1].description != NULL)
		  pmSeSt[pmStkP+1].description->checks = pmSeSt[pmStkP+2].description->checks;
	    }
          }

	  if (pmSeSt[pmStkP+2].description->does != NULL) {
            if (pmSeSt[pmStkP+1].description != NULL && pmSeSt[pmStkP+1].description->does != NULL)
	        /* DESCRIPTION DOES already declared */
	        lmLogv(&pmSeSt[pmStkP+2].description->doesSrcp, 204, sevERR, "DESCRIPTION", "class/instance", NULL);
	    else 
		if (pmSeSt[pmStkP+1].description != NULL)
		  pmSeSt[pmStkP+1].description->does = pmSeSt[pmStkP+2].description->does;
          }

	  if (pmSeSt[pmStkP+1].description == NULL)
	    pmSeSt[pmStkP+1].description = pmSeSt[pmStkP+2].description;

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

	if (pmSeSt[pmStkP+2].mentioned != NULL) {
            if (pmSeSt[pmStkP+1].mentioned != NULL)
                /* MENTIONED already declared */
	        lmLogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "MENTIONED", "class/instance", NULL);
	    else {
                pmSeSt[pmStkP+1].mentioned = pmSeSt[pmStkP+2].mentioned;
		pmSeSt[pmStkP+1].mentionedSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].indefinite != NULL) {
            if (pmSeSt[pmStkP+1].indefinite != NULL)
                /* Indefinite Article/Form already declared */
	        lmLogv(&pmSeSt[pmStkP+2].indefinite->srcp, 204, sevERR, "[Indefinite] Article/Form", "class/instance", NULL);
            else
	        pmSeSt[pmStkP+1].indefinite = pmSeSt[pmStkP+2].indefinite;
        }

	if (pmSeSt[pmStkP+2].definite != NULL) {
            if (pmSeSt[pmStkP+1].definite != NULL)
                /* Definite Article/Form already declared */
	        lmLogv(&pmSeSt[pmStkP+2].definite->srcp, 204, sevERR, "Definite Article/Form", "class/instance", NULL);
            else
	        pmSeSt[pmStkP+1].definite = pmSeSt[pmStkP+2].definite;
        }

	if (pmSeSt[pmStkP+2].negative != NULL) {
            if (pmSeSt[pmStkP+1].negative != NULL)
                /* Negative Article/Form already declared */
	        lmLogv(&pmSeSt[pmStkP+2].negative->srcp, 204, sevERR, "Negative Article/Form", "class/instance", NULL);
            else
	        pmSeSt[pmStkP+1].negative = pmSeSt[pmStkP+2].negative;
        }

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXIT_LIST);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VERB_LIST);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, SCRIPT_LIST);
    	break;}
    case 114: { /* <property> = <name>; */
#line 979 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 115: { /* <property> = <pronoun>; */
#line 985 "alan.pmk"
{ List *idList = pmSeSt[pmStkP+1].idList;
	Srcp pronounsSrcp = pmSeSt[pmStkP+1].pronounsSrcp;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].pronouns = idList;
	pmSeSt[pmStkP+1].pronounsSrcp = pronounsSrcp;
    }	break;}
    case 110: { /* <property> = <where> __genSym#8; */
#line 993 "alan.pmk"
{
        Where *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 121: { /* <property> = <is> <attributes>; */
#line 1003 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 116: { /* <property> = <initialize>; */
#line 1009 "alan.pmk"
{
        Initialize *initialize = pmSeSt[pmStkP+1].initialize;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].initialize = initialize;
    }	break;}
    case 111: { /* <property> = <container_properties>; */
#line 1017 "alan.pmk"
{
        Container *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 112: { /* <property> = <description>; */
#line 1027 "alan.pmk"
{
	Description *description = pmSeSt[pmStkP+1].description;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */

	pmSeSt[pmStkP+1].description = description;
    }	break;}
    case 117: { /* <property> = <entered>; */
#line 1036 "alan.pmk"
{
        List *entered = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].entered = entered;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 118: { /* <property> = <mentioned>; */
#line 1046 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].mentioned = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 113: { /* <property> = __genSym#9 <article_or_form>; */
#line 1056 "alan.pmk"
{ Article *indefinite = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].indefinite = indefinite;
    }	break;}
    case 119: { /* <property> = 'definite' <article_or_form>; */
#line 1064 "alan.pmk"
{ Article *definite = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].definite = definite;
    }	break;}
    case 120: { /* <property> = 'negative' <article_or_form>; */
#line 1072 "alan.pmk"
{ Article *article = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].negative = article;
    }	break;}
    case 123: { /* <property> = <exit>; */
#line 1080 "alan.pmk"
{ Exit *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 124: { /* <property> = <verb>; */
#line 1086 "alan.pmk"
{ Verb *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 122: { /* <property> = <script>; */
#line 1092 "alan.pmk"
{ Script *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 129: { /* <exit> = 'exit' <id_list> 'to' ID <optional_exit_body> '.'; */
#line 1102 "alan.pmk"

	pmSeSt[pmStkP+1].ext = newExit(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].idList,
				pmSeSt[pmStkP+4].id,
				pmSeSt[pmStkP+5].chks,
				pmSeSt[pmStkP+5].stms);
	if (pmSeSt[pmStkP+5].id != NULL) { /* END-id given */
	    if (compareStrings(pmSeSt[pmStkP+2].str, pmSeSt[pmStkP+5].id->string) != 0)
		lmLog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].str);
	}
    	break;}
    case 130: { /* <optional_exit_body> =; */
#line 1117 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 131: { /* <optional_exit_body> = <optional_checks> <optional_does> 'end' 'exit' <optional_id>; */
#line 1124 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmLog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 132: { /* <is> = 'is'; */
#line 1139 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 133: { /* <is> = 'are'; */
#line 1144 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 134: { /* <is> = 'has'; */
#line 1149 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 135: { /* <is> = 'can'; */
#line 1154 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 136: { /* <optional_description> =; */
#line 1161 "alan.pmk"

	pmSeSt[pmStkP+1].description = NULL;
    	break;}
    case 137: { /* <optional_description> = <description>; */
#line 1166 "alan.pmk"

	pmSeSt[pmStkP+1].description = pmSeSt[pmStkP+1].description;
    	break;}
    case 138: { /* <description> = 'description' <optional_checks> <optional_does>; */
#line 1173 "alan.pmk"

	if (pmSeSt[pmStkP+2].chks == NULL && pmSeSt[pmStkP+3].stms == NULL) {
	  /* Create a NOP-statement */
	  List *stms = newList(newStatement(&pmSySt[pmStkP+1].srcp,
					NOP_STATEMENT), STATEMENT_LIST);
	  pmSeSt[pmStkP+1].description = newDescription(nulsrcp, NULL, pmSySt[pmStkP+1].srcp, stms);
	} else {
	  Srcp checkSrcp, stmSrcp;
	  if (pmSeSt[pmStkP+2].chks != NULL)
	    checkSrcp = pmSeSt[pmStkP+2].srcp;
	  else
	    checkSrcp = nulsrcp;
	  if (pmSeSt[pmStkP+3].stms != NULL)
	    stmSrcp = pmSeSt[pmStkP+3].srcp;
	  else
	    stmSrcp = pmSySt[pmStkP+1].srcp;

	  pmSeSt[pmStkP+1].description = newDescription(checkSrcp, pmSeSt[pmStkP+2].chks, stmSrcp, pmSeSt[pmStkP+3].stms);

	  if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	    lmLog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
	}
    	break;}
    case 139: { /* <description> = 'description' <statements>; */
#line 1198 "alan.pmk"

	pmSeSt[pmStkP+1].description = newDescription(nulsrcp, NULL,
					pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].stms);
    	break;}
    case 142: { /* <article> = 'article'; */
#line 1211 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					newList(newStatement(&pmSySt[pmStkP+1].srcp, NOP_STATEMENT),
						STATEMENT_LIST),
					FALSE);
    	break;}
    case 143: { /* <article> = 'article' <statements>; */
#line 1219 "alan.pmk"

	 pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					pmSeSt[pmStkP+2].stms,
					FALSE);
    	break;}
    case 144: { /* <form> = 'form'; */
#line 1228 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					newList(newStatement(&pmSySt[pmStkP+1].srcp,
								NOP_STATEMENT),
						STATEMENT_LIST),
					TRUE);
    	break;}
    case 145: { /* <form> = 'form' <statements>; */
#line 1237 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					pmSeSt[pmStkP+2].stms,
					TRUE);
    	break;}
    case 146: { /* <entered> = 'entered' <statements>; */
#line 1247 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 147: { /* <initialize> = 'initialize' <statements>; */
#line 1256 "alan.pmk"

	pmSeSt[pmStkP+1].initialize = newInitialize(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].stms);
    	break;}
    case 148: { /* <mentioned> = 'mentioned' <statements>; */
#line 1264 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 149: { /* <name> = 'name' <ids> <optional_full_stop>; */
#line 1273 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 150: { /* <pronoun> = 'pronoun' <id_list> <optional_full_stop>; */
#line 1280 "alan.pmk"

	pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+2].idList;
	pmSeSt[pmStkP+1].pronounsSrcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 153: { /* <container_properties> = __genSym#10 <optionally_opaque> 'container' <container_body>; */
#line 1294 "alan.pmk"

	pmSeSt[pmStkP+1].cnt = newContainer(newContainerBody(pmSySt[pmStkP+3].srcp,
					pmSeSt[pmStkP+2].opaque,
					pmSeSt[pmStkP+4].id,
					pmSeSt[pmStkP+4].lims,
					pmSeSt[pmStkP+4].stms,
					pmSeSt[pmStkP+4].stms2,
					pmSeSt[pmStkP+4].chks,
					pmSeSt[pmStkP+4].stms3));
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+3].srcp;
    	break;}
    case 156: { /* <optionally_opaque> =; */
#line 1310 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = FALSE; 	break;}
    case 157: { /* <optionally_opaque> = 'opaque'; */
#line 1312 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = TRUE; 	break;}
    case 158: { /* <container_body> = <optional_taking> <optional_limits> <optional_header> <optional_empty> <optional_extract>; */
#line 1318 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+2].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+3].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+4].stms;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+5].chks;
	pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+5].stms;
    	break;}
    case 159: { /* <container_body> = '.'; */
#line 1328 "alan.pmk"

	pmSeSt[pmStkP+1].id  = NULL;
	pmSeSt[pmStkP+1].lims  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
	pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms3 = NULL;
    	break;}
    case 160: { /* <optional_taking> =; */
#line 1340 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 161: { /* <optional_taking> = 'taking' ID '.'; */
#line 1345 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 162: { /* <optional_limits> =; */
#line 1353 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 163: { /* <optional_limits> = 'limits' <limits>; */
#line 1358 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 164: { /* <limits> = <limit>; */
#line 1365 "alan.pmk"

	pmSeSt[pmStkP+1].lims = newList(pmSeSt[pmStkP+1].lim, LIMIT_LIST);
    	break;}
    case 165: { /* <limits> = <limits> <limit>; */
#line 1370 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMIT_LIST);
    	break;}
    case 166: { /* <limit> = <limit_attribute> <else_or_then> <statements>; */
#line 1377 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSeSt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 167: { /* <else_or_then> = 'else'; */
#line 1385 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 168: { /* <else_or_then> = 'then'; */
#line 1386 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 169: { /* <limit_attribute> = <attribute_definition>; */
#line 1391 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 170: { /* <limit_attribute> = 'count' Integer; */
#line 1396 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSySt[pmStkP+1].srcp, newId(pmSySt[pmStkP+1].srcp, "count"), val(pmSySt[pmStkP+2].chars));
    	break;}
    case 171: { /* <optional_header> =; */
#line 1403 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 172: { /* <optional_header> = 'header' <statements>; */
#line 1408 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 173: { /* <optional_empty> =; */
#line 1415 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 174: { /* <optional_empty> = 'else' <statements>; */
#line 1420 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 175: { /* <optional_extract> =; */
#line 1427 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 176: { /* <optional_extract> = 'extract' <optional_checks> <optional_does>; */
#line 1433 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
	if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	  lmLog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
    	break;}
    case 177: { /* <optional_extract> = 'extract' <statements>; */
#line 1441 "alan.pmk"

	pmSeSt[pmStkP+1].chk = NULL;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 178: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1451 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newEvent(&pmSeSt[pmStkP+1].srcp,
				pmSeSt[pmStkP+1].id,
				pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmLog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 179: { /* <event_header> = 'event' ID; */
#line 1464 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 180: { /* <event_tail> = 'end' 'event' <optional_id> '.'; */
#line 1472 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 181: { /* <script> = 'script' ID __genSym#11 <optional_description> <step_list>; */
#line 1480 "alan.pmk"

	if (pmSeSt[pmStkP+4].description != NULL &&
        pmSeSt[pmStkP+4].description->checks != NULL)
	  lmLog(&pmSeSt[pmStkP+4].description->checkSrcp,
      		228, sevERR, "");
	pmSeSt[pmStkP+1].scr = newScript(&pmSySt[pmStkP+1].srcp,
                              pmSeSt[pmStkP+2].id,
			                  pmSeSt[pmStkP+4].description,
			                  pmSeSt[pmStkP+5].stps);
    	break;}
    case 184: { /* <step_list> = <step>; */
#line 1494 "alan.pmk"

	pmSeSt[pmStkP+1].stps = newList(pmSeSt[pmStkP+1].stp, STEP_LIST);
    	break;}
    case 185: { /* <step_list> = <step_list> <step>; */
#line 1499 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STEP_LIST);
    	break;}
    case 186: { /* <step> = 'step' <statements>; */
#line 1506 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 187: { /* <step> = 'step' 'after' <expression> __genSym#12 <statements>; */
#line 1514 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+3].exp,
				NULL,
				pmSeSt[pmStkP+5].stms);
    	break;}
    case 188: { /* <step> = 'step' 'wait' 'until' <expression> __genSym#13 <statements>; */
#line 1522 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
				NULL,
				pmSeSt[pmStkP+4].exp,
				pmSeSt[pmStkP+6].stms);
    	break;}
    case 193: { /* <rule> = 'when' <expression> <then> <statements> <optional_end_when>; */
#line 1533 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newRule(&pmSySt[pmStkP+1].srcp,
		 		pmSeSt[pmStkP+2].exp,
				pmSeSt[pmStkP+4].stms);
    	break;}
    case 200: { /* <start> = 'start' <where> '.' <optional_statements>; */
#line 1553 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 201: { /* <optional_statements> =; */
#line 1562 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 202: { /* <optional_statements> = <statements>; */
#line 1567 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 203: { /* <statements> = <statement>; */
#line 1574 "alan.pmk"

	pmSeSt[pmStkP+1].stms = newList(pmSeSt[pmStkP+1].stm, STATEMENT_LIST);
    	break;}
    case 204: { /* <statements> = <statements> <statement>; */
#line 1579 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STATEMENT_LIST);
    	break;}
    case 205: { /* <statement> = <output_statement>; */
#line 1586 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 207: { /* <statement> = <manipulation_statement>; */
#line 1591 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 210: { /* <statement> = <assignment_statement>; */
#line 1596 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 212: { /* <statement> = <conditional_statement>; */
#line 1601 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 211: { /* <statement> = <repetition_statement>; */
#line 1606 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 208: { /* <statement> = <actor_statement>; */
#line 1611 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 209: { /* <statement> = <event_statement>; */
#line 1616 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 206: { /* <statement> = <special_statement>; */
#line 1621 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 213: { /* <output_statement> = STRING; */
#line 1629 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, PRINT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 214: { /* <output_statement> = 'describe' <what> '.'; */
#line 1636 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newDescribeStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 215: { /* <output_statement> = 'say' <say_form> <expression> '.'; */
#line 1641 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.say.form = pmSeSt[pmStkP+2].sayForm;
    	break;}
    case 216: { /* <output_statement> = 'list' <primary> '.'; */
#line 1648 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newListStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 217: { /* <output_statement> = 'show' ID '.'; */
#line 1653 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newShowStatement(pmSySt[pmStkP+1].srcp, newResource(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id));
    	break;}
    case 218: { /* <output_statement> = 'play' ID '.'; */
#line 1658 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newPlayStatement(pmSySt[pmStkP+1].srcp, newResource(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id));
    	break;}
    case 219: { /* <output_statement> = 'style' ID '.'; */
#line 1663 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStyleStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].id);
    	break;}
    case 220: { /* <say_form> =; */
#line 1670 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_SIMPLE; 	break;}
    case 221: { /* <say_form> = 'the'; */
#line 1672 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_DEFINITE; 	break;}
    case 222: { /* <say_form> = 'an'; */
#line 1674 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_INDEFINITE; 	break;}
    case 223: { /* <say_form> = 'it'; */
#line 1676 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_PRONOUN; 	break;}
    case 224: { /* <say_form> = 'no'; */
#line 1678 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_NEGATIVE; 	break;}
    case 225: { /* <manipulation_statement> = 'empty' <primary> <optional_where> '.'; */
#line 1685 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newEmptyStatement(pmSySt[pmStkP+1].srcp,
							  pmSeSt[pmStkP+2].exp,
							  pmSeSt[pmStkP+3].whr);
    	break;}
    case 226: { /* <manipulation_statement> = 'locate' <primary> <where> '.'; */
#line 1692 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newLocateStatement(pmSySt[pmStkP+1].srcp,
							   pmSeSt[pmStkP+2].exp,
							   pmSeSt[pmStkP+3].whr);
    	break;}
    case 227: { /* <manipulation_statement> = 'include' <primary> 'in' <what> '.'; */
#line 1699 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newIncludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 228: { /* <manipulation_statement> = 'exclude' <primary> 'from' <what> '.'; */
#line 1706 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newExcludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 230: { /* <event_statement> = 'schedule' <what> <optional_where> 'after' <expression> '.'; */
#line 1717 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newScheduleStatement(pmSySt[pmStkP+1].srcp,
						      pmSeSt[pmStkP+2].exp,
						      pmSeSt[pmStkP+3].whr,
						      pmSeSt[pmStkP+5].exp);
    	break;}
    case 229: { /* <event_statement> = 'cancel' <what> '.'; */
#line 1725 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newCancelStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 231: { /* <assignment_statement> = 'make' <primary> <something> '.'; */
#line 1734 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, MAKE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].present;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 235: { /* <assignment_statement> = 'set' <attribute_reference> 'to' <expression> '.'; */
#line 1742 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SET_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 233: { /* <assignment_statement> = 'increase' <attribute_reference> <optional_by_clause> '.'; */
#line 1749 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, INCREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 234: { /* <assignment_statement> = 'decrease' <attribute_reference> <optional_by_clause> '.'; */
#line 1756 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DECREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 232: { /* <assignment_statement> = 'strip' <optional_first_or_last> <optional_expression> <optional_word_or_character> 'from' <expression> <optional_into> '.'; */
#line 1763 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STRIP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.strip.first = pmSeSt[pmStkP+2].first;
	pmSeSt[pmStkP+1].stm->fields.strip.count = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.wordOrChar = pmSeSt[pmStkP+4].wordOrChar;
	pmSeSt[pmStkP+1].stm->fields.strip.from = pmSeSt[pmStkP+6].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.into = pmSeSt[pmStkP+7].exp;
    	break;}
    case 236: { /* <optional_by_clause> =; */
#line 1775 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 237: { /* <optional_by_clause> = 'by' <expression>; */
#line 1780 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 238: { /* <optional_first_or_last> =; */
#line 1787 "alan.pmk"
 pmSeSt[pmStkP+1].first = TRUE; 	break;}
    case 239: { /* <optional_first_or_last> = 'first'; */
#line 1789 "alan.pmk"
 pmSeSt[pmStkP+1].first = TRUE; 	break;}
    case 240: { /* <optional_first_or_last> = 'last'; */
#line 1791 "alan.pmk"
 pmSeSt[pmStkP+1].first = FALSE; 	break;}
    case 241: { /* <optional_word_or_character> =; */
#line 1796 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 242: { /* <optional_word_or_character> = 'word'; */
#line 1798 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 243: { /* <optional_word_or_character> = 'words'; */
#line 1800 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = TRUE; 	break;}
    case 244: { /* <optional_word_or_character> = 'character'; */
#line 1802 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = FALSE; 	break;}
    case 245: { /* <optional_word_or_character> = 'characters'; */
#line 1804 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = FALSE; 	break;}
    case 246: { /* <optional_into> =; */
#line 1809 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 247: { /* <optional_into> = 'into' <expression>; */
#line 1812 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp; 	break;}
    case 248: { /* <conditional_statement> = <if_statement>; */
#line 1818 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 249: { /* <conditional_statement> = <depending_statement>; */
#line 1823 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 250: { /* <if_statement> = 'if' <expression> 'then' <statements> <optional_elsif_list> <optional_else_part> 'end' 'if' '.'; */
#line 1831 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	if (pmSeSt[pmStkP+5].stms == NULL)
	    pmSeSt[pmStkP+1].stm->fields.iff.els = pmSeSt[pmStkP+6].stms;
	else {
	    /* Connect the else_part to the elsif_list */
	    (getLastListNode(pmSeSt[pmStkP+5].stms))->member.stm->fields.iff.els =
			pmSeSt[pmStkP+6].stms;
	    pmSeSt[pmStkP+5].stms->next = NULL;
	    pmSeSt[pmStkP+1].stm->fields.iff.els = pmSeSt[pmStkP+5].stms;
	}
    	break;}
    case 251: { /* <optional_elsif_list> =; */
#line 1849 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 252: { /* <optional_elsif_list> = <elsif_list>; */
#line 1854 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 253: { /* <elsif_list> = 'elsif' <expression> 'then' <statements>; */
#line 1861 "alan.pmk"
{
	Statement *stm;

	stm = newStatement(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = newList(stm, STATEMENT_LIST);
    }	break;}
    case 254: { /* <elsif_list> = <elsif_list> 'elsif' <expression> 'then' <statements>; */
#line 1874 "alan.pmk"
{
	Statement *stm;

	stm = newStatement(&pmSySt[pmStkP+2].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+5].stms;
	stm->fields.iff.els = NULL;

	/* Now connect it to the previous ELSIFs, I.e connect the */
	/* else-part of the last element in the elsif_list to this */
	/* IF-statement and also concat this element to the list so */
	/* we'll know were the end is next time */
	(getLastListNode(pmSeSt[pmStkP+1].stms))->member.stm->fields.iff.els = newList(stm, STATEMENT_LIST);
	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, stm, STATEMENT_LIST);
    }	break;}
    case 255: { /* <optional_else_part> =; */
#line 1893 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 256: { /* <optional_else_part> = 'else' <statements>; */
#line 1898 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 257: { /* <depending_statement> = 'depending' 'on' <primary> <depend_cases> 'end' __genSym#15 '.'; */
#line 1905 "alan.pmk"
{ Statement *stm;
	stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPEND_STATEMENT);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 260: { /* <depend_cases> = <depend_case>; */
#line 1916 "alan.pmk"

	pmSeSt[pmStkP+1].cases = newList(pmSeSt[pmStkP+1].stm, CASE_LIST);
    	break;}
    case 261: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1921 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASE_LIST);
    	break;}
    case 263: { /* <depend_case> = <right_hand_side> 'then' <statements>; */
#line 1928 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+2].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 262: { /* <depend_case> = 'else' <statements>; */
#line 1935 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 264: { /* <repetition_statement> = <for_each> ID <optional_loop_filters> 'do' <statements> 'end' <for_each> __genSym#16; */
#line 1945 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newEachStatement(pmSeSt[pmStkP+1].srcp,
							pmSeSt[pmStkP+2].id,
							pmSeSt[pmStkP+3].exps,
							pmSeSt[pmStkP+5].stms);
    	break;}
    case 267: { /* <optional_loop_filters> =; */
#line 1955 "alan.pmk"

	pmSeSt[pmStkP+1].exps = NULL;
    	break;}
    case 268: { /* <optional_loop_filters> = <filters>; */
#line 1960 "alan.pmk"

	pmSeSt[pmStkP+1].exps = pmSeSt[pmStkP+1].exps;
    	break;}
    case 269: { /* <optional_loop_filters> = 'between' <arithmetic> 'and' <arithmetic>; */
#line 1965 "alan.pmk"

	pmSeSt[pmStkP+1].exps = newList(newBetweenExpression(pmSySt[pmStkP+1].srcp,
					     				NULL,
					     				FALSE,
					     				pmSeSt[pmStkP+2].exp,
					     				pmSeSt[pmStkP+4].exp),
						EXPRESSION_LIST);
    	break;}
    case 270: { /* <for_each> = 'for'; */
#line 1977 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 271: { /* <for_each> = 'each'; */
#line 1979 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 272: { /* <for_each> = 'for' 'each'; */
#line 1981 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 274: { /* <actor_statement> = 'use' 'script' ID <optional_for_actor> '.'; */
#line 1987 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newUseStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+3].id, pmSeSt[pmStkP+4].exp);
    	break;}
    case 273: { /* <actor_statement> = 'stop' <what> '.'; */
#line 1992 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STOP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.stop.actor = pmSeSt[pmStkP+2].exp;
    	break;}
    case 275: { /* <optional_for_actor> =; */
#line 2000 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 276: { /* <optional_for_actor> = 'for' <what>; */
#line 2005 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 277: { /* <special_statement> = 'quit' '.'; */
#line 2013 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, QUIT_STATEMENT);
    	break;}
    case 278: { /* <special_statement> = 'look' '.'; */
#line 2018 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, LOOK_STATEMENT);
    	break;}
    case 279: { /* <special_statement> = 'save' '.'; */
#line 2023 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAVE_STATEMENT);
    	break;}
    case 280: { /* <special_statement> = 'restore' '.'; */
#line 2028 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTORE_STATEMENT);
    	break;}
    case 281: { /* <special_statement> = 'restart' '.'; */
#line 2033 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTART_STATEMENT);
    	break;}
    case 282: { /* <special_statement> = 'score' <optional_integer> '.'; */
#line 2038 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SCORE_STATEMENT);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scoreCount;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 285: { /* <special_statement> = 'visits' Integer '.'; */
#line 2049 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, VISITS_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 283: { /* <special_statement> = 'transcript' <on_or_off> '.'; */
#line 2055 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, TRANSCRIPT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.transcript.on_or_off = pmSeSt[pmStkP+2].on_or_off;
    	break;}
    case 284: { /* <special_statement> = 'system' STRING '.'; */
#line 2061 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SYSTEM_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 286: { /* <on_or_off> = 'on'; */
#line 2070 "alan.pmk"
 pmSeSt[pmStkP+1].on_or_off = TRUE; 	break;}
    case 287: { /* <on_or_off> = 'off'; */
#line 2073 "alan.pmk"
 pmSeSt[pmStkP+1].on_or_off = FALSE; 	break;}
    case 288: { /* <optional_expression> =; */
#line 2080 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 289: { /* <optional_expression> = <expression>; */
#line 2083 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp; 	break;}
    case 290: { /* <expression> = <term>; */
#line 2088 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 291: { /* <expression> = <expression> 'or' <term>; */
#line 2093 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
						pmSeSt[pmStkP+1].exp,
						FALSE,
						OR_OPERATOR,
						pmSeSt[pmStkP+3].exp);
    	break;}
    case 292: { /* <term> = <factor>; */
#line 2104 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 293: { /* <term> = <term> 'and' <factor>; */
#line 2109 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
					  pmSeSt[pmStkP+1].exp,
					  FALSE,
					  AND_OPERATOR,
					  pmSeSt[pmStkP+3].exp);
    	break;}
    case 294: { /* <factor> = <arithmetic>; */
#line 2120 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 296: { /* <factor> = <factor> <optional_not> <relop> <arithmetic>; */
#line 2125 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+3].srcp,
					    pmSeSt[pmStkP+1].exp,
					    pmSeSt[pmStkP+2].present,
					    pmSeSt[pmStkP+3].op,
					    pmSeSt[pmStkP+4].exp);
    	break;}
    case 295: { /* <factor> = <factor> <optional_not> <where>; */
#line 2134 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhereExpression(pmSeSt[pmStkP+3].srcp,
					   pmSeSt[pmStkP+1].exp,
					   pmSeSt[pmStkP+2].present,
					   pmSeSt[pmStkP+3].whr);
    	break;}
    case 298: { /* <factor> = <factor> <optional_not> 'between' <arithmetic> 'and' <arithmetic>; */
#line 2142 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBetweenExpression(pmSySt[pmStkP+3].srcp,
					     pmSeSt[pmStkP+1].exp,
					     pmSeSt[pmStkP+2].present,
					     pmSeSt[pmStkP+4].exp,
					     pmSeSt[pmStkP+6].exp);
    	break;}
    case 297: { /* <factor> = <factor> <optional_not> 'contains' <arithmetic>; */
#line 2151 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+3].srcp,
					    pmSeSt[pmStkP+1].exp,
					    pmSeSt[pmStkP+2].present,
					    CONTAINS_OPERATOR,
					    pmSeSt[pmStkP+4].exp);
    	break;}
    case 299: { /* <arithmetic> = <primary>; */
#line 2163 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 301: { /* <arithmetic> = <primary> 'isa' ID; */
#line 2168 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].exp, FALSE, pmSeSt[pmStkP+3].id);
    	break;}
    case 302: { /* <arithmetic> = <primary> <is> <something>; */
#line 2173 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id,
					 pmSeSt[pmStkP+3].present, pmSeSt[pmStkP+1].exp);
    	break;}
    case 303: { /* <arithmetic> = <arithmetic> <binop> <primary>; */
#line 2179 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+2].srcp,
						pmSeSt[pmStkP+1].exp,
						FALSE,
						pmSeSt[pmStkP+2].op,
						pmSeSt[pmStkP+3].exp);
    	break;}
    case 300: { /* <arithmetic> = <aggregate> <filters>; */
#line 2188 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAggregateExpression(pmSeSt[pmStkP+1].srcp,
							pmSeSt[pmStkP+1].agr,
							pmSeSt[pmStkP+1].id,
							pmSeSt[pmStkP+2].exps);
    	break;}
    case 304: { /* <filters> = <filter>; */
#line 2199 "alan.pmk"

        pmSeSt[pmStkP+1].exps = newList(pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 305: { /* <filters> = <filters> ',' <filter>; */
#line 2204 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(pmSeSt[pmStkP+1].exps, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 306: { /* <filter> = <optional_not> <where>; */
#line 2223 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhereExpression(pmSeSt[pmStkP+2].srcp,
						      NULL,
						      pmSeSt[pmStkP+1].present,
						      pmSeSt[pmStkP+2].whr);
    	break;}
    case 307: { /* <filter> = <optional_not> 'isa' ID; */
#line 2231 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, NULL, pmSeSt[pmStkP+1].present, pmSeSt[pmStkP+3].id);
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 308: { /* <filter> = <is> <something>; */
#line 2237 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+1].srcp,
						pmSeSt[pmStkP+2].id,
						pmSeSt[pmStkP+2].present, NULL);
    	break;}
    case 310: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 2246 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+2].srcp,
						     NULL,
						     pmSeSt[pmStkP+1].present,
						     pmSeSt[pmStkP+2].op,
						     pmSeSt[pmStkP+3].exp);
    	break;}
    case 312: { /* <right_hand_side> = <optional_not> 'between' <arithmetic> 'and' <arithmetic>; */
#line 2255 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBetweenExpression(pmSySt[pmStkP+2].srcp,
						      NULL,
						      pmSeSt[pmStkP+1].present,
						      pmSeSt[pmStkP+3].exp,
						      pmSeSt[pmStkP+5].exp);
    	break;}
    case 311: { /* <right_hand_side> = <optional_not> 'contains' <factor>; */
#line 2264 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
						     NULL,
						     pmSeSt[pmStkP+1].present,
						     CONTAINS_OPERATOR,
						     pmSeSt[pmStkP+3].exp);

    	break;}
    case 318: { /* <primary> = '(' <expression> ')'; */
#line 2279 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 316: { /* <primary> = <optional_minus> Integer; */
#line 2284 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp,
					      pmSeSt[pmStkP+1].minus?
							-val(pmSySt[pmStkP+2].chars):
							val(pmSySt[pmStkP+2].chars));
    	break;}
    case 313: { /* <primary> = STRING; */
#line 2292 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp,
                                             pmSySt[pmStkP+1].fpos,
                                             pmSySt[pmStkP+1].len);
    	break;}
    case 314: { /* <primary> = <what>; */
#line 2299 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 320: { /* <primary> = 'random' <primary> 'to' <primary>; */
#line 2304 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newRandomRangeExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp, pmSeSt[pmStkP+4].exp);
    	break;}
    case 319: { /* <primary> = 'random' <optional_transitivity> 'in' <primary>; */
#line 2309 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newRandomInExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+4].exp, pmSeSt[pmStkP+2].transitivity);
    	break;}
    case 315: { /* <primary> = 'score'; */
#line 2314 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newScoreExpression(pmSySt[pmStkP+1].srcp);
    	break;}
    case 317: { /* <primary> = '{' <optional_members> '}'; */
#line 2319 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newSetExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].set);
    	break;}
    case 322: { /* <aggregate> = <aggregator> 'of' ID; */
#line 2327 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = pmSeSt[pmStkP+1].agr;
        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 321: { /* <aggregate> = 'count'; */
#line 2334 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = COUNT_AGGREGATE;
        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 323: { /* <aggregator> = 'max'; */
#line 2343 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = MAX_AGGREGATE;
    	break;}
    case 324: { /* <aggregator> = 'min'; */
#line 2349 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = MIN_AGGREGATE;
    	break;}
    case 325: { /* <aggregator> = 'sum'; */
#line 2355 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = SUM_AGGREGATE;
    	break;}
    case 326: { /* <something> = <optional_not> ID; */
#line 2364 "alan.pmk"

        pmSeSt[pmStkP+1].present = pmSeSt[pmStkP+1].present;
        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 327: { /* <what> = <simple_what>; */
#line 2372 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newWhatExpression(pmSeSt[pmStkP+1].srcp, pmSeSt[pmStkP+1].wht);
    	break;}
    case 328: { /* <what> = <attribute_reference>; */
#line 2377 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 329: { /* <simple_what> = ID; */
#line 2384 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatId(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id);
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 331: { /* <simple_what> = 'current' 'actor'; */
#line 2390 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatActor(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 332: { /* <simple_what> = 'current' 'location'; */
#line 2396 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatLocation(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 330: { /* <simple_what> = 'this'; */
#line 2402 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatThis(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 333: { /* <attribute_reference> = ID 'of' <what>; */
#line 2411 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].id, FALSE, pmSeSt[pmStkP+3].exp);
    	break;}
    case 334: { /* <attribute_reference> = <what> ':' ID; */
#line 2416 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id, FALSE, pmSeSt[pmStkP+1].exp);
    	break;}
    case 335: { /* <optional_where> =; */
#line 2423 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&nulsrcp, FALSE, WHERE_DEFAULT, NULL);
    	break;}
    case 336: { /* <optional_where> = <where>; */
#line 2428 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 337: { /* <where> = <optional_transitivity> 'here'; */
#line 2435 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_HERE, NULL);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 338: { /* <where> = <optional_transitivity> 'nearby'; */
#line 2441 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_NEARBY, NULL);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 339: { /* <where> = <optional_transitivity> 'at' <primary>; */
#line 2447 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_AT, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 340: { /* <where> = <optional_transitivity> 'in' <primary>; */
#line 2453 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_IN, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 341: { /* <where> = <optional_transitivity> 'near' <what>; */
#line 2459 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_NEAR, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 342: { /* <binop> = '+'; */
#line 2467 "alan.pmk"

        pmSeSt[pmStkP+1].op = PLUS_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 343: { /* <binop> = '-'; */
#line 2473 "alan.pmk"

        pmSeSt[pmStkP+1].op = MINUS_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 344: { /* <binop> = '*'; */
#line 2479 "alan.pmk"

        pmSeSt[pmStkP+1].op = MULT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 345: { /* <binop> = '/'; */
#line 2485 "alan.pmk"

        pmSeSt[pmStkP+1].op = DIV_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 346: { /* <relop> = '<>'; */
#line 2494 "alan.pmk"

        pmSeSt[pmStkP+1].op   = NE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 347: { /* <relop> = '='; */
#line 2500 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EQ_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 348: { /* <relop> = '=='; */
#line 2506 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EXACT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 349: { /* <relop> = '>='; */
#line 2512 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 350: { /* <relop> = '<='; */
#line 2518 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 351: { /* <relop> = '>'; */
#line 2524 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 352: { /* <relop> = '<'; */
#line 2530 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 353: { /* <optional_qual> =; */
#line 2540 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 354: { /* <optional_qual> = 'before'; */
#line 2545 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 355: { /* <optional_qual> = 'after'; */
#line 2551 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 356: { /* <optional_qual> = 'only'; */
#line 2557 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 357: { /* <optional_not> =; */
#line 2566 "alan.pmk"

        pmSeSt[pmStkP+1].present = FALSE;
    	break;}
    case 358: { /* <optional_not> = 'not'; */
#line 2571 "alan.pmk"

        pmSeSt[pmStkP+1].present = TRUE;
    	break;}
    case 359: { /* <optional_transitivity> =; */
#line 2578 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = DEFAULT_TRANSITIVITY;
    	break;}
    case 360: { /* <optional_transitivity> = 'transitively'; */
#line 2583 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = TRANSITIVELY;
    	break;}
    case 361: { /* <optional_transitivity> = 'directly'; */
#line 2588 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = DIRECTLY;
    	break;}
    case 362: { /* <optional_transitivity> = 'indirectly'; */
#line 2593 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = INDIRECTLY;
    	break;}
    case 363: { /* <optional_id> =; */
#line 2600 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 364: { /* <optional_id> = ID; */
#line 2605 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 365: { /* <ids> = ID; */
#line 2612 "alan.pmk"

        pmSeSt[pmStkP+1].idList = newList(pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 366: { /* <ids> = <ids> ID; */
#line 2617 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, ID_LIST);
    	break;}
    case 367: { /* <id_list> = ID; */
#line 2624 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = newList(pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 368: { /* <id_list> = <id_list> ',' ID; */
#line 2630 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, ID_LIST);
    	break;}
    case 369: { /* <optional_integer> =; */
#line 2638 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 370: { /* <optional_integer> = Integer; */
#line 2643 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 371: { /* <optional_minus> =; */
#line 2650 "alan.pmk"

        pmSeSt[pmStkP+1].minus = FALSE;
    	break;}
    case 372: { /* <optional_minus> = '-'; */
#line 2655 "alan.pmk"

        pmSeSt[pmStkP+1].minus = TRUE;
    	break;}
    case 373: { /* ID = Identifier; */
#line 2662 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 374: { /* ID = 'location'; */
#line 2673 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 375: { /* ID = 'actor'; */
#line 2678 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 376: { /* ID = 'opaque'; */
#line 2683 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 377: { /* ID = 'visits'; */
#line 2688 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 378: { /* ID = 'contains'; */
#line 2693 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 379: { /* ID = 'on'; */
#line 2698 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 380: { /* ID = 'it'; */
#line 2703 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 381: { /* ID = 'of'; */
#line 2708 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 382: { /* ID = 'first'; */
#line 2713 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 383: { /* ID = 'into'; */
#line 2718 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 384: { /* ID = 'taking'; */
#line 2723 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 385: { /* ID = 'off'; */
#line 2728 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    default: break; }
}/*pmPaSema()*/

