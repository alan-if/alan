/*----------------------------------------------------------------------*\

				EXP.C
			   Expression Nodes

\*----------------------------------------------------------------------*/

#include "util.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "exp.h"		/* EXP-nodes */
#include "atr.h"		/* ATR-nodes */
#include "whr.h"		/* WHR-nodes */
#include "wht.h"		/* WHT-nodes */
#include "id.h"		/* ID-nodes */
#include "elm.h"		/* ELM-nodes */
#include "ins.h"		/* INS-nodes */


#include "emit.h"

#include "acode.h"
#include "encode.h"

#include "dump.h"




/*======================================================================

  eqtyp()

  Check if two types are equal. If one is TYPUNK they are.

  */
Bool eqtyp(TypeKind typ1,	/* IN - types to compare */
	   TypeKind typ2)
{
  if (typ1 == TYPERR || typ2 == TYPERR) syserr("Unintialised type in eqtyp()");
  return (typ1 == TYPUNK || typ2 == TYPUNK || typ1 == typ2);
}



/*======================================================================

  newexp()

  Allocates and initialises an expnod.

 */
ExpNod *newexp(Srcp *srcp,	/* IN - Source Position */
	       ExpKind class)	/* IN - The expression class */
{
  ExpNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ExpNod);

  new->srcp = *srcp;
  new->class = class;
  new->not = FALSE;

  return(new);
}




/*----------------------------------------------------------------------
  anexpwhr()

  Analyze a WHR expression.

 */
