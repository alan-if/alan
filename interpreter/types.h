#ifndef _ATYPES_H_
#define _ATYPES_H_
/*----------------------------------------------------------------------*\

  types.h

  Header file for the Alan interpreter module.

\*----------------------------------------------------------------------*/


#include "sysdep.h"
#include "acode.h"


/* CONSTANTS */

#ifndef __mac__
#ifndef TRUE
#define TRUE (0==0)
#endif
#ifndef FALSE
#define FALSE (!TRUE)
#endif
#endif

#define ACTMIN (header->actmin)
#define ACTMAX (header->actmax)
#define OBJMIN (header->objmin)
#define OBJMAX (header->objmax)
#define LOCMIN (header->locmin)
#define LOCMAX (header->locmax)
#define CNTMIN (header->cntmin)
#define CNTMAX (header->cntmax)
#define LITMIN (header->locmax+1)
#define LITMAX (header->locmax+1+litCount)
#define EVTMIN (header->evtmin)
#define EVTMAX (header->evtmax)

#define HERO ACTMIN


#define addrTo(x) (&memory[x])

/* The word classes are represented as numbers but in the dictonary they are generated as bits */
#define isVerb(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_VRB))!=0)
#define isConj(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_CONJ))!=0)
#define isBut(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_BUT))!=0)
#define isThem(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_THEM))!=0)
#define isIt(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_IT))!=0)
#define isNoun(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_NOUN))!=0)
#define isAdj(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_ADJ))!=0)
#define isPrep(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_PREP))!=0)
#define isAll(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_ALL))!=0)
#define isDir(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_DIR))!=0)
#define isNoise(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_NOISE))!=0)
#define isLiteral(word) (word >= dictsize)


/* TYPES */

#ifndef __mac__
typedef int Boolean;		/* Boolean values within interpreter */
#endif

/* Amachine variables */
typedef struct CurVars {
  int
    vrb,
    obj,
    loc,
    act,
    tick,
    score,
    visits;
} CurVars;

/* The various tables */
typedef struct WrdElem {	/* Dictionary */
  Aaddr wrd;			/* ACODE address to string */
  Aword class;			/* Word class */
  Aword code;
  Aaddr adjrefs;		/* Address to reference list */
  Aaddr nounrefs;		/* Address to reference list */
} WrdElem;

typedef struct ActElem {	/* ACTOR TABLE */
  Aword loc;			/* Location */
  Abool describe;		/* Description flag */
  Aaddr nam;			/* Address to name printing code */
  Aaddr atrs;			/* Address to attribute list */
  Aword cont;			/* Code for the container props if any */
  Aword script;			/* Which script is he using */
  Aaddr scradr;			/* Address to script table */
  Aword step;
  Aword count;
  Aaddr vrbs;
  Aaddr dscr;			/* Address of description code */
} ActElem;

typedef struct ScrElem {	/* SCRIPT TABLE */
  Aword code;			/* Script number */
  Aaddr dscr;			/* Optional description statements */
  Aaddr steps;			/* Address to steps */
} ScrElem;

typedef struct StepElem {	/* STEP TABLE */
  Aword after;			/* After how many ticks? */
  Aaddr exp;			/* Address to expression saying when */
  Aaddr stm;			/* Address to the actual code */
} StepElem;

typedef struct LocElem {	/* LOCATION TABLE */
  Aaddr nams;			/* Address of name printing code */
  Aaddr dscr;			/* Address of description code */
  Aaddr does;			/* Address of does code */
  Aword describe;		/* Description flag & counter */
  Aaddr atrs;			/* Address of attribute list */
  Aaddr exts;			/* Address of exit list */
  Aaddr vrbs;			/* Address of local verb list */
} LocElem;

typedef struct ExtElem {	/* EXIT TABLE structure */
  Abool done;			/* Flag for reverse/convert process */
  Aword code;			/* Direction code */
  Aaddr checks;			/* Address of check table */
  Aaddr action;			/* Address of action code */
  Aword next;			/* Number of next location */
} ExtElem;

typedef struct ChkElem {	/* CHECK TABLE */
  Aaddr exp;			/* ACODE address to expression code */
  Aaddr stms;			/* ACODE address to statement code */
} ChkElem;

typedef struct VrbElem {	/* VERB TABLE */
  Aword code;			/* Code for the verb */
  Aaddr alts;			/* Address to alternatives */
} VrbElem;

typedef struct StxElem {	/* SYNTAX TABLE */
  Aword code;			/* Code for verb word */
  Aaddr elms;			/* Address to element tables */
} StxElem;

