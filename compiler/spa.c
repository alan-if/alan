/*--------------------------------------------------------------------*\
    spa.c				Date: 1993-03-11/reibert@roo

    spa -- standard process of arguments (SoftLabs way)

    Author: Reibert Arbring.
    	    Copyright (c) 1989 - 1993 SoftLab ab

    Legal Notice: As in spa.h

    History:
    4.0    - 1992-03-09/reibert@mac  -- SPA_ITEM outdated, "locale" messages,
    				    	mac adaption, many new features, files, alert
----
    3.1    - 1992-02-26/reibert@roo  -- SPA_PRINT_DEFAULT
    3.0(3) - 1993-02-16/thoni@rabbit -- SPA_Set with '--' requires no argument
    3.0(2) - 1992-02-26/reibert@roo  -- Ambiguous was misspelled, spaArgumentNo
    3.0(1) - 1991-09-23/reibert@roo  -- SPA_Set with '--' sets all on (good?)
    3.0    - 1991-08-28/reibert@roo  -- SPA_KeyWord, all SPA_funs as arguments
----
    2.4    - 1990-11-27/reibert@roo  -- SPA_Report, SPA_Toggle etc. removed
    2.3(2) - 1990-11-20/reibert@roo  -- ANSI-C && C++
    2.3(1) - 1990-04-24/reibert@roo  -- Some cleanup
    2.3    - 90-04-18/Reibert Olsson -- Multiple help lines, default settings
    2.2    - 90-02-13/Reibert Olsson -- SPA_Report
    2.1    - 89-12-03/Reibert Olsson -- SPA_Set, errfun is a SPA_FUNCTION,
   		 		        SPA_Toggle
    2.0(1) - 89-11-27/Reibert Olsson
    2.0    - 89-11-20/Reibert Olsson -- Adapted to SoftLab rules, SPA
----
    1.0    - 89-08-09/Reibert Olsson
\*--------------------------------------------------------------------*/
#include "spa.h"
#if _SPA_H_!=4
error "SPA header file version 4 required"
#endif

#include <stdio.h>
#include <string.h>
#ifdef __NEWC__
#include <stdlib.h>
#include <stdarg.h>
#else
#include <varargs.h>
#endif

typedef int boolean;
#define FALSE 0
#define TRUE 1

#define PRIVATE static
#define PUBLIC

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
#ifndef SPA_LANG
#define SPA_LANG 0		/* Language for messages (English) */
#endif

#if SPA_LANG==46		/* Swedish */
char *SpaFlagKeyWords[] = {
    "AV",    "PÅ",
    "NEJ",   "JA",
    "FALSK", "SANN",
    "NIL",   "T",
    NULL
};
char *SpaDefaultFormat = " (standard: %s)";
char *SpaStrArg = "Parametrar:";
char *SpaStrOpt = "Tillval:";
char *SpaStrUsg = "Användning:";
char *SpaStrNMK = "Nyckelord passar ej";
char *SpaStrAMK = "Tvetydigt keyword";
char *SpaStrNMO = "Tillval passar ej";
char *SpaStrAMO = "Tvetydigt tillval";
char *SpaStrILF = "Okänd funktionstyp";
char *SpaStrISC = "Otillåtet mängdtecken";
char *SpaStrIVE = "Heltal förväntat";
char *SpaStrRVE = "Flyttal förväntat";
char *SpaStrFRE = "Filen kunde ej öppnas för läsning";
char *SpaStrFWE = "Filen kunde ej öppnas för skrivning";
char *SpaStrTMA = "Överflödig parameter";
char *SpaStrAE = "Parameterfel";

char *SpaAlertStr[] = {
    "Avlusning", "Information", "Varning", "Fel",
    "Allvarligt fel", "Internt fel", "Okänt fel",
    "Exekveringen avbryts"
};

#else				/* English */

char *SpaFlagKeyWords[] = {	/* The table for flag as argument */
    "OFF",   "ON",
    "FALSE", "TRUE",
    "NO",    "YES",
    "NIL",   "T",
    NULL
};

