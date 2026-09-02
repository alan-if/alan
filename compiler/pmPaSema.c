/*----------------------------------------------------------------------*\

    pmPaSema.c

    ParserMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


#include "acode.h"
#include "add_x.h"
#include "adv_x.h"
#include "alt.h"
#include "article_x.h"
#include "atr_x.h"
#include "chk_x.h"
#include "cla_x.h"
#include "cnt_x.h"
#include "description_x.h"
#include "elm_x.h"
#include "evt.h"
#include "exp_x.h"
#include "ext_x.h"
#include "id_x.h"
#include "initialize_x.h"
#include "ins_x.h"
#include "lim.h"
#include "lmlog.h"
#include "lst_x.h"
#include "msg.h"
#include "opt.h"
#include "prop_x.h"
#include "res_x.h"
#include "resource_x.h"
#include "rul_x.h"
#include "sco.h"
#include "scr_x.h"
#include "smScan.h"
#include "srcp_x.h"
#include "stm_x.h"
#include "stp_x.h"
#include "str.h"
#include "stx_x.h"
#include "sym_x.h"
#include "syn.h"
#include "token.h"
#include "types.h"
#include "util.h"
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
    bool wordOrChar;
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
    bool present;
    bool opaque;
    OperatorKind op;
    bool on_or_off;
    bool not;
    Article *negative;
    List *nams;
    List *nam;
    List *msgs;
    Message *msg;
    bool minus;
    bool isMeta;
    Srcp mentionedSrcp;
    List *mentioned;
    List *lims;
    LimNod *lim;
    bool isForm;
    List *inss;
    Instance *ins;
    Article *indefinite;
    List *idList2;
    List *idList;
    Id *id;
    List *form;
    bool first;
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
    bool definiteIsForm;
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

#line 81 "alan.pmk"


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
#line 202 "alan.pmk"
    switch (rule) {
    case 8: { /* <option> = ID '.'; */
