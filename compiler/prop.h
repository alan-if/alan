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


/* Currently we handle the following properties:

	Initial location
	Name(s)
	Attributes
	Description Check
	Description Statements
	Entered Statements
	Defininte Article/Form
	Indefinite Article/Form
	Mentioned Clause
	Container
	Verbs
	Scripts
	Exits
*/
#define NOOFPROPS 14

/* Types */

typedef struct Properties {
  IdNode *id;			/* Identifier */
  Bool predefined;
  Aaddr idAddress;
  IdNode *parentId;		/* The instances parent */
  Where *whr;			/* Where is it initially? */
  List *names;			/* List of lists of adjectives and noun (ids)*/
  Srcp pronounsSrcp;
  List *pronouns;		/* List of words that can be used as pronouns */
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
  Bool definiteIsForm;		/* Is the definite statements Form (or Article) */
  Srcp definiteSrcp;
  FormKind definiteType;	/* Form or Article? */
  Aword definiteAddress;	/* ACODE address to Definite statements */
  List *indefinite;		/* Indefinite Form/Article printing statements */
  Bool indefiniteIsForm;	/* Is the definite statements Form (or Article) */
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
