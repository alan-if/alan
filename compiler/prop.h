#ifndef _PROP_H_
#define _PROP_H_
/*----------------------------------------------------------------------*\

			       PROP.H
			    Property Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "types.h"
#include "lst.h"
#include "whr.h"
#include "cnt.h"
#include "form.h"

#include "../interpreter/acode.h"


/* Types */

typedef struct Properties {
  IdNode *id;			/* Identifier */
  Bool predefined;
  Aaddr idAddress;
  IdNode *parentId;		/* The instances parent */
  List *names;			/* List of lists of adjectives and noun (ids)*/
  Where *whr;			/* Where is it initially? */
  List *attributes;
  Aword attributeAddress;	/* ACODE address to attribute list */
  Srcp descriptionCheckSrcp;
  List *descriptionChecks;
  Aword descriptionChecksAddress;/* ACODE address to description checks */
  Srcp descriptionSrcp;
  List *descriptionStatements;	/* DESCRIPTION statements */
  Aword descriptionAddress;	/* ACODE address to DESCRIPTION code */
  Srcp enteredSrcp;
  List *enteredStatements;	/* ENTERED statements */
  Aword enteredAddress;		/* ACODE address to ENTERED code */
  List *definite;		/* Definite Form/Article printing statements */
  Srcp definiteSrcp;
  FormKind definiteType;	/* Form or Article? */
  Aword definiteAddress;	/* ACODE address to Definite statements */
  List *indefinite;		/* Indefinite Form/Article printing statements */
  Srcp indefiniteSrcp;
  FormKind indefiniteType;	/* Form or Article? */
  Aword indefiniteAddress;	/* ACODE address to Indefinite statements */
  List *mentioned;		/* Short (inventory like) statements */
  Srcp mentionedSrcp;
  Aword mentionedAddress;	/* ACODE address to short description (mentioned or name) code */
  Container *container;		/* Pointer to container property node */
  List *verbs;			/* List of verbs */
  Aword verbsAddress;		/* ACODE address to local verb table */
  List *scripts;		/* List of scripts */
  Aaddr scriptsAddress;		/* ACODE address to scripts */
  List *exits;			/* List of exits */
  Aaddr exitsAddress;		/* ACODE address to exits table */
} Properties;


#endif
