/*----------------------------------------------------------------------*\

				EXP.C
			   Expression Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "exp.h"		/* EXP-nodes */
#include "atr.h"		/* ATR-nodes */
#include "whr.h"		/* WHR-nodes */
#include "wht.h"		/* WHT-nodes */
#include "nam.h"		/* NAM-nodes */
#include "elm.h"		/* ELM-nodes */
#include "obj.h"		/* OBJ-nodes */
#include "loc.h"		/* LOC-nodes */
#include "act.h"		/* ACT-nodes */


#include "emit.h"

#include "acode.h"
#include "encode.h"

#include "dump.h"




/*======================================================================

  eqtyp()

  Check if two types are equal. If one is TYPUNK they are.

  */
#ifdef _PROTOTYPES_
Boolean eqtyp(TypeKind typ1, TypeKind typ2)
                         	/* IN - types to compare */
#else
Boolean eqtyp(typ1, typ2)
     TypeKind typ1, typ2;	/* IN - types to compare */
#endif
{
  if (typ1 == TYPERR || typ2 == TYPERR) syserr("Unintialised type in eqtyp()");
  return (typ1 == TYPUNK || typ2 == TYPUNK || typ1 == typ2);
}



/*======================================================================

  newexp()

  Allocates and initialises an expnod.

 */
#ifdef _PROTOTYPES_
ExpNod *newexp(Srcp *srcp, ExpKind class)
                		/* IN - Source Position */
                   		/* IN - The expression class */
#else
ExpNod *newexp(srcp, class)
     Srcp *srcp;		/* IN - Source Position */
     ExpKind class;		/* IN - The expression class */
#endif
{
  ExpNod *new;			/* The newly allocated area */

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
#ifdef _PROTOTYPES_
static void anexpwhr(ExpNod *exp, EvtNod *evt, List *pars)
                 		/* IN - The expression to analyze */
                 		/* IN - Possibly inside event */
                		/* IN - Possible parameters */
#else
static void anexpwhr(exp, evt, pars)
     ExpNod *exp;		/* IN - The expression to analyze */
     EvtNod *evt;		/* IN - Possibly inside event */
     List *pars;		/* IN - Possible parameters */
#endif
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
      symcheck(&sym, &elm, exp->fields.whr.wht->fields.wht.wht->nam,
	       NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
      break;
    default:
      syserr("Unrecognized switch in anwhr().");
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
      symcheck(&sym, &elm, exp->fields.whr.whr->wht->nam, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
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
      syserr("Unrecognized switch in anexpwhr().");
      break;
    }
    break;
  case WHR_IN:
    cntcheck(exp->fields.whr.whr->wht, pars);
    break;
  default:
    syserr("Unrecognized switch in anexpwhr().");
    break;
  }

  exp->typ = TYPBOOL;
}



/*----------------------------------------------------------------------
  anatr()

  Analyze an ATR expression.

  */
#ifdef _PROTOTYPES_
static void anatr(ExpNod *exp, EvtNod *evt, List *pars)
                 		/* IN - The expression to analyze */
                 		/* IN - Possibly inside Event? */
                		/* IN - List of parameters */
#else
static void anatr(exp, evt, pars)
     ExpNod *exp;		/* IN - The expression to analyze */
     EvtNod *evt;		/* IN - Possibly inside Event? */
     List *pars;		/* IN - List of parameters */
