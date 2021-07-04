#ifndef _SPA_H_
#define _SPA_H_ 42
/* ---------------------------------------------------------------------- *\
   spa.h                    Date: 1995-04-13/reibert@home

   spa -- standard process of arguments in SoftLabs way

   Written by Reibert Arbring.

   This module (SPA) implements a data-driven method to process user
   arguments in a standard way.

   The stuff to use in this file is prefixed with "SPA", direct use of
   anything prefixed with "_SPA" should be avoided if possible.

   This file (due to C's quirks) is not really readable,
   so please see spa(3).

   Mail to:
   reibert@softlab.se
   tools@softlab.se


   Legal Notice:

   Copyright (c) 1989 - 1995 SoftLab ab.
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
   Datalinjen 1
   S-583 30  Linköping
   SWEDEN
   \* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------- */
/* Use these macros to declare functions callabel from SPA */

#define SPA_FUN(NAME) void NAME(char *prettyName, char *rawName, int on)
typedef SPA_FUN(SpaFun);    /* C type for SPA_FUN */

#define SPA_ERRFUN(NAME) void NAME(char sev, char *msg, char *add)
typedef SPA_ERRFUN(SpaErrFun);  /* C type for SPA_ERRFUN */


/* ---------------------------------------------------------------------- */
typedef enum {          /* The services supplied by SPA */
    _SPA_None = 0,      /* No action */
    _SPA_Flag,          /* Standard setting of boolean (int) */
    _SPA_Bits,          /* Set bitmap */
    _SPA_Integer,       /* Set integer */
    _SPA_String,        /* Set string */
    _SPA_KeyWord,       /* Set integer index */
    _SPA_Function,      /* Call a function */
    _SPA_Help,          /* Help lister, calls fun1 before and */
    /* fun2 after the help listing */
    _SPA_InFile,        /* Set input file */
    _SPA_OutFile,       /* Set output file !MUST Be after _SPA_InFile! */
    _SPA_Float,         /* Set float */
    _SPA_Comment,       /* Help comment, e.g empty line */
    _SPA_Private        /* !MUST be last! */
} _SPA_TYPE;

typedef struct _SPA_ITEM {  /* PRIVATE structure for items */
    char *name;         /*  1    Item name */
    char *help;         /*  2    Simple help-string */
    SpaFun *postFun;    /*  3    Function called after match */
    _SPA_TYPE type;     /*  4    Item type */
    bool b, *bp;        /*  5    Flag */
    int i, *ip;         /*  7    Bits, Integer, KeyWord(IX) */
    char *s, **sp;      /*  9    String (** keywords, filename) */
    float f, *fp;       /* 11    Float */
    FILE *F, **FP;      /* 13    In/out-file */
    SpaFun *hFun;       /* 15    Function called before help */
} _SPA_ITEM;


/* ---------------------------------------------------------------------- */
#define SPA_DECLARE(N) _SPA_ITEM N[] = {

#define SPA_FLAG(n,h,variable,data,p)             {.name=n, .help=h, .postFun=p, .type=_SPA_Flag, .b=data, .bp=&(variable) },
#define SPA_INTEGER(n,h,variable,data,p)          {.name=n, .help=h, .postFun=p, .type=_SPA_Integer, .i=data, .ip=&(variable) },
#define SPA_STRING(n,h,variable,data,p)           {.name=n, .help=h, .postFun=p, .type=_SPA_String, .s=data, .sp=&(variable) },
#define SPA_FLOAT(n,h,variable,data,p)            {.name=n, .help=h, .postFun=p, .type=_SPA_Float, .f=data, .fp=&(variable) },
#define SPA_INFILE(n,h,variable,filename,p)       {.name=n, .help=h, .postFun=p, .type=_SPA_InFile, .s=filename, .F=NULL, .FP=&(variable) },
#define SPA_OUTFILE(n,h,variable,filename,p)      {.name=n, .help=h, .postFun=p, .type=_SPA_OutFile, .s=filename, .F=NULL, .FP, &(variable) },
#define SPA_BITS(n,h,variable,flagChars,data,p)   {.name=n, .help=h, .postFun=p, .type=_SPA_Bits, .i=(int)data, .ip=(int *)&(variable), .s=flagChars },
#define SPA_KEYWORD(n,h,variable,keywords,data,p) {.name=n, .help=h, .postFun=p, .type=_SPA_KeyWord, .i=(int)data, .ip=(int *)&(variable), .sp=keywords },
#define SPA_FUNCTION(n,h,p)                       {.name=n, .help=h, .postFun=p, .type=_SPA_Function },
#define SPA_COMMENT(h)                            {.help=h, .type=_SPA_Comment },
#define SPA_HELP(n,h,f,p)                         {.name=n, .help=h, .postFun=p, .type=_SPA_Help, .hFun=f },

#define SPA_END {0},{0}};


#define spaProcess(C,V,A,O,E) _spaProcess(C,V,A,O,E)


extern int _spaProcess(     /* Go thru user arguments and options */
    int argc,               /* IN - As to main */
    char *argv[],           /* IN - As to main */
    _SPA_ITEM arguments[],  /* IN - User arguments (use SPA_DECLARE)
                               NULL => "superfluous arg" */
    _SPA_ITEM options[],    /* IN - User options (use SPA_DECLARE)
                               NULL => -help only */
    SpaErrFun *errfun       /* IN - Error reciever (a SPA_ERRFUN)
                               NULL => internal routine */
    );                      /* Returns number of user arguments */

extern char *spaArgument(   /* Get an argv-item; Use from a SPA_FUN */
    int offset              /* IN - Offset from current (==0) */
    );                      /* Returns req. argument (or NULL) */

extern char *spaArgumentNo( /* Get an argv-item; Use from a SPA_FUN */
    int offset              /* IN - Absolute position */
    );                      /* Returns req. argument (or NULL) */

extern void  spaSkip(       /* Skip argv-items; Use from a SPA_FUN */
    int offset              /* IN - Number to skip from current */
    );

extern void spaAlert(       /* Error notification; Exits on severe errors */
    char sev,               /* IN - [DIWEFS] */
    char * fmt,             /* IN - printf-format for additional things */
    ...                     /* IN - additional things */
    );

extern void spaExit(        /* (Clean up and then) exit. */
    int exitCode            /* IN - code transmitted to exit() */
    );


extern char SpaAlertLevel;  /* Alert on or above this level,
                               one of D, I, W, E, F, S */
extern char *SpaAlertName;  /* Program name for alerts,
                               NULL 0 => tail argv[0] */
#endif

/* -- EoF --------------------------------------------------------------- */
