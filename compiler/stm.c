/*----------------------------------------------------------------------*\

                                STM.C
                                Statement Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"                /* ADV-node */
#include "sym.h"                /* SYM-nodes */
#include "lst.h"                /* LST-nodes */
#include "nam.h"                /* NAM-nodes */
#include "exp.h"                /* EXP-nodes */
#include "atr.h"                /* ATR-nodes */
#include "whr.h"                /* WHR-nodes */
#include "wht.h"                /* WHT-nodes */
#include "stm.h"                /* STM-nodes */
#include "obj.h"                /* OBJ-nodes */
#include "loc.h"                /* LOC-nodes */
#include "scr.h"                /* SCR-nodes */
#include "act.h"                /* ACT-nodes */
#include "sco.h"                /* SCORES */
#include "opt.h"                /* OPTIONS */

#include "emit.h"
#include "encode.h"

#include "dump.h"




/*======================================================================

  newstm()

  Allocates and initialises a stmnod.

  */
StmNod *newstm(Srcp *srcp,	/* IN - Source Position */
	       StmKind class)	/* IN - The statement class */
{
  StmNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(StmNod);

  new->srcp = *srcp;
  new->class = class;

  return(new);
}



/*----------------------------------------------------------------------

  andescribe()

  Analyze a DESCRIBE statement.

  */
static void andescribe(StmNod *stm, /* IN - The statement to analyze */
		       EvtNod *evt, /* IN - Possibly inside Event? */
		       List *pars) /* IN - Possible syntax parameters */
{
  SymNod *sym;
  ElmNod *elm;

  switch (stm->fields.describe.wht->wht) {
  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&stm->fields.describe.wht->srcp, 409, sevERR, "");
    break;
  case WHT_LOC:
    break;
  case WHT_ACT:
    if (evt != NULL)
      lmLog(&stm->fields.describe.wht->srcp, 412, sevERR, "");
    break;
  case WHT_ID:
    symcheck(&sym, &elm, stm->fields.describe.wht->nam, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT,
             NAMANY, pars);
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }
}



/*----------------------------------------------------------------------

  ansay()

  Analyze a SAY statement.

  */
static void ansay(StmNod *stm,	/* IN - The statement to analyze */
		  EvtNod *evt,	/* IN - Possibly inside Event? */
		  List *pars)	/* IN - Possible syntax parameters */
{
  anexp(stm->fields.say.exp, evt, pars);
}


/*----------------------------------------------------------------------

  anlist()

  Analyze a LIST statement.

  */
static void anlist(StmNod *stm,	/* IN - The statement to analyze */
		   List *pars)	/* IN - Possible syntax parameters */
{
  cntcheck(stm->fields.list.wht, pars);
}


/*----------------------------------------------------------------------

  anempty()

  Analyze an EMPTY statement.

  */
static void anempty(StmNod *stm, /* IN - The statement to analyze */
		    EvtNod *evt, /* IN - Inside event? */
		    List *pars)	/* IN - Possible syntax parameters */
{
  cntcheck(stm->fields.list.wht, pars);
  anwhr(stm->fields.empty.whr, evt, pars);
}


/*----------------------------------------------------------------------

  anlocate()

  Analyze a LOCATE statement.

  */
static void anlocate(StmNod *stm, /* IN - The statement to analyze */
		     EvtNod *evt, /* IN - Possibly inside actor */
		     List *pars) /* IN - Possible syntax parameters */
{
  SymNod *sym;
  ElmNod *elm;

  switch (stm->fields.locate.wht->wht) {
  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&stm->fields.locate.wht->srcp, 409, sevERR, "");
    break;
  case WHT_ACT:
    if (evt != NULL)
      lmLog(&stm->fields.locate.wht->srcp, 412, sevERR, "");
    break;
  case WHT_LOC:
    lmLog(&stm->srcp, 311, sevERR, "an Object or an Actor");
    break;
  case WHT_ID:
    symcheck(&sym, &elm, stm->fields.locate.wht->nam, NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
    
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }

  anwhr(stm->fields.locate.whr, evt, pars);
  switch (stm->fields.locate.whr->whr) {
  case WHR_HERE:
  case WHR_AT:
    break;
  case WHR_IN:
    if (stm->fields.locate.wht->wht == WHT_ACT)
      lmLog(&stm->srcp, 402, sevERR, "");
    else if (stm->fields.locate.wht->wht == WHT_ID)
      if (sym != NULL && sym->class == NAMACT)
        lmLog(&stm->srcp, 402, sevERR, "");
      else if (elm != NULL && elm->res != NULL && ((elm->res->classes & NAMACT) != 0 || (elm->res->classes & NAMCACT) != 0))
        lmLog(&stm->srcp, 402, sevERR, "");
    break;
  case WHR_NEAR:
    lmLog(&stm->srcp, 415, sevERR, "LOCATE");
    break;
  default:
    syserr("Unrecognized switch in anlocate()");
    break;
  }
}