static void anexpwhr(ExpNod *exp, /* IN - The expression to analyze */
		     EvtNod *evt, /* IN - Possibly inside event */
		     List *pars) /* IN - Possible parameters */
{
  SymNod *sym;
  ElmNod *elm;

  anexp(exp->fields.whr.wht, evt, pars);
  if (exp->fields.whr.wht->class != EXPWHT)
    lmLog(&exp->fields.whr.wht->srcp, 311, sevERR, "an Object or an Actor");
  else {
    switch (exp->fields.whr.wht->fields.wht.wht->wht) {
    case WHT_OBJ:
      if (pars == NULL)
	lmLog(&exp->fields.whr.wht->srcp, 409, sevERR, "");
      break;
    case WHT_ACT:
      if (evt != NULL)
	lmLog(&exp->fields.whr.wht->srcp, 412, sevERR, "");
      break;
    case WHT_LOC:
      lmLog(&exp->fields.whr.wht->srcp, 311, sevERR, "an Object or an Actor");
      break;
    case WHT_ID:
#ifndef FIXME
    syserr("UNIMPL: newevt() - symbol code handling");
#else
      idcheck(&sym, &elm, exp->fields.whr.wht->fields.wht.wht->id,
	       NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
    new->id->code = newsym(id->str, NAMEVT, new);
#endif
      break;
    default:
      syserr("Unrecognized switch in anwhr()");
      break;
    }
  }

  switch (exp->fields.whr.whr->whr) {
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHR_AT:
    switch (exp->fields.whr.whr->wht->wht) {
    case WHT_ID:
#ifndef FIXME
    syserr("UNIMPL: anexpwhr() - namcheck handling");
#else
      namcheck(&sym, &elm, exp->fields.whr.whr->wht->id, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
#endif
      break;
    case WHT_LOC:
      exp->fields.whr.whr->whr = WHR_HERE;
      break;
    case WHT_OBJ:
      if (pars == NULL)
	lmLog(&exp->fields.whr.whr->wht->srcp, 409, sevERR, "");
      break;
    case WHT_ACT:
    if (evt != NULL)
      lmLog(&exp->fields.whr.whr->srcp, 412, sevERR, "");
      break;
    default:
      syserr("Unrecognized switch in anexpwhr()");
      break;
    }
    break;
  case WHR_IN:
#ifndef FIXME
    syserr("UNIMPL: cntcheck");
#else
    cntcheck(exp->fields.whr.whr->wht, pars);
#endif
    break;
  default:
    syserr("Unrecognized switch in anexpwhr()");
    break;
  }

  exp->typ = TYPBOOL;
}



/*----------------------------------------------------------------------
  anatr()

  Analyze an ATR expression.

  */
static void anatr(ExpNod *exp,	/* IN - The expression to analyze */
		  EvtNod *evt,	/* IN - Possibly inside Event? */
		  List *pars)	/* IN - List of parameters */
{
  AtrNod *atr;
  SymNod *sym;			/* The symbol table node */
  ElmNod *elm;			/* A parameter element */

  if (exp->fields.atr.wht->class == EXPWHT) {
    switch (exp->fields.atr.wht->fields.wht.wht->wht) {
      
    case WHT_ACT:
      if (evt != NULL)
	lmLog(&exp->fields.atr.wht->fields.wht.wht->srcp, 412, sevERR, "");
      else {
	atr = findatr(exp->fields.atr.atr->string, adv.aatrs, adv.atrs);
	if (atr == NULL) {		/* attribute not found globally */
	  lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "ACTOR");
	  exp->typ = TYPUNK;
	} else {
	  exp->fields.atr.atr->symbol->code = atr->id->symbol->code;
	  exp->typ = atr->typ;
	}
      }
      break;

    case WHT_LOC:
      atr = findatr(exp->fields.atr.atr->string, adv.latrs, adv.atrs);
      if (atr == NULL) {		/* attribute not found globally */
	lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "LOCATION");
	exp->typ = TYPUNK;
      } else {
	exp->fields.atr.atr->symbol->code = atr->id->symbol->code;
	exp->typ = atr->typ;
      }
      break;

    case WHT_OBJ:
      if (pars == NULL)
	lmLog(&exp->fields.atr.wht->fields.wht.wht->srcp, 409, sevERR, "");
      atr = findatr(exp->fields.atr.atr->string, adv.oatrs, adv.atrs);
      if (atr == NULL) {		/* attribute not found globally */
	lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "OBJECT");
	exp->typ = TYPUNK;
      } else {
	exp->fields.atr.atr->symbol->code = atr->id->symbol->code;
	exp->typ = atr->typ;
      }
      break;

    case WHT_ID:
#ifndef FIXME
    syserr("UNIMPL: anatr() - namcheck handling");
#else
      namcheck(&sym, &elm, exp->fields.atr.wht->fields.wht.wht->id,
	       NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
#endif
      atr = NULL;
      if (elm) {
	/* It was an element, i.e. syntax parameter */
	atr = paramatr(exp->fields.atr.atr, elm);
	if (atr == NULL) {	/* Not a default attribute */
	  lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "a parameter");
	  exp->typ = TYPUNK;
	} else {
	  exp->fields.atr.atr->symbol->code = atr->id->symbol->code;
	  exp->typ = atr->typ;
	}
      } else if (sym) {
#ifndef FIXME
      syserr("UNIMPLEMENTED - anatr() : attribute to identifier");
#else
	switch (sym->class) {
	case NAMLOC:
	  atr = findatr(exp->fields.atr.atr->str, ((LocNod *)sym->ref)->atrs, adv.latrs);
	  break;
	case NAMOBJ:
	  atr = findatr(exp->fields.atr.atr->str, ((ObjNod *)sym->ref)->atrs, adv.oatrs);
	  break;
	case NAMACT:
	  atr = findatr(exp->fields.atr.atr->str, ((ActNod *)sym->ref)->atrs, adv.aatrs);
	  break;
	default:
	  break;
	}
#endif
	if (atr == NULL) {	/* Attribute not found locally */
	  /* Try general default attributes */
	  if ((atr = findatr(exp->fields.atr.atr->string, adv.atrs, NULL)) == NULL) {
	    /* Still didn't find it */
	    lmLog(&exp->fields.atr.atr->srcp, 315, sevERR,
		  exp->fields.atr.wht->fields.wht.wht->id->string);
	    exp->typ = TYPUNK;
	  }
	}
      }
      if (atr != NULL) {
	exp->fields.atr.atr->symbol->code = atr->id->symbol->code;
	exp->typ = atr->typ;
      } else
	exp->typ = TYPUNK;
      break;

    default:
      syserr("Unrecognized switch in anatr()");
      break;
    }
  } else
    lmLog(&exp->srcp, 420, sevERR, "attribute reference");
}


/*----------------------------------------------------------------------
  anbin()

  Analyze a binary expression and find out its type.

 */
