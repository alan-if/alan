/*--------------------------------------------------------------------*\
    spa.c				Date: 1992-02-27/reibert@roo

    spa -- standard process of arguments (SoftLabs way)
          
    Author: Reibert Olsson.		
    	    Copyright (c) 1991 SoftLab ab

    History:
    3.0(2) - 1992-02-26/reibert@roo  -- Ambiguous was misspelled, spaArgumentNo,
    					SPA_PRINT_DEFAULT
    3.0(1) - 1991-09-23/reibert@roo  -- SPA_Set with -- sets all on (good?)
    3.0    - 1991-08-28/reibert@roo  -- SPA_Keyword, all SPA_funs as arguments
    2.4    - 1990-11-27/reibert@roo  -- SPA_Report, SPA_Toggle etc. removed
    2.3(2) - 1990-11-20/reibert@roo  -- ANSI-C && C++
    2.3(1) - 1990-04-24/reibert@roo  -- Some cleanup
    2.3    - 90-04-18/Reibert Olsson -- Multiple help lines, default settings
    2.2    - 90-02-13/Reibert Olsson -- SPA_Report
    2.1    - 89-12-03/Reibert Olsson -- SPA_Set, errfun is a SPA_FUNCTION,
   		 		        SPA_Toggle
    2.0(1) - 89-11-27/Reibert Olsson
    2.0    - 89-11-20/Reibert Olsson -- Adapted to SoftLab rules, SPA
    1.0    - 89-08-09/Reibert Olsson

    (See spa.h)
--
As one cannot initialize unions in CC, the following elegant declaration for 
generic arguments won't be used:
typedef union value {
    int    Boolean;
    int   *pBoolean;
    int    Integer;
    int   *pInteger;
    void (*pFunction)();
    char **pString;
    char  *String;
    void  *pAny;
} _SPA_VALUE;
\*--------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

typedef int boolean;
#define FALSE 0
#define TRUE 1

#define PRIVATE static
#define PUBLIC

#include "spa.h"

#ifndef SPA_IGNORE_CASE
#define SPA_IGNORE_CASE	TRUE	/* Unsensitive to the case of options */
#endif

#ifndef SPA_MATCH_PREFIX
#define SPA_MATCH_PREFIX TRUE	/* Unique prefix is matching options */
#endif

#ifndef SPA_PRINT_DEFAULT
#define SPA_PRINT_DEFAULT TRUE	/* Spa tries to report default in help */
#endif

#ifndef SPA_PRINT_ITEM_SZ
#define SPA_PRINT_ITEM_SZ 17	/* The item width in help printing */
#endif


/*----------------------------------------------------------------------
   These are to make the parameters to spaProcess accessibel throughout
   this file 
*/
PRIVATE char **pArgV;
PRIVATE int pArgC;
PRIVATE int pArg;		/* Current arg, index in pArgV */
PRIVATE _SPA_ITEM *pArguments;
PRIVATE _SPA_ITEM *pOptions;

#ifdef __NEWC__

#define safeExecute(fun, item, raw, nod) \
  if (fun) (*(void (*)(char *, char *, int, _SPA_VALUE))fun)(\
    (item->name), raw, nod, item->arg2)
/*  if (fun) (*(SpaFun)fun)(\
    (item->name), raw, nod, (item->arg2)) */

#else

#define safeExecute(fun, item, raw, nod) \
  if (fun) (*(void (*)())fun)(item->name, raw, nod, item->arg2)

#endif

PRIVATE SpaFun *pErrFun;	/* Points to errorfunction */

#define spaErr(p, r, n, a) (*pErrFun)(p, r, n, (_SPA_VALUE)(a))


#if SPA_IGNORE_CASE
#include <ctype.h>
#define lwr(C) (isupper(C)? tolower(C): C)
#else
#define lwr(C) C
#endif

#define _SPA_EXACT (-TRUE)

/*----------------------------------------------------------------------
    pErr()

    Default error printing, goes to "stderr".
*/
PRIVATE SPA_FUNCTION(pErr)
{
    fprintf(stderr, "SPA(processing arguments): %c(%d): %s: %s\n",
	    arg, numberOf, rawName, prettyName);
}