/*----------------------------------------------------------------------

  anmake()

  Analyze a MAKE statement.

  */
static void anmake(StmNod *stm,	/* IN - The statement to analyze */
		   EvtNod *evt,	/* IN - inside an Event? */
		   List *pars)	/* IN - Possible syntax parameters */
{
  SymNod *sym;
  ElmNod *elm;
  AtrNod *atr = NULL;

  switch (stm->fields.make.wht->wht) {
  case WHT_ACT:
    if (evt != NULL)
      lmLog(&stm->fields.make.wht->srcp, 412, sevERR, "");
    else {
      atr = findatr(stm->fields.make.atr->str, adv.aatrs, adv.atrs);
      if (atr == NULL)          /* Attribute not found globally */
        lmLog(&stm->fields.make.atr->srcp, 404, sevERR, "ACTOR");
      else
        stm->fields.make.atr->code = atr->nam->code;
    }
    break;
  case WHT_LOC:
    atr = findatr(stm->fields.make.atr->str, adv.latrs, adv.atrs);
    if (atr == NULL)            /* Attribute not found globally */
      lmLog(&stm->fields.make.atr->srcp, 404, sevERR, "LOCATION");
    else
      stm->fields.make.atr->code = atr->nam->code;
    break;
  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&stm->fields.make.wht->srcp, 409, sevERR, "");
    atr = findatr(stm->fields.make.atr->str, adv.oatrs, adv.atrs);
    if (atr == NULL)            /* Attribute not found globally */
      lmLog(&stm->fields.make.atr->srcp, 404, sevERR, "OBJECT");
    else
      stm->fields.make.atr->code = atr->nam->code;
    break;
  case WHT_ID:
    symcheck(&sym, &elm, stm->fields.make.wht->nam, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT,
             NAMANY, pars);
    if (elm) {
      atr = paramatr(stm->fields.make.atr, elm);
      if (atr == NULL)          /* Not a default attribute */
        lmLog(&stm->fields.make.atr->srcp, 404, sevERR, "a parameter");
    } else if (sym) {
      atr = symatr(stm->fields.make.atr, sym);
      if (atr == NULL)
        lmLog(&stm->fields.make.atr->srcp, 315, sevERR,
              stm->fields.make.wht->nam->str);
    }
    if (atr != NULL)
      if (atr->typ != TYPBOOL)
        lmLog(&stm->fields.make.atr->srcp, 408, sevERR, "MAKE statement");
      else
        stm->fields.make.atr->code = atr->nam->code;
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }
}

  


/*----------------------------------------------------------------------

  anset()

  Analyze a SET statement

  */