static void anbin(ExpNod *exp,                 
		  EvtNod *evt,	/* IN - Possibly inside event */
		  List *pars)	/* IN - List of parameters available */
{
  anexp(exp->fields.bin.left, evt, pars);
  anexp(exp->fields.bin.right, evt, pars);

  switch (exp->fields.bin.op) {
  case OP_AND:
  case OP_OR:
    if (!eqtyp(exp->fields.bin.left->typ, TYPBOOL))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "boolean", "AND/OR", NULL);
    if (!eqtyp(exp->fields.bin.right->typ, TYPBOOL))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "boolean", "AND/OR", NULL);
    exp->typ = TYPBOOL;
    break;

  case OP_NE:
  case OP_EQ:
    if (!eqtyp(exp->fields.bin.left->typ, exp->fields.bin.right->typ))
      lmLog(&exp->srcp, 331, sevERR, "expression");
    else if (exp->fields.bin.left->typ != TYPUNK && exp->fields.bin.right->typ != TYPUNK)
      if (exp->fields.bin.left->typ == TYPENT) {
	if (exp->fields.bin.left->fields.wht.wht->wht == WHT_ID &&
	    exp->fields.bin.right->fields.wht.wht->wht == WHT_ID)
#ifndef FIXME
	  syserr("UNIMPL: anbin() - parameter type");
#else
	  if (exp->fields.bin.left->fields.wht.wht->id->kind != NAMPAR
	      && exp->fields.bin.right->fields.wht.wht->id->kind != NAMPAR)
#endif
	    lmLog(&exp->srcp, 417, sevINF, NULL);
      }
    exp->typ = TYPBOOL;
    break;

  case OP_EXACT:
    if (!eqtyp(exp->fields.bin.left->typ, TYPSTR))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "string", "'=='", NULL);
    if (!eqtyp(exp->fields.bin.right->typ, TYPSTR))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "string", "'=='", NULL);
    break;
	    
  case OP_LE:
  case OP_GE:
  case OP_LT:
  case OP_GT:
    if (!eqtyp(exp->fields.bin.left->typ, TYPINT))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "integer", "relational", NULL);
    if (!eqtyp(exp->fields.bin.right->typ, TYPINT))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "integer", "relational", NULL);
    exp->typ = TYPBOOL;
    break;

  case OP_PLUS:
  case OP_MINUS:
  case OP_MULT:
  case OP_DIV:
    if (!eqtyp(exp->fields.bin.left->typ, TYPINT))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "integer", "arithmetic", NULL);
    if (!eqtyp(exp->fields.bin.right->typ, TYPINT))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "integer", "arithmetic", NULL);
    exp->typ = TYPINT;
    break;

  case OP_CONTAINS:
    if (!eqtyp(exp->fields.bin.left->typ, TYPSTR))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "string", "'CONTAINS'", NULL);
    if (!eqtyp(exp->fields.bin.right->typ, TYPSTR))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "string", "'CONTAINS'", NULL);
    exp->typ = TYPBOOL;
    break;

  default:
    syserr("unrecognized binary operator in anbin()");
    break;    
  }
}



/*----------------------------------------------------------------------
  anagr()

  Analyze an aggregate expression.

 */
static void anagr(ExpNod *exp,	/* IN - The expression to analyze */
		  EvtNod *evt,	/* IN - Possibly inside Event? */
		  List *pars)
{
  AtrNod *atr = NULL;

  exp->typ = TYPINT;
  if (exp->fields.agr.agr != AGR_COUNT) {
    atr = findatr(exp->fields.agr.atr->string, adv.oatrs, adv.atrs);
    if (atr == NULL) {		/* attribute not found globally */
      lmLog(&exp->fields.agr.atr->srcp, 404, sevERR,
	    "OBJECT in aggregate expression");
      exp->typ = TYPUNK;
    } else if (!eqtyp(TYPINT, atr->typ)) {
      lmLog(&exp->fields.agr.atr->srcp, 418, sevERR, "");
      exp->typ = TYPUNK;
    } else
      exp->fields.agr.atr->symbol->code = atr->id->symbol->code;
  }

  anwhr(exp->fields.agr.whr, evt, pars);
}


/*----------------------------------------------------------------------
  anrnd()

  Analyse a random expression.

  */