#endif
{
  AtrNod *atr;
  SymNod *sym;			/* The symbol table node */
  ElmNod *elm;			/* A parameter element */
  List *defatrs;		/* Default attributes to consult */

  switch (exp->fields.atr.wht->fields.wht.wht->wht) {

  case WHT_ACT:
    if (evt != NULL)
      lmLog(&exp->fields.atr.wht->fields.wht.wht->srcp, 412, sevERR, "");
    else {
      atr = findatr(exp->fields.atr.atr->str, adv.aatrs);
      if (atr == NULL) {		/* attribute not found globally */
	lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "ACTOR");
	exp->typ = TYPUNK;
      } else {
	exp->fields.atr.atr->code = atr->nam->code;
	exp->typ = atr->typ;
      }
    }
    break;

  case WHT_LOC:
    atr = findatr(exp->fields.atr.atr->str, adv.latrs);
    if (atr == NULL) {		/* attribute not found globally */
      lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "LOCATION");
      exp->typ = TYPUNK;
    } else {
      exp->fields.atr.atr->code = atr->nam->code;
      exp->typ = atr->typ;
    }
    break;

  case WHT_OBJ:
    if (pars == NULL)
      lmLog(&exp->fields.atr.wht->fields.wht.wht->srcp, 409, sevERR, "");
    atr = findatr(exp->fields.atr.atr->str, adv.oatrs);
    if (atr == NULL) {		/* attribute not found globally */
      lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "OBJECT");
      exp->typ = TYPUNK;
    } else {
      exp->fields.atr.atr->code = atr->nam->code;
      exp->typ = atr->typ;
    }
    break;

  case WHT_ID:
    symcheck(&sym, &elm, exp->fields.atr.wht->fields.wht.wht->nam,
	     NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
    /* If it was a parameter the parameter must be of a single class */
    /* and the attribute a default attribute for that class */
    atr = NULL;
    if (elm) {
      if (elm->res == NULL || elm->res->single) {
	if (elm->res == NULL || (elm->res->classes & NAMOBJ != 0) || (elm->res->classes & NAMCOBJ) != 0)
	  atr = findatr(exp->fields.atr.atr->str, adv.oatrs);
	else if ((elm->res->classes & NAMACT) != 0 || (elm->res->classes & NAMCACT) != 0)
	  atr = findatr(exp->fields.atr.atr->str, adv.aatrs);
	else
	  lmLog(&exp->fields.atr.atr->srcp, 406, sevERR, "");
	if (atr == NULL) {	/* Not a default attribute */
	  lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "a parameter");
	  exp->typ = TYPUNK;
	} else {
	  exp->fields.atr.atr->code = atr->nam->code;
	  exp->typ = atr->typ;
	}
      } else
	lmLog(&exp->fields.atr.atr->srcp, 405, sevERR, "attribute expression");
    } else if (sym) {
      switch (sym->class) {
      case NAMLOC:
	sym = lookup(exp->fields.atr.wht->fields.wht.wht->nam->str);
	atr = findatr(exp->fields.atr.atr->str, ((LocNod *)sym->ref)->atrs);
	defatrs = adv.latrs;	/* Default attributes are location attrs. */
	break;
      case NAMOBJ:
	sym = lookup(exp->fields.atr.wht->fields.wht.wht->nam->str);
	atr = findatr(exp->fields.atr.atr->str, ((ObjNod *)sym->ref)->atrs);
	defatrs = adv.oatrs;	/* Default attributes are object attrs. */
	break;
      case NAMACT:
	sym = lookup(exp->fields.atr.wht->fields.wht.wht->nam->str);
	atr = findatr(exp->fields.atr.atr->str, ((ActNod *)sym->ref)->atrs);
	defatrs = adv.aatrs;	/* Default attributes are actor attrs. */
	break;
      default:
	defatrs = NULL;
	break;
      }
      if (atr == NULL) {	/* Attribute not found locally */
	/* Try default attributes */
	if ((atr = findatr(exp->fields.atr.atr->str, defatrs)) == NULL) {
	  /* Still didn't find it */
	  lmLog(&exp->fields.atr.atr->srcp, 315, sevERR,
		exp->fields.atr.wht->fields.wht.wht->nam->str);
	  exp->typ = TYPUNK;
	}
      }
    }
    if (atr != NULL) {
      exp->fields.atr.atr->code = atr->nam->code;
      exp->typ = atr->typ;
    } else
      exp->typ = TYPUNK;
    break;

  default:
    syserr("Unrecognized switch in anatr().");
    break;
  }
}


/*----------------------------------------------------------------------
  anbin()

  Analyze a binary expression and find out its type.

 */