static void anset(StmNod *stm,	/* IN - The statement to analyze */
		  EvtNod *evt,	/* IN - inside an Event? */
		  List *pars)	/* IN - Possible syntax parameters */
{
  SymNod *sym;
  ElmNod *elm;
  AtrNod *atr;

  switch (stm->fields.set.wht->wht) {
  case WHT_ACT:
    if (evt != NULL)
      lmLog(&stm->fields.set.wht->srcp, 412, sevERR, "");
    else {
      atr = findatr(stm->fields.set.atr->str, adv.aatrs, adv.atrs);
      if (atr == NULL)          /* attribute not found globally */
        lmLog(&stm->fields.set.atr->srcp, 404, sevERR, "ACTOR");
      else
        stm->fields.set.atr->code = atr->nam->code;
    }
    break;
  case WHT_LOC:
    atr = findatr(stm->fields.set.atr->str, adv.latrs, adv.atrs);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.set.atr->srcp, 404, sevERR, "LOCATION");
    else
      stm->fields.set.atr->code = atr->nam->code;
    break;
  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&stm->fields.set.wht->srcp, 409, sevERR, "");
    atr = findatr(stm->fields.set.atr->str, adv.oatrs, adv.atrs);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.set.atr->srcp, 404, sevERR, "OBJECT");
    else
      stm->fields.set.atr->code = atr->nam->code;
    break;
  case WHT_ID:
    symcheck(&sym, &elm, stm->fields.set.wht->nam, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT,
             NAMANY, pars);
    if (elm) {
      atr = paramatr(stm->fields.set.atr, elm);
      if (atr == NULL)          /* Not a default attribute */
        lmLog(&stm->fields.set.atr->srcp, 404, sevERR, "a parameter");
    } else if (sym) {
      atr = symatr(stm->fields.set.atr, sym);
      if (atr == NULL)
        lmLog(&stm->fields.set.atr->srcp, 315, sevERR,
              stm->fields.set.wht->nam->str);
    }
    if (atr)
      if (atr->typ != TYPINT && atr->typ != TYPSTR)
        lmLog(&stm->fields.set.atr->srcp, 419, sevERR, "Target for");
      else
        stm->fields.set.atr->code = atr->nam->code;
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }

  if (stm->fields.set.exp != NULL) {
    anexp(stm->fields.set.exp, evt, pars);
    if (stm->fields.set.exp->typ != TYPINT &&
        stm->fields.set.exp->typ != TYPSTR)
      lmLog(&stm->fields.set.exp->srcp, 419, sevERR, "Expression in");
    if (atr && !eqtyp(stm->fields.set.exp->typ, atr->typ))
      lmLog(&stm->srcp, 331, sevERR, "SET statement");
  }
}


/*----------------------------------------------------------------------

  anincr()

  Analyze a INCR/DECR statement

  */
static void anincr(StmNod *stm,	/* IN - The statement to analyze */
		   EvtNod *evt,	/* IN - inside an Event? */
		   List *pars)	/* IN - Possible syntax parameters */
{
  SymNod *sym;
  ElmNod *elm;
  AtrNod *atr;

  switch (stm->fields.incr.wht->wht) {
  case WHT_ACT:
    if (evt != NULL)
      lmLog(&stm->fields.incr.wht->srcp, 412, sevERR, "");
    else {
      atr = findatr(stm->fields.incr.atr->str, adv.aatrs, adv.atrs);
      if (atr == NULL)          /* attribute not found globally */
        lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "ACTOR");
      else
        stm->fields.incr.atr->code = atr->nam->code;
    }
    break;
  case WHT_LOC:
    atr = findatr(stm->fields.incr.atr->str, adv.latrs, adv.atrs);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "LOCATION");
    else
      stm->fields.incr.atr->code = atr->nam->code;
    break;
  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&stm->fields.incr.wht->srcp, 409, sevERR, "");
    atr = findatr(stm->fields.incr.atr->str, adv.oatrs, adv.atrs);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "OBJECT");
    else
      stm->fields.incr.atr->code = atr->nam->code;
    break;
  case WHT_ID:
    symcheck(&sym, &elm, stm->fields.incr.wht->nam, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT,
             NAMANY, pars);
    if (elm) {
      atr = paramatr(stm->fields.incr.atr, elm);
      if (atr == NULL)          /* Not a default attribute */
        lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "a parameter");
    } else if (sym) {
      atr = symatr(stm->fields.incr.atr, sym);
      if (atr == NULL)
        lmLog(&stm->fields.incr.atr->srcp, 315, sevERR,
              stm->fields.incr.wht->nam->str);
    }
    if (atr)
      if (atr->typ != TYPINT)
        lmLog(&stm->fields.incr.atr->srcp, 413, sevERR, "INCREASE/DECREASE");
      else
        stm->fields.incr.atr->code = atr->nam->code;
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }

  if (stm->fields.incr.step != NULL) {
    anexp(stm->fields.incr.step, evt, pars);
    if (stm->fields.incr.step->typ != TYPINT)
      lmLog(&stm->fields.incr.step->srcp, 413, sevERR, "INCREASE/DECREASE");
  }
}