#line 222 "alan.pmk"

        optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, true);
    	break;}
    case 9: { /* <option> = 'no' ID '.'; */
#line 227 "alan.pmk"

        optBool(pmSeSt[pmStkP+2].id->string, &pmSeSt[pmStkP+2].id->srcp, false);
    	break;}
    case 10: { /* <option> = ID ID '.'; */
#line 232 "alan.pmk"

        optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 11: { /* <option> = ID Integer '.'; */
#line 237 "alan.pmk"

        optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 20: { /* <declaration> = <synonyms>; */
#line 250 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns);	break;}
    case 16: { /* <declaration> = <messages>; */
#line 253 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 21: { /* <declaration> = <syntax>; */
#line 256 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 22: { /* <declaration> = <verb>; */
#line 259 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VERB_LIST); 	break;}
    case 17: { /* <declaration> = <class>; */
#line 262 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLASS_LIST); 	break;}
    case 23: { /* <declaration> = <addition>; */
#line 265 "alan.pmk"
 adv.adds = concat(adv.adds, pmSeSt[pmStkP+1].add, ADD_LIST); 	break;}
    case 18: { /* <declaration> = <instance>; */
#line 268 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSTANCE_LIST); 	break;}
    case 24: { /* <declaration> = <event>; */
#line 271 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVENT_LIST); 	break;}
    case 19: { /* <declaration> = <rule>; */
#line 274 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULE_LIST); 	break;}
    case 15: { /* <declaration> = <prompt>; */
#line 277 "alan.pmk"

      if (adv.prompt != NULL)
        lmlog(&pmSeSt[pmStkP+1].srcp, 308, sevWAR, "PROMPT");
      adv.prompt = pmSeSt[pmStkP+1].prompt;
    	break;}
    case 25: { /* <prompt> = 'prompt' <statements>; */
#line 288 "alan.pmk"

      pmSeSt[pmStkP+1].prompt = pmSeSt[pmStkP+2].stms;
      pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 26: { /* <attributes> = <attribute_definition> '.'; */
#line 297 "alan.pmk"

        pmSeSt[pmStkP+1].atrs = newList(pmSeSt[pmStkP+1].atr, ATTRIBUTE_LIST);
    	break;}
    case 27: { /* <attributes> = <attributes> <attribute_definition> '.'; */
#line 302 "alan.pmk"

        pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATTRIBUTE_LIST);
    	break;}
    case 28: { /* <attribute_definition> = ID; */
#line 309 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, true);
    	break;}
    case 29: { /* <attribute_definition> = 'not' ID; */
#line 314 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id, false);
    	break;}
    case 32: { /* <attribute_definition> = ID <optional_minus> Integer; */
#line 319 "alan.pmk"

        if (pmSeSt[pmStkP+2].minus)
           pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, -val(pmSySt[pmStkP+3].chars));
        else
           pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, val(pmSySt[pmStkP+3].chars));
    	break;}
    case 30: { /* <attribute_definition> = ID STRING; */
#line 327 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newStringAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 31: { /* <attribute_definition> = ID ID; */
#line 331 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newReferenceAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+2].id);
    	break;}
    case 33: { /* <attribute_definition> = ID '{' <optional_members> '}'; */
#line 336 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newSetAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id,
                                                      newSetExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+3].set));
    	break;}
    case 34: { /* <optional_members> =; */
#line 344 "alan.pmk"
 pmSeSt[pmStkP+1].set = NULL; 	break;}
    case 35: { /* <optional_members> = <set_members>; */
#line 347 "alan.pmk"
 pmSeSt[pmStkP+1].set = pmSeSt[pmStkP+1].set; 	break;}
    case 36: { /* <set_members> = <set_member>; */
#line 352 "alan.pmk"

        pmSeSt[pmStkP+1].set = newList(pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 37: { /* <set_members> = <set_members> ',' <set_member>; */
#line 357 "alan.pmk"

        pmSeSt[pmStkP+1].set = concat(pmSeSt[pmStkP+1].set, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 38: { /* <set_member> = <what>; */
#line 364 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 40: { /* <set_member> = <optional_minus> Integer; */
#line 369 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].minus?-val(pmSySt[pmStkP+2].chars):val(pmSySt[pmStkP+2].chars));
    	break;}
    case 39: { /* <set_member> = STRING; */
#line 374 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].fpos, pmSySt[pmStkP+1].len);
    	break;}
    case 41: { /* <synonyms> = 'synonyms' <synonym_list>; */
#line 382 "alan.pmk"

        pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 42: { /* <synonym_list> = <synonym_declaration>; */
#line 389 "alan.pmk"

        pmSeSt[pmStkP+1].syns = newList(pmSeSt[pmStkP+1].syn, SYNONYM_LIST);
    	break;}
    case 43: { /* <synonym_list> = <synonym_list> <synonym_declaration>; */
#line 394 "alan.pmk"

        pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNONYM_LIST);
    	break;}
    case 44: { /* <synonym_declaration> = <id_list> '=' ID '.'; */
#line 401 "alan.pmk"

        pmSeSt[pmStkP+1].syn = newSynonym(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id);
    	break;}
    case 45: { /* <messages> = 'message' <message_list>; */
#line 409 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 46: { /* <message_list> = <message>; */
#line 416 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = newList(pmSeSt[pmStkP+1].msg, MESSAGE_LIST);
    	break;}
    case 47: { /* <message_list> = <message_list> <message>; */
#line 421 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MESSAGE_LIST);
    	break;}
    case 48: { /* <message> = ID ':' <statements>; */
#line 428 "alan.pmk"

        pmSeSt[pmStkP+1].msg = newMessage(&pmSySt[pmStkP+2].srcp,
                                    pmSeSt[pmStkP+1].id,
                                    pmSeSt[pmStkP+3].stms);
    	break;}
    case 49: { /* <syntax> = 'syntax' <syntax_list>; */
#line 439 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 50: { /* <syntax_list> = <syntax_item>; */
#line 446 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = newList(pmSeSt[pmStkP+1].stx, SYNTAX_LIST);
    	break;}
    case 51: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 451 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, SYNTAX_LIST);
    	break;}
    case 52: { /* <syntax_item> = ID '=' <syntax_elements> <optional_syntax_restrictions>; */
#line 458 "alan.pmk"

        pmSeSt[pmStkP+1].stx = newSyntax(pmSySt[pmStkP+2].srcp,
                                       pmSeSt[pmStkP+1].id,
                                       concat(pmSeSt[pmStkP+3].elms,
                                              newEndOfSyntax(),
                                              ELEMENT_LIST),
                                       pmSeSt[pmStkP+4].ress,
	                                   pmSeSt[pmStkP+4].srcp);
    	break;}
    case 53: { /* <syntax_elements> = <syntax_element>; */
#line 471 "alan.pmk"

        pmSeSt[pmStkP+1].elms = newList(pmSeSt[pmStkP+1].elm, ELEMENT_LIST);
    	break;}
    case 54: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 476 "alan.pmk"

        pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
                                         pmSeSt[pmStkP+2].elm,
                                         ELEMENT_LIST);
    	break;}
    case 55: { /* <syntax_element> = ID; */
#line 485 "alan.pmk"

        pmSeSt[pmStkP+1].elm = newWordElement(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id);
    	break;}
    case 56: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 491 "alan.pmk"

        pmSeSt[pmStkP+1].elm = newParameterElement(pmSeSt[pmStkP+2].id->srcp,
                                                    pmSeSt[pmStkP+2].id,
                                                    pmSeSt[pmStkP+4].bits);
    	break;}
    case 57: { /* <optional_indicators> =; */
#line 500 "alan.pmk"

        pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 58: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 505 "alan.pmk"

        pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 59: { /* <indicator> = '*'; */
#line 512 "alan.pmk"

        pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 60: { /* <indicator> = '!'; */
#line 517 "alan.pmk"

        pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 61: { /* <syntax_restriction_clauses> = <syntax_restriction>; */
#line 530 "alan.pmk"

        pmSeSt[pmStkP+1].ress = newList(pmSeSt[pmStkP+1].res, RESTRICTION_LIST);
    	break;}
    case 62: { /* <syntax_restriction_clauses> = <syntax_restriction_clauses> 'and' <syntax_restriction>; */
#line 535 "alan.pmk"

        pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
                                                    pmSeSt[pmStkP+3].res, RESTRICTION_LIST);
    	break;}
    case 63: { /* <syntax_restriction> = ID 'isa' <restriction_class> 'else' <statements>; */
#line 543 "alan.pmk"

        pmSeSt[pmStkP+1].res = newRestriction(pmSySt[pmStkP+2].srcp,
                                                   pmSeSt[pmStkP+1].id,
                                                   pmSeSt[pmStkP+3].restrictionKind,
                                                   pmSeSt[pmStkP+3].id,
                                                   pmSeSt[pmStkP+5].stms);
    	break;}
    case 64: { /* <restriction_class> = ID; */
#line 554 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
        pmSeSt[pmStkP+1].restrictionKind = ID_RESTRICTION;
    	break;}
    case 65: { /* <restriction_class> = 'container'; */
#line 560 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
        pmSeSt[pmStkP+1].restrictionKind = CONTAINER_RESTRICTION;
    	break;}
    case 66: { /* <optional_syntax_restrictions> = '.'; */
#line 568 "alan.pmk"

        pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 67: { /* <optional_syntax_restrictions> = 'where' <syntax_restriction_clauses>; */
#line 573 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 68: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 583 "alan.pmk"

        pmSeSt[pmStkP+1].vrb = newVerb(&pmSeSt[pmStkP+1].srcp,
                              pmSeSt[pmStkP+1].idList,
                              pmSeSt[pmStkP+2].alts,
                              pmSeSt[pmStkP+1].isMeta);
        if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
            if (compareStrings(pmSeSt[pmStkP+1].str, pmSeSt[pmStkP+3].id->string) != 0)
                lmlog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].str);
        }
    	break;}
    case 69: { /* <verb_header> = <optional_meta> 'verb' <id_list>; */
#line 597 "alan.pmk"

        pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+2].srcp;
        pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+3].str;
        pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+3].idList;
        pmSeSt[pmStkP+1].isMeta = pmSeSt[pmStkP+1].isMeta;
    	break;}
    case 71: { /* <optional_meta> = 'meta'; */
#line 606 "alan.pmk"
 pmSeSt[pmStkP+1].isMeta = true; 	break;}
    case 70: { /* <optional_meta> =; */
#line 607 "alan.pmk"
 pmSeSt[pmStkP+1].isMeta = false; 	break;}
    case 72: { /* <verb_body> = <simple_verb_body>; */
#line 612 "alan.pmk"

        pmSeSt[pmStkP+1].alts = newList(newAlternative(pmSeSt[pmStkP+1].srcp,
                                                   NULL,
                                                   pmSeSt[pmStkP+1].chks,
                                                   pmSeSt[pmStkP+1].qual,
                                                   pmSeSt[pmStkP+1].stms),
                                    ALTERNATIVE_LIST);
    	break;}
    case 73: { /* <verb_body> = <verb_alternatives>; */
#line 622 "alan.pmk"

        pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 74: { /* <verb_alternatives> = <verb_alternative>; */
#line 629 "alan.pmk"

        pmSeSt[pmStkP+1].alts = newList(pmSeSt[pmStkP+1].alt, ALTERNATIVE_LIST);
    	break;}
    case 75: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 634 "alan.pmk"

        pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
                                           pmSeSt[pmStkP+2].alt,
                                           ALTERNATIVE_LIST);
    	break;}
    case 76: { /* <verb_alternative> = 'when' ID <simple_verb_body>; */
#line 643 "alan.pmk"

        pmSeSt[pmStkP+1].alt = newAlternative(pmSySt[pmStkP+1].srcp,
                                                 pmSeSt[pmStkP+2].id,
                                                 pmSeSt[pmStkP+3].chks,
                                                 pmSeSt[pmStkP+3].qual,
                                                 pmSeSt[pmStkP+3].stms);
    	break;}
    case 77: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 654 "alan.pmk"

        if (pmSeSt[pmStkP+2].stms == NULL) /* No DOES */
            pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        else
            pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+2].srcp;
        pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 78: { /* <verb_tail> = 'end' 'verb' <optional_id> '.'; */
#line 667 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 79: { /* <optional_checks> =; */
#line 675 "alan.pmk"

        pmSeSt[pmStkP+1].chks = NULL;
        pmSeSt[pmStkP+1].srcp = nulsrcp;
    	break;}
    case 80: { /* <optional_checks> = 'check' <statements>; */
#line 681 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].chks = newList(newCheck(NULL, pmSeSt[pmStkP+2].stms), CHECK_LIST);
    	break;}
    case 81: { /* <optional_checks> = 'check' <check_list>; */
#line 687 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 82: { /* <check_list> = <check>; */
#line 695 "alan.pmk"

        pmSeSt[pmStkP+1].chks = newList(pmSeSt[pmStkP+1].chk, CHECK_LIST);
    	break;}
    case 83: { /* <check_list> = <check_list> 'and' <check>; */
#line 700 "alan.pmk"

        pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHECK_LIST);
    	break;}
    case 84: { /* <check> = <expression> 'else' <statements>; */
#line 707 "alan.pmk"

        pmSeSt[pmStkP+1].chk = newCheck(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 85: { /* <optional_does> =; */
#line 715 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
        pmSeSt[pmStkP+1].srcp = nulsrcp;
        pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 86: { /* <optional_does> = <does>; */
#line 722 "alan.pmk"

        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+1].qual;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 87: { /* <does> = 'does' <optional_qual> <statements>; */
#line 731 "alan.pmk"

        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 88: { /* <class> = 'every' ID <optional_heritage> <properties> <class_tail>; */
#line 742 "alan.pmk"

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
                lmlog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 89: { /* <class_tail> = 'end' 'every' <optional_id> __genSym#1; */
#line 772 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 92: { /* <addition> = 'add' 'to' __genSym#2 ID <optional_heritage> <properties> <add_tail>; */
#line 779 "alan.pmk"

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
		lmlog(&pmSeSt[pmStkP+7].id->srcp, 200, sevWAR, pmSeSt[pmStkP+4].id->string);
    	break;}
    case 95: { /* <add_tail> = 'end' 'add' __genSym#3 <optional_id> __genSym#4; */
#line 809 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+4].id; 	break;}
    case 100: { /* <instance> = 'the' ID <optional_heritage> <properties> <instance_tail>; */
#line 816 "alan.pmk"

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
		lmlog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 101: { /* <instance_tail> = 'end' 'the' <optional_id> __genSym#5; */
#line 846 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 104: { /* <optional_heritage> =; */
#line 851 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 105: { /* <optional_heritage> = <heritage>; */
#line 856 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 106: { /* <heritage> = 'isa' ID __genSym#6; */
#line 863 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 109: { /* <properties> =; */
#line 868 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out all fields */
    	break;}
    case 110: { /* <properties> = <properties> <property>; */
#line 873 "alan.pmk"

	pmSeSt[pmStkP+1].nams = concat(pmSeSt[pmStkP+1].nams, pmSeSt[pmStkP+2].nam, NAME_LIST);

        if (pmSeSt[pmStkP+2].pronouns != NULL) {
            if (pmSeSt[pmStkP+1].pronouns != NULL) 
                /* WHERE clause already declared */
	        lmlogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "PRONOUN", "class/instance", NULL);
	    else {
		pmSeSt[pmStkP+1].pronouns = combine(pmSeSt[pmStkP+1].pronouns, pmSeSt[pmStkP+2].pronouns);
		pmSeSt[pmStkP+1].pronounsSrcp = pmSeSt[pmStkP+2].pronounsSrcp;
	    }
        }

        if (pmSeSt[pmStkP+2].whr != NULL) {
            if (pmSeSt[pmStkP+1].whr != NULL) 
                /* WHERE clause already declared */
	        lmlogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "WHERE", "class/instance", NULL);
	    else
	        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+2].whr;
        }

	pmSeSt[pmStkP+1].atrs = combine(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atrs);

	if (pmSeSt[pmStkP+2].initialize != NULL) {
            if (pmSeSt[pmStkP+1].initialize != NULL)
                /* INITIALIZE properties already declared */
	        lmlogv(&pmSeSt[pmStkP+2].initialize->srcp, 204, sevERR, "INITIALIZE property", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].initialize = pmSeSt[pmStkP+2].initialize;
        }

	if (pmSeSt[pmStkP+2].cnt != NULL) {
            if (pmSeSt[pmStkP+1].cnt != NULL)
                /* CONTAINER properties already declared */
	        lmlogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "CONTAINER properties", "class/instance", NULL);
	    else 
	        pmSeSt[pmStkP+1].cnt = pmSeSt[pmStkP+2].cnt;
        }

	if (pmSeSt[pmStkP+2].description != NULL) {
	  if (pmSeSt[pmStkP+2].description->checks != NULL) {
            if(pmSeSt[pmStkP+1].description != NULL && pmSeSt[pmStkP+1].description->checks != NULL)
                /* DESCRIPTION CHECK already declared */
	        lmlogv(&pmSeSt[pmStkP+2].description->checkSrcp, 204, sevERR, "DESCRIPTION CHECK", "class/instance", NULL);
	    else {
		if (pmSeSt[pmStkP+1].description != NULL)
		  pmSeSt[pmStkP+1].description->checks = pmSeSt[pmStkP+2].description->checks;
	    }
          }

	  if (pmSeSt[pmStkP+2].description->does != NULL) {
            if (pmSeSt[pmStkP+1].description != NULL && pmSeSt[pmStkP+1].description->does != NULL)
	        /* DESCRIPTION DOES already declared */
	        lmlogv(&pmSeSt[pmStkP+2].description->doesSrcp, 204, sevERR, "DESCRIPTION", "class/instance", NULL);
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
	        lmlogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "ENTERED", "class/instance", NULL);
	    else {
                pmSeSt[pmStkP+1].entered = pmSeSt[pmStkP+2].entered;
		pmSeSt[pmStkP+1].enteredSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].mentioned != NULL) {
            if (pmSeSt[pmStkP+1].mentioned != NULL)
                /* MENTIONED already declared */
	        lmlogv(&pmSeSt[pmStkP+2].srcp, 204, sevERR, "MENTIONED", "class/instance", NULL);
	    else {
                pmSeSt[pmStkP+1].mentioned = pmSeSt[pmStkP+2].mentioned;
		pmSeSt[pmStkP+1].mentionedSrcp = pmSeSt[pmStkP+2].srcp;
	    }
        }

	if (pmSeSt[pmStkP+2].indefinite != NULL) {
            if (pmSeSt[pmStkP+1].indefinite != NULL)
                /* Indefinite Article/Form already declared */
	        lmlogv(&pmSeSt[pmStkP+2].indefinite->srcp, 204, sevERR, "[Indefinite] Article/Form", "class/instance", NULL);
            else
	        pmSeSt[pmStkP+1].indefinite = pmSeSt[pmStkP+2].indefinite;
        }

	if (pmSeSt[pmStkP+2].definite != NULL) {
            if (pmSeSt[pmStkP+1].definite != NULL)
                /* Definite Article/Form already declared */
	        lmlogv(&pmSeSt[pmStkP+2].definite->srcp, 204, sevERR, "Definite Article/Form", "class/instance", NULL);
            else
	        pmSeSt[pmStkP+1].definite = pmSeSt[pmStkP+2].definite;
        }

	if (pmSeSt[pmStkP+2].negative != NULL) {
            if (pmSeSt[pmStkP+1].negative != NULL)
                /* Negative Article/Form already declared */
	        lmlogv(&pmSeSt[pmStkP+2].negative->srcp, 204, sevERR, "Negative Article/Form", "class/instance", NULL);
            else
	        pmSeSt[pmStkP+1].negative = pmSeSt[pmStkP+2].negative;
        }

	pmSeSt[pmStkP+1].exts = concat(pmSeSt[pmStkP+1].exts, pmSeSt[pmStkP+2].ext, EXIT_LIST);
	pmSeSt[pmStkP+1].vrbs = concat(pmSeSt[pmStkP+1].vrbs, pmSeSt[pmStkP+2].vrb, VERB_LIST);
	pmSeSt[pmStkP+1].scrs = concat(pmSeSt[pmStkP+1].scrs, pmSeSt[pmStkP+2].scr, SCRIPT_LIST);
    	break;}
    case 115: { /* <property> = <name>; */
#line 990 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 116: { /* <property> = <pronoun>; */
#line 996 "alan.pmk"
{ List *idList = pmSeSt[pmStkP+1].idList;
	Srcp pronounsSrcp = pmSeSt[pmStkP+1].pronounsSrcp;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].pronouns = idList;
	pmSeSt[pmStkP+1].pronounsSrcp = pronounsSrcp;
    }	break;}
    case 111: { /* <property> = <where> __genSym#7; */
#line 1004 "alan.pmk"
{
        Where *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 122: { /* <property> = <is> <attributes>; */
#line 1014 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 117: { /* <property> = <initialize>; */
#line 1020 "alan.pmk"
{
        Initialize *initialize = pmSeSt[pmStkP+1].initialize;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].initialize = initialize;
    }	break;}
    case 112: { /* <property> = <container_properties>; */
#line 1028 "alan.pmk"
{
        Container *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 113: { /* <property> = <description>; */
#line 1038 "alan.pmk"
{
	Description *description = pmSeSt[pmStkP+1].description;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */

	pmSeSt[pmStkP+1].description = description;
    }	break;}
    case 118: { /* <property> = <entered>; */
#line 1047 "alan.pmk"
{
        List *entered = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].entered = entered;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 119: { /* <property> = <mentioned>; */
#line 1057 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].mentioned = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 114: { /* <property> = __genSym#8 <article_or_form>; */
#line 1067 "alan.pmk"
{ Article *indefinite = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].indefinite = indefinite;
    }	break;}
    case 120: { /* <property> = 'definite' <article_or_form>; */
#line 1075 "alan.pmk"
{ Article *definite = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].definite = definite;
    }	break;}
    case 121: { /* <property> = 'negative' <article_or_form>; */
#line 1083 "alan.pmk"
{ Article *article = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].negative = article;
    }	break;}
    case 124: { /* <property> = <exit>; */
#line 1091 "alan.pmk"
{ Exit *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 125: { /* <property> = <verb>; */
#line 1097 "alan.pmk"
{ Verb *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 123: { /* <property> = <script>; */
#line 1103 "alan.pmk"
{ Script *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 130: { /* <exit> = 'exit' <id_list> 'to' ID <optional_exit_body> '.'; */
#line 1113 "alan.pmk"

	pmSeSt[pmStkP+1].ext = newExit(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+2].idList,
				pmSeSt[pmStkP+4].id,
				pmSeSt[pmStkP+5].chks,
				pmSeSt[pmStkP+5].stms);
	if (pmSeSt[pmStkP+5].id != NULL) { /* END-id given */
	    if (compareStrings(pmSeSt[pmStkP+2].str, pmSeSt[pmStkP+5].id->string) != 0)
		lmlog(&pmSeSt[pmStkP+5].id->srcp, 200, sevWAR, pmSeSt[pmStkP+2].str);
	}
    	break;}
    case 131: { /* <optional_exit_body> =; */
#line 1128 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 132: { /* <optional_exit_body> = <optional_checks> <optional_does> 'end' 'exit' <optional_id>; */
#line 1135 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmlog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 133: { /* <is> = 'is'; */
#line 1150 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 134: { /* <is> = 'are'; */
#line 1155 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 135: { /* <is> = 'has'; */
#line 1160 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 136: { /* <is> = 'can'; */
#line 1165 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 137: { /* <optional_description> =; */
#line 1172 "alan.pmk"

	pmSeSt[pmStkP+1].description = NULL;
    	break;}
    case 138: { /* <optional_description> = <description>; */
#line 1177 "alan.pmk"

	pmSeSt[pmStkP+1].description = pmSeSt[pmStkP+1].description;
    	break;}
    case 139: { /* <description> = 'description' <optional_checks> <optional_does>; */
#line 1184 "alan.pmk"

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
	    lmlog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
	}
    	break;}
    case 140: { /* <description> = 'description' <statements>; */
#line 1209 "alan.pmk"

	pmSeSt[pmStkP+1].description = newDescription(nulsrcp, NULL,
					pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].stms);
    	break;}
    case 143: { /* <article> = 'article'; */
#line 1222 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					newList(newStatement(&pmSySt[pmStkP+1].srcp, NOP_STATEMENT),
						STATEMENT_LIST),
					false);
    	break;}
    case 144: { /* <article> = 'article' <statements>; */
#line 1230 "alan.pmk"

	 pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					pmSeSt[pmStkP+2].stms,
					false);
    	break;}
    case 145: { /* <form> = 'form'; */
#line 1239 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					newList(newStatement(&pmSySt[pmStkP+1].srcp,
								NOP_STATEMENT),
						STATEMENT_LIST),
					true);
    	break;}
    case 146: { /* <form> = 'form' <statements>; */
#line 1248 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					pmSeSt[pmStkP+2].stms,
					true);
    	break;}
    case 147: { /* <entered> = 'entered' <statements>; */
#line 1258 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 148: { /* <initialize> = 'initialize' <statements>; */
#line 1267 "alan.pmk"

	pmSeSt[pmStkP+1].initialize = newInitialize(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].stms);
    	break;}
    case 149: { /* <mentioned> = 'mentioned' <statements>; */
#line 1275 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 150: { /* <name> = 'name' <ids> <optional_full_stop>; */
#line 1284 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 151: { /* <pronoun> = 'pronoun' <id_list> <optional_full_stop>; */
#line 1291 "alan.pmk"

	pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+2].idList;
	pmSeSt[pmStkP+1].pronounsSrcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 154: { /* <container_properties> = __genSym#9 <optionally_opaque> 'container' <container_body>; */
#line 1305 "alan.pmk"

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
    case 157: { /* <optionally_opaque> =; */
#line 1321 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = false; 	break;}
    case 158: { /* <optionally_opaque> = 'opaque'; */
#line 1323 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = true; 	break;}
    case 159: { /* <container_body> = <optional_taking> <optional_limits> <optional_header> <optional_empty> <optional_extract>; */
#line 1329 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+2].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+3].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+4].stms;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+5].chks;
	pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+5].stms;
    	break;}
    case 160: { /* <container_body> = '.'; */
#line 1339 "alan.pmk"

	pmSeSt[pmStkP+1].id  = NULL;
	pmSeSt[pmStkP+1].lims  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
	pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms3 = NULL;
    	break;}
    case 161: { /* <optional_taking> =; */
#line 1351 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 162: { /* <optional_taking> = 'taking' ID '.'; */
#line 1356 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 163: { /* <optional_limits> =; */
#line 1364 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 164: { /* <optional_limits> = 'limits' <limits>; */
#line 1369 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 165: { /* <limits> = <limit>; */
#line 1376 "alan.pmk"

	pmSeSt[pmStkP+1].lims = newList(pmSeSt[pmStkP+1].lim, LIMIT_LIST);
    	break;}
    case 166: { /* <limits> = <limits> <limit>; */
#line 1381 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMIT_LIST);
    	break;}
    case 167: { /* <limit> = <limit_attribute> <else_or_then> <statements>; */
#line 1388 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSeSt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 168: { /* <else_or_then> = 'else'; */
#line 1396 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 169: { /* <else_or_then> = 'then'; */
#line 1397 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 170: { /* <limit_attribute> = <attribute_definition>; */
#line 1402 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 171: { /* <limit_attribute> = 'count' Integer; */
#line 1407 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSySt[pmStkP+1].srcp, newId(pmSySt[pmStkP+1].srcp, "count"), val(pmSySt[pmStkP+2].chars));
    	break;}
    case 172: { /* <optional_header> =; */
#line 1414 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 173: { /* <optional_header> = 'header' <statements>; */
#line 1419 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 174: { /* <optional_empty> =; */
#line 1426 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 175: { /* <optional_empty> = 'else' <statements>; */
#line 1431 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 176: { /* <optional_extract> =; */
#line 1438 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 177: { /* <optional_extract> = 'extract' <optional_checks> <optional_does>; */
#line 1444 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
	if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	  lmlog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
    	break;}
    case 178: { /* <optional_extract> = 'extract' <statements>; */
#line 1452 "alan.pmk"

	pmSeSt[pmStkP+1].chk = NULL;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 179: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1462 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newEvent(&pmSeSt[pmStkP+1].srcp,
				pmSeSt[pmStkP+1].id,
				pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmlog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 180: { /* <event_header> = 'event' ID; */
#line 1475 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 181: { /* <event_tail> = 'end' 'event' <optional_id> '.'; */
#line 1483 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 182: { /* <script> = 'script' ID __genSym#10 <optional_description> <step_list>; */
#line 1491 "alan.pmk"

	if (pmSeSt[pmStkP+4].description != NULL &&
        pmSeSt[pmStkP+4].description->checks != NULL)
	  lmlog(&pmSeSt[pmStkP+4].description->checkSrcp,
      		228, sevERR, "");
	pmSeSt[pmStkP+1].scr = newScript(&pmSySt[pmStkP+1].srcp,
                              pmSeSt[pmStkP+2].id,
			                  pmSeSt[pmStkP+4].description,
			                  pmSeSt[pmStkP+5].stps);
    	break;}
    case 185: { /* <step_list> = <step>; */
#line 1505 "alan.pmk"

	pmSeSt[pmStkP+1].stps = newList(pmSeSt[pmStkP+1].stp, STEP_LIST);
    	break;}
    case 186: { /* <step_list> = <step_list> <step>; */
#line 1510 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STEP_LIST);
    	break;}
    case 187: { /* <step> = 'step' <statements>; */
#line 1517 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 188: { /* <step> = 'step' 'after' <expression> __genSym#11 <statements>; */
#line 1525 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+3].exp,
				NULL,
				pmSeSt[pmStkP+5].stms);
    	break;}
    case 189: { /* <step> = 'step' 'wait' 'until' <expression> __genSym#12 <statements>; */
#line 1533 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
				NULL,
				pmSeSt[pmStkP+4].exp,
				pmSeSt[pmStkP+6].stms);
    	break;}
    case 194: { /* <rule> = 'when' <expression> <then> <statements> <optional_end_when>; */
#line 1544 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newRule(&pmSySt[pmStkP+1].srcp,
		 		pmSeSt[pmStkP+2].exp,
				pmSeSt[pmStkP+4].stms);
    	break;}
    case 201: { /* <start> = 'start' <where> '.' <optional_statements>; */
#line 1564 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 202: { /* <optional_statements> =; */
#line 1573 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 203: { /* <optional_statements> = <statements>; */
#line 1578 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 204: { /* <statements> = <statement>; */
#line 1585 "alan.pmk"

	pmSeSt[pmStkP+1].stms = newList(pmSeSt[pmStkP+1].stm, STATEMENT_LIST);
    	break;}
    case 205: { /* <statements> = <statements> <statement>; */
#line 1590 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STATEMENT_LIST);
    	break;}
    case 206: { /* <statement> = <output_statement>; */
#line 1597 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 208: { /* <statement> = <manipulation_statement>; */
#line 1602 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 211: { /* <statement> = <assignment_statement>; */
#line 1607 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 213: { /* <statement> = <conditional_statement>; */
#line 1612 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 212: { /* <statement> = <repetition_statement>; */
#line 1617 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 209: { /* <statement> = <actor_statement>; */
#line 1622 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 210: { /* <statement> = <event_statement>; */
#line 1627 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 207: { /* <statement> = <special_statement>; */
#line 1632 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 214: { /* <output_statement> = STRING; */
#line 1640 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, PRINT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 215: { /* <output_statement> = 'describe' <what> '.'; */
#line 1647 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newDescribeStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 216: { /* <output_statement> = 'say' <say_form> <expression> '.'; */
#line 1652 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.say.form = pmSeSt[pmStkP+2].sayForm;
    	break;}
    case 217: { /* <output_statement> = 'list' <primary> '.'; */
#line 1659 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newListStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 218: { /* <output_statement> = 'show' ID '.'; */
#line 1664 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newShowStatement(pmSySt[pmStkP+1].srcp, newResource(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id));
    	break;}
    case 219: { /* <output_statement> = 'play' ID '.'; */
#line 1669 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newPlayStatement(pmSySt[pmStkP+1].srcp, newResource(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id));
    	break;}
    case 220: { /* <output_statement> = 'style' ID '.'; */
#line 1674 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStyleStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].id);
    	break;}
    case 221: { /* <say_form> =; */
#line 1681 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_SIMPLE; 	break;}
    case 222: { /* <say_form> = 'the'; */
#line 1683 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_DEFINITE; 	break;}
    case 223: { /* <say_form> = 'an'; */
#line 1685 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_INDEFINITE; 	break;}
    case 224: { /* <say_form> = 'it'; */
#line 1687 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_PRONOUN; 	break;}
    case 225: { /* <say_form> = 'no'; */
#line 1689 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_NEGATIVE; 	break;}
    case 226: { /* <manipulation_statement> = 'empty' <primary> <optional_where> '.'; */
#line 1696 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newEmptyStatement(pmSySt[pmStkP+1].srcp,
							  pmSeSt[pmStkP+2].exp,
							  pmSeSt[pmStkP+3].whr);
    	break;}
    case 227: { /* <manipulation_statement> = 'locate' <primary> <where> '.'; */
#line 1703 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newLocateStatement(pmSySt[pmStkP+1].srcp,
							   pmSeSt[pmStkP+2].exp,
							   pmSeSt[pmStkP+3].whr);
    	break;}
    case 228: { /* <manipulation_statement> = 'include' <primary> 'in' <what> '.'; */
#line 1710 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newIncludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 229: { /* <manipulation_statement> = 'exclude' <primary> 'from' <what> '.'; */
#line 1717 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newExcludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 231: { /* <event_statement> = 'schedule' <what> <optional_where> 'after' <expression> '.'; */
#line 1728 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newScheduleStatement(pmSySt[pmStkP+1].srcp,
						      pmSeSt[pmStkP+2].exp,
						      pmSeSt[pmStkP+3].whr,
						      pmSeSt[pmStkP+5].exp);
    	break;}
    case 230: { /* <event_statement> = 'cancel' <what> '.'; */
#line 1736 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newCancelStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 232: { /* <assignment_statement> = 'make' <primary> <something> '.'; */
#line 1745 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, MAKE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 236: { /* <assignment_statement> = 'set' <attribute_reference> 'to' <expression> '.'; */
#line 1753 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SET_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 234: { /* <assignment_statement> = 'increase' <attribute_reference> <optional_by_clause> '.'; */
#line 1760 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, INCREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 235: { /* <assignment_statement> = 'decrease' <attribute_reference> <optional_by_clause> '.'; */
#line 1767 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DECREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 233: { /* <assignment_statement> = 'strip' <optional_first_or_last> <optional_expression> <optional_word_or_character> 'from' <expression> <optional_into> '.'; */
#line 1774 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STRIP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.strip.first = pmSeSt[pmStkP+2].first;
	pmSeSt[pmStkP+1].stm->fields.strip.count = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.wordOrChar = pmSeSt[pmStkP+4].wordOrChar;
	pmSeSt[pmStkP+1].stm->fields.strip.from = pmSeSt[pmStkP+6].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.into = pmSeSt[pmStkP+7].exp;
    	break;}
    case 237: { /* <optional_by_clause> =; */
#line 1786 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 238: { /* <optional_by_clause> = 'by' <expression>; */
#line 1791 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 239: { /* <optional_first_or_last> =; */
#line 1798 "alan.pmk"
 pmSeSt[pmStkP+1].first = true; 	break;}
    case 240: { /* <optional_first_or_last> = 'first'; */
#line 1800 "alan.pmk"
 pmSeSt[pmStkP+1].first = true; 	break;}
    case 241: { /* <optional_first_or_last> = 'last'; */
#line 1802 "alan.pmk"
 pmSeSt[pmStkP+1].first = false; 	break;}
    case 242: { /* <optional_word_or_character> =; */
#line 1807 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = true; 	break;}
    case 243: { /* <optional_word_or_character> = 'word'; */
#line 1809 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = true; 	break;}
    case 244: { /* <optional_word_or_character> = 'words'; */
#line 1811 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = true; 	break;}
    case 245: { /* <optional_word_or_character> = 'character'; */
#line 1813 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = false; 	break;}
    case 246: { /* <optional_word_or_character> = 'characters'; */
#line 1815 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = false; 	break;}
    case 247: { /* <optional_into> =; */
#line 1820 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 248: { /* <optional_into> = 'into' <expression>; */
#line 1823 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp; 	break;}
    case 249: { /* <conditional_statement> = <if_statement>; */
#line 1829 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 250: { /* <conditional_statement> = <depending_statement>; */
#line 1834 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 251: { /* <if_statement> = 'if' <expression> 'then' <statements> <optional_elsif_list> <optional_else_part> 'end' 'if' '.'; */
#line 1842 "alan.pmk"

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
    case 252: { /* <optional_elsif_list> =; */
#line 1860 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 253: { /* <optional_elsif_list> = <elsif_list>; */
#line 1865 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 254: { /* <elsif_list> = 'elsif' <expression> 'then' <statements>; */
#line 1872 "alan.pmk"
{
	Statement *stm;

	stm = newStatement(&pmSySt[pmStkP+1].srcp, IF_STATEMENT);
	stm->fields.iff.exp = pmSeSt[pmStkP+2].exp;
	stm->fields.iff.thn = pmSeSt[pmStkP+4].stms;
	stm->fields.iff.els = NULL;

	/* Now make a list of the ELSIF */
	pmSeSt[pmStkP+1].stms = newList(stm, STATEMENT_LIST);
    }	break;}
    case 255: { /* <elsif_list> = <elsif_list> 'elsif' <expression> 'then' <statements>; */
#line 1885 "alan.pmk"
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
    case 256: { /* <optional_else_part> =; */
#line 1904 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 257: { /* <optional_else_part> = 'else' <statements>; */
#line 1909 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 258: { /* <depending_statement> = 'depending' 'on' <primary> <depend_cases> 'end' __genSym#14 '.'; */
#line 1916 "alan.pmk"
{ Statement *stm;
	stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPEND_STATEMENT);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 261: { /* <depend_cases> = <depend_case>; */
#line 1927 "alan.pmk"

	pmSeSt[pmStkP+1].cases = newList(pmSeSt[pmStkP+1].stm, CASE_LIST);
    	break;}
    case 262: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1932 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASE_LIST);
    	break;}
    case 264: { /* <depend_case> = <right_hand_side> 'then' <statements>; */
#line 1939 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+2].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 263: { /* <depend_case> = 'else' <statements>; */
#line 1946 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 265: { /* <repetition_statement> = <for_each> ID <optional_loop_filters> 'do' <statements> 'end' <for_each> __genSym#15; */
#line 1956 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newEachStatement(pmSeSt[pmStkP+1].srcp,
							pmSeSt[pmStkP+2].id,
							pmSeSt[pmStkP+3].exps,
							pmSeSt[pmStkP+5].stms);
    	break;}
    case 268: { /* <optional_loop_filters> =; */
#line 1966 "alan.pmk"

	pmSeSt[pmStkP+1].exps = NULL;
    	break;}
    case 269: { /* <optional_loop_filters> = <filters>; */
#line 1971 "alan.pmk"

	pmSeSt[pmStkP+1].exps = pmSeSt[pmStkP+1].exps;
    	break;}
    case 270: { /* <optional_loop_filters> = 'between' <arithmetic> 'and' <arithmetic>; */
#line 1976 "alan.pmk"

	pmSeSt[pmStkP+1].exps = newList(newBetweenExpression(pmSySt[pmStkP+1].srcp,
					     				NULL,
					     				false,
					     				pmSeSt[pmStkP+2].exp,
					     				pmSeSt[pmStkP+4].exp),
						EXPRESSION_LIST);
    	break;}
    case 271: { /* <for_each> = 'for'; */
#line 1988 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 272: { /* <for_each> = 'each'; */
#line 1990 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 273: { /* <for_each> = 'for' 'each'; */
#line 1992 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 275: { /* <actor_statement> = 'use' 'script' ID <optional_for_actor> '.'; */
#line 1998 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newUseStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+3].id, pmSeSt[pmStkP+4].exp);
    	break;}
    case 274: { /* <actor_statement> = 'stop' <what> '.'; */
#line 2003 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STOP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.stop.actor = pmSeSt[pmStkP+2].exp;
    	break;}
    case 276: { /* <optional_for_actor> =; */
#line 2011 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 277: { /* <optional_for_actor> = 'for' <what>; */
#line 2016 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 278: { /* <special_statement> = 'quit' '.'; */
#line 2024 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, QUIT_STATEMENT);
    	break;}
    case 279: { /* <special_statement> = 'look' '.'; */
#line 2029 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, LOOK_STATEMENT);
    	break;}
    case 280: { /* <special_statement> = 'save' '.'; */
#line 2034 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAVE_STATEMENT);
    	break;}
    case 281: { /* <special_statement> = 'restore' '.'; */
#line 2039 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTORE_STATEMENT);
    	break;}
    case 282: { /* <special_statement> = 'restart' '.'; */
#line 2044 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTART_STATEMENT);
    	break;}
    case 283: { /* <special_statement> = 'score' <optional_integer> '.'; */
#line 2049 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SCORE_STATEMENT);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scoreCount;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 286: { /* <special_statement> = 'visits' Integer '.'; */
#line 2060 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, VISITS_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 284: { /* <special_statement> = 'transcript' <on_or_off> '.'; */
#line 2066 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, TRANSCRIPT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.transcript.on_or_off = pmSeSt[pmStkP+2].on_or_off;
    	break;}
    case 285: { /* <special_statement> = 'system' STRING '.'; */
#line 2072 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SYSTEM_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 287: { /* <on_or_off> = 'on'; */
#line 2081 "alan.pmk"
 pmSeSt[pmStkP+1].on_or_off = true; 	break;}
    case 288: { /* <on_or_off> = 'off'; */
#line 2084 "alan.pmk"
 pmSeSt[pmStkP+1].on_or_off = false; 	break;}
    case 289: { /* <optional_expression> =; */
#line 2091 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 290: { /* <optional_expression> = <expression>; */
#line 2094 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp; 	break;}
    case 291: { /* <expression> = <term>; */
#line 2099 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 292: { /* <expression> = <expression> 'or' <term>; */
#line 2104 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
						pmSeSt[pmStkP+1].exp,
						false,
						OR_OPERATOR,
						pmSeSt[pmStkP+3].exp);
    	break;}
    case 293: { /* <term> = <factor>; */
#line 2115 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 294: { /* <term> = <term> 'and' <factor>; */
#line 2120 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
					  pmSeSt[pmStkP+1].exp,
					  false,
					  AND_OPERATOR,
					  pmSeSt[pmStkP+3].exp);
    	break;}
    case 295: { /* <factor> = <arithmetic>; */
#line 2131 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 297: { /* <factor> = <factor> <optional_not> <relop> <arithmetic>; */
#line 2136 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+3].srcp,
					    pmSeSt[pmStkP+1].exp,
					    pmSeSt[pmStkP+2].present,
					    pmSeSt[pmStkP+3].op,
					    pmSeSt[pmStkP+4].exp);
    	break;}
    case 296: { /* <factor> = <factor> <optional_not> <where>; */
#line 2145 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhereExpression(pmSeSt[pmStkP+3].srcp,
					   pmSeSt[pmStkP+1].exp,
					   pmSeSt[pmStkP+2].present,
					   pmSeSt[pmStkP+3].whr);
    	break;}
    case 299: { /* <factor> = <factor> <optional_not> 'between' <arithmetic> 'and' <arithmetic>; */
#line 2153 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBetweenExpression(pmSySt[pmStkP+3].srcp,
					     pmSeSt[pmStkP+1].exp,
					     pmSeSt[pmStkP+2].present,
					     pmSeSt[pmStkP+4].exp,
					     pmSeSt[pmStkP+6].exp);
    	break;}
    case 298: { /* <factor> = <factor> <optional_not> 'contains' <arithmetic>; */
#line 2162 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+3].srcp,
					    pmSeSt[pmStkP+1].exp,
					    pmSeSt[pmStkP+2].present,
					    CONTAINS_OPERATOR,
					    pmSeSt[pmStkP+4].exp);
    	break;}
    case 300: { /* <arithmetic> = <primary>; */
#line 2174 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 302: { /* <arithmetic> = <primary> 'isa' ID; */
#line 2179 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].exp, false, pmSeSt[pmStkP+3].id);
    	break;}
    case 303: { /* <arithmetic> = <primary> <is> <something>; */
#line 2184 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id,
					 pmSeSt[pmStkP+3].not, pmSeSt[pmStkP+1].exp);
    	break;}
    case 304: { /* <arithmetic> = <arithmetic> <binop> <primary>; */
#line 2190 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+2].srcp,
						pmSeSt[pmStkP+1].exp,
						false,
						pmSeSt[pmStkP+2].op,
						pmSeSt[pmStkP+3].exp);
    	break;}
    case 301: { /* <arithmetic> = <aggregate> <filters>; */
#line 2199 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAggregateExpression(pmSeSt[pmStkP+1].srcp,
							pmSeSt[pmStkP+1].agr,
							pmSeSt[pmStkP+1].id,
							pmSeSt[pmStkP+2].exps);
    	break;}
    case 305: { /* <filters> = <filter>; */
#line 2210 "alan.pmk"

        pmSeSt[pmStkP+1].exps = newList(pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 306: { /* <filters> = <filters> ',' <filter>; */
#line 2215 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(pmSeSt[pmStkP+1].exps, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 307: { /* <filter> = <optional_not> <where>; */
#line 2234 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhereExpression(pmSeSt[pmStkP+2].srcp,
						      NULL,
						      pmSeSt[pmStkP+1].present,
						      pmSeSt[pmStkP+2].whr);
    	break;}
    case 308: { /* <filter> = <optional_not> 'isa' ID; */
#line 2242 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, NULL, pmSeSt[pmStkP+1].present, pmSeSt[pmStkP+3].id);
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 309: { /* <filter> = <is> <something>; */
#line 2248 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+1].srcp,
						pmSeSt[pmStkP+2].id,
						pmSeSt[pmStkP+2].not, NULL);
    	break;}
    case 311: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 2257 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+2].srcp,
						     NULL,
						     pmSeSt[pmStkP+1].present,
						     pmSeSt[pmStkP+2].op,
						     pmSeSt[pmStkP+3].exp);
    	break;}
    case 313: { /* <right_hand_side> = <optional_not> 'between' <arithmetic> 'and' <arithmetic>; */
#line 2266 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBetweenExpression(pmSySt[pmStkP+2].srcp,
						      NULL,
						      pmSeSt[pmStkP+1].present,
						      pmSeSt[pmStkP+3].exp,
						      pmSeSt[pmStkP+5].exp);
    	break;}
    case 312: { /* <right_hand_side> = <optional_not> 'contains' <factor>; */
#line 2275 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
						     NULL,
						     pmSeSt[pmStkP+1].present,
						     CONTAINS_OPERATOR,
						     pmSeSt[pmStkP+3].exp);

    	break;}
    case 319: { /* <primary> = '(' <expression> ')'; */
#line 2290 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 317: { /* <primary> = <optional_minus> Integer; */
#line 2295 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp,
					      pmSeSt[pmStkP+1].minus?
							-val(pmSySt[pmStkP+2].chars):
							val(pmSySt[pmStkP+2].chars));
    	break;}
    case 314: { /* <primary> = STRING; */
#line 2303 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp,
                                             pmSySt[pmStkP+1].fpos,
                                             pmSySt[pmStkP+1].len);
    	break;}
    case 315: { /* <primary> = <what>; */
#line 2310 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 321: { /* <primary> = 'random' <primary> 'to' <primary>; */
#line 2315 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newRandomRangeExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp, pmSeSt[pmStkP+4].exp);
    	break;}
    case 320: { /* <primary> = 'random' <optional_transitivity> 'in' <primary>; */
#line 2320 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newRandomInExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+4].exp, pmSeSt[pmStkP+2].transitivity);
    	break;}
    case 316: { /* <primary> = 'score'; */
#line 2325 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newScoreExpression(pmSySt[pmStkP+1].srcp);
    	break;}
    case 318: { /* <primary> = '{' <optional_members> '}'; */
#line 2330 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newSetExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].set);
    	break;}
    case 323: { /* <aggregate> = <aggregator> 'of' ID; */
#line 2338 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = pmSeSt[pmStkP+1].agr;
        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 322: { /* <aggregate> = 'count'; */
#line 2345 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = COUNT_AGGREGATE;
        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 324: { /* <aggregator> = 'max'; */
#line 2354 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = MAX_AGGREGATE;
    	break;}
    case 325: { /* <aggregator> = 'min'; */
#line 2360 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = MIN_AGGREGATE;
    	break;}
    case 326: { /* <aggregator> = 'sum'; */
#line 2366 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = SUM_AGGREGATE;
    	break;}
    case 327: { /* <something> = <optional_not> ID; */
#line 2376 "alan.pmk"

        pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].present;
        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 328: { /* <what> = <simple_what>; */
#line 2384 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newWhatExpression(pmSeSt[pmStkP+1].srcp, pmSeSt[pmStkP+1].wht);
    	break;}
    case 329: { /* <what> = <attribute_reference>; */
#line 2389 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 330: { /* <simple_what> = ID; */
#line 2396 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatId(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id);
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 332: { /* <simple_what> = 'current' 'actor'; */
#line 2402 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatActor(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 333: { /* <simple_what> = 'current' 'location'; */
#line 2408 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatLocation(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 331: { /* <simple_what> = 'this'; */
#line 2414 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatThis(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 334: { /* <attribute_reference> = ID 'of' <what>; */
#line 2423 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].id, false, pmSeSt[pmStkP+3].exp);
    	break;}
    case 335: { /* <attribute_reference> = <what> ':' ID; */
#line 2428 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id, false, pmSeSt[pmStkP+1].exp);
    	break;}
    case 336: { /* <optional_where> =; */
#line 2435 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&nulsrcp, DEFAULT_TRANSITIVITY, WHERE_DEFAULT, NULL);
    	break;}
    case 337: { /* <optional_where> = <where>; */
#line 2440 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 338: { /* <where> = <optional_transitivity> 'here'; */
#line 2447 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_HERE, NULL);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 339: { /* <where> = <optional_transitivity> 'nearby'; */
#line 2453 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_NEARBY, NULL);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 340: { /* <where> = <optional_transitivity> 'at' <primary>; */
#line 2459 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_AT, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 341: { /* <where> = <optional_transitivity> 'in' <primary>; */
#line 2465 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_IN, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 342: { /* <where> = <optional_transitivity> 'near' <what>; */
#line 2471 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_NEAR, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 343: { /* <binop> = '+'; */
#line 2479 "alan.pmk"

        pmSeSt[pmStkP+1].op = PLUS_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 344: { /* <binop> = '-'; */
#line 2485 "alan.pmk"

        pmSeSt[pmStkP+1].op = MINUS_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 345: { /* <binop> = '*'; */
#line 2491 "alan.pmk"

        pmSeSt[pmStkP+1].op = MULT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 346: { /* <binop> = '/'; */
#line 2497 "alan.pmk"

        pmSeSt[pmStkP+1].op = DIV_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 347: { /* <relop> = '<>'; */
#line 2506 "alan.pmk"

        pmSeSt[pmStkP+1].op   = NE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 348: { /* <relop> = '='; */
#line 2512 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EQ_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 349: { /* <relop> = '=='; */
#line 2518 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EXACT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 350: { /* <relop> = '>='; */
#line 2524 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 351: { /* <relop> = '<='; */
#line 2530 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 352: { /* <relop> = '>'; */
#line 2536 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 353: { /* <relop> = '<'; */
#line 2542 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 354: { /* <optional_qual> =; */
#line 2552 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 355: { /* <optional_qual> = 'before'; */
#line 2557 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 356: { /* <optional_qual> = 'after'; */
#line 2563 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 357: { /* <optional_qual> = 'only'; */
#line 2569 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 358: { /* <optional_not> =; */
#line 2578 "alan.pmk"

        pmSeSt[pmStkP+1].present = false;
    	break;}
    case 359: { /* <optional_not> = 'not'; */
#line 2583 "alan.pmk"

        pmSeSt[pmStkP+1].present = true;
    	break;}
    case 360: { /* <optional_transitivity> =; */
#line 2590 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = DEFAULT_TRANSITIVITY;
    	break;}
    case 361: { /* <optional_transitivity> = 'transitively'; */
#line 2595 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = TRANSITIVELY;
    	break;}
    case 362: { /* <optional_transitivity> = 'directly'; */
#line 2600 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = DIRECTLY;
    	break;}
    case 363: { /* <optional_transitivity> = 'indirectly'; */
#line 2605 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = INDIRECTLY;
    	break;}
    case 364: { /* <optional_id> =; */
#line 2612 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 365: { /* <optional_id> = ID; */
#line 2617 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 366: { /* <ids> = ID; */
#line 2624 "alan.pmk"

        pmSeSt[pmStkP+1].idList = newList(pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 367: { /* <ids> = <ids> ID; */
#line 2629 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, ID_LIST);
    	break;}
    case 368: { /* <id_list> = ID; */
#line 2636 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = newList(pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 369: { /* <id_list> = <id_list> ',' ID; */
#line 2642 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, ID_LIST);
    	break;}
    case 370: { /* <optional_integer> =; */
#line 2650 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 371: { /* <optional_integer> = Integer; */
#line 2655 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 372: { /* <optional_minus> =; */
#line 2662 "alan.pmk"

        pmSeSt[pmStkP+1].minus = false;
    	break;}
    case 373: { /* <optional_minus> = '-'; */
#line 2667 "alan.pmk"

        pmSeSt[pmStkP+1].minus = true;
    	break;}
    case 374: { /* ID = Identifier; */
#line 2674 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 375: { /* ID = 'location'; */
#line 2685 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 376: { /* ID = 'actor'; */
#line 2690 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 377: { /* ID = 'opaque'; */
#line 2695 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 378: { /* ID = 'visits'; */
#line 2700 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 379: { /* ID = 'contains'; */
#line 2705 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 380: { /* ID = 'on'; */
#line 2710 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 381: { /* ID = 'it'; */
#line 2715 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 382: { /* ID = 'of'; */
#line 2720 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 383: { /* ID = 'first'; */
#line 2725 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 384: { /* ID = 'into'; */
#line 2730 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 385: { /* ID = 'taking'; */
#line 2735 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 386: { /* ID = 'off'; */
#line 2740 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    default: break; }
}/*pmPaSema()*/

