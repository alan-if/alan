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

#include "lmlog.h"

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
#line 205 "alan.pmk"
    switch (rule) {
    case 8: { /* <option> = ID '.'; */
#line 225 "alan.pmk"

        optBool(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, true);
    	break;}
    case 9: { /* <option> = 'no' ID '.'; */
#line 230 "alan.pmk"

        optBool(pmSeSt[pmStkP+2].id->string, &pmSeSt[pmStkP+2].id->srcp, false);
    	break;}
    case 10: { /* <option> = ID ID '.'; */
#line 235 "alan.pmk"

        optenum(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+2].id->string);
    	break;}
    case 11: { /* <option> = ID Integer '.'; */
#line 240 "alan.pmk"

        optint(pmSeSt[pmStkP+1].id->string, &pmSeSt[pmStkP+1].id->srcp, val(pmSySt[pmStkP+2].chars));
    	break;}
    case 20: { /* <declaration> = <synonyms>; */
#line 253 "alan.pmk"
 adv.syns = combine(adv.syns, pmSeSt[pmStkP+1].syns);	break;}
    case 16: { /* <declaration> = <messages>; */
#line 256 "alan.pmk"
 adv.msgs = combine(adv.msgs, pmSeSt[pmStkP+1].msgs); 	break;}
    case 21: { /* <declaration> = <syntax>; */
#line 259 "alan.pmk"
 adv.stxs = combine(adv.stxs, pmSeSt[pmStkP+1].stxs); 	break;}
    case 22: { /* <declaration> = <verb>; */
#line 262 "alan.pmk"
 adv.vrbs = concat(adv.vrbs, pmSeSt[pmStkP+1].vrb, VERB_LIST); 	break;}
    case 17: { /* <declaration> = <class>; */
#line 265 "alan.pmk"
 adv.clas = concat(adv.clas, pmSeSt[pmStkP+1].cla, CLASS_LIST); 	break;}
    case 23: { /* <declaration> = <addition>; */
#line 268 "alan.pmk"
 adv.adds = concat(adv.adds, pmSeSt[pmStkP+1].add, ADD_LIST); 	break;}
    case 18: { /* <declaration> = <instance>; */
#line 271 "alan.pmk"
 adv.inss = concat(adv.inss, pmSeSt[pmStkP+1].ins, INSTANCE_LIST); 	break;}
    case 24: { /* <declaration> = <event>; */
#line 274 "alan.pmk"
 adv.evts = concat(adv.evts, pmSeSt[pmStkP+1].evt, EVENT_LIST); 	break;}
    case 19: { /* <declaration> = <rule>; */
#line 277 "alan.pmk"
 adv.ruls = concat(adv.ruls, pmSeSt[pmStkP+1].rul, RULE_LIST); 	break;}
    case 15: { /* <declaration> = <prompt>; */
#line 280 "alan.pmk"

      if (adv.prompt != NULL)
        lmlog(&pmSeSt[pmStkP+1].srcp, 308, sevWAR, "PROMPT");
      adv.prompt = pmSeSt[pmStkP+1].prompt;
    	break;}
    case 25: { /* <prompt> = 'prompt' <statements>; */
#line 291 "alan.pmk"

      pmSeSt[pmStkP+1].prompt = pmSeSt[pmStkP+2].stms;
      pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 26: { /* <attributes> = <attribute_definition> '.'; */
#line 300 "alan.pmk"

        pmSeSt[pmStkP+1].atrs = newList(pmSeSt[pmStkP+1].atr, ATTRIBUTE_LIST);
    	break;}
    case 27: { /* <attributes> = <attributes> <attribute_definition> '.'; */
#line 305 "alan.pmk"

        pmSeSt[pmStkP+1].atrs = concat(pmSeSt[pmStkP+1].atrs, pmSeSt[pmStkP+2].atr, ATTRIBUTE_LIST);
    	break;}
    case 28: { /* <attribute_definition> = ID; */
#line 312 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, true);
    	break;}
    case 29: { /* <attribute_definition> = 'not' ID; */
#line 317 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newBooleanAttribute(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id, false);
    	break;}
    case 32: { /* <attribute_definition> = ID <optional_minus> Integer; */
#line 322 "alan.pmk"

        if (pmSeSt[pmStkP+2].minus)
           pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, -val(pmSySt[pmStkP+3].chars));
        else
           pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, val(pmSySt[pmStkP+3].chars));
    	break;}
    case 30: { /* <attribute_definition> = ID STRING; */
#line 330 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newStringAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSySt[pmStkP+2].fpos, pmSySt[pmStkP+2].len);
    	break;}
    case 31: { /* <attribute_definition> = ID ID; */
#line 334 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newReferenceAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+2].id);
    	break;}
    case 33: { /* <attribute_definition> = ID '{' <optional_members> '}'; */
#line 339 "alan.pmk"

        pmSeSt[pmStkP+1].atr = newSetAttribute(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id,
                                                      newSetExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+3].set));
    	break;}
    case 34: { /* <optional_members> =; */
#line 347 "alan.pmk"
 pmSeSt[pmStkP+1].set = NULL; 	break;}
    case 35: { /* <optional_members> = <set_members>; */
#line 350 "alan.pmk"
 pmSeSt[pmStkP+1].set = pmSeSt[pmStkP+1].set; 	break;}
    case 36: { /* <set_members> = <set_member>; */
#line 355 "alan.pmk"

        pmSeSt[pmStkP+1].set = newList(pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 37: { /* <set_members> = <set_members> ',' <set_member>; */
#line 360 "alan.pmk"

        pmSeSt[pmStkP+1].set = concat(pmSeSt[pmStkP+1].set, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 38: { /* <set_member> = <what>; */
#line 367 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 40: { /* <set_member> = <optional_minus> Integer; */
#line 372 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].minus?-val(pmSySt[pmStkP+2].chars):val(pmSySt[pmStkP+2].chars));
    	break;}
    case 39: { /* <set_member> = STRING; */
#line 377 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].fpos, pmSySt[pmStkP+1].len);
    	break;}
    case 41: { /* <synonyms> = 'synonyms' <synonym_list>; */
#line 385 "alan.pmk"

        pmSeSt[pmStkP+1].syns = pmSeSt[pmStkP+2].syns;
    	break;}
    case 42: { /* <synonym_list> = <synonym_declaration>; */
#line 392 "alan.pmk"

        pmSeSt[pmStkP+1].syns = newList(pmSeSt[pmStkP+1].syn, SYNONYM_LIST);
    	break;}
    case 43: { /* <synonym_list> = <synonym_list> <synonym_declaration>; */
#line 397 "alan.pmk"

        pmSeSt[pmStkP+1].syns = concat(pmSeSt[pmStkP+1].syns, pmSeSt[pmStkP+2].syn, SYNONYM_LIST);
    	break;}
    case 44: { /* <synonym_declaration> = <id_list> '=' ID '.'; */
#line 404 "alan.pmk"

        pmSeSt[pmStkP+1].syn = newSynonym(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id);
    	break;}
    case 45: { /* <messages> = 'message' <message_list>; */
#line 412 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = pmSeSt[pmStkP+2].msgs;
    	break;}
    case 46: { /* <message_list> = <message>; */
#line 419 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = newList(pmSeSt[pmStkP+1].msg, MESSAGE_LIST);
    	break;}
    case 47: { /* <message_list> = <message_list> <message>; */
#line 424 "alan.pmk"

        pmSeSt[pmStkP+1].msgs = concat(pmSeSt[pmStkP+1].msgs, pmSeSt[pmStkP+2].msg, MESSAGE_LIST);
    	break;}
    case 48: { /* <message> = ID ':' <statements>; */
#line 431 "alan.pmk"

        pmSeSt[pmStkP+1].msg = newMessage(&pmSySt[pmStkP+2].srcp,
                                    pmSeSt[pmStkP+1].id,
                                    pmSeSt[pmStkP+3].stms);
    	break;}
    case 49: { /* <syntax> = 'syntax' <syntax_list>; */
#line 442 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = pmSeSt[pmStkP+2].stxs;
    	break;}
    case 50: { /* <syntax_list> = <syntax_item>; */
#line 449 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = newList(pmSeSt[pmStkP+1].stx, SYNTAX_LIST);
    	break;}
    case 51: { /* <syntax_list> = <syntax_list> <syntax_item>; */
#line 454 "alan.pmk"

        pmSeSt[pmStkP+1].stxs = concat(pmSeSt[pmStkP+1].stxs, pmSeSt[pmStkP+2].stx, SYNTAX_LIST);
    	break;}
    case 52: { /* <syntax_item> = ID '=' <syntax_elements> <optional_syntax_restrictions>; */
#line 461 "alan.pmk"

        pmSeSt[pmStkP+1].stx = newSyntax(pmSySt[pmStkP+2].srcp,
                                       pmSeSt[pmStkP+1].id,
                                       concat(pmSeSt[pmStkP+3].elms,
                                              newEndOfSyntax(),
                                              ELEMENT_LIST),
                                       pmSeSt[pmStkP+4].ress,
	                                   pmSeSt[pmStkP+4].srcp);
    	break;}
    case 53: { /* <syntax_elements> = <syntax_element>; */
#line 474 "alan.pmk"

        pmSeSt[pmStkP+1].elms = newList(pmSeSt[pmStkP+1].elm, ELEMENT_LIST);
    	break;}
    case 54: { /* <syntax_elements> = <syntax_elements> <syntax_element>; */
#line 479 "alan.pmk"

        pmSeSt[pmStkP+1].elms = concat(pmSeSt[pmStkP+1].elms,
                                         pmSeSt[pmStkP+2].elm,
                                         ELEMENT_LIST);
    	break;}
    case 55: { /* <syntax_element> = ID; */
#line 488 "alan.pmk"

        pmSeSt[pmStkP+1].elm = newWordElement(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id);
    	break;}
    case 56: { /* <syntax_element> = '(' ID ')' <optional_indicators>; */
#line 494 "alan.pmk"

        pmSeSt[pmStkP+1].elm = newParameterElement(pmSeSt[pmStkP+2].id->srcp,
                                                    pmSeSt[pmStkP+2].id,
                                                    pmSeSt[pmStkP+4].bits);
    	break;}
    case 57: { /* <optional_indicators> =; */
#line 503 "alan.pmk"

        pmSeSt[pmStkP+1].bits = 0;
    	break;}
    case 58: { /* <optional_indicators> = <optional_indicators> <indicator>; */
#line 508 "alan.pmk"

        pmSeSt[pmStkP+1].bits |= pmSeSt[pmStkP+2].bits;
    	break;}
    case 59: { /* <indicator> = '*'; */
#line 515 "alan.pmk"

        pmSeSt[pmStkP+1].bits = MULTIPLEBIT;
    	break;}
    case 60: { /* <indicator> = '!'; */
#line 520 "alan.pmk"

        pmSeSt[pmStkP+1].bits = OMNIBIT;
    	break;}
    case 61: { /* <syntax_restriction_clauses> = <syntax_restriction>; */
#line 533 "alan.pmk"

        pmSeSt[pmStkP+1].ress = newList(pmSeSt[pmStkP+1].res, RESTRICTION_LIST);
    	break;}
    case 62: { /* <syntax_restriction_clauses> = <syntax_restriction_clauses> 'and' <syntax_restriction>; */
#line 538 "alan.pmk"

        pmSeSt[pmStkP+1].ress = concat(pmSeSt[pmStkP+1].ress,
                                                    pmSeSt[pmStkP+3].res, RESTRICTION_LIST);
    	break;}
    case 63: { /* <syntax_restriction> = ID 'isa' <restriction_class> 'else' <statements>; */
#line 546 "alan.pmk"

        pmSeSt[pmStkP+1].res = newRestriction(pmSySt[pmStkP+2].srcp,
                                                   pmSeSt[pmStkP+1].id,
                                                   pmSeSt[pmStkP+3].restrictionKind,
                                                   pmSeSt[pmStkP+3].id,
                                                   pmSeSt[pmStkP+5].stms);
    	break;}
    case 64: { /* <restriction_class> = ID; */
#line 557 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
        pmSeSt[pmStkP+1].restrictionKind = ID_RESTRICTION;
    	break;}
    case 65: { /* <restriction_class> = 'container'; */
#line 563 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
        pmSeSt[pmStkP+1].restrictionKind = CONTAINER_RESTRICTION;
    	break;}
    case 66: { /* <optional_syntax_restrictions> = '.'; */
#line 571 "alan.pmk"

        pmSeSt[pmStkP+1].ress = NULL;
    	break;}
    case 67: { /* <optional_syntax_restrictions> = 'where' <syntax_restriction_clauses>; */
#line 576 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].ress = pmSeSt[pmStkP+2].ress;
    	break;}
    case 68: { /* <verb> = <verb_header> <verb_body> <verb_tail>; */
#line 586 "alan.pmk"

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
#line 600 "alan.pmk"

        pmSeSt[pmStkP+1].srcp   = pmSySt[pmStkP+2].srcp;
        pmSeSt[pmStkP+1].str    = pmSeSt[pmStkP+3].str;
        pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+3].idList;
        pmSeSt[pmStkP+1].isMeta = pmSeSt[pmStkP+1].isMeta;
    	break;}
    case 71: { /* <optional_meta> = 'meta'; */
#line 609 "alan.pmk"
 pmSeSt[pmStkP+1].isMeta = true; 	break;}
    case 70: { /* <optional_meta> =; */
#line 610 "alan.pmk"
 pmSeSt[pmStkP+1].isMeta = false; 	break;}
    case 72: { /* <verb_body> = <simple_verb_body>; */
#line 615 "alan.pmk"

        pmSeSt[pmStkP+1].alts = newList(newAlternative(pmSeSt[pmStkP+1].srcp,
                                                   NULL,
                                                   pmSeSt[pmStkP+1].chks,
                                                   pmSeSt[pmStkP+1].qual,
                                                   pmSeSt[pmStkP+1].stms),
                                    ALTERNATIVE_LIST);
    	break;}
    case 73: { /* <verb_body> = <verb_alternatives>; */
#line 625 "alan.pmk"

        pmSeSt[pmStkP+1].alts = pmSeSt[pmStkP+1].alts;
    	break;}
    case 74: { /* <verb_alternatives> = <verb_alternative>; */
#line 632 "alan.pmk"

        pmSeSt[pmStkP+1].alts = newList(pmSeSt[pmStkP+1].alt, ALTERNATIVE_LIST);
    	break;}
    case 75: { /* <verb_alternatives> = <verb_alternatives> <verb_alternative>; */
#line 637 "alan.pmk"

        pmSeSt[pmStkP+1].alts = concat(pmSeSt[pmStkP+1].alts,
                                           pmSeSt[pmStkP+2].alt,
                                           ALTERNATIVE_LIST);
    	break;}
    case 76: { /* <verb_alternative> = 'when' ID <simple_verb_body>; */
#line 646 "alan.pmk"

        pmSeSt[pmStkP+1].alt = newAlternative(pmSySt[pmStkP+1].srcp,
                                                 pmSeSt[pmStkP+2].id,
                                                 pmSeSt[pmStkP+3].chks,
                                                 pmSeSt[pmStkP+3].qual,
                                                 pmSeSt[pmStkP+3].stms);
    	break;}
    case 77: { /* <simple_verb_body> = <optional_checks> <optional_does>; */
#line 657 "alan.pmk"

        if (pmSeSt[pmStkP+2].stms == NULL) /* No DOES */
            pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        else
            pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+2].srcp;
        pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 78: { /* <verb_tail> = 'end' 'verb' <optional_id> '.'; */
#line 670 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 79: { /* <optional_checks> =; */
#line 678 "alan.pmk"

        pmSeSt[pmStkP+1].chks = NULL;
        pmSeSt[pmStkP+1].srcp = nulsrcp;
    	break;}
    case 80: { /* <optional_checks> = 'check' <statements>; */
#line 684 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].chks = newList(newCheck(NULL, pmSeSt[pmStkP+2].stms), CHECK_LIST);
    	break;}
    case 81: { /* <optional_checks> = 'check' <check_list>; */
#line 690 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
    	break;}
    case 82: { /* <check_list> = <check>; */
#line 698 "alan.pmk"

        pmSeSt[pmStkP+1].chks = newList(pmSeSt[pmStkP+1].chk, CHECK_LIST);
    	break;}
    case 83: { /* <check_list> = <check_list> 'and' <check>; */
#line 703 "alan.pmk"

        pmSeSt[pmStkP+1].chks = concat(pmSeSt[pmStkP+1].chks, pmSeSt[pmStkP+3].chk, CHECK_LIST);
    	break;}
    case 84: { /* <check> = <expression> 'else' <statements>; */
#line 710 "alan.pmk"

        pmSeSt[pmStkP+1].chk = newCheck(pmSeSt[pmStkP+1].exp, pmSeSt[pmStkP+3].stms);
    	break;}
    case 85: { /* <optional_does> =; */
#line 718 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
        pmSeSt[pmStkP+1].srcp = nulsrcp;
        pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 86: { /* <optional_does> = <does>; */
#line 725 "alan.pmk"

        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+1].qual;
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 87: { /* <does> = 'does' <optional_qual> <statements>; */
#line 734 "alan.pmk"

        pmSeSt[pmStkP+1].qual = pmSeSt[pmStkP+2].qual;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 88: { /* <class> = 'every' ID <optional_heritage> <properties> <class_tail>; */
#line 745 "alan.pmk"

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
#line 775 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 92: { /* <addition> = 'add' 'to' __genSym#2 ID <optional_heritage> <properties> <add_tail>; */
#line 782 "alan.pmk"

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
#line 812 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+4].id; 	break;}
    case 100: { /* <instance> = 'the' ID <optional_heritage> <properties> <instance_tail>; */
#line 819 "alan.pmk"

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
#line 849 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id; 	break;}
    case 104: { /* <optional_heritage> =; */
#line 854 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 105: { /* <optional_heritage> = <heritage>; */
#line 859 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 106: { /* <heritage> = 'isa' ID __genSym#6; */
#line 866 "alan.pmk"
 pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id; 	break;}
    case 109: { /* <properties> =; */
#line 871 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out all fields */
    	break;}
    case 110: { /* <properties> = <properties> <property>; */
#line 876 "alan.pmk"

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
#line 993 "alan.pmk"
{ List *nam = pmSeSt[pmStkP+1].nam;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].nam = nam;
    }	break;}
    case 116: { /* <property> = <pronoun>; */
#line 999 "alan.pmk"
{ List *idList = pmSeSt[pmStkP+1].idList;
	Srcp pronounsSrcp = pmSeSt[pmStkP+1].pronounsSrcp;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].pronouns = idList;
	pmSeSt[pmStkP+1].pronounsSrcp = pronounsSrcp;
    }	break;}
    case 111: { /* <property> = <where> __genSym#7; */
#line 1007 "alan.pmk"
{
        Where *whr = pmSeSt[pmStkP+1].whr;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].whr = whr;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 122: { /* <property> = <is> <attributes>; */
#line 1017 "alan.pmk"

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].atrs = pmSeSt[pmStkP+2].atrs;
    	break;}
    case 117: { /* <property> = <initialize>; */
#line 1023 "alan.pmk"
{
        Initialize *initialize = pmSeSt[pmStkP+1].initialize;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].initialize = initialize;
    }	break;}
    case 112: { /* <property> = <container_properties>; */
#line 1031 "alan.pmk"
{
        Container *cnt = pmSeSt[pmStkP+1].cnt;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].cnt = cnt;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 113: { /* <property> = <description>; */
#line 1041 "alan.pmk"
{
	Description *description = pmSeSt[pmStkP+1].description;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */

	pmSeSt[pmStkP+1].description = description;
    }	break;}
    case 118: { /* <property> = <entered>; */
#line 1050 "alan.pmk"
{
        List *entered = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].entered = entered;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 119: { /* <property> = <mentioned>; */
#line 1060 "alan.pmk"
{
        List *ment = pmSeSt[pmStkP+1].stms;
        Srcp srcp = pmSeSt[pmStkP+1].srcp;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].mentioned = ment;
	pmSeSt[pmStkP+1].srcp = srcp;
    }	break;}
    case 114: { /* <property> = __genSym#8 <article_or_form>; */
#line 1070 "alan.pmk"
{ Article *indefinite = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].indefinite = indefinite;
    }	break;}
    case 120: { /* <property> = 'definite' <article_or_form>; */
#line 1078 "alan.pmk"
{ Article *definite = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].definite = definite;
    }	break;}
    case 121: { /* <property> = 'negative' <article_or_form>; */
#line 1086 "alan.pmk"
{ Article *article = pmSeSt[pmStkP+2].article;

	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].negative = article;
    }	break;}
    case 124: { /* <property> = <exit>; */
#line 1094 "alan.pmk"
{ Exit *ext = pmSeSt[pmStkP+1].ext;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].ext = ext;
    }	break;}
    case 125: { /* <property> = <verb>; */
#line 1100 "alan.pmk"
{ Verb *vrb = pmSeSt[pmStkP+1].vrb;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].vrb = vrb;
    }	break;}
    case 123: { /* <property> = <script>; */
#line 1106 "alan.pmk"
{ Script *scr = pmSeSt[pmStkP+1].scr;
	memset(&pmSeSt[pmStkP+1], 0, sizeof(pmSeSt[pmStkP+1])); /* Zero out other fields */
	pmSeSt[pmStkP+1].scr = scr;
    }	break;}
    case 130: { /* <exit> = 'exit' <id_list> 'to' ID <optional_exit_body> '.'; */
#line 1116 "alan.pmk"

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
#line 1131 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 132: { /* <optional_exit_body> = <optional_checks> <optional_does> 'end' 'exit' <optional_id>; */
#line 1138 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+1].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	if (pmSeSt[pmStkP+2].qual != QUAL_DEFAULT)
	     lmlog(&pmSeSt[pmStkP+2].srcp, 210, sevERR, "");
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+5].id;
    	break;}
    case 133: { /* <is> = 'is'; */
#line 1153 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 134: { /* <is> = 'are'; */
#line 1158 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 135: { /* <is> = 'has'; */
#line 1163 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 136: { /* <is> = 'can'; */
#line 1168 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 137: { /* <optional_description> =; */
#line 1175 "alan.pmk"

	pmSeSt[pmStkP+1].description = NULL;
    	break;}
    case 138: { /* <optional_description> = <description>; */
#line 1180 "alan.pmk"

	pmSeSt[pmStkP+1].description = pmSeSt[pmStkP+1].description;
    	break;}
    case 139: { /* <description> = 'description' <optional_checks> <optional_does>; */
#line 1187 "alan.pmk"

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
#line 1212 "alan.pmk"

	pmSeSt[pmStkP+1].description = newDescription(nulsrcp, NULL,
					pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].stms);
    	break;}
    case 143: { /* <article> = 'article'; */
#line 1225 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					newList(newStatement(&pmSySt[pmStkP+1].srcp, NOP_STATEMENT),
						STATEMENT_LIST),
					false);
    	break;}
    case 144: { /* <article> = 'article' <statements>; */
#line 1233 "alan.pmk"

	 pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					pmSeSt[pmStkP+2].stms,
					false);
    	break;}
    case 145: { /* <form> = 'form'; */
#line 1242 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					newList(newStatement(&pmSySt[pmStkP+1].srcp,
								NOP_STATEMENT),
						STATEMENT_LIST),
					true);
    	break;}
    case 146: { /* <form> = 'form' <statements>; */
#line 1251 "alan.pmk"

	pmSeSt[pmStkP+1].article = newArticle(pmSySt[pmStkP+1].srcp,
					pmSeSt[pmStkP+2].stms,
					true);
    	break;}
    case 147: { /* <entered> = 'entered' <statements>; */
#line 1261 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 148: { /* <initialize> = 'initialize' <statements>; */
#line 1270 "alan.pmk"

	pmSeSt[pmStkP+1].initialize = newInitialize(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].stms);
    	break;}
    case 149: { /* <mentioned> = 'mentioned' <statements>; */
#line 1278 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 150: { /* <name> = 'name' <ids> <optional_full_stop>; */
#line 1287 "alan.pmk"

	pmSeSt[pmStkP+1].nam = pmSeSt[pmStkP+2].idList;
    	break;}
    case 151: { /* <pronoun> = 'pronoun' <id_list> <optional_full_stop>; */
#line 1294 "alan.pmk"

	pmSeSt[pmStkP+1].idList = pmSeSt[pmStkP+2].idList;
	pmSeSt[pmStkP+1].pronounsSrcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 154: { /* <container_properties> = __genSym#9 <optionally_opaque> 'container' <container_body>; */
#line 1308 "alan.pmk"

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
#line 1324 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = false; 	break;}
    case 158: { /* <optionally_opaque> = 'opaque'; */
#line 1326 "alan.pmk"
 pmSeSt[pmStkP+1].opaque = true; 	break;}
    case 159: { /* <container_body> = <optional_taking> <optional_limits> <optional_header> <optional_empty> <optional_extract>; */
#line 1332 "alan.pmk"

	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+1].id;
	pmSeSt[pmStkP+1].lims  = pmSeSt[pmStkP+2].lims;
	pmSeSt[pmStkP+1].stms  = pmSeSt[pmStkP+3].stms;
	pmSeSt[pmStkP+1].stms2 = pmSeSt[pmStkP+4].stms;
	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+5].chks;
	pmSeSt[pmStkP+1].stms3 = pmSeSt[pmStkP+5].stms;
    	break;}
    case 160: { /* <container_body> = '.'; */
#line 1342 "alan.pmk"

	pmSeSt[pmStkP+1].id  = NULL;
	pmSeSt[pmStkP+1].lims  = NULL;
	pmSeSt[pmStkP+1].stms  = NULL;
	pmSeSt[pmStkP+1].stms2 = NULL;
	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms3 = NULL;
    	break;}
    case 161: { /* <optional_taking> =; */
#line 1354 "alan.pmk"

	pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 162: { /* <optional_taking> = 'taking' ID '.'; */
#line 1359 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 163: { /* <optional_limits> =; */
#line 1367 "alan.pmk"

	pmSeSt[pmStkP+1].lims = NULL;
    	break;}
    case 164: { /* <optional_limits> = 'limits' <limits>; */
#line 1372 "alan.pmk"

	pmSeSt[pmStkP+1].lims = pmSeSt[pmStkP+2].lims;
    	break;}
    case 165: { /* <limits> = <limit>; */
#line 1379 "alan.pmk"

	pmSeSt[pmStkP+1].lims = newList(pmSeSt[pmStkP+1].lim, LIMIT_LIST);
    	break;}
    case 166: { /* <limits> = <limits> <limit>; */
#line 1384 "alan.pmk"

	pmSeSt[pmStkP+1].lims = concat(pmSeSt[pmStkP+1].lims, pmSeSt[pmStkP+2].lim, LIMIT_LIST);
    	break;}
    case 167: { /* <limit> = <limit_attribute> <else_or_then> <statements>; */
#line 1391 "alan.pmk"

	pmSeSt[pmStkP+1].lim = newlim(&pmSeSt[pmStkP+2].srcp,
			    pmSeSt[pmStkP+1].atr,
			    pmSeSt[pmStkP+3].stms);
    	break;}
    case 168: { /* <else_or_then> = 'else'; */
#line 1399 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 169: { /* <else_or_then> = 'then'; */
#line 1400 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 170: { /* <limit_attribute> = <attribute_definition>; */
#line 1405 "alan.pmk"

	pmSeSt[pmStkP+1].atr = pmSeSt[pmStkP+1].atr;
    	break;}
    case 171: { /* <limit_attribute> = 'count' Integer; */
#line 1410 "alan.pmk"

	pmSeSt[pmStkP+1].atr = newIntegerAttribute(pmSySt[pmStkP+1].srcp, newId(pmSySt[pmStkP+1].srcp, "count"), val(pmSySt[pmStkP+2].chars));
    	break;}
    case 172: { /* <optional_header> =; */
#line 1417 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 173: { /* <optional_header> = 'header' <statements>; */
#line 1422 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 174: { /* <optional_empty> =; */
#line 1429 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 175: { /* <optional_empty> = 'else' <statements>; */
#line 1434 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 176: { /* <optional_extract> =; */
#line 1441 "alan.pmk"

	pmSeSt[pmStkP+1].chks = NULL;
	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 177: { /* <optional_extract> = 'extract' <optional_checks> <optional_does>; */
#line 1447 "alan.pmk"

	pmSeSt[pmStkP+1].chks = pmSeSt[pmStkP+2].chks;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+3].stms;
	if (pmSeSt[pmStkP+3].qual != QUAL_DEFAULT)
	  lmlog(&pmSeSt[pmStkP+3].srcp, 210, sevERR, "");
    	break;}
    case 178: { /* <optional_extract> = 'extract' <statements>; */
#line 1455 "alan.pmk"

	pmSeSt[pmStkP+1].chk = NULL;
	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 179: { /* <event> = <event_header> <statements> <event_tail>; */
#line 1465 "alan.pmk"

	pmSeSt[pmStkP+1].evt = newEvent(&pmSeSt[pmStkP+1].srcp,
				pmSeSt[pmStkP+1].id,
				pmSeSt[pmStkP+2].stms);
	if (pmSeSt[pmStkP+3].id != NULL) { /* END-id given */
	    if (!equalId(pmSeSt[pmStkP+1].id, pmSeSt[pmStkP+3].id))
		lmlog(&pmSeSt[pmStkP+3].id->srcp, 200, sevWAR, pmSeSt[pmStkP+1].id->string);
	}
    	break;}
    case 180: { /* <event_header> = 'event' ID; */
#line 1478 "alan.pmk"

	pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
	pmSeSt[pmStkP+1].id  = pmSeSt[pmStkP+2].id;
    	break;}
    case 181: { /* <event_tail> = 'end' 'event' <optional_id> '.'; */
#line 1486 "alan.pmk"

	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 182: { /* <script> = 'script' ID __genSym#10 <optional_description> <step_list>; */
#line 1494 "alan.pmk"

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
#line 1508 "alan.pmk"

	pmSeSt[pmStkP+1].stps = newList(pmSeSt[pmStkP+1].stp, STEP_LIST);
    	break;}
    case 186: { /* <step_list> = <step_list> <step>; */
#line 1513 "alan.pmk"

	pmSeSt[pmStkP+1].stps = concat(pmSeSt[pmStkP+1].stps, pmSeSt[pmStkP+2].stp, STEP_LIST);
    	break;}
    case 187: { /* <step> = 'step' <statements>; */
#line 1520 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
			   0,
			   NULL,
			   pmSeSt[pmStkP+2].stms);
    	break;}
    case 188: { /* <step> = 'step' 'after' <expression> __genSym#11 <statements>; */
#line 1528 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
				pmSeSt[pmStkP+3].exp,
				NULL,
				pmSeSt[pmStkP+5].stms);
    	break;}
    case 189: { /* <step> = 'step' 'wait' 'until' <expression> __genSym#12 <statements>; */
#line 1536 "alan.pmk"

	pmSeSt[pmStkP+1].stp = newStep(&pmSySt[pmStkP+1].srcp,
				NULL,
				pmSeSt[pmStkP+4].exp,
				pmSeSt[pmStkP+6].stms);
    	break;}
    case 194: { /* <rule> = 'when' <expression> <then> <statements> <optional_end_when>; */
#line 1547 "alan.pmk"

	pmSeSt[pmStkP+1].rul = newRule(&pmSySt[pmStkP+1].srcp,
		 		pmSeSt[pmStkP+2].exp,
				pmSeSt[pmStkP+4].stms);
    	break;}
    case 201: { /* <start> = 'start' <where> '.' <optional_statements>; */
#line 1567 "alan.pmk"

	adv.whr	 = pmSeSt[pmStkP+2].whr;
	adv.stms = pmSeSt[pmStkP+4].stms;
    	break;}
    case 202: { /* <optional_statements> =; */
#line 1576 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 203: { /* <optional_statements> = <statements>; */
#line 1581 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 204: { /* <statements> = <statement>; */
#line 1588 "alan.pmk"

	pmSeSt[pmStkP+1].stms = newList(pmSeSt[pmStkP+1].stm, STATEMENT_LIST);
    	break;}
    case 205: { /* <statements> = <statements> <statement>; */
#line 1593 "alan.pmk"

	pmSeSt[pmStkP+1].stms = concat(pmSeSt[pmStkP+1].stms, pmSeSt[pmStkP+2].stm, STATEMENT_LIST);
    	break;}
    case 206: { /* <statement> = <output_statement>; */
#line 1600 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 208: { /* <statement> = <manipulation_statement>; */
#line 1605 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 211: { /* <statement> = <assignment_statement>; */
#line 1610 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 213: { /* <statement> = <conditional_statement>; */
#line 1615 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 212: { /* <statement> = <repetition_statement>; */
#line 1620 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 209: { /* <statement> = <actor_statement>; */
#line 1625 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 210: { /* <statement> = <event_statement>; */
#line 1630 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 207: { /* <statement> = <special_statement>; */
#line 1635 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 214: { /* <output_statement> = STRING; */
#line 1643 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, PRINT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.print.fpos = pmSySt[pmStkP+1].fpos;
	pmSeSt[pmStkP+1].stm->fields.print.len = pmSySt[pmStkP+1].len;
    	break;}
    case 215: { /* <output_statement> = 'describe' <what> '.'; */
#line 1650 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newDescribeStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 216: { /* <output_statement> = 'say' <say_form> <expression> '.'; */
#line 1655 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAY_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.say.exp = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.say.form = pmSeSt[pmStkP+2].sayForm;
    	break;}
    case 217: { /* <output_statement> = 'list' <primary> '.'; */
#line 1662 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newListStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 218: { /* <output_statement> = 'show' ID '.'; */
#line 1667 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newShowStatement(pmSySt[pmStkP+1].srcp, newResource(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id));
    	break;}
    case 219: { /* <output_statement> = 'play' ID '.'; */
#line 1672 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newPlayStatement(pmSySt[pmStkP+1].srcp, newResource(pmSeSt[pmStkP+2].id->srcp, pmSeSt[pmStkP+2].id));
    	break;}
    case 220: { /* <output_statement> = 'style' ID '.'; */
#line 1677 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStyleStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].id);
    	break;}
    case 221: { /* <say_form> =; */
#line 1684 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_SIMPLE; 	break;}
    case 222: { /* <say_form> = 'the'; */
#line 1686 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_DEFINITE; 	break;}
    case 223: { /* <say_form> = 'an'; */
#line 1688 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_INDEFINITE; 	break;}
    case 224: { /* <say_form> = 'it'; */
#line 1690 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_PRONOUN; 	break;}
    case 225: { /* <say_form> = 'no'; */
#line 1692 "alan.pmk"
 pmSeSt[pmStkP+1].sayForm = SAY_NEGATIVE; 	break;}
    case 226: { /* <manipulation_statement> = 'empty' <primary> <optional_where> '.'; */
#line 1699 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newEmptyStatement(pmSySt[pmStkP+1].srcp,
							  pmSeSt[pmStkP+2].exp,
							  pmSeSt[pmStkP+3].whr);
    	break;}
    case 227: { /* <manipulation_statement> = 'locate' <primary> <where> '.'; */
#line 1706 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newLocateStatement(pmSySt[pmStkP+1].srcp,
							   pmSeSt[pmStkP+2].exp,
							   pmSeSt[pmStkP+3].whr);
    	break;}
    case 228: { /* <manipulation_statement> = 'include' <primary> 'in' <what> '.'; */
#line 1713 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newIncludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 229: { /* <manipulation_statement> = 'exclude' <primary> 'from' <what> '.'; */
#line 1720 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newExcludeStatement(pmSySt[pmStkP+1].srcp,
							    pmSeSt[pmStkP+2].exp,
							    pmSeSt[pmStkP+4].exp);
    	break;}
    case 231: { /* <event_statement> = 'schedule' <what> <optional_where> 'after' <expression> '.'; */
#line 1731 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newScheduleStatement(pmSySt[pmStkP+1].srcp,
						      pmSeSt[pmStkP+2].exp,
						      pmSeSt[pmStkP+3].whr,
						      pmSeSt[pmStkP+5].exp);
    	break;}
    case 230: { /* <event_statement> = 'cancel' <what> '.'; */
#line 1739 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newCancelStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp);
    	break;}
    case 232: { /* <assignment_statement> = 'make' <primary> <something> '.'; */
#line 1748 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, MAKE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.make.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.make.not = pmSeSt[pmStkP+3].not;
	pmSeSt[pmStkP+1].stm->fields.make.atr = pmSeSt[pmStkP+3].id;
    	break;}
    case 236: { /* <assignment_statement> = 'set' <attribute_reference> 'to' <expression> '.'; */
#line 1756 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SET_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.set.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.set.exp = pmSeSt[pmStkP+4].exp;
    	break;}
    case 234: { /* <assignment_statement> = 'increase' <attribute_reference> <optional_by_clause> '.'; */
#line 1763 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, INCREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 235: { /* <assignment_statement> = 'decrease' <attribute_reference> <optional_by_clause> '.'; */
#line 1770 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DECREASE_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.incr.wht = pmSeSt[pmStkP+2].exp;
	pmSeSt[pmStkP+1].stm->fields.incr.step = pmSeSt[pmStkP+3].exp;
    	break;}
    case 233: { /* <assignment_statement> = 'strip' <optional_first_or_last> <optional_expression> <optional_word_or_character> 'from' <expression> <optional_into> '.'; */
#line 1777 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STRIP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.strip.first = pmSeSt[pmStkP+2].first;
	pmSeSt[pmStkP+1].stm->fields.strip.count = pmSeSt[pmStkP+3].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.wordOrChar = pmSeSt[pmStkP+4].wordOrChar;
	pmSeSt[pmStkP+1].stm->fields.strip.from = pmSeSt[pmStkP+6].exp;
	pmSeSt[pmStkP+1].stm->fields.strip.into = pmSeSt[pmStkP+7].exp;
    	break;}
    case 237: { /* <optional_by_clause> =; */
#line 1789 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 238: { /* <optional_by_clause> = 'by' <expression>; */
#line 1794 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 239: { /* <optional_first_or_last> =; */
#line 1801 "alan.pmk"
 pmSeSt[pmStkP+1].first = true; 	break;}
    case 240: { /* <optional_first_or_last> = 'first'; */
#line 1803 "alan.pmk"
 pmSeSt[pmStkP+1].first = true; 	break;}
    case 241: { /* <optional_first_or_last> = 'last'; */
#line 1805 "alan.pmk"
 pmSeSt[pmStkP+1].first = false; 	break;}
    case 242: { /* <optional_word_or_character> =; */
#line 1810 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = true; 	break;}
    case 243: { /* <optional_word_or_character> = 'word'; */
#line 1812 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = true; 	break;}
    case 244: { /* <optional_word_or_character> = 'words'; */
#line 1814 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = true; 	break;}
    case 245: { /* <optional_word_or_character> = 'character'; */
#line 1816 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = false; 	break;}
    case 246: { /* <optional_word_or_character> = 'characters'; */
#line 1818 "alan.pmk"
 pmSeSt[pmStkP+1].wordOrChar = false; 	break;}
    case 247: { /* <optional_into> =; */
#line 1823 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 248: { /* <optional_into> = 'into' <expression>; */
#line 1826 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp; 	break;}
    case 249: { /* <conditional_statement> = <if_statement>; */
#line 1832 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 250: { /* <conditional_statement> = <depending_statement>; */
#line 1837 "alan.pmk"

	pmSeSt[pmStkP+1].stm = pmSeSt[pmStkP+1].stm;
    	break;}
    case 251: { /* <if_statement> = 'if' <expression> 'then' <statements> <optional_elsif_list> <optional_else_part> 'end' 'if' '.'; */
#line 1845 "alan.pmk"

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
#line 1863 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 253: { /* <optional_elsif_list> = <elsif_list>; */
#line 1868 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+1].stms;
    	break;}
    case 254: { /* <elsif_list> = 'elsif' <expression> 'then' <statements>; */
#line 1875 "alan.pmk"
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
#line 1888 "alan.pmk"
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
#line 1907 "alan.pmk"

	pmSeSt[pmStkP+1].stms = NULL;
    	break;}
    case 257: { /* <optional_else_part> = 'else' <statements>; */
#line 1912 "alan.pmk"

	pmSeSt[pmStkP+1].stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 258: { /* <depending_statement> = 'depending' 'on' <primary> <depend_cases> 'end' __genSym#14 '.'; */
#line 1919 "alan.pmk"
{ Statement *stm;
	stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPEND_STATEMENT);
	stm->fields.depend.exp = pmSeSt[pmStkP+3].exp;
	stm->fields.depend.cases = pmSeSt[pmStkP+4].cases;

	pmSeSt[pmStkP+1].stm = stm;
    }	break;}
    case 261: { /* <depend_cases> = <depend_case>; */
#line 1930 "alan.pmk"

	pmSeSt[pmStkP+1].cases = newList(pmSeSt[pmStkP+1].stm, CASE_LIST);
    	break;}
    case 262: { /* <depend_cases> = <depend_cases> <depend_case>; */
#line 1935 "alan.pmk"

	pmSeSt[pmStkP+1].cases = concat(pmSeSt[pmStkP+1].cases, pmSeSt[pmStkP+2].stm, CASE_LIST);
    	break;}
    case 264: { /* <depend_case> = <right_hand_side> 'then' <statements>; */
#line 1942 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+2].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = pmSeSt[pmStkP+1].exp;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+3].stms;
    	break;}
    case 263: { /* <depend_case> = 'else' <statements>; */
#line 1949 "alan.pmk"

        pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, DEPENDCASE_STATEMENT);
        pmSeSt[pmStkP+1].stm->fields.depcase.exp = NULL;
        pmSeSt[pmStkP+1].stm->fields.depcase.stms = pmSeSt[pmStkP+2].stms;
    	break;}
    case 265: { /* <repetition_statement> = <for_each> ID <optional_loop_filters> 'do' <statements> 'end' <for_each> __genSym#15; */
#line 1959 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newEachStatement(pmSeSt[pmStkP+1].srcp,
							pmSeSt[pmStkP+2].id,
							pmSeSt[pmStkP+3].exps,
							pmSeSt[pmStkP+5].stms);
    	break;}
    case 268: { /* <optional_loop_filters> =; */
#line 1969 "alan.pmk"

	pmSeSt[pmStkP+1].exps = NULL;
    	break;}
    case 269: { /* <optional_loop_filters> = <filters>; */
#line 1974 "alan.pmk"

	pmSeSt[pmStkP+1].exps = pmSeSt[pmStkP+1].exps;
    	break;}
    case 270: { /* <optional_loop_filters> = 'between' <arithmetic> 'and' <arithmetic>; */
#line 1979 "alan.pmk"

	pmSeSt[pmStkP+1].exps = newList(newBetweenExpression(pmSySt[pmStkP+1].srcp,
					     				NULL,
					     				false,
					     				pmSeSt[pmStkP+2].exp,
					     				pmSeSt[pmStkP+4].exp),
						EXPRESSION_LIST);
    	break;}
    case 271: { /* <for_each> = 'for'; */
#line 1991 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 272: { /* <for_each> = 'each'; */
#line 1993 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 273: { /* <for_each> = 'for' 'each'; */
#line 1995 "alan.pmk"
 pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp; 	break;}
    case 275: { /* <actor_statement> = 'use' 'script' ID <optional_for_actor> '.'; */
#line 2001 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newUseStatement(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+3].id, pmSeSt[pmStkP+4].exp);
    	break;}
    case 274: { /* <actor_statement> = 'stop' <what> '.'; */
#line 2006 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, STOP_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.stop.actor = pmSeSt[pmStkP+2].exp;
    	break;}
    case 276: { /* <optional_for_actor> =; */
#line 2014 "alan.pmk"

	pmSeSt[pmStkP+1].exp = NULL;
    	break;}
    case 277: { /* <optional_for_actor> = 'for' <what>; */
#line 2019 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 278: { /* <special_statement> = 'quit' '.'; */
#line 2027 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, QUIT_STATEMENT);
    	break;}
    case 279: { /* <special_statement> = 'look' '.'; */
#line 2032 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, LOOK_STATEMENT);
    	break;}
    case 280: { /* <special_statement> = 'save' '.'; */
#line 2037 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SAVE_STATEMENT);
    	break;}
    case 281: { /* <special_statement> = 'restore' '.'; */
#line 2042 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTORE_STATEMENT);
    	break;}
    case 282: { /* <special_statement> = 'restart' '.'; */
#line 2047 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, RESTART_STATEMENT);
    	break;}
    case 283: { /* <special_statement> = 'score' <optional_integer> '.'; */
#line 2052 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SCORE_STATEMENT);
	if (pmSeSt[pmStkP+2].str != NULL) {
	    pmSeSt[pmStkP+1].stm->fields.score.score =
						val(pmSeSt[pmStkP+2].str);
	    pmSeSt[pmStkP+1].stm->fields.score.count = ++scoreCount;
	} else
	    pmSeSt[pmStkP+1].stm->fields.score.count = 0;
    	break;}
    case 286: { /* <special_statement> = 'visits' Integer '.'; */
#line 2063 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, VISITS_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.visits.count = val(pmSySt[pmStkP+2].chars);
    	break;}
    case 284: { /* <special_statement> = 'transcript' <on_or_off> '.'; */
#line 2069 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, TRANSCRIPT_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.transcript.on_or_off = pmSeSt[pmStkP+2].on_or_off;
    	break;}
    case 285: { /* <special_statement> = 'system' STRING '.'; */
#line 2075 "alan.pmk"

	pmSeSt[pmStkP+1].stm = newStatement(&pmSySt[pmStkP+1].srcp, SYSTEM_STATEMENT);
	pmSeSt[pmStkP+1].stm->fields.system.fpos = pmSySt[pmStkP+2].fpos;
	pmSeSt[pmStkP+1].stm->fields.system.len = pmSySt[pmStkP+2].len;
    	break;}
    case 287: { /* <on_or_off> = 'on'; */
#line 2084 "alan.pmk"
 pmSeSt[pmStkP+1].on_or_off = true; 	break;}
    case 288: { /* <on_or_off> = 'off'; */
#line 2087 "alan.pmk"
 pmSeSt[pmStkP+1].on_or_off = false; 	break;}
    case 289: { /* <optional_expression> =; */
#line 2094 "alan.pmk"
 pmSeSt[pmStkP+1].exp = NULL; 	break;}
    case 290: { /* <optional_expression> = <expression>; */
#line 2097 "alan.pmk"
 pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp; 	break;}
    case 291: { /* <expression> = <term>; */
#line 2102 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 292: { /* <expression> = <expression> 'or' <term>; */
#line 2107 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
						pmSeSt[pmStkP+1].exp,
						false,
						OR_OPERATOR,
						pmSeSt[pmStkP+3].exp);
    	break;}
    case 293: { /* <term> = <factor>; */
#line 2118 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 294: { /* <term> = <term> 'and' <factor>; */
#line 2123 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
					  pmSeSt[pmStkP+1].exp,
					  false,
					  AND_OPERATOR,
					  pmSeSt[pmStkP+3].exp);
    	break;}
    case 295: { /* <factor> = <arithmetic>; */
#line 2134 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 297: { /* <factor> = <factor> <optional_not> <relop> <arithmetic>; */
#line 2139 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+3].srcp,
					    pmSeSt[pmStkP+1].exp,
					    pmSeSt[pmStkP+2].present,
					    pmSeSt[pmStkP+3].op,
					    pmSeSt[pmStkP+4].exp);
    	break;}
    case 296: { /* <factor> = <factor> <optional_not> <where>; */
#line 2148 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhereExpression(pmSeSt[pmStkP+3].srcp,
					   pmSeSt[pmStkP+1].exp,
					   pmSeSt[pmStkP+2].present,
					   pmSeSt[pmStkP+3].whr);
    	break;}
    case 299: { /* <factor> = <factor> <optional_not> 'between' <arithmetic> 'and' <arithmetic>; */
#line 2156 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBetweenExpression(pmSySt[pmStkP+3].srcp,
					     pmSeSt[pmStkP+1].exp,
					     pmSeSt[pmStkP+2].present,
					     pmSeSt[pmStkP+4].exp,
					     pmSeSt[pmStkP+6].exp);
    	break;}
    case 298: { /* <factor> = <factor> <optional_not> 'contains' <arithmetic>; */
#line 2165 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+3].srcp,
					    pmSeSt[pmStkP+1].exp,
					    pmSeSt[pmStkP+2].present,
					    CONTAINS_OPERATOR,
					    pmSeSt[pmStkP+4].exp);
    	break;}
    case 300: { /* <arithmetic> = <primary>; */
#line 2177 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 302: { /* <arithmetic> = <primary> 'isa' ID; */
#line 2182 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].exp, false, pmSeSt[pmStkP+3].id);
    	break;}
    case 303: { /* <arithmetic> = <primary> <is> <something>; */
#line 2187 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id,
					 pmSeSt[pmStkP+3].not, pmSeSt[pmStkP+1].exp);
    	break;}
    case 304: { /* <arithmetic> = <arithmetic> <binop> <primary>; */
#line 2193 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+2].srcp,
						pmSeSt[pmStkP+1].exp,
						false,
						pmSeSt[pmStkP+2].op,
						pmSeSt[pmStkP+3].exp);
    	break;}
    case 301: { /* <arithmetic> = <aggregate> <filters>; */
#line 2202 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAggregateExpression(pmSeSt[pmStkP+1].srcp,
							pmSeSt[pmStkP+1].agr,
							pmSeSt[pmStkP+1].id,
							pmSeSt[pmStkP+2].exps);
    	break;}
    case 305: { /* <filters> = <filter>; */
#line 2213 "alan.pmk"

        pmSeSt[pmStkP+1].exps = newList(pmSeSt[pmStkP+1].exp, EXPRESSION_LIST);
    	break;}
    case 306: { /* <filters> = <filters> ',' <filter>; */
#line 2218 "alan.pmk"

        pmSeSt[pmStkP+1].exps = concat(pmSeSt[pmStkP+1].exps, pmSeSt[pmStkP+3].exp, EXPRESSION_LIST);
    	break;}
    case 307: { /* <filter> = <optional_not> <where>; */
#line 2237 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newWhereExpression(pmSeSt[pmStkP+2].srcp,
						      NULL,
						      pmSeSt[pmStkP+1].present,
						      pmSeSt[pmStkP+2].whr);
    	break;}
    case 308: { /* <filter> = <optional_not> 'isa' ID; */
#line 2245 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIsaExpression(pmSySt[pmStkP+2].srcp, NULL, pmSeSt[pmStkP+1].present, pmSeSt[pmStkP+3].id);
	pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 309: { /* <filter> = <is> <something>; */
#line 2251 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSeSt[pmStkP+1].srcp,
						pmSeSt[pmStkP+2].id,
						pmSeSt[pmStkP+2].not, NULL);
    	break;}
    case 311: { /* <right_hand_side> = <optional_not> <relop> <primary>; */
#line 2260 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSeSt[pmStkP+2].srcp,
						     NULL,
						     pmSeSt[pmStkP+1].present,
						     pmSeSt[pmStkP+2].op,
						     pmSeSt[pmStkP+3].exp);
    	break;}
    case 313: { /* <right_hand_side> = <optional_not> 'between' <arithmetic> 'and' <arithmetic>; */
#line 2269 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBetweenExpression(pmSySt[pmStkP+2].srcp,
						      NULL,
						      pmSeSt[pmStkP+1].present,
						      pmSeSt[pmStkP+3].exp,
						      pmSeSt[pmStkP+5].exp);
    	break;}
    case 312: { /* <right_hand_side> = <optional_not> 'contains' <factor>; */
#line 2278 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newBinaryExpression(pmSySt[pmStkP+2].srcp,
						     NULL,
						     pmSeSt[pmStkP+1].present,
						     CONTAINS_OPERATOR,
						     pmSeSt[pmStkP+3].exp);

    	break;}
    case 319: { /* <primary> = '(' <expression> ')'; */
#line 2293 "alan.pmk"

	pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+2].exp;
    	break;}
    case 317: { /* <primary> = <optional_minus> Integer; */
#line 2298 "alan.pmk"

	pmSeSt[pmStkP+1].exp = newIntegerExpression(pmSySt[pmStkP+2].srcp,
					      pmSeSt[pmStkP+1].minus?
							-val(pmSySt[pmStkP+2].chars):
							val(pmSySt[pmStkP+2].chars));
    	break;}
    case 314: { /* <primary> = STRING; */
#line 2306 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newStringExpression(pmSySt[pmStkP+1].srcp,
                                             pmSySt[pmStkP+1].fpos,
                                             pmSySt[pmStkP+1].len);
    	break;}
    case 315: { /* <primary> = <what>; */
#line 2313 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 321: { /* <primary> = 'random' <primary> 'to' <primary>; */
#line 2318 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newRandomRangeExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].exp, pmSeSt[pmStkP+4].exp);
    	break;}
    case 320: { /* <primary> = 'random' <optional_transitivity> 'in' <primary>; */
#line 2323 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newRandomInExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+4].exp, pmSeSt[pmStkP+2].transitivity);
    	break;}
    case 316: { /* <primary> = 'score'; */
#line 2328 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newScoreExpression(pmSySt[pmStkP+1].srcp);
    	break;}
    case 318: { /* <primary> = '{' <optional_members> '}'; */
#line 2333 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newSetExpression(pmSySt[pmStkP+1].srcp, pmSeSt[pmStkP+2].set);
    	break;}
    case 323: { /* <aggregate> = <aggregator> 'of' ID; */
#line 2341 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = pmSeSt[pmStkP+1].agr;
        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+3].id;
    	break;}
    case 322: { /* <aggregate> = 'count'; */
#line 2348 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = COUNT_AGGREGATE;
        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 324: { /* <aggregator> = 'max'; */
#line 2357 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = MAX_AGGREGATE;
    	break;}
    case 325: { /* <aggregator> = 'min'; */
#line 2363 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = MIN_AGGREGATE;
    	break;}
    case 326: { /* <aggregator> = 'sum'; */
#line 2369 "alan.pmk"

        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
        pmSeSt[pmStkP+1].agr = SUM_AGGREGATE;
    	break;}
    case 327: { /* <something> = <optional_not> ID; */
#line 2379 "alan.pmk"

        pmSeSt[pmStkP+1].not = pmSeSt[pmStkP+1].present;
        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+2].id;
    	break;}
    case 328: { /* <what> = <simple_what>; */
#line 2387 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newWhatExpression(pmSeSt[pmStkP+1].srcp, pmSeSt[pmStkP+1].wht);
    	break;}
    case 329: { /* <what> = <attribute_reference>; */
#line 2392 "alan.pmk"

        pmSeSt[pmStkP+1].exp = pmSeSt[pmStkP+1].exp;
    	break;}
    case 330: { /* <simple_what> = ID; */
#line 2399 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatId(pmSeSt[pmStkP+1].id->srcp, pmSeSt[pmStkP+1].id);
        pmSeSt[pmStkP+1].srcp = pmSeSt[pmStkP+1].id->srcp;
    	break;}
    case 332: { /* <simple_what> = 'current' 'actor'; */
#line 2405 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatActor(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 333: { /* <simple_what> = 'current' 'location'; */
#line 2411 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatLocation(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 331: { /* <simple_what> = 'this'; */
#line 2417 "alan.pmk"

        pmSeSt[pmStkP+1].wht = newWhatThis(pmSySt[pmStkP+1].srcp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 334: { /* <attribute_reference> = ID 'of' <what>; */
#line 2426 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].id, false, pmSeSt[pmStkP+3].exp);
    	break;}
    case 335: { /* <attribute_reference> = <what> ':' ID; */
#line 2431 "alan.pmk"

        pmSeSt[pmStkP+1].exp = newAttributeExpression(pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+3].id, false, pmSeSt[pmStkP+1].exp);
    	break;}
    case 336: { /* <optional_where> =; */
#line 2438 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&nulsrcp, DEFAULT_TRANSITIVITY, WHERE_DEFAULT, NULL);
    	break;}
    case 337: { /* <optional_where> = <where>; */
#line 2443 "alan.pmk"

        pmSeSt[pmStkP+1].whr = pmSeSt[pmStkP+1].whr;
    	break;}
    case 338: { /* <where> = <optional_transitivity> 'here'; */
#line 2450 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_HERE, NULL);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 339: { /* <where> = <optional_transitivity> 'nearby'; */
#line 2456 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_NEARBY, NULL);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 340: { /* <where> = <optional_transitivity> 'at' <primary>; */
#line 2462 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_AT, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 341: { /* <where> = <optional_transitivity> 'in' <primary>; */
#line 2468 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_IN, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 342: { /* <where> = <optional_transitivity> 'near' <what>; */
#line 2474 "alan.pmk"

        pmSeSt[pmStkP+1].whr = newWhere(&pmSySt[pmStkP+2].srcp, pmSeSt[pmStkP+1].transitivity, WHERE_NEAR, pmSeSt[pmStkP+3].exp);
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+2].srcp;
    	break;}
    case 343: { /* <binop> = '+'; */
#line 2482 "alan.pmk"

        pmSeSt[pmStkP+1].op = PLUS_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 344: { /* <binop> = '-'; */
#line 2488 "alan.pmk"

        pmSeSt[pmStkP+1].op = MINUS_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 345: { /* <binop> = '*'; */
#line 2494 "alan.pmk"

        pmSeSt[pmStkP+1].op = MULT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 346: { /* <binop> = '/'; */
#line 2500 "alan.pmk"

        pmSeSt[pmStkP+1].op = DIV_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 347: { /* <relop> = '<>'; */
#line 2509 "alan.pmk"

        pmSeSt[pmStkP+1].op   = NE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 348: { /* <relop> = '='; */
#line 2515 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EQ_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 349: { /* <relop> = '=='; */
#line 2521 "alan.pmk"

        pmSeSt[pmStkP+1].op   = EXACT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 350: { /* <relop> = '>='; */
#line 2527 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 351: { /* <relop> = '<='; */
#line 2533 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LE_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 352: { /* <relop> = '>'; */
#line 2539 "alan.pmk"

        pmSeSt[pmStkP+1].op   = GT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 353: { /* <relop> = '<'; */
#line 2545 "alan.pmk"

        pmSeSt[pmStkP+1].op   = LT_OPERATOR;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 354: { /* <optional_qual> =; */
#line 2555 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_DEFAULT;
    	break;}
    case 355: { /* <optional_qual> = 'before'; */
#line 2560 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_BEFORE;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 356: { /* <optional_qual> = 'after'; */
#line 2566 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_AFTER;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 357: { /* <optional_qual> = 'only'; */
#line 2572 "alan.pmk"

        pmSeSt[pmStkP+1].qual = QUAL_ONLY;
        pmSeSt[pmStkP+1].srcp = pmSySt[pmStkP+1].srcp;
    	break;}
    case 358: { /* <optional_not> =; */
#line 2581 "alan.pmk"

        pmSeSt[pmStkP+1].present = false;
    	break;}
    case 359: { /* <optional_not> = 'not'; */
#line 2586 "alan.pmk"

        pmSeSt[pmStkP+1].present = true;
    	break;}
    case 360: { /* <optional_transitivity> =; */
#line 2593 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = DEFAULT_TRANSITIVITY;
    	break;}
    case 361: { /* <optional_transitivity> = 'transitively'; */
#line 2598 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = TRANSITIVELY;
    	break;}
    case 362: { /* <optional_transitivity> = 'directly'; */
#line 2603 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = DIRECTLY;
    	break;}
    case 363: { /* <optional_transitivity> = 'indirectly'; */
#line 2608 "alan.pmk"

        pmSeSt[pmStkP+1].transitivity = INDIRECTLY;
    	break;}
    case 364: { /* <optional_id> =; */
#line 2615 "alan.pmk"

        pmSeSt[pmStkP+1].id = NULL;
    	break;}
    case 365: { /* <optional_id> = ID; */
#line 2620 "alan.pmk"

        pmSeSt[pmStkP+1].id = pmSeSt[pmStkP+1].id;
    	break;}
    case 366: { /* <ids> = ID; */
#line 2627 "alan.pmk"

        pmSeSt[pmStkP+1].idList = newList(pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 367: { /* <ids> = <ids> ID; */
#line 2632 "alan.pmk"

        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+2].id, ID_LIST);
    	break;}
    case 368: { /* <id_list> = ID; */
#line 2639 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSeSt[pmStkP+1].id->string);
        pmSeSt[pmStkP+1].idList = newList(pmSeSt[pmStkP+1].id, ID_LIST);
    	break;}
    case 369: { /* <id_list> = <id_list> ',' ID; */
#line 2645 "alan.pmk"

        pmSeSt[pmStkP+1].str = pmSeSt[pmStkP+1].str;
        pmSeSt[pmStkP+1].idList = concat(pmSeSt[pmStkP+1].idList, pmSeSt[pmStkP+3].id, ID_LIST);
    	break;}
    case 370: { /* <optional_integer> =; */
#line 2653 "alan.pmk"

        pmSeSt[pmStkP+1].str = NULL;
    	break;}
    case 371: { /* <optional_integer> = Integer; */
#line 2658 "alan.pmk"

        pmSeSt[pmStkP+1].str = newString(pmSySt[pmStkP+1].chars);
    	break;}
    case 372: { /* <optional_minus> =; */
#line 2665 "alan.pmk"

        pmSeSt[pmStkP+1].minus = false;
    	break;}
    case 373: { /* <optional_minus> = '-'; */
#line 2670 "alan.pmk"

        pmSeSt[pmStkP+1].minus = true;
    	break;}
    case 374: { /* ID = Identifier; */
#line 2677 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 375: { /* ID = 'location'; */
#line 2688 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 376: { /* ID = 'actor'; */
#line 2693 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 377: { /* ID = 'opaque'; */
#line 2698 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 378: { /* ID = 'visits'; */
#line 2703 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 379: { /* ID = 'contains'; */
#line 2708 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 380: { /* ID = 'on'; */
#line 2713 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 381: { /* ID = 'it'; */
#line 2718 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 382: { /* ID = 'of'; */
#line 2723 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 383: { /* ID = 'first'; */
#line 2728 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 384: { /* ID = 'into'; */
#line 2733 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 385: { /* ID = 'taking'; */
#line 2738 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    case 386: { /* ID = 'off'; */
#line 2743 "alan.pmk"

        pmSeSt[pmStkP+1].id = newId(pmSySt[pmStkP+1].srcp, pmSySt[pmStkP+1].chars);
    	break;}
    default: break; }
}/*pmPaSema()*/