/*----------------------------------------------------------------------

  anschedule()

  Analyze a SCHEDULE statement.

  */
static void anschedule(StmNod *stm, /* IN - The statement to analyze */
		       EvtNod *evt, /* IN - inside an Event? */
		       List *pars) /* IN - Possible syntax parameters */
{
  SymNod *sym;
  ElmNod *elm;

  symcheck(&sym, &elm, stm->fields.schedule.nam, NAMEVT, NAMANY, NULL);

  /* Now lookup where */
  anwhr(stm->fields.schedule.whr, evt, pars);
  switch (stm->fields.schedule.whr->whr) {
  case WHR_DEFAULT:
    stm->fields.schedule.whr->whr = WHR_HERE;
    break;
  case WHR_HERE:
  case WHR_AT:
    break;
  case WHR_IN:
  case WHR_NEAR:
    lmLog(&stm->fields.schedule.whr->srcp, 415, sevERR, "SCHEDULE");
    break;
  default:
    syserr("Unrecognized switch in anschedule()");
    break;
  }
}


/*----------------------------------------------------------------------

  ancancel()

  Analyze a CANCEL statement.

  */
static void ancancel(StmNod *stm) /* IN - The statement to analyze */
{
  SymNod *sym;
  ElmNod *elm;

  symcheck(&sym, &elm, stm->fields.cancel.nam, NAMEVT, NAMANY, NULL);
}


/*----------------------------------------------------------------------

  anif()

  Analyze an IF statement.

  */
static void anif(StmNod *stm,	/* IN - The statement to analyze */
		 ActNod *act,	/* IN - Possibly inside Actor */
		 EvtNod *evt,	/* IN - Possibly inside Event */
		 List *pars)	/* IN - Possible syntax parameters */
{
  anexp(stm->fields.iff.exp, evt, pars);
  if (!eqtyp(stm->fields.iff.exp->typ, TYPBOOL))
    lmLogv(&stm->fields.iff.exp->srcp, 330, sevERR, "boolean", "'IF'", NULL);
  anstms(stm->fields.iff.thn, act, evt, pars);
  if (stm->fields.iff.els != NULL)
    anstms(stm->fields.iff.els, act, evt, pars);
}


/*----------------------------------------------------------------------

  anuse()

  Analyze a USE statement. It must refer a script that is defined
  within the mentioned actor. If the actor is not specified the
  actor is assumed to be the one we are in (it is an error if we are
  not).

  */
static void anuse(StmNod *stm,	/* IN - Statement to analyze */
		  ActNod *act,	/* IN - Possibly inside Actor */
		  List *pars)	/* IN - Possible syntax parameters */
{
  SymNod *sym;
  ElmNod *elm;
  List *lst;

  if (stm->fields.use.actor == NULL && act == NULL)
    lmLog(&stm->srcp, 401, sevERR, "");
  else {
    if (stm->fields.use.actor != NULL) {
      /* Lookup actors node */
      symcheck(&sym, &elm, stm->fields.use.actor, NAMACT+NAMCACT, NAMANY, pars);
      act = NULL;
      if (elm)
        lmLog(&stm->fields.use.actor->srcp, 410, sevERR, "USE statement");
      else if (sym)
        act = sym->ref;
    }
    if (act != NULL) {
      /* Check if script is defined */
      for (lst = act->scrs; lst != NULL; lst = lst->next) {
        if (stm->fields.use.script != NULL) {
          /* A name was given find this */
          if (lst->element.scr->nam != NULL && eqnams(lst->element.scr->nam, stm->fields.use.script))
            stm->fields.use.scriptno = lst->element.scr->code;
            break;
        } else {
          /* Use the number to find it */
          if (lst->element.scr->code == stm->fields.use.scriptno)
            break;
        }
      }
      if (lst == NULL)
        if (stm->fields.use.script != NULL)
          lmLog(&stm->fields.use.script->srcp, 400, sevERR, act->nam->str);
        else
          lmLog(&stm->srcp, 400, sevERR, act->nam->str);
    }
  }
}  



