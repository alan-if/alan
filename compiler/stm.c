/*----------------------------------------------------------------------*\

                                STM.C
                                Statement Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "whr_x.h"
#include "atr_x.h"
#include "lst_x.h"

#include "lmList.h"

#include "adv.h"                /* ADV-node */
#include "exp.h"                /* EXP-nodes */
#include "wht.h"                /* WHT-nodes */
#include "stm.h"                /* STM-nodes */
#include "scr.h"                /* SCR-nodes */
#include "ins.h"                /* INS-nodes */
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

  switch (stm->fields.describe.wht->kind) {
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
    sym = symcheck(stm->fields.describe.wht->id, INSTANCE_SYMBOL, pars);
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
#ifndef FIXME
  syserr("UNIMPL: anlist() - cntcheck");
#else
  cntcheck(stm->fields.list.wht, pars);
#endif
}


/*----------------------------------------------------------------------

  anempty()

  Analyze an EMPTY statement.

  */
static void anempty(StmNod *stm, /* IN - The statement to analyze */
		    EvtNod *evt, /* IN - Inside event? */
		    List *pars)	/* IN - Possible syntax parameters */
{
#ifndef FIXME
  syserr("UNIMPL: anempty() - cntcheck");
#else
  cntcheck(stm->fields.list.wht, pars);
#endif
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
  switch (stm->fields.locate.wht->kind) {
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
#ifndef FIXME
    syserr("UNIMPL: namcheck() in anlocate()");
#else
    namcheck(&sym, &elm, stm->fields.locate.wht->nam, NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
#endif    
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }

  anwhr(stm->fields.locate.whr, evt, pars);
  switch (stm->fields.locate.whr->kind) {
  case WHR_HERE:
  case WHR_AT:
    break;
  case WHR_IN:
    if (stm->fields.locate.wht->kind == WHT_ACT)
      lmLog(&stm->srcp, 402, sevERR, "");
    else if (stm->fields.locate.wht->kind == WHT_ID) {
#ifndef FIXME
      syserr("UNIMPL: anlocate()");
#else
      if (sym != NULL && sym->kind == NAMACT)
        lmLog(&stm->srcp, 402, sevERR, "");
      else if (elm != NULL && elm->res != NULL
	       && ((elm->res->classbits & NAMACT) != 0
		   || (elm->res->classbits & NAMCACT) != 0))
        lmLog(&stm->srcp, 402, sevERR, "");
#endif
    }
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

  switch (stm->fields.make.wht->kind) {
  case WHT_ACT:
    if (evt != NULL)
      lmLog(&stm->fields.make.wht->srcp, 412, sevERR, "");
    else {
      atr = findAttribute(NULL, stm->fields.make.atr);
      if (atr == NULL)          /* Attribute not found globally */
        lmLog(&stm->fields.make.atr->srcp, 404, sevERR, "ACTOR");
      else
        stm->fields.make.atr->code = atr->id->code;
    }
    break;
  case WHT_LOC:
    atr = findAttribute(NULL, stm->fields.make.atr);
    if (atr == NULL)            /* Attribute not found globally */
      lmLog(&stm->fields.make.atr->srcp, 404, sevERR, "LOCATION");
    else
      stm->fields.make.atr->code = atr->id->code;
    break;
  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&stm->fields.make.wht->srcp, 409, sevERR, "");
    atr = findAttribute(NULL, stm->fields.make.atr);
    if (atr == NULL)            /* Attribute not found globally */
      lmLog(&stm->fields.make.atr->srcp, 404, sevERR, "OBJECT");
    else
      stm->fields.make.atr->code = atr->id->code;
    break;
  case WHT_ID:
    atr = resolveAttributeReference(stm->fields.make.wht, stm->fields.make.atr);
    if (atr != NULL) {
      if (atr->typ != TYPBOOL)
        lmLog(&stm->fields.make.atr->srcp, 408, sevERR, "MAKE statement");
      else
        stm->fields.make.atr->code = atr->id->code;
    }
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
  AtrNod *atr;

  switch (stm->fields.set.wht->kind) {
  case WHT_ACT:
    if (evt != NULL)
      lmLog(&stm->fields.set.wht->srcp, 412, sevERR, "");
    else {
      atr = findAttribute(NULL, stm->fields.set.atr);
      if (atr == NULL)          /* attribute not found globally */
        lmLog(&stm->fields.set.atr->srcp, 404, sevERR, "ACTOR");
      else
        stm->fields.set.atr->code = atr->id->code;
    }
    break;

  case WHT_LOC:
    atr = findAttribute(NULL, stm->fields.set.atr);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.set.atr->srcp, 404, sevERR, "LOCATION");
    else
      stm->fields.set.atr->code = atr->id->code;
    break;

  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&stm->fields.set.wht->srcp, 409, sevERR, "");
    atr = findAttribute(NULL, stm->fields.set.atr);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.set.atr->srcp, 404, sevERR, "OBJECT");
    else
      stm->fields.set.atr->code = atr->id->code;
    break;

  case WHT_ID:
#ifdef FIXME
    namcheck(&sym, &elm, stm->fields.set.wht->nam, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT,
             NAMANY, pars);
    if (elm) {
      atr = paramatr(stm->fields.set.atr, elm);
      if (atr == NULL)          /* Not a default attribute */
        lmLog(&stm->fields.set.atr->srcp, 404, sevERR, "a parameter");
    } else if (sym) {
      atr = symatr(stm->fields.set.atr, sym);
      if (atr == NULL)
        lmLog(&stm->fields.set.atr->srcp, 315, sevERR,
              stm->fields.set.wht->id->string);
    }
#endif
    atr = resolveAttributeReference(stm->fields.set.wht, stm->fields.set.atr);
    if (atr) {
      if (atr->typ != TYPINT && atr->typ != TYPSTR)
        lmLog(&stm->fields.set.atr->srcp, 419, sevERR, "Target for");
      else
        stm->fields.set.atr->code = atr->id->code;
    }
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

  switch (stm->fields.incr.wht->kind) {
  case WHT_ACT:
    if (evt != NULL)
      lmLog(&stm->fields.incr.wht->srcp, 412, sevERR, "");
    else {
      atr = findAttribute(NULL, stm->fields.incr.atr);
      if (atr == NULL)          /* attribute not found globally */
        lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "ACTOR");
      else
        stm->fields.incr.atr->code = atr->id->code;
    }
    break;
  case WHT_LOC:
    atr = findAttribute(NULL, stm->fields.incr.atr);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "LOCATION");
    else
      stm->fields.incr.atr->code = atr->id->code;
    break;
  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&stm->fields.incr.wht->srcp, 409, sevERR, "");
    atr = findAttribute(NULL, stm->fields.incr.atr);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "OBJECT");
    else
      stm->fields.incr.atr->code = atr->id->code;
    break;
  case WHT_ID:
    atr = resolveAttributeReference(stm->fields.incr.wht, stm->fields.incr.atr);
    if (atr) {
      if (atr->typ != TYPINT)
        lmLog(&stm->fields.incr.atr->srcp, 413, sevERR, "INCREASE/DECREASE");
      else
        stm->fields.incr.atr->code = atr->id->code;
    }
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

  sym = symcheck(stm->fields.schedule.id, EVENT_ID, NULL);

  /* Now lookup where */
  anwhr(stm->fields.schedule.whr, evt, pars);
  switch (stm->fields.schedule.whr->kind) {
  case WHR_DEFAULT:
    stm->fields.schedule.whr->kind = WHR_HERE;
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

  /* Analyze the when expression */
  anexp(stm->fields.schedule.when, evt, pars);
  if (stm->fields.schedule.when->typ != TYPINT)
    lmLog(&stm->fields.schedule.when->srcp, 413, sevERR, "when-clause of SCHEDULE statement");

}


