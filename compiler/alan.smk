%%OPTIONS
	Prefix 'sm';

%%DECLARATIONS

#include "sysdep.h"

#include "types.h"

#include "alan.h"
#include "lmList.h"
#include "encode.h"

/* For open, read & close */
#ifdef __sun__
#include <unistd.h>
#include <fcntl.h>
#endif
#ifdef __vms__
#include <unixio.h>
#endif
#ifdef __dos__
#include <io.h>
#endif


/* PUBLIC */
smContext lexContext = NULL;	/* Scanner context */

int scannedLines();

%%CODE

/* PRIVATE */
static lines = 0;		/* Updated at end of each file */

Boolean smScanEnter(fnm)
	char fnm[];		/* IN - Name of file to open */
{
  smContext this;

  this = smNew(sm_main_Scanner);
  if (fnm == NULL)
    this->fd = 0;
  else if ((this->fd = open(fnm,0)) < 0)
    return FALSE;
  else {
    this->fileName = fnm;
    this->fileNo = fileNo++;
    this->previous = lexContext;
    lexContext = this;
  }
  return TRUE;
}

int scannedLines()
{
  return(lines - 1);
}


%%CONTEXT

  smContext previous;
  int fd;
  char *fileName;
  int fileNo;


%%READER

  if (verbose) printf(".");
  return read(this->fd, smBuffer, smLength);


%%POSTHOOK

  smToken->srcp.file = this->fileNo;


%%MAP

  [a-}]		= [A-\]];
  [\xe0-\xf6]	= [\xc0-\xd6];
  [\xf8-\xfe]	= [\xd8-\xde];


%%DEFINITIONS

  letter	= [A-\]\xc0-\xd6\xd8-\xde];
  digit		= [0-9];

------------------------------------------------------------------------------

%%SCANNER main

%%RULES

  INTEGER = digit+
    %%
	smToken->chars[smCopy(this, smToken->chars, 0, this->smLength)] = '\0';
    %%;


  ID = letter (letter ! digit ! '_')*			-- normal id
    %%
	smToken->chars[smCopy(this, smToken->chars, 0, this->smLength)] = '\0';
	(void) strlow(smToken->chars);
    %%;

  ID = '\'' ([^\']!'\'''\'')* '\''	-- quoted id
    %%{
	char *c;

	smToken->chars[smCopy(this, smToken->chars, 1, this->smLength-1)] = '\0';
	for (c = strchr(smToken->chars, '\''); c; c = strchr(c, '\'')) {
	    strcpy(c, &c[1]);
	    c++;
	}
    }%%;


  STRING = '"' ([^"]!'"''"')* '"'
    %%
      int len = 0;		/* The total length of the copied data */
      Boolean space = FALSE;
      int i, c;

      smToken->fpos = ftell(txtfil); /* Remember where it starts */

      for (i = 1; i < this->smLength-1; i++) {
	/* Write the character */
	if (isspace(c = this->smText[i])) {
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

  EndOfText = _EndOfText
    %%
      lines += this->smLine;
      close(this->fd);
      if (this->previous) {
	lexContext = this->previous;
	smDelete(this);
        return smScan(lexContext, smToken);
      }
    %%;

%%SKIP

  blanks = [ \n\t]+;

  comment = '--' [^\n]* [\n];

  include = '$INCLUDE' 
    %%
      Srcp srcp, start;
      Token token;
      int i;
      char c;

      smScrOff(sm_main_ID_Token);
      smScan(this, `&token);		/* Get file name */
      smScrOn(sm_main_ID_Token);
      if (token.code == sm_main_ID_Token) {
	/* Found an ID which is a file name */
	do {
	  i = smSkip(this, 1);
	  c = this->smText[this->smLength-1];
	} while(c != '\n' && i != 0); /* Skip to end of line or EOF */

	srcp = token.srcp;	/* Insert the file before next line */
	srcp.line++;
	srcp.col = 1;

	if (smScanEnter(token.chars)) {
	  start.file = fileNo-1;
	  start.line = 0;	/* Start at beginning */
	  lmEnter(`&srcp, `&start, token.chars);
	  /* Use the new scanner to get next token and return it */
	  return smScan(lexContext, smToken);
	} else
	  lmLog(`&token.srcp, 199, sevFAT, token.chars);
      } else
	lmLog(`&token.srcp, 151, sevFAT, token.chars); /* Not a file name */
  %%;