/*----------------------------------------------------------------------

  anstm()

  Analyze one statement.

  */
static void anstm(StmNod *stm,	/* IN - The statement to analyze */
		  ActNod *act,	/* IN - Possibly inside Actor */
		  EvtNod *evt,	/* IN - Possibly inside Event */
		  List *pars)	/* IN - Possible syntax parameters */
{
  switch (stm->class) {
  case STM_NOP:
  case STM_PRINT:
  case STM_QUIT:
  case STM_LOOK:
  case STM_SAVE:
  case STM_RESTORE:
  case STM_RESTART:
  case STM_VISITS:
  case STM_SYSTEM:
    /* Nothing to analyse */
    break;
  case STM_SCORE:
    if (stm->fields.score.count != 0) {
      adv.scores[stm->fields.score.count] = stm->fields.score.score;
      scotot += stm->fields.score.score;
    }
    break;
  case STM_DESCRIBE:
    andescribe(stm, evt, pars);
    break;
  case STM_SAY:
    ansay(stm, evt, pars);
    break;
  case STM_LIST:
    anlist(stm, pars);
    break;
  case STM_EMPTY:
    anempty(stm, evt, pars);
    break;
  case STM_LOCATE:
    anlocate(stm, evt, pars);
    break;
  case STM_MAKE:
    anmake(stm, evt, pars);
    break;
  case STM_SET:
    anset(stm, evt, pars);
    break;
  case STM_INCR:
  case STM_DECR:
    anincr(stm, evt, pars);
    break;
  case STM_SCHEDULE:
    anschedule(stm, evt, pars);
    break;
  case STM_CANCEL:
    ancancel(stm);
    break;
  case STM_IF:
    anif(stm, act, evt, pars);
    break;
  case STM_USE:
    anuse(stm, act, pars);
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }
}



/*======================================================================

  anstms()

  Analyze all statements in a list.

  */
void anstms(List *stms,		/* IN - The list of statements to analyze */
	    ActNod *act,	/* IN - Within Actor? */
	    EvtNod *evt,	/* IN - Within Event? */
	    List *pars)		/* IN - Possible syntax parameters */
{
  while (stms != NULL) {
    anstm(stms->element.stm, act, evt, pars);
    stms = stms->next;
  }
}


/*----------------------------------------------------------------------

  geprint()

  Generate the code for a PRINT-stm. The text is found and copied to the
  data file (and encoded if requested!).

  */
static void geprint(StmNod *stm) /* IN - The statement to generate */
{
  encode(&stm->fields.print.fpos, &stm->fields.print.len);
  emit0(C_CONST, stm->fields.print.len);
  emit0(C_CONST, stm->fields.print.fpos);
  emit0(C_STMOP, I_PRINT);
}



/*----------------------------------------------------------------------

  gescore()

  Generate a SCORE statement

  */
static void gescore(StmNod *stm) /* IN - The statement to generate */
{
  emit0(C_CONST, stm->fields.score.count);
  emit0(C_STMOP, I_SCORE);
}



/*----------------------------------------------------------------------

  gedescribe()

  Generate code to implement a DESCRIBE statement.

  */
static void gedescribe(StmNod *stm) /* IN - Statement */
{
  switch (stm->fields.describe.wht->wht) {

  case WHT_OBJ:
    emit0(C_CONST, 1);
    emit0(C_CURVAR, V_PARAM);
    break;

  case WHT_LOC:
    emit0(C_CURVAR, V_CURLOC);
    break;

  case WHT_ACT:
    emit0(C_CURVAR, V_CURACT);
    break;

  case WHT_ID:
    genam(stm->fields.describe.wht->nam);
    break;

  default:
    unimpl(&stm->srcp, "Code Generator");
    return;
  }
  emit0(C_STMOP, I_DESCRIBE);
}


/*----------------------------------------------------------------------

  gesay()

  Generate code for a SAY statement.

  */