/*----------------------------------------------------------------------

  ancancel()

  Analyze a CANCEL statement.

  */
static void ancancel(StmNod *stm) /* IN - The statement to analyze */
{
  SymNod *sym;

  sym = symcheck(stm->fields.cancel.id, EVENT_ID, NULL);
}


/*----------------------------------------------------------------------

  anif()

  Analyze an IF statement.

  */
static void anif(StmNod *stm,	/* IN - The statement to analyze */
		 InsNod *ins,	/* IN - Possibly inside Instance */
		 EvtNod *evt,	/* IN - Possibly inside Event */
		 List *pars)	/* IN - Possible syntax parameters */
{
  anexp(stm->fields.iff.exp, evt, pars);
  if (!eqtyp(stm->fields.iff.exp->typ, TYPBOOL))
    lmLogv(&stm->fields.iff.exp->srcp, 330, sevERR, "boolean", "'IF'", NULL);
  anstms(stm->fields.iff.thn, ins, evt, pars);
  if (stm->fields.iff.els != NULL)
    anstms(stm->fields.iff.els, ins, evt, pars);
}


/*----------------------------------------------------------------------

  anuse()

  Analyze a USE statement. It must refer a script that is defined
  within the mentioned actor. If the actor is not specified the
  actor is assumed to be the one we are in (it is an error if we are
  not).

  */