#ifdef _PROTOTYPES_
static void anbin(ExpNod *exp, EvtNod *evt, List *pars)
                 
                 		/* IN - Possibly inside event */
                
#else
static void anbin(exp, evt, pars)
     ExpNod *exp;
     EvtNod *evt;		/* IN - Possibly inside event */
     List *pars;
#endif
{
  anexp(exp->fields.bin.left, evt, pars);
  anexp(exp->fields.bin.right, evt, pars);

  switch (exp->fields.bin.op) {
  case OP_AND:
  case OP_OR:
    if (!eqtyp(exp->fields.bin.left->typ, TYPBOOL))
      lmLog(&exp->fields.bin.left->srcp, 330, sevERR, "boolean");
    if (!eqtyp(exp->fields.bin.right->typ, TYPBOOL))
      lmLog(&exp->fields.bin.right->srcp, 330, sevERR, "boolean");
    exp->typ = TYPBOOL;
    break;

  case OP_NE:
  case OP_EQ:
    if (!eqtyp(exp->fields.bin.left->typ, exp->fields.bin.right->typ))
      lmLog(&exp->srcp, 331, sevERR, "expression");
    exp->typ = TYPBOOL;
    break;

  case OP_EXACT:
    if (!eqtyp(exp->fields.bin.left->typ, TYPSTR))
      lmLog(&exp->fields.bin.left->srcp, 330, sevERR, "string");
    if (!eqtyp(exp->fields.bin.right->typ, TYPSTR))
      lmLog(&exp->fields.bin.right->srcp, 330, sevERR, "string");
    break;
	    
  case OP_LE:
  case OP_GE:
  case OP_LT:
  case OP_GT:
    if (!eqtyp(exp->fields.bin.left->typ, TYPINT))
      lmLog(&exp->fields.bin.left->srcp, 330, sevERR, "integer");
    if (!eqtyp(exp->fields.bin.right->typ, TYPINT))
      lmLog(&exp->fields.bin.right->srcp, 330, sevERR, "integer");
    exp->typ = TYPBOOL;
    break;

  case OP_PLUS:
  case OP_MINUS:
    if (!eqtyp(exp->fields.bin.left->typ, TYPINT))
      lmLog(&exp->fields.bin.left->srcp, 330, sevERR, "integer");
    if (!eqtyp(exp->fields.bin.right->typ, TYPINT))
      lmLog(&exp->fields.bin.right->srcp, 330, sevERR, "integer");
    exp->typ = TYPINT;
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
#ifdef _PROTOTYPES_
static void anagr(ExpNod *exp, EvtNod *evt, List *pars)
                 		/* IN - The expression to analyze */
                 		/* IN - Possibly inside Event? */
                
#else
static void anagr(exp, evt, pars)
     ExpNod *exp;		/* IN - The expression to analyze */
     EvtNod *evt;		/* IN - Possibly inside Event? */
     List *pars;
#endif
{
  AtrNod *atr = NULL;

  exp->typ = TYPINT;
  if (exp->fields.agr.agr != AGR_COUNT) {
    atr = findatr(exp->fields.agr.atrnam->str, adv.oatrs);
    if (atr == NULL) {		/* attribute not found globally */
      lmLog(&exp->fields.agr.atrnam->srcp, 404, sevERR,
	    "OBJECT in aggregate expression");
      exp->typ = TYPUNK;
    } else if (!eqtyp(TYPINT, atr->typ)) {
      lmLog(&exp->fields.agr.atrnam->srcp, 418, sevERR, "");
      exp->typ = TYPUNK;
    } else
      exp->fields.agr.atrnam->code = atr->nam->code;
  }

  anwhr(exp->fields.agr.whr, evt, pars);
}


/*----------------------------------------------------------------------
  anrnd()

  Analyse a random expression.

  */
#ifdef _PROTOTYPES_
static void anrnd(ExpNod *exp)
                 		/* IN - Expression to analyse */
#else
static void anrnd(exp)
     ExpNod *exp;		/* IN - Expression to analyse */
#endif
{
  if (exp->fields.rnd.from == exp->fields.rnd.to)
    lmLog(&exp->srcp, 416, sevINF, "");
  exp->typ = TYPINT;
}


/*----------------------------------------------------------------------

  anexpwht()

  Analyse a WHT expression.

  */
#ifdef _PROTOTYPES_
static void anexpwht(ExpNod *exp, EvtNod *evt, List *pars)
                 		/* IN - Expression to analyse */
                 		/* IN - Possibly inside Event? */
                		/* IN - Possible parameter list in this context */
#else
static void anexpwht(exp, evt, pars)
     ExpNod *exp;		/* IN - Expression to analyse */
     EvtNod *evt;		/* IN - Possibly inside Event? */
     List *pars;		/* IN - Possible parameter list in this context */
#endif
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
    symcheck(&sym, &par, exp->fields.wht.wht->nam, NAMACT+NAMOBJ+NAMCOBJ+NAMCACT+NAMLOC+NAMCNT+NAMNUM+NAMSTR, NAMANY, pars);
    if (par) {
      if (par->res)
	if (par->res->classes & (NAMACT+NAMOBJ+NAMLOC+NAMCOBJ+NAMCACT+NAMCNT))
	  exp->typ = TYPENT;
	else if (par->res->classes & NAMSTR)
	  exp->typ = TYPSTR;
        else
	  exp->typ = TYPINT;
      else
	exp->typ = TYPENT;
    } else if (sym) {
      exp->typ = TYPENT;
    } else
      exp->typ = TYPUNK;
    break;

  default:
    syserr("Unrecognized switch in anexpwht().");
    break;
  }
}