char *SpaDefaultFormat = " (default: %s)"; /* How to print default values */
char *SpaStrArg = "Arguments:";
char *SpaStrOpt = "Options:";
char *SpaStrUsg = "Usage:";
char *SpaStrNMK = "Non-matching keyword";
char *SpaStrAMK = "Ambiguous keyword";
char *SpaStrNMO = "Non-matching option";
char *SpaStrAMO = "Ambiguous option";
char *SpaStrILF = "Illegal function type";
char *SpaStrISC = "Illegal set character";
char *SpaStrIVE = "Integer value expected";
char *SpaStrRVE = "Real value expected";
char *SpaStrFRE = "File not opened for reading";
char *SpaStrFWE = "File not opened for writing";
char *SpaStrTMA = "Superfluous argument";
char *SpaStrAE = "Argument error";

char *SpaAlertStr[] = {
    "Debug", "Information", "Warning", "Error",
    "Fatal error", "Internal error", "Unknown error",
    "Execution stopped"
};


#endif

char *SpaProgramName = "";	/* Tail of argv[0] */

PUBLIC char SpaAlertLevel = 'I'; /* Print this level and higher */

/*----------------------------------------------------------------------
   These are to make the parameters to spaProcess accessible throughout
   this file 
*/
PRIVATE char **pArgV;
PRIVATE int pArgC;
PRIVATE int pArg;		/* Current arg, index in pArgV */
PRIVATE _SPA_ITEM *pArguments;
PRIVATE _SPA_ITEM *pOptions;

#ifdef __NEWC__

#define safeExecute(fun, item, raw, on) \
  if (fun) (*(void (*)(char *, char *, int))fun)((item->name), raw, on)
/*  if (fun) (*(SpaFun)fun)((item->name), raw, on) */

#define FUNCTION(N,A) N(
#define PROCEDURE(N,A) void N(
#define IN(T,N) T N
#define OUT(T,N) T* N
#define IS )
#define X ,

#else

#define safeExecute(fun, item, raw, on) \
  if (fun) (*(void (*)())fun)(item->name, raw, on)

#define FUNCTION(N,A) N A
#define PROCEDURE(N,A) void N A
#define IN(T,N) T N;
#define OUT(T,N) T* N;
#define X 
#define IS

#endif


PRIVATE SpaErrFun *pErrFun;	/* Points to errorfunction */

#define spaErr(m, a, s) (*pErrFun)(s, m, a)

PRIVATE struct {
    FILE *deffile;
    char *iomode;
    char *errmsg;
} fileDefault[2] = {		/* We assume _SPA_OutFile-_SPA_InFile == 1 */
    { stdin, "r", NULL },	/* Set errmsg before use! */
    { stdout, "w", NULL }
};
#define mode(T) (fileDefault[(T)-_SPA_InFile].iomode)
#define file(T) (fileDefault[(T)-_SPA_InFile].deffile)
#define emsg(T) (fileDefault[(T)-_SPA_InFile].errmsg)

#if SPA_IGNORE_CASE
#include <ctype.h>
#define lwr(C) (isupper(C)? tolower(C): C)
#else
#define lwr(C) C
#endif

#define _SPA_EXACT (-TRUE)


/*----------------------------------------------------------------------
    Return SPA_MATCH_PREFIX if a true prefix is found, and _SPA_EXACT
    if both strings are equal. The string is stripped of any comments
    (i.e blank and following).
*/
PRIVATE int FUNCTION(match, (p, s))
    IN(register char, *p) X	/* User supplied argv-item */
    IN(register char, *s)	/* SPA_ITEM string */
IS {
    while (*p) {
    	if (*s<=' ' || lwr(*s)!=lwr(*p)) return FALSE;
    	s++; p++;
    }
    return (*s>' '? SPA_MATCH_PREFIX: _SPA_EXACT);
}


/*----------------------------------------------------------------------
    Go thru a datastructure and match() to get any hits. Returns the
    number of hits, and sets first found index (or -1).
*/
PRIVATE int FUNCTION(find, (ai, kws, kwSz, kwO, found))
    IN(char, *ai) X		/* User supplied argv-item */
    IN(register char, kws[]) X	/* The matching words */
    IN(int, kwSz) X		/* Size of kws */
    IN(int, kwO) X		/* Offset to kws.name */
    OUT(int, found)		/* The found items index */