/*----------------------------------------------------------------------
    match()

    Return SPA_MATCH_PREFIX if a true prefix is found, and _SPA_EXACT
    if both strings are equal. The string is stripped of any comments
    (i.e blank and following).
*/
PRIVATE int match(p, s)
    register char *p;		/* IN - User supplied argv-item */
    register char *s;		/* IN - SPA_ITEM string */
{
    while (*p) {
    	if (*s<=' ' || lwr(*s)!=lwr(*p)) return FALSE;
    	s++; p++;
    }
    return (*s>' '? SPA_MATCH_PREFIX: _SPA_EXACT);
}


/*----------------------------------------------------------------------
    find()

    Go thru a datastructure and match() to get any hits. Returns the
    number of hits, and sets first found index (or -1).
*/
PRIVATE int find(ai, kws, kwSz, kwO, found)
    char *ai;			/* IN - User supplied argv-item */
    char kws[];			/* IN - The matching words */
    int kwSz;			/* IN - Size of kws */
    int kwO;			/* IN - Offset to kws.name */
    int *found;			/* OUT - The found items index */
{
    int c, i, o;
    int hits = 0;

    *found = -1;
    for (o=kwO, i=0; *(char **)(&kws[o]); i++, o += kwSz) {
	c= match(ai, *(char **)(&kws[o]));
	if (c!=FALSE) {
	    if (*found==-1) *found = i;
	    if (c==_SPA_EXACT) { hits = 1; break; }
	    else hits++;
	}
    }
    return hits;
}

/*----------------------------------------------------------------------
    findKeyWord()

    Go thru the keywords and match() to get any hits. Returns the
    found items index or 0 (i.e. the default).
*/
PRIVATE int findKeyWord(thisWord, keyWords)
    char *thisWord;	/* IN - User supplied argv-item */
    char *keyWords[];	/* IN - The keywords */
{
    int found;

    switch (find(thisWord, (char *)keyWords, sizeof(char *), 0, &found)) {
    case 0:
	spaErr("Non-matching keyword", thisWord, SPA_NOT_FOUND, 'E');
    	break;
    case 1: 
    	break;
    default:
	spaErr("Ambiguous keyword", thisWord, SPA_AMBIGUOUS, 'E');
	break;
    }
    return found<0 ? 0: found;
}


/*----------------------------------------------------------------------
    printItem()

    Print help "line" for one SPA_ITEM.
*/
PRIVATE void printItem(name, help, def, set)
    char *name;			/* IN - Name of item*/
    register char *help;	/* IN - Help string */
    char *def;			/* IN - Default value string */
    register char *set;		/* IN - Points to set string (or is NULL) */
{
    printf("  %-*s ", SPA_PRINT_ITEM_SZ, name);
    if (strlen(name)>SPA_PRINT_ITEM_SZ)
	printf("\n  %-*s ", SPA_PRINT_ITEM_SZ, "");
    if (help) {
	printf("-- ");
	for (;;) {
	    while (*help) {
		putchar(*help);
		if (*help++ == '\n') break;
	    }
	    if (!*help) break;
	    printf("  %-*s    ", SPA_PRINT_ITEM_SZ, "");
	    if (set) printf("%c -- ", *set++);
	}
    }
#if SPA_PRINT_DEFAULT
    if (*def) printf(" (%s)", def);
#endif
    putchar('\n');
}


/*----------------------------------------------------------------------
    reportItem()

    Report one SPA_ITEM.
*/
PRIVATE void reportItem(item, argument)
    register _SPA_ITEM *item;
    boolean argument;
{
    char buf[128], def[128];	/* Is this enough? Too much? No test ahead! */
    char *set = NULL;

    if (item->type==SPA_None) return;

    /* Set name */
    *buf = argument? '<': '-';
    switch (item->type) {
    case SPA_Flag:
	sprintf(buf+1, "[-]%s", item->name);
	break;
    case SPA_Set:
	set = (char *)item->arg2;
	sprintf(buf+1, "[-]%s {%s}", item->name, set);
	break;
    default:
	strcpy(buf+1, item->name);
	break;
    }
    if (argument) strcat(buf, ">");
    
    /* Set default values */
    *def = 0;
    switch (item->type) {	
    case SPA_Flag:
	strcpy(def, (int)item->arg2? "TRUE": "FALSE");
	break;
    case SPA_Numeric:
	sprintf(def, "%d", (int)item->arg2);
	break;
#ifdef SPA_FLOAT
    case SPA_Float:
	sprintf(def, "%g", *(float *)(item->arg2));
	break;
#endif
    case SPA_String:
	if (item->arg2) strcpy(def, (char*)item->arg2);
	break;
    case SPA_Keyword: {
	register char **kws = (char **)item->arg2;
	if (kws) {
	    for (;*kws;) {
		strcat(def, *kws++);
		if (*kws) strcat(def, ", ");
	    }
	}
    } break;
    default:
	break;
    }
    printItem(buf, item->help, def, set);
}


