#ifndef _NAM_H_
#define _NAM_H_
/*----------------------------------------------------------------------*\

				NAM.H
			      Name Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "alan.h"
#include "srcp.h"
#include "lst.h"

#include "acode.h"


/* Types: */

typedef long NamKind;
/* Make sure these are compatible with acode */
/* Used for class checks in syntax handling */
#define NAMOBJ 1L		
#define NAMCNT ((long)NAMOBJ<<1)
#define NAMACT ((long)NAMCNT<<1)
#define NAMNUM ((long)NAMACT<<1)
#define NAMSTR ((long)NAMNUM<<1)
/* 4f_Awful hack until conts are fixed */
#define NAMCOBJ ((long)NAMSTR<<1)
#define NAMCACT ((long)NAMCOBJ<<1)

#define NAMDIR ((long)NAMCACT<<1)
#define NAMLOC ((long)NAMDIR<<1)
#define NAMVRB ((long)NAMLOC<<1)
#define NAMATR ((long)NAMVRB<<1)
#define NAMEVT ((long)NAMATR<<1)
#define NAMPAR ((long)NAMEVT<<1)
#define NAMWRD ((long)NAMPAR<<1)
#define NAMRUL ((long)NAMWRD<<1)
#define NAMUNK ((long)NAMRUL<<1)
#define NAMANY (-1L)			/* Any class or property */


typedef struct NamNod {		/* NAME */
  Srcp srcp;			/* Source position of the name */
  char *str;			/* String representation */
  NamKind kind;			/* What kind of name */
  int code;			/* AN - Code for this name */
} NamNod;



/* Data: */


/* Methods: */

#ifdef _PROTOTYPES_

/* Create a new Name node */
extern NamNod *newnam(Srcp *srcp,
		      char str[]);

/* Compare two names */
extern Boolean eqnams(NamNod *nam1, NamNod *nam2);

/* Analyse names, returns length of name text */
extern int annams(List *nams,
		  NamNod *nam,
		  Boolean caps);

/* Generate a reference to a name depending on class */
extern void genam(NamNod *nam);

/* Dump a Name node */
extern void dunam(NamNod *nam);

#else
extern NamNod *newnam();
extern Boolean eqnams();
extern int annams();
extern void genam();
extern void dunam();
#endif

#endif