static void anrnd(ExpNod *exp,	/* IN - Expression to analyse */
		  EvtNod *evt,	/* IN - Possibly inside Event? */
		  List *pars)
{
  exp->typ = TYPINT;
  anexp(exp->fields.rnd.from, evt, pars);
  if (!eqtyp(TYPINT, exp->fields.rnd.from->typ)) {
    lmLog(&exp->fields.rnd.from->srcp, 413, sevERR, "RANDOM");
    exp->typ = TYPUNK;
  }

  anexp(exp->fields.rnd.to, evt, pars);
  if (!eqtyp(TYPINT, exp->fields.rnd.to->typ)) {
    lmLog(&exp->fields.rnd.to->srcp, 413, sevERR, "RANDOM");
    exp->typ = TYPUNK;
  }
}


/*----------------------------------------------------------------------

  anexpwht()

  Analyse a WHT expression.

  */
static void anexpwht(ExpNod *exp, /* IN - Expression to analyse */
		     EvtNod *evt, /* IN - Possibly inside Event? */
		     List *pars) /* IN - Possible parameter list in this context */
{
  ElmNod *par;
  SymNod *sym;

  switch (exp->fields.wht.wht->wht) {
  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&exp->fields.wht.wht->srcp, 409, sevERR, "");
    exp->typ = TYPENT;
    break;

  case WHT_LOC:
    exp->typ = TYPENT;
    break;

  case WHT_ACT:
    if (evt != NULL)
      lmLog(&exp->fields.wht.wht->srcp, 412, sevERR, "");
    exp->typ = TYPENT;
    break;

  case WHT_ID:
#ifndef FIXME
    syserr("UNIMPL: anexpwht() - namcheck");
#else
    namcheck(&sym, &par, exp->fields.wht.wht->id, NAMACT+NAMOBJ+NAMCOBJ+NAMCACT+NAMLOC+NAMCNT+NAMNUM+NAMSTR, NAMANY, pars);
    if (par) {
      if (par->res)
	if (par->res->classbits & NAMNUM)
	  exp->typ = TYPINT;
	else if (par->res->classbits & NAMSTR)
	  exp->typ = TYPSTR;
        else
	  exp->typ = TYPENT;
      else
	exp->typ = TYPENT;
    } else if (sym && sym->class != NAMUNK)
      exp->typ = TYPENT;
    else
#endif
      exp->typ = TYPUNK;
    break;

  default:
    syserr("Unrecognized switch in anexpwht()");
    break;
  }
}


/*----------------------------------------------------------------------

  anexpbtw()

  Analyse a BTW expression.

  */
