/* spa.h			       	Date: 1993-02-16/thoni@rabbit

   spa -- standard process of arguments in SoftLabs way

   Version 3.0(3)

   Written by Reibert Olsson.		Copyrigth (c) 1991 SoftLab ab.

Description:
   This module (SPA) implements a data-driven method to process user arguments
   in a standard way.

   The stuff to use in this file is prefixed with "SPA_", direct use of
   anything prefixed with "_SPA_" or "_spa_" should be avoided if possible.

   Also see spa(3).

*/
#ifndef _SPA_H_
#define _SPA_H_

#ifdef __STDC__
#define __NEWC__
#endif
#ifdef __cplusplus
#define __NEWC__
#endif

typedef enum {			/* The services supplied by SPA,
				   indicates use of arg1 & arg2 */
    SPA_None,			/* No action */
    SPA_Flag,			/* Standard setting of arg1 as boolean (int) */
    SPA_Set,			/* Set arg1 as bitmap */
    SPA_Numeric,		/* -"-         integer */
    SPA_String,			/* -"-         string */
    SPA_Keyword,		/* -"-         integer index */
    SPA_Function,		/* Call arg1 as a function */
    SPA_Help			/* Built-in help, calls arg1 before and */
				/* arg2 after the help listing */
#define SPA_Argument SPA_String	/* compatibility with SPA 2.X */
#ifdef SPA_FLOAT
   ,SPA_Float			/* Only if wanted, keep last */
#endif
} SPA_TYPE;

typedef unsigned long _SPA_VALUE; /* Must accomodate values (pointers, ints etc.)
				   - long is safe on most systems */

typedef struct {
    char *name;			/* Array of legal names */
    char *help;			/* Simple help-string */
    SPA_TYPE type;		/* Action on this argument */
    _SPA_VALUE arg1;		/* 1st generic argument */
    _SPA_VALUE arg2;		/* 2nd generic argument */
} _SPA_ITEM;


#define SPA_DECLARE(N) 		_SPA_ITEM N[] = {
#define SPA_ITEM(N,H,T,A1,A2) 	{(N),(H),(T),(_SPA_VALUE)(A1),(_SPA_VALUE)(A2)},
#define SPA_END			{NULL,NULL,SPA_None,0L,0L}};


typedef enum {			/* Error codes in "numberOf" */
    SPA_ILLEGAL_TYPE= 9,	/* Illegal function type in item */
    SPA_NOT_FOUND= 10,		/* Non-matching word */
    SPA_AMBIGUOUS= 11,		/* Ambiguous word */
    SPA_NOT_IN_SET= 20,		/* Illegal set-character */
    SPA_NOT_NUMERIC= 21		/* Illegal number */
} SPA_ERROR_CODE;


/* -- functions -- */

#ifdef __NEWC__

/* Use this macro to declare a function callabel from SPA */
#define SPA_FUNCTION(NAME) void NAME(\
    char *prettyName,\
    char *rawName, \
    int numberOf,\
    _SPA_VALUE arg)

typedef SPA_FUNCTION(SpaFun);	/* C type for SPA_FUNCTIONS */

int spaProcess(			/* Go thru user arguments and options */
    int argc,		/* IN - As to main */
    char *argv[],		/* IN - As to main */
    _SPA_ITEM arguments[],	/* IN - User arguments (use SPA_DECLARE) */
    _SPA_ITEM options[],	/* IN - User options (use SPA_DECLARE) */
    SpaFun *errfun		/* IN - Error reciever (a SPA_FUNCTION)
				        NULL => internal routine */
);				/* Returns number of user arguments */

char *spaArgument(		/* Get an argv-item; Use from a SPA_FUNCTION */
    int offset			/* IN - Offset from current (==0) */
);				/* Returns required argument (or NULL) */

char *spaArgumentNo(		/* Get an argv-item; Use from a SPA_FUNCTION */
    int offset			/* IN - Absolute position */
);				/* Returns required argument (or NULL) */

void  spaSkip(			/* Skip argv-items; Use from a SPA_FUNCTION */
    int offset			/* IN - Number to skip from current */
);


#else

#define SPA_FUNCTION(NAME) void NAME(prettyName, rawName, numberOf, arg)\
    char *prettyName;\
    char *rawName;\
    int numberOf;\
    _SPA_VALUE arg;


typedef void SpaFun();

int spaProcess();
char *spaArgument();
char *spaArgumentNo();
void spaSkip();

#endif

#endif

/* -- EoF -- */