IS {
    register int i, o;
    int c, hits = 0;

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
    Go thru the keywords and match() to get any hits. Returns the
    found items index or the default.
*/
PRIVATE int FUNCTION(findKeyWord, (thisWord, keyWords, def))
    IN(char, *thisWord) X	/* User supplied argv-item */
    IN(char, *keyWords[]) X	/* The keywords */
    IN(int, def)		/* The default index */
IS {
    int found;

    switch (find(thisWord, (char *)keyWords, sizeof(char *), 0, &found)) {
    case 0:
	spaErr(SpaStrNMK, thisWord, 'E');
    	break;
    case 1: 
    	break;
    default:
	spaErr(SpaStrAMK, thisWord, 'E');
	break;
    }
    return found<0 ? def: found;
}


/*----------------------------------------------------------------------
    printItem()

    Print help line for one SPA_ITEM.
*/
PRIVATE PROCEDURE(printItem, (name, help, def, set, kws))
    IN(char *, name) X		/* Name of item */
    IN(register char *, help) X	/* Help string */
    IN(char *, def) X		/* Default value string */
    IN(register char *, set) X	/* Points to set string (or is NULL) */
    IN(char **,kws)		/* Points to keyword array (or is NULL) */
IS {
    boolean nl = FALSE;
    
    printf("  %-*s ", SPA_PRINT_ITEM_SZ, name);
    if (strlen(name)>SPA_PRINT_ITEM_SZ)
	printf("\n  %-*s ", SPA_PRINT_ITEM_SZ, "");
    if (help) {
	printf("-- ");
	for (;;) {
	    for (;*help; help++) {
		if (*help=='\n') { help++; nl = TRUE; break; }
		putchar(*help);
	    }
	    if (!*help) break;
	    printf("\n  %-*s    ", SPA_PRINT_ITEM_SZ, "");
	    if (set) printf("%c -- ", *set++);
	    if (kws && *kws) printf("%s -- ", *kws++);
	}
#if SPA_PRINT_DEFAULT
	if (*def) { 
	    if (nl) printf("\n  %-*s   ", SPA_PRINT_ITEM_SZ, "");
	    printf(SpaDefaultFormat, def);
	}
#endif
    }
    putchar('\n');
}


/*----------------------------------------------------------------------
    pAName()

    Print a SPA_ITEMs name as argument. Static area!
*/
PRIVATE char pName[128];	/* Used by pOName */

PRIVATE char* FUNCTION(pAName, (item))
    IN(_SPA_ITEM *, item)
IS {
    switch (item->type) {
    case _SPA_Flag:
	sprintf(pName, "[%s|%s]", SpaFlagKeyWords[1], SpaFlagKeyWords[0]);
	break;
    case _SPA_Bits:
	sprintf(pName, "{%s}", (char *)item->arg);
	break;
    default:
	sprintf(pName, "<%s>", item->name);
	break;
    }
    return pName;
}


/*----------------------------------------------------------------------
    pOName()

    Print a SPA_ITEMs name as option. Static area!
*/
PRIVATE char* FUNCTION(pOName, (item))
    IN(_SPA_ITEM *, item)
IS {
    switch (item->type) {
    case _SPA_Flag:
	sprintf(pName, "-[-]%s", item->name);
	break;
    case _SPA_Bits:
	sprintf(pName, "-[-]%s {%s}", item->name, (char *)item->arg);
	break;
    default:
	sprintf(pName, "-%s", item->name);
	break;
    }
    return pName;
}


/*----------------------------------------------------------------------
    reportItem()

    Report one SPA_ITEM.
*/
PRIVATE PROCEDURE(reportItem, (item, name))
    IN(register _SPA_ITEM *, item) X
    IN(char *, name)
IS {
    char def[128];	/* Is this enough? Too much? No test ahead! */
    char *set = NULL;
    char **kws = NULL;

    if (item->type==_SPA_None) return;
    
    /* Set default values */
    *def = 0;
#if SPA_PRINT_DEFAULT
    switch (item->type) {	
    case _SPA_Flag:
	strcpy(def, (int)item->def? SpaFlagKeyWords[1]: SpaFlagKeyWords[0]);
	break;
    case _SPA_Integer:
	sprintf(def, "%d", (int)item->def);
	break;
    case _SPA_Float:
	sprintf(def, "%g", *(float *)(item->def));
	break;
    case _SPA_String:
    case _SPA_InFile:
    case _SPA_OutFile:
	if (item->def) strcpy(def, (char*)item->def);
	break;
    case _SPA_KeyWord:
	kws = (char **)item->arg;
	strcpy(def, kws[(int)item->def]);
	break;
    case _SPA_Bits: {
	register int i = 0, j = 1;
	def[0] = '{';
	set = (char *)item->arg;	
	for (; set[i]; i++) {
	    if ((1<<i)&(int)item->def) def[j++] = set[i];
	}
	def[j++] = '}'; def[j] = 0;
    } break;
    default:
	break;
    }
#endif
    printItem(name, item->help, def, set, kws);
}


/*----------------------------------------------------------------------
    report()

    Make a report list out of the arguments and options.
*/
PRIVATE PROCEDURE(report, (args, opts))
    IN(register _SPA_ITEM, args[]) X
    IN(register _SPA_ITEM, opts[])
IS {
    register int i;

    if (args[0].name && *args[0].name) printf("\n%s\n", SpaStrArg);
    for (i= 0; args[i].name && *args[i].name; i++)
	reportItem(&args[i], pAName(&args[i]));

    if (opts[0].name) printf("\n%s\n", SpaStrOpt);
    for (i= 0; opts[i].name; i++)
	reportItem(&opts[i], pOName(&opts[i]));

}


/*----------------------------------------------------------------------
    Assert that the file was opened.
*/
PRIVATE PROCEDURE(assertFile, (item))
    IN(register _SPA_ITEM, *item)
IS {
    if (!*((FILE **)item->var)) { /* open failure */
	spaErr(emsg(item->type), pArgV[pArg], 'E');
	*((FILE **)item->var) = file(item->type);
	*((char **)item->arg) = "";
    }
}

/*----------------------------------------------------------------------
    Set default values for one SPA_ITEM.
*/
PRIVATE PROCEDURE(setDefault, (item))
    IN(register _SPA_ITEM, *item)
IS {   /* CAVEAT: Much coersion/casting lies ahead, liable to errors. */
    switch (item->type) {
    case _SPA_Flag:
    case _SPA_Integer:
    case _SPA_KeyWord:
    case _SPA_Bits:
	*((int *)item->var) = (int)item->def;
	break;
    case _SPA_Float:
	*((float *)item->var) = *(float *)(item->def);
	break;
    case _SPA_String:
	*((char **)item->var) = (char *)item->def;
	break;
    case _SPA_InFile:
    case _SPA_OutFile:
	*((char **)item->arg) = (char *)item->def;
	if (*((char **)item->arg) && **((char **)item->arg))
	    *((FILE **)item->var) = fopen(*(char **)item->arg, mode(item->type));
	else *((FILE **)item->var) = file(item->type);
	assertFile(item);
	break;
    case _SPA_Function:
    case _SPA_Help:
    case _SPA_None:
	break;
    default:
	spaErr(SpaStrILF, item->name, 'F');
	break;
    }
}


/*----------------------------------------------------------------------
    execute()

    Execute an SPA_ITEM.
*/
PRIVATE PROCEDURE(execute, (item, option, on))
    IN(register _SPA_ITEM, *item) X
    IN(boolean, option) X	/* True if argv was an option */
    IN(register boolean, on)	/* True if argv was an on-option */
IS {   /* CAVEAT: Much coersion/casting lies ahead, liable to errors. */
    if (item) {

	if (option)
	    switch (item->type) {
	    case _SPA_Flag:
	    case _SPA_Function:
	    case _SPA_Help:
	    case _SPA_None:
		break;
	    default:
	    /* An option using next argv-item goes here */
		if (++pArg>=pArgC) {
		    --pArg;	/* Too far, backup */
		    setDefault(item);
		}
	    }
	
	switch (item->type) {
	case _SPA_Flag:
	    if (option) *((int *)item->var)= on;
	    else {		/* Parse the argument */
		*((int *)item->var) =
		    findKeyWord(pArgV[pArg], SpaFlagKeyWords, (int)item->def)&1 ;
	    }
	    break;
	case _SPA_Bits: {
	    register char *arg, *bp;
	    boolean bon = on;

	    for (arg= pArgV[pArg]; *arg; arg++) { /* Go thru argument */
		if (*arg=='-') { bon= !bon; continue; }
		for (bp= (char *)item->arg; *bp; bp++) /* and descriptor*/
		    if (lwr(*arg)==lwr(*bp)) break;
		if (*bp) {
		    if (bon)
			*((int *)item->var) |= 1<<(bp-(char *)item->arg);
		    else
			*((int *)item->var) &= ~(1<<(bp-(char *)item->arg));
		} else {
		    static char tmp[128];
		    sprintf(tmp, "%s: %c", item->name, *arg);
		    spaErr(SpaStrISC, tmp, 'W');
		}
	    }
	} break;
	case _SPA_Integer:
	    if (sscanf(pArgV[pArg], "%i", (int *)(item->var))!=1) {
		/* Number not ok */
		setDefault(item);
		spaErr(SpaStrIVE, pArgV[pArg], 'W');
	    }
	    break;
	case _SPA_Float:
	    if (sscanf(pArgV[pArg], "%g", (float *)(item->var))!=1) {
		/* Number not ok */
		setDefault(item);
		spaErr(SpaStrRVE, pArgV[pArg], 'W');
	    }
	    break;
	case _SPA_String:
	    *((char **)item->var)= pArgV[pArg];
	    break;
	case _SPA_KeyWord:
	    *((int *)item->var)= findKeyWord(pArgV[pArg], (char **)item->arg, (int)item->def);
	    break;
	case _SPA_InFile:
	case _SPA_OutFile:
	    *((char **)item->arg) = pArgV[pArg];
	    if (*((char **)item->arg) && **((char **)item->arg)) {
		if (*((FILE **)item->var)==file(item->type))
		    *((FILE **)item->var) = fopen(*(char **)item->arg, mode(item->type));
		else
		    *((FILE **)item->var) =
			freopen(*(char **)item->arg, mode(item->type), *((FILE **)item->var));
	    } else *((FILE **)item->var) = file(item->type);
	    assertFile(item);
	    break;
	case _SPA_Help:
	    safeExecute(item->var, item, pArgV[pArg], on);
	    report(pArguments, pOptions);
	    break;
	case _SPA_Function:
	case _SPA_None:
	    break;
	default:
	    spaErr(SpaStrILF, pArgV[pArg], 'S');
	    break;		/* This means that someone has dribbled with the */
	    /* option tables */
	}

	safeExecute(item->postFun, item, pArgV[pArg], on);
    }
}


/*----------------------------------------------------------------------
    option()

    Detect if an argv-item is an option, if so do as specified.
*/
PRIVATE boolean FUNCTION(option, (argvItem, options))
    IN(register char, *argvItem) X	/* User argument */
    IN(_SPA_ITEM, options[])		/* Possible options */
IS {
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
  	        spaErr(SpaStrNMO, argvItem, 'E');
	        break;
    	    case 1:
	        execute(&options[found], TRUE, start==1);
	        break;
    	    default:
  	        spaErr(SpaStrAMO, argvItem, 'E');
	        break;
    	    }
    	    return TRUE;
    	}
    }
    return FALSE;
}