/*----------------------------------------------------------------------
    report()

    Make a report list out of the arguments and options.
*/
PRIVATE void report(args, opts)
    register _SPA_ITEM args[];
    register _SPA_ITEM opts[];
{
    register int i;

    for (i= 0; args[i].name; i++) {
	if (args[i].name && *args[i].name!=0) {
	    if (i==0) printf("\nArguments:\n");
	    reportItem(&args[i], TRUE);
	}
    }

    if (opts[0].name) printf("\nOptions:\n");
    for (i= 0; opts[i].name; i++) {
	reportItem(&opts[i], FALSE);
    }
}


/*----------------------------------------------------------------------
    setDefault()

    Set default values for one SPA_ITEM.
*/
PRIVATE void setDefault(item)
    register _SPA_ITEM *item;
{   /* CAVEAT: Much coersion/casting lies ahead, liable to errors. */
    register int i;

    switch (item->type) {
    case SPA_Flag:
    case SPA_Numeric:
	*((int *)item->arg1)= (int)item->arg2;
	break;
#ifdef SPA_FLOAT
    case SPA_Float:
	*((float *)item->arg1)= *(float *)(item->arg2);
	break;
#endif
    case SPA_Keyword:		/* First item (0) is default*/
    case SPA_Set:		/* Empty set (0) - the only thing we can do */
	*((int *)item->arg1)= 0;
	break;
    case SPA_String:
	*((char **)item->arg1)= (char *)item->arg2;
	break;
    case SPA_Function:
    case SPA_Help:
    case SPA_None:
	break;
    default:
	spaErr("Illegal function type", item->name, SPA_ILLEGAL_TYPE, 'F');
	break;
    }
}


/*----------------------------------------------------------------------
    execute()

    Execute an SPA_ITEM.
*/
PRIVATE void execute(item, option, on)
    register _SPA_ITEM *item;
    boolean option;		/* IN - True if argv was an option */
    register boolean on;	/* IN - True if argv was an on-option */
{   /* CAVEAT: Much coersion/casting lies ahead, liable to errors. */
    if (item) {

	if (option && item->type!=SPA_Flag && item->type!=SPA_Help
	    && (item->type==SPA_Set?on:TRUE)) {
	    /* An option consuming next argument goes here */
	    if (++pArg>=pArgC) {
		--pArg;		/* Too far, backup */
		setDefault(item);
		return;
	    }
	}

	switch (item->type) {
	case SPA_Flag:
	    if (option) *((int *)item->arg1)= on;
	    else { /* Parse the arg */
		/* The table should be user definable ??? */
		static char *kwt[] = { "FALSE", "TRUE",
				       "OFF",   "ON",
				       "NO",    "YES", NULL };
		*((int *)item->arg1)=
		    findKeyWord(pArgV[pArg], kwt)&1 ;
	    }
	    break;
	case SPA_Set: {
	    register char *arg, *bp;

	    if (!on)
	      *((int *)item->arg1) = -1; /* Set all if --opt */
	    else {
	      for (arg= pArgV[pArg]; *arg; arg++) { /* Go thru argument */
		for (bp= (char *)item->arg2; *bp; bp++) /* and descriptor*/
		  if (lwr(*arg)==lwr(*bp)) break;
		if (*bp) {
		  if (on)
			*((int *)item->arg1) |= 1<<(bp-(char *)item->arg2);
		  else
		    *((int *)item->arg1) &= ~(1<<(bp-(char *)item->arg2));
		} else {
		  spaErr("Illegal set character", pArgV[pArg], SPA_NOT_IN_SET, 'W');
		}
	      }
	    }
	} break;
	case SPA_Numeric:
	    if (sscanf(pArgV[pArg], "%i", (int *)(item->arg1))!=1) {
		/* Number not ok */
		setDefault(item);
		spaErr("Integer value expected", pArgV[pArg], SPA_NOT_NUMERIC, 'W');
	    }
	    break;
#ifdef SPA_FLOAT
	case SPA_Float:
	    if (sscanf(pArgV[pArg], "%g", (float *)(item->arg1))!=1) {
		/* Number not ok */
		setDefault(item);
		spaErr("Real value expected", pArgV[pArg], SPA_NOT_NUMERIC, 'W');
	    }
	    break;
#endif
	case SPA_String:
	    *((char **)item->arg1)= pArgV[pArg];
	    break;
	case SPA_Keyword:
	    *((int *)item->arg1)= findKeyWord(pArgV[pArg], (char **)item->arg2);
	    break;
	case SPA_Function:
	    safeExecute(item->arg1, item, pArgV[pArg], (int)on);
	    break;
	case SPA_Help:
	    safeExecute(item->arg1, item, pArgV[pArg], (int)on);
	    report(pArguments, pOptions);
	    safeExecute(item->arg2, item, pArgV[pArg], (int)on);
	    break;
	case SPA_None:
	    break;
	default:
	    spaErr("Illegal function type", pArgV[pArg], SPA_ILLEGAL_TYPE, 'S');
	    break;/* This means that someone has dribbled with the */
		  /* option tables */
	}
    }
}