static void anuse(StmNod *stm,	/* IN - Statement to analyze */
		  InsNod *ins,	/* IN - Possibly inside Instance */
		  List *pars)	/* IN - Possible syntax parameters */
{
  SymNod *sym;
  ElmNod *elm;
  List *lst;

  if (stm->fields.use.actor == NULL && ins == NULL)
    lmLog(&stm->srcp, 401, sevERR, "");
  else {
    if (stm->fields.use.actor != NULL) {
      /* Lookup actors node */
      sym = symcheck(stm->fields.use.actor, INSTANCE_SYMBOL, pars);
      ins = NULL;
      if (elm)
        lmLog(&stm->fields.use.actor->srcp, 410, sevERR, "USE statement");
      else if (sym)
        /* FIXME : ins = sym->ref */;
    }
    if (ins != NULL && ins->slots != NULL) {

      /* Loop over actors scripts to check if script is defined */
      for (lst = ins->slots->scrs; lst != NULL; lst = lst->next) {
        if (stm->fields.use.script != NULL) {
          /* A name was used as reference */
          if (lst->element.scr->id != NULL &&
	      equalId(lst->element.scr->id, stm->fields.use.script)) {
	    stm->fields.use.scriptno = lst->element.scr->code;
	    break;		/* Found it so break loop */
	  }
	} else {
	  /* A number was used */
	  if (lst->element.scr->code == stm->fields.use.scriptno)
	    break;		/* Found it so break loop */
	}
      }
      if (lst == NULL) {
        if (stm->fields.use.script != NULL)
          lmLog(&stm->fields.use.script->srcp, 400, sevERR, ins->slots->id->string);
        else
          lmLog(&stm->srcp, 400, sevERR, ins->slots->id->string);
      }
    }
  }
}  


/*----------------------------------------------------------------------

  andep()

  Analyze a DEPENDING statement. It has partial expressions in the
  cases which must be connected to the depend expression.

  */