/***********************************************************************
    Builtin functions.
*/

/*----------------------------------------------------------------------
    biErrFun()

    Error function, prints on stderr.
*/
PRIVATE SPA_ERRFUN(biErrFun) {
}

PRIVATE SPA_FUN(biExit) { exit(1); (prettyName, rawName, on); }

PRIVATE SPA_FUN(biArgTooMany) {
    spaErr(SpaStrTMA, rawName, 'W');
    (prettyName, on);
}

PRIVATE SPA_DECLARE(biArguments)
  SPA_FUNCTION("", "", biArgTooMany)
SPA_END

#ifdef __NEWC__
PRIVATE SPA_FUN(biUsage);	/* Forward */
#else
PRIVATE void biUsage();
#endif

PRIVATE SPA_DECLARE(biOptions)
#if SPA_LANG==46
  SPA_HELP("Hjälp", "Ger denna utskrift", biUsage, biExit)
#else
  SPA_HELP("help", "this help", biUsage, biExit)
#endif
SPA_END

PRIVATE SPA_FUN(biUsage) {
    register int i;

    printf("%s %s", SpaStrUsg, SpaProgramName);
    if (pOptions!=biOptions) {
    	printf(" [");
	for (i=0; pOptions[i].name; i++)
	    if (pOptions[i].type && *pOptions[i].name) {
		printf(pOName(&pOptions[i]));
		if (pOptions[i+1].name && *pOptions[i+1].name) printf("|");
	    }
    	printf("]...");
    }
    if (pArguments!=biArguments) {
	for (i=0; pArguments[i].name; i++)
	    if (pArguments[i].type && *pArguments[i].name) 
		printf(" %s", pAName(&pArguments[i]));
    }
    printf("\n");
    (prettyName, rawName, on);
}