/*----------------------------------------------------------------------
    option()

    Detect if an argv-item is an option, if so do as specified.
*/
PRIVATE boolean option(argvItem, options)
    register char *argvItem;	/* IN - User argument */
    _SPA_ITEM options[];	/* IN - Possible options */
{
    int found;

    if (argvItem[0]=='-') {
    	register int start= (argvItem[1]=='-'? 2: 1);
    	if (argvItem[start]) {
   	    switch (find(&argvItem[start],
			 (char *)options,
			 sizeof(_SPA_ITEM),
			 (int)((unsigned long)&options[0].name -
			       (unsigned long)&options[0]),
			 &found)) {
    	    case 0:
  	        spaErr("Non-matching option", argvItem, SPA_NOT_FOUND, 'E');
	        break;
    	    case 1:
	        execute(&options[found], TRUE, start==1);
	        break;
    	    default:
  	        spaErr("Ambiguous option", argvItem, SPA_AMBIGUOUS, 'E');
	        break;
    	    }
    	    return TRUE;
    	}
    }
    return FALSE;
}


/*======================================================================
    spaArgument()

    Return Nth relative argument, NULL if outside argv.
*/
PUBLIC char *spaArgument(n)
    int n;
{
    int t= pArg+n;
    
    return ( (t>=pArgC || t<0)? NULL: pArgV[t] );
}


/*======================================================================
    spaArgumentNo()

    Return Nth argument in argv, NULL if outside argv.
*/
PUBLIC char *spaArgumentNo(n)
    int n;
{
    return ( (n>=pArgC || n<0)? NULL: pArgV[n] );
}


/*======================================================================
    spaSkip()

    Skip arguments N steps forward (or backward), stays inside argv.
*/
PUBLIC void spaSkip(n)
    int n;
{
    int t= pArg+n;

    pArg= ( (t>=pArgC)? pArgC: (t<0? 0: t) );
}


/*======================================================================
    spaProcess()

    Will walk through the arguments and try to act according to spec's.
    Must not be called recursivly.
*/
PUBLIC int spaProcess(argc, argv, arguments, options, errfun)
    const int argc;
    const char *argv[];
    _SPA_ITEM arguments[], options[];
    SpaFun *errfun;
{
    register int argNum, nArgs;

    pArgC= argc;
    pArgV= (char **)argv;
    pOptions= options;
    pArguments= arguments;
    pErrFun= (errfun? errfun: pErr);

    for (nArgs= 0; arguments[nArgs].name; nArgs++) setDefault(&arguments[nArgs]);
    for (nArgs= 0; options[nArgs].name; nArgs++) setDefault(&options[nArgs]);

    for (argNum= nArgs= 0, pArg= 1; pArg<pArgC; pArg++)
	if (!option(pArgV[pArg], options)) {
	    nArgs++;
	    if (arguments[argNum].name) {
		execute(&arguments[argNum], FALSE, FALSE);
		if (arguments[argNum+1].name) argNum++;
	    }
	}
    return nArgs;
}

/* == EoF =========================================================== */