static void gesay(StmNod *stm)	/* IN - The statement to analyze */
{
  geexp(stm->fields.say.exp);
  switch (stm->fields.say.exp->typ) {
  case TYPINT:
    emit0(C_STMOP, I_SAYINT);
    break;
  case TYPSTR:
    emit0(C_STMOP, I_SAYSTR);
    break;
  case TYPENT:
    emit0(C_STMOP, I_SAY);
    break;
  default:
    unimpl(&stm->srcp, "Code Generator");
    break;
  }
}


/*----------------------------------------------------------------------

  gelist()

  Generate code to implement a LIST statement.

  */
static void gelist(StmNod *stm)	/* IN - Statement */
{
  if (stm->fields.list.wht->wht == WHT_ID) {
    genam(stm->fields.list.wht->nam);
    emit0(C_STMOP, I_LIST);
  } else
    unimpl(&stm->srcp, "Code Generator");
}



/*----------------------------------------------------------------------

  geempty()

  Generate code to implement the EMPTY statement.

  */
static void geempty(StmNod *stm) /* IN - Statement */
{
  if (stm->fields.empty.wht->wht == WHT_ID) {
    gewhr(stm->fields.empty.whr);
    genam(stm->fields.empty.wht->nam);
    emit0(C_STMOP, I_EMPTY);
  } else
    unimpl(&stm->srcp, "Code Generator");
}



/*----------------------------------------------------------------------

  gelocate()

  Generate code to implement a LOCATE statement.

  */
static void gelocate(StmNod *stm) /* IN - Statement */
{
  gewhr(stm->fields.locate.whr);
  gewht(stm->fields.locate.wht);
  emit0(C_STMOP, I_LOCATE);
}



/*----------------------------------------------------------------------

  gemake()

  Generate code to implement a MAKE statement.

  */
static void gemake(StmNod *stm)	/* IN - Statement */
{
  emit0(C_CONST, !stm->fields.make.not);
  emit0(C_CONST, stm->fields.make.atr->code);
  gewht(stm->fields.make.wht);
  emit0(C_STMOP, I_MAKE);
}




/*----------------------------------------------------------------------

  geset()

  Generate code to implement a SET statement.

  */
static void geset(StmNod *stm)	/* IN - Statement */
{
  geexp(stm->fields.set.exp);

  emit0(C_CONST, stm->fields.set.atr->code);
  gewht(stm->fields.set.wht);
  if (stm->fields.set.exp->typ == TYPSTR)
    emit0(C_STMOP, I_STRSET);
  else
    emit0(C_STMOP, I_SET);
}



/*----------------------------------------------------------------------

  geincr()

  Generate code to implement a INCR/DECR statement.

  */
static void geincr(StmNod *stm)	/* IN - Statement */
{
  if (stm->fields.incr.step != NULL)
    geexp(stm->fields.incr.step);
  else
    emit0(C_CONST, 1);

  emit0(C_CONST, stm->fields.incr.atr->code);
  gewht(stm->fields.incr.wht);
  if (stm->class == STM_INCR)
    emit0(C_STMOP, I_INCR);
  else
    emit0(C_STMOP, I_DECR);
}



/*----------------------------------------------------------------------

  geschedule()

  Generate code to implement a SCHEDULE statement.

  */
static void geschedule(StmNod *stm) /* IN - Statement */
{
  emit0(C_CONST, stm->fields.schedule.when);

  /* NOTE: we can't use gewhr() because the semantics of the schedule */
  /* statement is such that at scheduling AT something does not mean */
  /* where that something is now but where it is when the event is run! */
  switch (stm->fields.schedule.whr->whr) {
  case WHR_DEFAULT:
  case WHR_HERE:
    emit0(C_CURVAR, V_CURLOC);
    break;
    
  case WHR_AT:
    gewht(stm->fields.schedule.whr->wht);
    break;

  default:
    unimpl(&stm->srcp, "Code Generator");
    return;
  }
  genam(stm->fields.schedule.nam);
  emit0(C_STMOP, I_SCHEDULE);
}


/*----------------------------------------------------------------------

  gecancel()

  Generate code to implement the CANCEL statement.

  */
static void gecancel(StmNod *stm) /* IN - Statement to generate */
{
  genam(stm->fields.schedule.nam);
  emit0(C_STMOP, I_CANCEL);
}