/***********************************************************************
    Public functions.
*/

/*======================================================================
    spaArgument()

    Return Nth relative argument, NULL if outside argv.
*/
PUBLIC char * FUNCTION(spaArgument, (n))
    IN(int, n)
IS {
    register int t= pArg+n;
    
    return ( (t>=pArgC || t<0)? NULL: pArgV[t] );
}


/*======================================================================
    spaArgumentNo()

    Return Nth argument in argv, NULL if outside argv.
*/
PUBLIC char * FUNCTION(spaArgumentNo, (n))
    IN(register int, n)
IS {
    return ( (n>=pArgC || n<0)? NULL: pArgV[n] );
}


/*======================================================================
    spaSkip()

    Skip arguments N steps forward (or backward), stays inside argv.
*/
PUBLIC PROCEDURE(spaSkip, (n))
    IN(int, n)
IS {
    register int t= pArg+n;

    pArg= ( (t>=pArgC)? pArgC: (t<0? 0: t) );
}


/*======================================================================
    spaProcess()

    Will walk through the arguments and try to act according to spec's.
    Must not be called recursivly.
*/
PUBLIC int FUNCTION(_spaProcess, (argc, argv, arguments, options, errfun))
    IN(int, argc) X
    IN(char, *argv[]) X
    IN(_SPA_ITEM, arguments[]) X
    IN(_SPA_ITEM, options[]) X
    IN(SpaErrFun, *errfun)