typedef struct ElmElem26 {	/* ELEMENT TABLES */
  Aword code;			/* Code for this element, 0 -> parameter */
  Abool multiple;		/* May be multiple (if parameter) */
  Aaddr next;			/* Address to next element table ... */
				/* ... or class check if EOS */
} ElmElem26;

typedef struct ElmElem {	/* ELEMENT TABLES */
  Aword code;			/* Code for this element, 0 -> parameter */
  Aword flags;			/* Flags for multiple/omni (if parameter) */
				/* CHANGED: v2.7 from Abool for multiple */
  Aaddr next;			/* Address to next element table ... */
				/* ... or class check if EOS */
} ElmElem;

typedef struct ClaElem {	/* CLASS DEFINITION TABLE */
  Aword code;			/* Parameter number */
  Aword classes;		/* Parameter classes */
  Aaddr stms;			/* Exception statements */
} ClaElem;

typedef struct AltElem {	/* VERB ALTERNATIVE TABLE */
  Abool done;			/* Flag for patching (reverse/convert) process */
  Aword param;			/* Parameter number */
  Aword qual;			/* Verb execution qualifier */
  Aaddr checks;			/* Address of the check table */
  Aaddr action;			/* Address of the action code */
} AltElem;

typedef struct AtrElem {	/* ATTRIBUTE LIST */
  Aword val;			/* Its value */
  Aaddr stradr;			/* Address to the name */
} AtrElem;

typedef struct ObjElem25 {	/* OBJECT TABLE of 2.5 format*/
  Aword loc;			/* Current location */
  Abool describe;		/* Describe flag */
  Aaddr atrs;			/* Address of attribute list */
  Aword cont;			/* Index to container properties if any */
  Aaddr vrbs;			/* Address to local verb table */
  Aaddr dscr1;			/* Address to Aword description code */
  Aaddr dscr2;			/* Address to short description code */
} ObjElem25;

typedef struct ObjElem {	/* OBJECT TABLE */
  Aword loc;			/* Current location */
  Abool describe;		/* Describe flag */
  Aaddr atrs;			/* Address of attribute list */
  Aword cont;			/* Index to container properties if any */
  Aaddr vrbs;			/* Address to local verb table */
  Aaddr dscr1;			/* Address to Aword description code */
  Aaddr art;			/* Article printing code? Else use default */
				/* INTRODUCED: v2.6 */
  Aaddr dscr2;			/* Address to short description code */
} ObjElem;

typedef struct CntElem {	/* CONTAINER TABLE */
  Aaddr lims;			/* Address to limit check code */
  Aaddr header;			/* Address to header code */
  Aaddr empty;			/* Address to empty code */
  Aword parent;			/* Object or actor index */
  Aaddr nam;			/* Address to statement printing name */
} CntElem;

typedef struct LimElem {	/* LIMIT Type */
  Aword atr;			/* Attribute that limits */
  Aword val;			/* And the limiting value */
  Aaddr stms;			/* Statements if fail */
} LimElem;

typedef struct RulElem {	/* RULE TABLE */
  Abool run;			/* Is rule already run? */
  Aaddr exp;			/* Address to expression code */
  Aaddr stms;			/* Address to run */
} RulElem;

typedef struct EvtElem {	/* EVENT TABLE */
  Aaddr stradr;			/* Address to name string */
  Aaddr code;			/* Address of code to run */
} EvtElem;

typedef struct EvtqElem {	/* EVENT QUEUE ELEMENT */
  int time;
  int event;
  int where;
} EvtqElem;

typedef struct IniElem {	/* STRING INITIALISATION TABLE */
  Aword fpos;			/* File position */
  Aword len;			/* Length */
  Aword adr;			/* Where to store the string */
} IniElem;

typedef struct MsgElem26 {	/* MESSAGE TABLE */
  Aword fpos;			/* File position */
  Aword len;			/* Length of message */
} MsgElem26;

typedef struct MsgElem {	/* MESSAGE TABLE */
  Aaddr stms;			/* Address to statements*/
				/* Changed v2.7 from fpos+len in .dat */
} MsgElem;


typedef struct ParamElem {	/* PARAMETER */
  Aword code;			/* Code for this parameter (0=multiple) */
  Aword firstWord;		/* Index to first word used by player */
  Aword lastWord;			/* d:o to last */
} ParamElem;

typedef enum Type {TYPNUM, TYPSTR} Type;

typedef struct LitElem {	/* LITERAL */
  Type type;
  Aword value;
} LitElem;

#define MAXPARAMS 9
#define MAXENTITY (header->actmax)

#endif