/*----------------------------------------------------------------------

  geif()

  Generate code to implement a IF statement.

  */
static void geif(StmNod *stm,	/* IN - Statement */
		 ActNod *act)	/* IN - Inside any actor */
{
  geexp(stm->fields.iff.exp);
  emit0(C_STMOP, I_IF);
  gestms(stm->fields.iff.thn, act);
  if (stm->fields.iff.els != NULL) {
    emit0(C_STMOP, I_ELSE);
    gestms(stm->fields.iff.els, act);
  }
  emit0(C_STMOP, I_ENDIF);
}



/*----------------------------------------------------------------------

  geuse()

  Generate USE statement.

  */
static void geuse(StmNod *stm, ActNod *act) /* IN - Statement */
{
  if (stm->fields.use.actor == NULL) { /* No actor specified, use current */
    emit0(C_CONST, stm->fields.use.scriptno);
    genam(act->nam);
    emit0(C_STMOP, I_USE);
  } else {
    emit0(C_CONST, stm->fields.use.scriptno);
    genam(stm->fields.use.actor);
    emit0(C_STMOP, I_USE);
  }
}



/*----------------------------------------------------------------------

  gesystem()

  Generate SYSTEM statement.

  */
static void gesystem(StmNod *stm, ActNod *act) /* IN - Statement */
{
  encode(&stm->fields.system.fpos, &stm->fields.system.len);
  emit0(C_CONST, stm->fields.system.len);
  emit0(C_CONST, stm->fields.system.fpos);
  emit0(C_STMOP, I_SYSTEM);
}



/*----------------------------------------------------------------------

  gestm()

  Generate code for one statement.

  */
static void gestm(StmNod *stm,	/* IN - The statement to generate */
		  ActNod *act)	/* IN - Inside actor? */
{
  switch (stm->class) {

  case STM_NOP:
    break;

  case STM_PRINT:
    geprint(stm);
    break;

  case STM_QUIT:
    emit0(C_STMOP, I_QUIT);
    break;

  case STM_LOOK:
    emit0(C_STMOP, I_LOOK);
    break;

  case STM_SAVE:
    emit0(C_STMOP, I_SAVE);
    break;

  case STM_RESTORE:
    emit0(C_STMOP, I_RESTORE);
    break;

  case STM_RESTART:
    emit0(C_STMOP, I_RESTART);
    break;

  case STM_VISITS:
    emit0(C_CONST, stm->fields.visits.count);
    emit0(C_STMOP, I_VISITS);
    break;

  case STM_SCORE:
    gescore(stm);
    break;

  case STM_DESCRIBE:
    gedescribe(stm);
    break;

  case STM_SAY:
    gesay(stm);
    break;

  case STM_LIST:
    gelist(stm);
    break;

  case STM_EMPTY:
    geempty(stm);
    break;

  case STM_LOCATE:
    gelocate(stm);
    break;

  case STM_MAKE:
    gemake(stm);
    break;

  case STM_SET:
    geset(stm);
    break;

  case STM_INCR:
  case STM_DECR:
    geincr(stm);
    break;

  case STM_SCHEDULE:
    geschedule(stm);
    break;

  case STM_CANCEL:
    gecancel(stm);
    break;

  case STM_IF:
    geif(stm, act);
    break;

  case STM_USE:
    geuse(stm, act);
    break;

  case STM_SYSTEM:
    gesystem(stm, act);
    break;

  default:
    unimpl(&stm->srcp, "Code Generator");
    break;
  }
}


/*======================================================================

  gestms()

  Generate code for all the statements in a list.

  */
void gestms(List *stms,		/* IN - The statements to generate */
	    ActNod *act)	/* IN - Inside any actor */
{
  while (stms != NULL) {
    gestm(stms->element.stm, act);
    stms = stms->next;
  }
}



/*======================================================================

  dustm()

  Dump a statement node.

  */
