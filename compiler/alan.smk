%%OPTIONS

	Prefix 'sm';
	Pack RDS, GCS, LES, Error;

%%IMPORT

#include "sysdep.h"

#include "types.h"

#include "alan.h"
#include "lmList.h"
#include "encode.h"

/* For open, read & close */
#ifdef __sun__
#include <sys/unistd.h>
#include <fcntl.h>
#endif
#ifdef __vms__
#include <unixio.h>
#endif
#ifdef __dos__
#include <io.h>
#endif
#ifdef __mac__
#include <fcntl.h>
#endif


%%EXPORT

#include "sysdep.h"
#include "types.h"

extern smScContext lexContext;

#ifdef _PROTOTYPES_
extern Bool smScanEnter(char fnm[]);
extern int scannedLines(void);
#else
extern Bool smScanEnter();
extern int scannedLines();
#endif

%%DECLARATIONS

#define COPYMAX (smThis->smLength>256?256:smThis->smLength)


/* PUBLIC */
smScContext lexContext = NULL;	/* Scanner context */

int scannedLines();


/* PRIVATE */
static lines = 0;		/* Updated at end of each file */

#ifdef _PROTOTYPES_
Bool smScanEnter(
	char fnm[]		/* IN - Name of file to open */
){
#else
Bool smScanEnter(fnm)
	char fnm[];		/* IN - Name of file to open */
{
#endif
  smScContext this;

  this = smScNew(sm_MAIN_MAIN_Scanner);
  if (fnm == NULL)
    this->fd = 0;
#ifndef _mac_
  else if ((this->fd = open(fnm, 0)) < 0)
#else
  else if ((this->fd = open(fnm, O_TEXT)) < 0)
#endif
    return FALSE;
  else {
    this->fileName = fnm;
    this->fileNo = fileNo++;
    this->previous = lexContext;
    lexContext = this;
  }
  return TRUE;
}

#ifdef _PROTOTYPES_
int scannedLines(void)
#else
int scannedLines()
#endif
{
  return(lines - 1);
}


%%CONTEXT

  smScContext previous;
  int fd;
  char *fileName;
  int fileNo;


%%READER

  if (verbose) {
	printf(".");
	fflush(NULL);
  }
  return read(smThis->fd, (char *)smBuffer, smLength);


%%POSTHOOK

  smToken->srcp.file = smThis->fileNo;
  if (smToken->code == sm_MAIN_ENDOFTEXT_Token) {
    lines += smThis->smLine;
    close(smThis->fd);
    if (smThis->previous) {
      lexContext = smThis->previous;
      smScDelete(smThis);
      return smScan(lexContext, smToken);
      }
  }

%%MAP

  [a-}]		= [A-\]];		-- include old-style nation characters
  [\xe0-\xf6]	= [\xC0-\xD6];
  [\xf8-\xfe]	= [\xD8-\xDE];

%%DEFINITIONS

  letter	= [A-\]\xC0-\xD6\xD8-\xDE];
  digit		= [0-9];

------------------------------------------------------------------------------
%%VOCABULARY main

%%SCANNER main

%%RULES

  INTEGER = digit+
    %%
	smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, COPYMAX)] = '\0';
    %%;


  ID = letter (letter ! digit ! '_')*			-- normal id
    %%
	smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, COPYMAX)] = '\0';
	(void) strlow(smToken->chars);
    %%;

  ID = '\'' ([^\']!'\'''\'')* '\''	-- quoted id
    %%{
	char *c;

	smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 1, COPYMAX-1)] = '\0';
	for (c = strchr(smToken->chars, '\''); c; c = strchr(c, '\'')) {
	    strcpy(c, &c[1]);
	    c++;
	}
    }%%;


  STRING = '"' ([^"]!'"''"')* '"'
    %%
      int len = 0;		/* The total length of the copied data */
      Bool space = FALSE;
      int i, c;

      smToken->fpos = ftell(txtfil); /* Remember where it starts */
      smThis->smText[smThis->smLength-1] = '\0';
#if ISO == 0
      toIso((char *)&smThis->smText[1], (char *)&smThis->smText[1]);
#endif

      for (i = 1; i < smThis->smLength-1; i++) {
	/* Write the character */
	if (isspace(c = smThis->smText[i])) {
	  if (!space) {		/* Are we looking at spaces? */
	    /* No, so output a space and remember */
	    putc(' ', txtfil);
	    incFreq(' ');
	    space = TRUE;
	    len++;
	  }
        } else {
	  putc(c, txtfil);
	  incFreq(c);
	  space = FALSE;
	  len++;
	  if (c == '"') i++;	/* skip second '"' */
	}
      }
      smToken->len = len;
    %%;

  Unknown = _Unknown;

  EndOfText = _EndOfText;

%%SKIP

  blanks = [ \n\t]+;

  comment = '--' [^\n]*;

  include = '$INCLUDE' 
    %%
      Srcp srcp, start;
      Token token;
      int i;
      char c;

      smThis->smScanner = sm_MAIN_FILENAME_Scanner;
      smScan(smThis, `&token);		/* Get file name */
      smThis->smScanner = sm_MAIN_MAIN_Scanner;
      if (token.code == sm_MAIN_ID_Token) {
	/* Found an ID which is a file name */
	do {
	  i = smScSkip(smThis, 1);
	  c = smThis->smText[smThis->smLength-1];
	} while (c != '\n' && i != 0); /* Skip to end of line or EOF */

	srcp = token.srcp;	/* Insert the file before next line */
	srcp.line++;
	srcp.col = 1;

	if (smScanEnter(token.chars)) {
	  start.file = fileNo-1;
	  start.line = 0;	/* Start at beginning */
	  lmLiEnter(`&srcp, `&start, token.chars);
	  /* Use the new scanner to get next token and return it */
	  return smScan(lexContext, smToken);
	} else
	  lmLog(`&token.srcp, 199, sevFAT, token.chars);
      } else
	lmLog(`&token.srcp, 151, sevFAT, token.chars); /* Not a file name */
  %%;


%%SCANNER filename

%%RULES

  ID = '\'' ([^\']!'\'''\'')* '\''	-- quoted id
    %%{
	smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 1, COPYMAX-1)] = '\0';
    }%%;

  Unknown = _Unknown;

  EndOfText = _EndOfText;

%%SKIP

  blanks = [ \n\t]+;