IS {
    register int a, n;
    register char *s;
    
    fileDefault[0].errmsg = SpaStrFRE;
    fileDefault[1].errmsg = SpaStrFWE;

    pArgC= argc;
    pArgV= (char **)argv;
    pOptions= (options? options: biOptions);
    pArguments= (arguments? arguments: biArguments);
    pErrFun= (errfun? errfun: biErrFun);

    if (!*SpaProgramName) {	/* If no name given, get it from argv[0] */
    	s = strrchr(argv[0], '/');
    	SpaProgramName = s? s+1: argv[0];
    }

    for (n= 0; pArguments[n].name; n++) setDefault(&pArguments[n]);
    for (a= -1, n= 0; pOptions[n].name; n++) {
	setDefault(&pOptions[n]);
    	if (pOptions[n].type==_SPA_Help) a = n;
    }
    if (a<0) {				/* No help declared */
	pOptions[n] = biOptions[0];	/* Insert builtin d:o */
    }

    for (a= n= 0, pArg= 1; pArg<pArgC; pArg++)
	if (!option(pArgV[pArg], pOptions)) {
	    n++;
	    if (pArguments[a].name) {
		execute(&pArguments[a], FALSE, TRUE);
		if (pArguments[a+1].name) a++;
	    }
	}
    return n;
}

#ifdef THINK_C
#include <console.h>

PUBLIC int FUNCTION(_spaPreProcess, (argc, argv, arguments, options, errfun))
    IN(int, *argc) X
    IN(char, **argv[]) X
    IN(_SPA_ITEM, arguments[]) X
    IN(_SPA_ITEM, options[]) X
    IN(SpaErrFun, *errfun)
IS {
   *argc = ccommand(argv);
   return _spaProcess(*argc, *argv, arguments, options, errfun);
}
#endif


/* == EoF =========================================================== */