static void andep(StmNod *stm,	/* IN - Statement to analyze */
		  InsNod *ins,	/* IN - Possibly inside Instance */
		  List *pars)	/* IN - Possible syntax parameters */
{
  List *cases;

 /* The expression will be analysed once for each case so no need to
    do this separately, is there?

    4f - performance may be somewhat improved by not re-analyze the
    expression for every case => some indication of an anlyzed
    expression must be available (the type?) in the expressions nodes */

  for (cases = stm->fields.depend.cases; cases != NULL; cases =
	 cases->next) {

    if (cases->element.stm->fields.depcase.exp != NULL) {
      /* Unless it is an ELSE clause set left hand of case expression
         to be the depend expression */
      switch (cases->element.stm->fields.depcase.exp->class) {
      case EXPBIN:
	cases->element.stm->fields.depcase.exp->fields.bin.left =
	  stm->fields.depend.exp;
	break;
      case EXPWHR:
	cases->element.stm->fields.depcase.exp->fields.whr.wht =
	  stm->fields.depend.exp;
	break;
      case EXPATR:
	cases->element.stm->fields.depcase.exp->fields.atr.wht =
	  stm->fields.depend.exp;
	break;
      case EXPBTW:
	cases->element.stm->fields.depcase.exp->fields.btw.val =
	  stm->fields.depend.exp;
	break;
      default:
	syserr("andep(): Unrecognized switch case on expkd.");
      }
    } else
      /* If this is an ELSE-case there can not be any other afterwards */
      if (cases->next != NULL)
	lmLog(&cases->element.stm->srcp, 335, sevERR, "");	

    /* Analyze the expression and the statements */
    anexp(cases->element.stm->fields.depcase.exp, NULL, pars);
    anstms(cases->element.stm->fields.depcase.stms, ins, NULL, pars);

  }
}



/*----------------------------------------------------------------------

  anstm()

  Analyze one statement.

  */