/*======================================================================

  anexp()

  Analyze one expression.

  */
#ifdef _PROTOTYPES_
void anexp(ExpNod *exp, EvtNod *evt, List *pars)
                 		/* IN - The expression to analyze */
                 		/* IN - Possibly inside event */
                		/* IN - Possible verb parameters */
#else
void anexp(exp, evt, pars)
     ExpNod *exp;		/* IN - The expression to analyze */
     EvtNod *evt;		/* IN - Possibly inside event */
     List *pars;		/* IN - Possible verb parameters */
#endif
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
    anrnd(exp);
    break;

  case EXPSCORE:
    exp->typ = TYPINT;
    break;

  case EXPWHT:
    anexpwht(exp, evt, pars);
    break;

  default:
    syserr("Unrecognized switch in anexp().");
    break;
  }
}



/*----------------------------------------------------------------------
  geexpbin()

  Generate a binary expression.

  */
#ifdef _PROTOTYPES_
static void geexpbin(ExpNod *exp)
                 	/* IN - Expression node */
#else
static void geexpbin(exp)
     ExpNod *exp;	/* IN - Expression node */
#endif
{
  geexp(exp->fields.bin.right);
  geexp(exp->fields.bin.left);
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
  }
  if (exp->not) emit0(C_STMOP, I_NOT);
}



/*----------------------------------------------------------------------
  geexpwhr()

  Generate a where-expression.

  */
#ifdef _PROTOTYPES_
static void geexpwhr(ExpNod *exp)
                 	/* IN - Expression node */
#else
static void geexpwhr(exp)
     ExpNod *exp;	/* IN - Expression node */
#endif
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
      genam(exp->fields.whr.whr->wht->nam);
      emit0(C_CONST, 1);
      emit0(C_CURVAR, V_PARAM);
      emit0(C_STMOP, I_IN);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_AT:
      genam(exp->fields.whr.whr->wht->nam);
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
      genam(exp->fields.whr.wht->fields.wht.wht->nam);
      emit0(C_STMOP, I_HERE);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_NEAR:
      gewht(exp->fields.whr.wht->fields.wht.wht);
      emit0(C_STMOP, I_NEAR);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_IN:
      genam(exp->fields.whr.whr->wht->nam);
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
#ifdef _PROTOTYPES_
static void geexpatr(ExpNod *exp)
                 	/* IN - Expression node */
