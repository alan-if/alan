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

#define HERO (header->theHero)
#define OBJECT (header->objectClassId)
#define LOCATION (header->locationClassId)
#define ACTOR (header->actorClassId)


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

typedef struct AdminEntry {	/* Administrative data about instances */
  Abool alreadyDescribed;
  Aint visitsCount;
} AdminEntry;


/* The various tables */
typedef struct WrdEntry {	/* Dictionary */
  Aaddr wrd;			/* ACODE address to string */
  Aword class;			/* Word class */
  Aword code;
  Aaddr adjrefs;		/* Address to reference list */
  Aaddr nounrefs;		/* Address to reference list */
} WrdEntry;

typedef struct ActEntry {	/* ACTOR TABLE */
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
} ActEntry;

typedef struct ScrEntry {	/* SCRIPT TABLE */
  Aword code;			/* Script number */
  Aaddr dscr;			/* Optional description statements */
  Aaddr steps;			/* Address to steps */
} ScrEntry;

typedef struct StepEntry {	/* STEP TABLE */
  Aword after;			/* After how many ticks? */
  Aaddr exp;			/* Address to expression saying when */
  Aaddr stm;			/* Address to the actual code */
} StepEntry;

typedef struct LocEntry {	/* LOCATION TABLE */
  Aaddr nams;			/* Address of name printing code */
  Aaddr dscr;			/* Address of description code */
  Aaddr does;			/* Address of does code */
  Aword describe;		/* Description flag & counter */
  Aaddr atrs;			/* Address of attribute list */
  Aaddr exts;			/* Address of exit list */
  Aaddr vrbs;			/* Address of local verb list */
} LocEntry;

typedef struct ChkEntry {	/* CHECK TABLE */
  Aaddr exp;			/* ACODE address to expression code */
  Aaddr stms;			/* ACODE address to statement code */
} ChkEntry;

typedef struct VrbEntry {	/* VERB TABLE */
  Aword code;			/* Code for the verb */
  Aaddr alts;			/* Address to alternatives */
} VrbEntry;

typedef struct StxEntry {	/* SYNTAX TABLE */
  Aword code;			/* Code for verb word */
  Aaddr elms;			/* Address to element tables */
} StxEntry;

typedef struct ElmEntry26 {	/* SYNTAX ELEMENT TABLES v2.6 */
  Aword code;			/* Code for this element, 0 -> parameter */
  Abool multiple;		/* May be multiple (if parameter) */
  Aaddr next;			/* Address to next element table ... */
				/* ... or class check if EOS */
} ElmEntry26;

typedef struct ElmEntry {	/* SYNTAX ELEMENT TABLES */
  Aword code;			/* Code for this element, 0 -> parameter */
  Aword flags;			/* Flags for multiple/omni (if parameter) */
				/* CHANGED: v2.7 from Abool for multiple */
  Aaddr next;			/* Address to next element table ... */
				/* ... or class check if EOS */
} ElmEntry;

typedef struct ClaEntry {	/* CLASS DEFINITION TABLE */
  Aword code;			/* Parameter number */
  Aword classes;		/* Parameter classes */
  Aaddr stms;			/* Exception statements */
} ClaEntry;

typedef struct AltEntry {	/* VERB ALTERNATIVE TABLE */
  Abool done;			/* Flag for patching (reverse/convert) process */
  Aword param;			/* Parameter number */
  Aword qual;			/* Verb execution qualifier */
  Aaddr checks;			/* Address of the check table */
  Aaddr action;			/* Address of the action code */
} AltEntry;

typedef struct AtrEntry {	/* ATTRIBUTE LIST */
  Aword val;			/* Its value */
  Aaddr stradr;			/* Address to the name */
} AtrEntry;

typedef struct ClassEntry {	/* CLASS TABLE */
  Aword code;			/* Own code */
  Aword parent;			/* Code for the parent class, 0 if none */
} ClassEntry;


/* InstanceEntry moved to acode.h */

typedef struct ObjEntry25 {	/* OBJECT TABLE of 2.5 format*/
  Aword loc;			/* Current location */
  Abool describe;		/* Describe flag */
  Aaddr atrs;			/* Address of attribute list */
  Aword cont;			/* Index to container properties if any */
  Aaddr vrbs;			/* Address to local verb table */
  Aaddr dscr1;			/* Address to Aword description code */
  Aaddr dscr2;			/* Address to short description code */
} ObjEntry25;

typedef struct ObjEntry {	/* OBJECT TABLE */
  Aword loc;			/* Current location */
  Abool describe;		/* Describe flag */
  Aaddr atrs;			/* Address of attribute list */
  Aword cont;			/* Index to container properties if any */
  Aaddr vrbs;			/* Address to local verb table */
  Aaddr dscr1;			/* Address to Aword description code */
  Aaddr art;			/* Article printing code? Else use default */
				/* INTRODUCED: v2.6 */
  Aaddr dscr2;			/* Address to short description code */
} ObjEntry;

typedef struct CntEntry {	/* CONTAINER TABLE */
  Aaddr lims;			/* Address to limit check code */
  Aaddr header;			/* Address to header code */
  Aaddr empty;			/* Address to empty code */
  Aword parent;			/* Object or actor index */
  Aaddr nam;			/* Address to statement printing name */
} CntEntry;

typedef struct LimEntry {	/* LIMIT Type */
  Aword atr;			/* Attribute that limits */
  Aword val;			/* And the limiting value */
  Aaddr stms;			/* Statements if fail */
} LimEntry;

typedef struct RulEntry {	/* RULE TABLE */
  Abool run;			/* Is rule already run? */
  Aaddr exp;			/* Address to expression code */
  Aaddr stms;			/* Address to run */
} RulEntry;

typedef struct EvtEntry {	/* EVENT TABLE */
  Aaddr stradr;			/* Address to name string */
  Aaddr code;			/* Address of code to run */
} EvtEntry;

typedef struct EvtqEntry {	/* EVENT QUEUE ENTRIES */
  int time;
  int event;
  int where;
} EvtqEntry;

typedef struct IniEntry {	/* STRING INITIALISATION TABLE */
  Aword fpos;			/* File position */
  Aword len;			/* Length */
  Aword adr;			/* Where to store the string */
} IniEntry;

typedef struct MsgEntry26 {	/* MESSAGE TABLE */
  Aword fpos;			/* File position */
  Aword len;			/* Length of message */
} MsgEntry26;

typedef struct MsgEntry {	/* MESSAGE TABLE */
  Aaddr stms;			/* Address to statements*/
				/* Changed v2.7 from fpos+len in .dat */
} MsgEntry;


typedef struct ParamEntry {	/* PARAMETER */
  Aword code;			/* Code for this parameter (0=multiple) */
  Aword firstWord;		/* Index to first word used by player */
  Aword lastWord;			/* d:o to last */
} ParamEntry;

typedef enum LiteralType {NUMERIC_LITERAL, STRING_LITERAL} LiteralType;

typedef struct LiteralEntry {	/* LITERAL */
  LiteralType type;
  Aword value;
} LiteralEntry;

#define MAXPARAMS 9
#define MAXENTITY (header->actmax)

#endif