void dustm(StmNod *stm)
{
  if (stm == NULL) {
    put("NULL");
    return;
  }

  put("STM: ");
  switch(stm->class) {
  case STM_PRINT:
    put("PRINT ");
    break;
  case STM_DESCRIBE:
    put("DESCRIBE ");
    break;
  case STM_SAY:
    put("SAY ");
    break;
  case STM_LIST:
    put("LIST ");
    break;
  case STM_IF:
    put("IF ");
    break;
  case STM_MAKE:
    put("MAKE ");
    break;
  case STM_SET:
    put("SET ");
    break;
  case STM_INCR:
    put("INCR ");
    break;
  case STM_DECR:
    put("DECR ");
    break;
  case STM_LOCATE:
    put("LOCATE ");
    break;
  case STM_EMPTY:
    put("EMPTY ");
    break;
  case STM_SCHEDULE:
    put("SCHEDULE ");
    break;
  case STM_CANCEL:
    put("CANCEL ");
    break;
  case STM_LOOK:
    put("LOOK ");
    break;
  case STM_QUIT:
    put("QUIT ");
    break;
  case STM_SCORE:
    put("SCORE ");
    break;
  case STM_USE:
    put("USE ");
    break;
  case STM_SAVE:
    put("SAVE ");
    break;
  case STM_RESTORE:
    put("RESTORE ");
    break;
  case STM_RESTART:
    put("RESTART ");
    break;
  case STM_VISITS:
    put("VISITS ");
    break;
  default:
    put("*** UNKNOWN ***");
    break;
  }
  dusrcp(&stm->srcp);

  switch(stm->class) {
  case STM_LOOK:
  case STM_QUIT:
  case STM_SAVE:
  case STM_RESTORE:
  case STM_RESTART:
    break;
  default:
    in();
    switch(stm->class) {
    case STM_PRINT:
      put("fpos: "); duint(stm->fields.print.fpos); nl();
      put("len: "); duint(stm->fields.print.len);
      break;
    case STM_SCORE:
      put("count: "); duint(stm->fields.score.count); nl();
      put("score: "); duint(stm->fields.score.score);
      break;
    case STM_DESCRIBE:
      put("wht: "); duwht(stm->fields.describe.wht);
      break;
    case STM_SAY:
      put("exp: "); duexp(stm->fields.say.exp);
      break;
    case STM_LIST:
      put("wht: "); duwht(stm->fields.list.wht);
      break;
    case STM_EMPTY:
      put("wht: "); duwht(stm->fields.empty.wht); nl();
      put("whr: "); duwhr(stm->fields.empty.whr);
      break;
    case STM_LOCATE:
      put("wht: "); duwht(stm->fields.locate.wht); nl();
      put("whr: "); duwhr(stm->fields.locate.whr);
      break;
    case STM_MAKE:
      put("wht: "); duwht(stm->fields.list.wht); nl();
      put("not: "); duBool(stm->fields.make.not); nl();
      put("atr: "); dunam(stm->fields.make.atr);
      break;
    case STM_SET:
      put("wht: "); duwht(stm->fields.set.wht); nl();
      put("atr: "); dunam(stm->fields.set.atr); nl();
      put("exp: "); duexp(stm->fields.set.exp);
      break;
    case STM_INCR:
    case STM_DECR:
      put("wht: "); duwht(stm->fields.incr.wht); nl();
      put("atr: "); dunam(stm->fields.incr.atr); nl();
      put("step: "); duexp(stm->fields.incr.step);
      break;
    case STM_SCHEDULE:
      put("nam: "); dunam(stm->fields.schedule.nam); nl();
      put("whr: "); duwhr(stm->fields.locate.whr); nl();
      put("when: "); duint(stm->fields.schedule.when);
      break;
    case STM_CANCEL:
      put("nam: "); dunam(stm->fields.cancel.nam);
      break;
    case STM_IF:
      put("exp: "); duexp(stm->fields.iff.exp); nl();
      put("thn: "); dulst(stm->fields.iff.thn, STMNOD); nl();
      put("els: "); dulst(stm->fields.iff.els, STMNOD);
      break;
    case STM_USE:
      put("script: "); dunam(stm->fields.use.script); nl();
      put("scriptno: "); duint(stm->fields.use.scriptno); nl();
      put("actor: "); dunam(stm->fields.use.actor);
      break;
    case STM_VISITS:
      put("count: "); duint(stm->fields.visits.count);
      break;
    default:
      break;
    }
    out();
  }
}