static void anstm(StmNod *stm,	/* IN - The statement to analyze */
		  InsNod *ins,	/* IN - Possibly inside Instance */
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
    anif(stm, ins, evt, pars);
    break;
  case STM_USE:
    anuse(stm, ins, pars);
    break;
  case STM_DEPEND:
    andep(stm, ins, pars);
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
	    InsNod *ins,	/* IN - Within Instance? */
	    EvtNod *evt,	/* IN - Within Event? */
	    List *pars)		/* IN - Possible syntax parameters */
{
  while (stms != NULL) {
    anstm(stms->element.stm, ins, evt, pars);
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
  switch (stm->fields.describe.wht->kind) {

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
    generateId(stm->fields.describe.wht->id);
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
  if (stm->fields.list.wht->kind == WHT_ID) {
    generateId(stm->fields.list.wht->id);
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
  if (stm->fields.empty.wht->kind == WHT_ID) {
    gewhr(stm->fields.empty.whr);
    generateId(stm->fields.empty.wht->id);
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
  geexp(stm->fields.schedule.when);

  /* NOTE: we can't use gewhr() because the semantics of the schedule */
  /* statement is such that at scheduling AT something does not mean */
  /* where that something is now but where it is when the event is run! */
  switch (stm->fields.schedule.whr->kind) {
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
  generateId(stm->fields.schedule.id);
  emit0(C_STMOP, I_SCHEDULE);
}


/*----------------------------------------------------------------------

  gecancel()

  Generate code to implement the CANCEL statement.

  */
static void gecancel(StmNod *stm) /* IN - Statement to generate */
{
  generateId(stm->fields.schedule.id);
  emit0(C_STMOP, I_CANCEL);
}

/*----------------------------------------------------------------------

  geif()

  Generate code to implement a IF statement.

  */
static void geif(StmNod *stm,	/* IN - Statement */
		 InsNod *ins)	/* IN - Inside any Instance */
{
  geexp(stm->fields.iff.exp);
  emit0(C_STMOP, I_IF);
  gestms(stm->fields.iff.thn, ins);
  if (stm->fields.iff.els != NULL) {
    emit0(C_STMOP, I_ELSE);
    gestms(stm->fields.iff.els, ins);
  }
  emit0(C_STMOP, I_ENDIF);
}



/*----------------------------------------------------------------------

  geuse()

  Generate USE statement.

  */
static void geuse(StmNod *stm, InsNod *ins) /* IN - Statement */
{
  if (stm->fields.use.actor == NULL) { /* No actor specified, use current */
    emit0(C_CONST, stm->fields.use.scriptno);
    generateId(ins->slots->id);
    emit0(C_STMOP, I_USE);
  } else {
    emit0(C_CONST, stm->fields.use.scriptno);
    generateId(stm->fields.use.actor);
    emit0(C_STMOP, I_USE);
  }
}



/*----------------------------------------------------------------------

  gedep()

  Generate DEPENDIN statement.

  4f - This is a bit non-optimal since the left hand side of the
  expression will be evaluated once for each case, but since the
  current code generation scheme for binary expressions generates the
  right hand expression first this is currently not possible without
  making the interpreter incompatible (but on the other hand this way
  is easier here)

  Code generation principle:

      DEPSTART

      DEPCASE--+
      exp1     |
      DEPEXEC   > repeat for each case
      stms1----+

      DEPELSE--+ optional
      stmsn----+

      DEPEND
  */
static void gedep(StmNod *stm, InsNod *ins) /* IN - Statement */
{
  List *cases;

  emit0(C_STMOP, I_DEPSTART);

  /* For each case: */
  for (cases = stm->fields.depend.cases; cases != NULL; cases =
	 cases->next) {
    /* If it is not the ELSE clause ... */
    if (cases->element.stm->fields.depcase.exp != NULL) {
      /* Generate a DEPCASE */
      emit0(C_STMOP, I_DEPCASE);
      /* ...and the expression */
      geexp(cases->element.stm->fields.depcase.exp);
      emit0(C_STMOP, I_DEPEXEC);
    } else
      emit0(C_STMOP, I_DEPELSE);
    /* ...and then the statments */
    gestms(cases->element.stm->fields.depcase.stms, ins);
  }
  emit0(C_STMOP, I_DEPEND);
}



/*----------------------------------------------------------------------

  gesystem()

  Generate SYSTEM statement.

  */
static void gesystem(StmNod *stm, InsNod *ins) /* IN - Statement */
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
		  InsNod *ins)	/* IN - Inside actor? */
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
    geif(stm, ins);
    break;

  case STM_USE:
    geuse(stm, ins);
    break;

  case STM_DEPEND:
    gedep(stm, ins);
    break;

  case STM_SYSTEM:
    gesystem(stm, ins);
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
	    InsNod *ins)	/* IN - Inside any Instance? */
{
  while (stms != NULL) {
    gestm(stms->element.stm, ins);
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
  dumpSrcp(&stm->srcp);

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
      put("fpos: "); dumpInt(stm->fields.print.fpos); nl();
      put("len: "); dumpInt(stm->fields.print.len);
      break;
    case STM_SCORE:
      put("count: "); dumpInt(stm->fields.score.count); nl();
      put("score: "); dumpInt(stm->fields.score.score);
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
      put("not: "); dumpBool(stm->fields.make.not); nl();
      put("atr: "); dumpId(stm->fields.make.atr);
      break;
    case STM_SET:
      put("wht: "); duwht(stm->fields.set.wht); nl();
      put("atr: "); dumpId(stm->fields.set.atr); nl();
      put("exp: "); duexp(stm->fields.set.exp);
      break;
    case STM_INCR:
    case STM_DECR:
      put("wht: "); duwht(stm->fields.incr.wht); nl();
      put("atr: "); dumpId(stm->fields.incr.atr); nl();
      put("step: "); duexp(stm->fields.incr.step);
      break;
    case STM_SCHEDULE:
      put("id: "); dumpId(stm->fields.schedule.id); nl();
      put("whr: "); duwhr(stm->fields.locate.whr); nl();
      put("when: "); duexp(stm->fields.schedule.when);
      break;
    case STM_CANCEL:
      put("id: "); dumpId(stm->fields.cancel.id);
      break;
    case STM_IF:
      put("exp: "); duexp(stm->fields.iff.exp); nl();
      put("thn: "); dulst(stm->fields.iff.thn, LIST_STM); nl();
      put("els: "); dulst(stm->fields.iff.els, LIST_STM);
      break;
    case STM_USE:
      put("script: "); dumpId(stm->fields.use.script); nl();
      put("scriptno: "); dumpInt(stm->fields.use.scriptno); nl();
      put("actor: "); dumpId(stm->fields.use.actor);
      break;
    case STM_VISITS:
      put("count: "); dumpInt(stm->fields.visits.count);
      break;
    default:
      break;
    }
    out();
  }
}
