/* spa.h			       	Date: 1993-03-11/reibert@roo

   spa -- standard process of arguments in SoftLabs way

   Version 4.0

   Written by Reibert Arbring.

Description:
   This module (SPA) implements a data-driven method to process user
   arguments in a standard way.

   The stuff to use in this file is prefixed with "SPA", direct use of
   anything prefixed with "_SPA" should be avoided if possible.

   This file (due to C's quirks) is not really readable,
   so please see spa(3).

Legal Notice:

		Copyright (c) 1989 - 1993 SoftLab ab.
                       All rights reserved.
             Notice of copyright on this source code 
              product does not indicate publication. 

This file is a product of SoftLab ab and is provided for unrestricted
use provided that this legend is included on all media and as a part
of the software program in whole or part.  Users may copy or modify
this file without charge, but are not authorized to license or
distribute it to anyone else except as part of a product or program
developed by the user.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.

This file is provided with no support and without any obligation on
the part of SoftLab ab to assist in its use, correction, modification
or enhancement.

SOFTLAB AB SHALL HAVE NO LIABILITY WITH RESPECT TO THE INFRINGEMENT OF
COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS FILE OR ANY PART
THEREOF.

In no event will SoftLab be liable for any lost revenue or profits
or other special, indirect and consequential damages, even if SoftLab
has been advised of the possibility of such damages.

	SoftLab ab
	Teknikringen 14
	S-583 30  Linköping
	SWEDEN
*/
#ifndef _SPA_H_
#define _SPA_H_ 4

#ifdef __STDC__
#define __NEWC__
#endif
#ifdef __cplusplus
#define __NEWC__
#endif
#ifdef THINK_C
#define __NEWC__
#endif

typedef enum {			/* The services supplied by SPA */
    _SPA_None = 0,		/* No action */
    _SPA_Flag,			/* Standard setting of boolean (int) */
    _SPA_Bits,			/* Set bitmap */
    _SPA_Integer,		/* Set integer */
    _SPA_String,		/* Set string */
    _SPA_KeyWord,		/* Set integer index */
    _SPA_Function,		/* Call a function */
    _SPA_Help,			/* Built-in help, calls fun1 before and
				   fun2 after the help listing */
    _SPA_InFile,		/* Set input file */
    _SPA_OutFile,		/* Set output file */
    _SPA_Float			/* Set float */
} _SPA_TYPE;

typedef long _SPA_ARG;		/* Must accomodate values (pointers, ints etc.)
				   - long is safe on most systems */

#define _SPA0 ((_SPA_ARG)0)	/* Own s(h)ort for NULL */


/* Use these macros to declare functions callabel from SPA */
#ifdef __NEWC__

#define SPA_FUN(NAME) void NAME(char *prettyName, char *rawName, int on)
typedef SPA_FUN(SpaFun);	/* C type for SPA_FUN */

#define SPA_ERRFUN(NAME) void NAME(char sev, char *msg, char *add)
typedef SPA_ERRFUN(SpaErrFun);	/* C type for SPA_ERRFUN */

#else

#define SPA_FUN(NAME) void NAME(prettyName, rawName, on)\
    char *prettyName; char *rawName; int on;
typedef void SpaFun();

#define SPA_ERRFUN(NAME) void NAME(sev, msg, add)\
    char sev; char *msg; char *add;
typedef void SpaErrFun();

#endif


typedef struct _SPA_ITEM {	/* Private structure for items */
    char *name;			/* Item name */
    char *help;			/* Simple help-string */
    _SPA_TYPE type;		/* Item type */
    void *var;			/* The variable to set */
    _SPA_ARG arg;		/* A generic argument */
    _SPA_ARG def;		/* Default generic argument */
    SpaFun *postFun;		/* Function called after match */
} _SPA_ITEM;


extern char SpaAlertLevel;	/* Alert on or above this level,
				   one of D, I, W, E, S, F */

#ifdef THINK_C
#define spaProcess(C,V,A,O,E) _spaPreProcess(&C,&V,A,O,E)
#else
#define spaProcess(C,V,A,O,E) _spaProcess(C,V,A,O,E)
#endif

#ifdef __NEWC__

#ifdef THINK_C
extern int _spaPreProcess(int *argc,
			  char **argv[],
			  _SPA_ITEM arguments[],
			  _SPA_ITEM options[],
			  SpaErrFun *errfun);
#endif

extern int _spaProcess(		/* Go thru user arguments and options */
    int argc,			/* IN - As to main */
    char *argv[],		/* IN - As to main */
    _SPA_ITEM arguments[],	/* IN - User arguments (use SPA_DECLARE) */
    _SPA_ITEM options[],	/* IN - User options (use SPA_DECLARE) */
    SpaErrFun *errfun		/* IN - Error reciever (a SPA_ERRFUN)
				        NULL => internal routine */
);				/* Returns number of user arguments */

extern char *spaArgument(	/* Get an argv-item; Use from a SPA_FUN */
    int offset			/* IN - Offset from current (==0) */
);				/* Returns required argument (or NULL) */

extern char *spaArgumentNo(	/* Get an argv-item; Use from a SPA_FUN */
    int offset			/* IN - Absolute position */
);				/* Returns required argument (or NULL) */

extern void  spaSkip(		/* Skip argv-items; Use from a SPA_FUN */
    int offset			/* IN - Number to skip from current */
);

#else

extern char *spaArgument();
extern char *spaArgumentNo();
extern void spaSkip();
extern void spaAlert();

#endif

#define SPA_DECLARE(N) _SPA_ITEM N[] = {

#define SPA_FLAG(N,H,V,D,F)	{(N),(H),_SPA_Flag,&(V),_SPA0,(_SPA_ARG)(D),(F)},
#define SPA_INTEGER(N,H,V,D,F)	{(N),(H),_SPA_Integer,&(V),_SPA0,(_SPA_ARG)(D),(F)},
#define SPA_FLOAT(N,H,V,D,F) 	{(N),(H),_SPA_Float,&(V),_SPA0,(_SPA_ARG)&(D),(F)},
#define SPA_STRING(N,H,V,D,F)	{(N),(H),_SPA_String,&(V),_SPA0,(_SPA_ARG)(D),(F)},
#define SPA_BITS(N,H,V,S,D,F)	{(N),(H),_SPA_Bits,&(V),(_SPA_ARG)(S),(_SPA_ARG)(D),(F)},
#define SPA_KEYWORD(N,H,V,K,D,F){(N),(H),_SPA_KeyWord,&(V),(_SPA_ARG)(K),(_SPA_ARG)(D),(F)},
#define SPA_INFILE(N,H,V,A,D,F)	{(N),(H),_SPA_InFile,&(V),(_SPA_ARG)(&(A)),(_SPA_ARG)(D),(F)},
#define SPA_OUTFILE(N,H,V,A,D,F){(N),(H),_SPA_OutFile,&(V),(_SPA_ARG)(&(A)),(_SPA_ARG)(D),(F)},
#define SPA_FUNCTION(N,H,F)	{(N),(H),_SPA_Function,(void *)0,_SPA0,_SPA0,(F)},
#define SPA_HELP(N,H,F1,F2)	{(N),(H),_SPA_Help,(void *)(F1),_SPA0,_SPA0,(F2)},

#define SPA_END {0}, {0} };

#endif
/* -- EoF -- */