static void anexpbtw(ExpNod *exp, /* IN - Expression to analyse */
		     EvtNod *evt, /* IN - Possibly inside Event? */
		     List *pars) /* IN - Possible parameter list in this context */
{
  anexp(exp->fields.btw.val, evt, pars);
  if (!eqtyp(exp->fields.btw.val->typ, TYPINT))
    lmLogv(&exp->fields.btw.val->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  anexp(exp->fields.btw.low, evt, pars);
  if (!eqtyp(exp->fields.btw.low->typ, TYPINT))
    lmLogv(&exp->fields.btw.low->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  anexp(exp->fields.btw.high, evt, pars);
  if (!eqtyp(exp->fields.btw.high->typ, TYPINT))
    lmLogv(&exp->fields.btw.high->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  exp->typ = TYPBOOL;
}


/*======================================================================

  anexp()

  Analyze one expression.

  */
void anexp(ExpNod *exp,		/* IN - The expression to analyze */
	   EvtNod *evt,		/* IN - Possibly inside event */
	   List *pars)		/* IN - Possible verb parameters */
{
  if (exp == NULL) return;	/* Ignore empty expressions (syntax error) */
  
  switch (exp->class) {
    
  case EXPWHR:
    anexpwhr(exp, evt, pars);
    break;
    
  case EXPATR:
    anatr(exp, evt, pars);
    break;
    
  case EXPBIN:
    anbin(exp, evt, pars);
    break;
    
  case EXPINT:
    exp->typ = TYPINT;
    break;
    
  case EXPSTR:
    exp->typ = TYPSTR;
    break;
    
  case EXPAGR:
    anagr(exp, evt, pars);
    break;
    
  case EXPRND:
    anrnd(exp, evt, pars);
    break;

  case EXPSCORE:
    exp->typ = TYPINT;
    break;

  case EXPWHT:
    anexpwht(exp, evt, pars);
    break;

  case EXPBTW:
    anexpbtw(exp, evt, pars);
    break;

  case EXPISA:
    /* FIXME unimplemented */
    unimpl(&exp->srcp, "analyzer");
    break;

  default:
    syserr("Unrecognized switch in anexp()");
    break;
  }
}



/*----------------------------------------------------------------------
  geexpbin()

  Generate a binary expression.

  */
static void geexpbin(ExpNod *exp) /* IN - Expression node */
{
  /* 4f - This is actually a non-intutive order since it would have
     been more natural do start with the left expression. Changing
     this will make the interpreter incompatible, but would enable a
     more efficient evaluation of the DEPEND statement since then we
     could evaluate the left expression only once and then DUP the
     result once for every CASE */
  geexp(exp->fields.bin.left);
  geexp(exp->fields.bin.right);
  switch (exp->fields.bin.op) {
  case OP_AND:
    emit0(C_STMOP, I_AND);
    break;
  case OP_OR:
    emit0(C_STMOP, I_OR);
    break;
  case OP_NE:
    emit0(C_STMOP, I_NE);
    break;
  case OP_EQ:
    if (exp->fields.bin.right->typ == TYPSTR)
      emit0(C_STMOP, I_STREQ);
    else
      emit0(C_STMOP, I_EQ);
    break;
  case OP_EXACT:
    emit0(C_STMOP, I_STREXACT);
    break;
  case OP_LE:
    emit0(C_STMOP, I_LE);
    break;
  case OP_GE:
    emit0(C_STMOP, I_GE);
    break;
  case OP_LT:
    emit0(C_STMOP, I_LT);
    break;
  case OP_GT:
    emit0(C_STMOP, I_GT);
    break;
  case OP_PLUS:
    emit0(C_STMOP, I_PLUS);
    break;
  case OP_MINUS:
    emit0(C_STMOP, I_MINUS);
    break;
  case OP_MULT:
    emit0(C_STMOP, I_MULT);
    break;
  case OP_DIV:
    emit0(C_STMOP, I_DIV);
    break;
  case OP_CONTAINS:
    emit0(C_STMOP, I_CONTAINS);
    break;
  }
  if (exp->not) emit0(C_STMOP, I_NOT);
}



/*----------------------------------------------------------------------
  geexpwhr()

  Generate a where-expression.

  */
static void geexpwhr(ExpNod *exp) /* IN - Expression node */
{
  switch(exp->fields.whr.wht->fields.wht.wht->wht) {
    
  case WHT_OBJ:
    switch (exp->fields.whr.whr->whr) {
    case WHR_NEAR:
      emit0(C_CONST, 1);
      emit0(C_CURVAR, V_PARAM);
      emit0(C_STMOP, I_NEAR);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_HERE:
      emit0(C_CONST, 1);
      emit0(C_CURVAR, V_PARAM);
      emit0(C_STMOP, I_HERE);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_IN:
      geid(exp->fields.whr.whr->wht->id);
      emit0(C_CONST, 1);
      emit0(C_CURVAR, V_PARAM);
      emit0(C_STMOP, I_IN);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_AT:
      geid(exp->fields.whr.whr->wht->id);
      emit0(C_STMOP, I_WHERE);
      break;
    default:
      unimpl(&exp->srcp, "Code Generator");
      emit0(C_CONST, 0);
      return;
    }
    break;
    
  case WHT_ID:
    switch (exp->fields.whr.whr->whr) {
    case WHR_HERE:
      geid(exp->fields.whr.wht->fields.wht.wht->id);
      emit0(C_STMOP, I_HERE);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_NEAR:
      gewht(exp->fields.whr.wht->fields.wht.wht);
      emit0(C_STMOP, I_NEAR);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_IN:
      geid(exp->fields.whr.whr->wht->id);
      gewht(exp->fields.whr.wht->fields.wht.wht);
      emit0(C_STMOP, I_IN);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_AT:
      gewht(exp->fields.whr.wht->fields.wht.wht);
      emit0(C_STMOP, I_WHERE);
      break;
    default:
      unimpl(&exp->srcp, "Code Generator");
      emit0(C_CONST, 0);
      return;
    }
    break;
    
  default:
    unimpl(&exp->srcp, "Code Generator");
    emit0(C_CONST, 0);
    return;
  }
  
  gewhr(exp->fields.whr.whr);
  emit0(C_STMOP, I_EQ);
  if (exp->not) emit0(C_STMOP, I_NOT);
}





/*----------------------------------------------------------------------
  geexpatr()

  Generate an attribute-expression.

 */
static void geexpatr(ExpNod *exp) /* IN - Expression node */
{
  emit0(C_CONST, exp->fields.atr.atr->symbol->code);
  gewht(exp->fields.atr.wht->fields.wht.wht);
  if (exp->typ == TYPSTR)
    emit0(C_STMOP, I_STRATTR);
  else
    emit0(C_STMOP, I_ATTRIBUTE);
  if (exp->not) emit0(C_STMOP, I_NOT);
}




/*----------------------------------------------------------------------
  geexpagr()

  Generate the code for an aggregate expression.

  */
static void geexpagr(ExpNod *exp) /* IN - The expression to generate */
{
  gewhr(exp->fields.agr.whr);

  if (exp->fields.agr.agr != AGR_COUNT)
    emit0(C_CONST, exp->fields.agr.atr->symbol->code);

  switch (exp->fields.agr.agr) {
  case AGR_SUM: emit0(C_STMOP, I_SUM); break;
  case AGR_MAX: emit0(C_STMOP, I_MAX); break;
  case AGR_COUNT: emit0(C_STMOP, I_COUNT); break;
  default: syserr("Unrecognized switch in geexpagr()");
  }
}



/*----------------------------------------------------------------------
  geexprnd()

  Generate code for a random expression.

  */
static void geexprnd(ExpNod *exp) /* IN - The expression to generate */
{
  geexp(exp->fields.rnd.from);
  geexp(exp->fields.rnd.to);
  emit0(C_STMOP, I_RND);
}



/*----------------------------------------------------------------------
  geexpscore()

  Generate the code for a SCORE expression.

  */
static void geexpscore(ExpNod *exp) /* IN - The expression to generate */
{
  emit0(C_CURVAR, V_SCORE);
}



/*----------------------------------------------------------------------
  geexpwht()

  Generate the code for a WHAT expression.

  */
static void geexpwht(ExpNod *exp) /* IN - The expression to generate */
{
  gewht(exp->fields.wht.wht);
}



/*----------------------------------------------------------------------
  geexpbtw()

  Generate code for a random expression.

  */
static void geexpbtw(ExpNod *exp) /* IN - The expression to generate */
{
  geexp(exp->fields.btw.val);
  geexp(exp->fields.btw.low);
  geexp(exp->fields.btw.high);
  emit0(C_STMOP, I_BTW);
  if (exp->not) emit0(C_STMOP, I_NOT);
}



/*======================================================================

  geexp()

  Generate the code for an expression.

  */
void geexp(ExpNod *exp)		/* IN - The expression to generate */
{
  if (exp == NULL) {
    emit0(C_CONST, 0);
    return;
  }
  
  switch (exp->class) {
    
  case EXPBIN:
    geexpbin(exp);
    break;
    
  case EXPWHR:
    geexpwhr(exp);
    break;
    
  case EXPATR:
    geexpatr(exp);
    break;
    
  case EXPINT:
    emit0(C_CONST, exp->fields.val.val);
    break;
    
  case EXPSTR:
    encode(&exp->fields.str.fpos, &exp->fields.str.len);
    emit0(C_CONST, exp->fields.str.len);
    emit0(C_CONST, exp->fields.str.fpos);
    emit0(C_STMOP, I_GETSTR);
    break;
    
  case EXPAGR:
    geexpagr(exp);
    break;
    
  case EXPRND:
    geexprnd(exp);
    break;
    
  case EXPSCORE:
    geexpscore(exp);
    break;

  case EXPWHT:
    geexpwht(exp);
    break;

  case EXPBTW:
    geexpbtw(exp);
    break;
    
  default:
    unimpl(&exp->srcp, "Code Generator");
    emit0(C_CONST, 0);
    return;
  }
}



/*----------------------------------------------------------------------
  duop()

  Dump an operator

  */
static void duop(OpKind op)
{
  switch (op) {
  case OP_AND:
    put("AND");
    break;
  case OP_OR:
    put("OR");
    break;
  case OP_NE:
    put("<>");
    break;
  case OP_EQ:
    put("=");
    break;
  case OP_EXACT:
    put("==");
    break;
  case OP_LE:
    put("<=");
    break;
  case OP_GE:
    put(">=");
    break;
  case OP_LT:
    put("LT");
    break;
  case OP_GT:
    put("GT");
    break;
  case OP_PLUS:
    put("PLUS");
    break;
  case OP_MINUS:
    put("MINUS");
    break;
  case OP_MULT:
    put("MULT");
    break;
  case OP_DIV:
    put("DIV");
    break;
  case OP_CONTAINS:
    put("CONTAINS");
    break;
  }
}


/*----------------------------------------------------------------------
  duagr()

  Dump an aggregate kind.

  */
static void duagr(AgrKind agr)
{
  switch (agr) {
  case AGR_SUM:
    put("SUM");
    break;
  case AGR_MAX:
    put("MAX");
    break;
  case AGR_COUNT:
    put("COUNT");
    break;
  }
}



/*======================================================================

  dutyp()

  Dump a type indication.

  */
void dutyp(TypeKind typ)
{
  switch (typ) {
  case TYPBOOL:
    put("BOOL");
    break;
  case TYPINT:
    put("INT");
    break;
  case TYPSTR:
    put("STR");
    break;
  default:
    put("UNDEFINED");
    break;
  }
}



/*======================================================================

  duexp()

  Dump an expression node.

 */
void duexp(ExpNod *exp)
{
  if (exp == NULL) {
    put("NULL");
    return;
  }

  put("EXP: ");
  switch (exp->class) {
  case EXPBIN:
    put("BIN ");
    break;
  case EXPWHR:
    put("WHR ");
    if (exp->not) put("NOT ");
    break;
  case EXPATR:
    if (exp->not) put("NOT ");
    put("ATR ");
    break;
  case EXPINT:
    put("INT ");
    break;
  case EXPSTR:
    put("STR ");
    break;
  case EXPAGR:
    put("AGR ");
    break;
  case EXPRND:
    put("RND");
    break;
  case EXPSCORE:
    put("SCORE");
    break;
  case EXPWHT:
    put("WHT");
    break; 
  case EXPBTW:
    if (exp->not) put("NOT ");
    put("BTW");
    break;
  default:
    put("*** ERROR *** ");
    break;
  }
  dusrcp(&exp->srcp);
  in();

  switch (exp->class) {
  case EXPWHR:
    put("wht: "); duexp(exp->fields.whr.wht); nl();
    put("whr: "); duwhr(exp->fields.whr.whr);
    break;
  case EXPATR:
    put("wht: "); duexp(exp->fields.atr.wht); nl();
    put("atr: "); dumpId(exp->fields.atr.atr);
    break;
  case EXPINT:
    put("val: "); duint(exp->fields.val.val);
    break;
  case EXPSTR:
    put("fpos: "); duint(exp->fields.str.fpos); nl();
    put("len: "); duint(exp->fields.str.len);
    break;
  case EXPBIN:
    put("operator: "); duop(exp->fields.bin.op); nl();
    put("left: "); duexp(exp->fields.bin.left); nl();
    put("right: "); duexp(exp->fields.bin.right);
    break;
  case EXPAGR:
    put("agr: "); duagr(exp->fields.agr.agr); nl();
    put("atr: "); dumpId(exp->fields.agr.atr); nl();
    put("whr: "); duwhr(exp->fields.agr.whr);
    break;
  case EXPRND:
    put("from: "); duexp(exp->fields.rnd.from); nl();
    put("to: "); duexp(exp->fields.rnd.to);
    break;
  case EXPWHT:
    put("wht: "); duwht(exp->fields.wht.wht);
    break;
  case EXPSCORE:
    break;
  case EXPBTW:
    put("val: "); duexp(exp->fields.btw.val); nl();
    put("low: "); duexp(exp->fields.btw.low); nl();
    put("high: "); duexp(exp->fields.btw.high);
    break;
  case EXPISA:
				/* FIXME */
    break;
  }
  out();
}