#else
static void geexpatr(exp)
     ExpNod *exp;	/* IN - Expression node */
#endif
{
  emit0(C_CONST, exp->fields.atr.atr->code);
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
#ifdef _PROTOTYPES_
static void geexpagr(ExpNod *exp)
                 	/* IN - The expression to generate */
#else
static void geexpagr(exp)
     ExpNod *exp;	/* IN - The expression to generate */
#endif
{
  gewhr(exp->fields.agr.whr);

  if (exp->fields.agr.agr != AGR_COUNT)
    emit0(C_CONST, exp->fields.agr.atrnam->code);

  switch (exp->fields.agr.agr) {
  case AGR_SUM: emit0(C_STMOP, I_SUM); break;
  case AGR_MAX: emit0(C_STMOP, I_MAX); break;
  case AGR_COUNT: emit0(C_STMOP, I_COUNT); break;
  default: syserr("Unrecognized switch in geexpagr().");
  }
}



/*----------------------------------------------------------------------
  geexprnd()

  Generate code for a random expression.

  */
#ifdef _PROTOTYPES_
static void geexprnd(ExpNod *exp)
                 	/* IN - The expression to generate */
#else
static void geexprnd(exp)
     ExpNod *exp;	/* IN - The expression to generate */
#endif
{
  if (exp->fields.rnd.from < exp->fields.rnd.to) {
    emit0(C_CONST, exp->fields.rnd.to);
    emit0(C_CONST, exp->fields.rnd.from);
  } else {
    emit0(C_CONST, exp->fields.rnd.from);
    emit0(C_CONST, exp->fields.rnd.to);
  }
  emit0(C_STMOP, I_RND);
}



/*----------------------------------------------------------------------
  geexpscore()

  Generate the code for a SCORE expression.

  */
#ifdef _PROTOTYPES_
static void geexpscore(ExpNod *exp)
                 	/* IN - The expression to generate */
#else
static void geexpscore(exp)
     ExpNod *exp;	/* IN - The expression to generate */
#endif
{
  emit0(C_CURVAR, V_SCORE);
}



/*----------------------------------------------------------------------
  geexpwht()

  Generate the code for a WHAT expression.

  */
#ifdef _PROTOTYPES_
static void geexpwht(ExpNod *exp)
                 	/* IN - The expression to generate */
#else
static void geexpwht(exp)
     ExpNod *exp;	/* IN - The expression to generate */
#endif
{
  gewht(exp->fields.wht.wht);
}



/*======================================================================

  geexp()

  Generate the code for an expression.

  */
#ifdef _PROTOTYPES_
void geexp(ExpNod *exp)
                 	/* IN - The expression to generate */
#else
void geexp(exp)
     ExpNod *exp;	/* IN - The expression to generate */
#endif
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
#ifdef _PROTOTYPES_
static void duop(OpKind op)
#else
static void duop(op)
     OpKind op;
#endif
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
  }
}


/*----------------------------------------------------------------------
  duagr()

  Dump an aggregate kind.

  */
#ifdef _PROTOTYPES_
static void duagr(AgrKind agr)
#else
static void duagr(agr)
     AgrKind agr;
#endif
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
#ifdef _PROTOTYPES_
void dutyp(TypeKind typ)
#else
void dutyp(typ)
     TypeKind typ;
#endif
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
#ifdef _PROTOTYPES_
void duexp(ExpNod *exp)
#else
void duexp(exp)
     ExpNod *exp;
#endif
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
    put("atr: "); dunam(exp->fields.atr.atr);
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
    put("atrnam: "); dunam(exp->fields.agr.atrnam); nl();
    put("whr: "); duwhr(exp->fields.agr.whr);
    break;
  case EXPRND:
    put("from: "); duint(exp->fields.rnd.from); nl();
    put("to: "); duint(exp->fields.rnd.to);
    break;
  case EXPWHT:
    put("wht: "); duwht(exp->fields.wht.wht);
    break;
  case EXPSCORE:
    break;
  }
  out();
}
