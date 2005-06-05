/*----------------------------------------------------------------------*\

	smScSema.c

	ScannerMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


#include "sysdep.h"

#include "types.h"

#include "alan.h"
#include "lmList.h"
#include "encode.h"

/* For open, read & close */
#ifdef __win__
#include <fcntl.h>
#endif
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
#ifdef __mac__
#include <fcntl.h>
#endif


/* END %%IMPORT */
#include "smScan.h"

/* %%DECLARATION */


#include "str.h"

#define COPYMAX (smThis->smLength>256?256:smThis->smLength)


/* PUBLIC */
smScContext lexContext = NULL;	/* Scanner context */

List *fileNames = NULL;

int scannedLines();


/* PRIVATE */
static int lines = 0;		/* Updated at end of each file */

Bool smScanEnter(char fnm[],	/* IN - Name of file to open */
		 Bool search	/* IN - Search the include paths */
){
  smScContext this;
  char fnmbuf[300] = "";

  this = smScNew(sm_MAIN_MAIN_Scanner);
  if (fnm == NULL)
    this->fd = 0;
  else {
    List *ip;

    if (search) {
      strcpy(fnmbuf, fnm);
#ifdef THINK_C
      if ((this->fd = open(fnmbuf, O_TEXT)) < 0) { /* Does automatic <cr> to <nl> conversion */
#else
      if ((this->fd = open(fnmbuf, 0)) < 0) {
#endif
	for (ip = importPaths; ip != NULL; ip = ip->next) {
	  strcpy(fnmbuf, ip->element.str);
#ifndef __mac__
	  if (ip->element.str[strlen(ip->element.str)] != '/')
	    strcat(fnmbuf, "/");
#endif
	  strcat(fnmbuf, fnm);
#ifdef THINK_C
	  if ((this->fd = open(fnmbuf, O_TEXT)) > 0)
#else
	  if ((this->fd = open(fnmbuf, 0)) > 0)
#endif
	    break;
	}
	if (ip == NULL)
	  return FALSE;
      }
    } else {
      strcat(fnmbuf, fnm);
#ifdef THINK_C
      if ((this->fd = open(fnmbuf, O_TEXT)) < 0)
#else
      if ((this->fd = open(fnmbuf, 0)) < 0)
#endif
	return FALSE;
    }
  }

  /* Remember the filename */
  this->fileName = newString(fnmbuf);
  fileNames = concat(fileNames, this->fileName, STRING_LIST);
  this->fileNo = fileNo++;
  this->previous = lexContext;
  lexContext = this;

  return TRUE;
}

int scannedLines(void)
{
  return(lines - 1);
}


extern unsigned char *smMap;
extern unsigned char *smDFAcolVal;
extern unsigned char *smDFAerrCol;
extern unsigned char smIsoMap[256];
extern unsigned char smIsoDFAcolVal[256];
extern unsigned char smIsoDFAerrCol[256];
extern unsigned char smMacMap[256];
extern unsigned char smMacDFAcolVal[256];
extern unsigned char smMacDFAerrCol[256];
extern unsigned char smDosMap[256];
extern unsigned char smDosDFAcolVal[256];
extern unsigned char smDosDFAerrCol[256];

static int charset;

void setCharacterSet(int set)
{
  charset = set;
  switch (set) {
  case 0:
    smMap = &smIsoMap[0];
    smDFAcolVal = &smIsoDFAcolVal[0];
    smDFAerrCol = &smIsoDFAerrCol[0];
    break;
  case 1:
    smMap = &smMacMap[0];
    smDFAcolVal = &smMacDFAcolVal[0];
    smDFAerrCol = &smMacDFAerrCol[0];
    break;
  case 2:
    smMap = &smDosMap[0];
    smDFAcolVal = &smDosDFAcolVal[0];
    smDFAerrCol = &smDosDFAerrCol[0];
    break;
  }
}



/* END %%DECLARATION */

int smScReader(
     smScContext smThis,
     unsigned char *smBuffer,
     unsigned int smLength)
{


#ifdef __MWERKS__
 /* Metrowerks does not do automatic <cr> to <nl> conversion on text files!!! */
  {
    int count, pos;

    count = read(smThis->fd, (char *)smBuffer, smLength);
    for (pos = 0; pos < count; pos++)
      if (smBuffer[pos] == '\r')
	smBuffer[pos] = '\n';

    return count;
  }
#else
  return read(smThis->fd, (char *)smBuffer, smLength);
#endif


}    


int smScAction(
     smScContext smThis,
     int smInternalCode,
     Token *smToken)
{
  enum {
    smSkipToken		= -1,
    smContinueToken	= -2
  };
  switch(smInternalCode) {
  case 137:		/* INTEGER*/ 
    {
	smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, COPYMAX)] = '\0';
    
}
    break;

  case 138:		/* IDENTIFIER*/ 
    {
	smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, COPYMAX)] = '\0';
        if (charset != NATIVECHARSET)
          toNative(smToken->chars, smToken->chars, charset);
    
}
    break;

  case 139:		/* IDENTIFIER*/ 
    {{
	char *c;

	/* If terminated by \n illegal! */
	if (smThis->smText[smThis->smLength-1] == '\n')
	  lmLog(&smToken->srcp, 152, sevERR, "");

	smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 1, COPYMAX-1)] = '\0';
	/* Replace any doubled quotes by single */
	for (c = strchr(smToken->chars, '\''); c; c = strchr(c, '\'')) {
	    strcpy(c, &c[1]);
	    c++;
	}
    }
}
    break;

  case 140:		/* STRING*/ 
    {
      int len = 0;		/* The total length of the copied data */
      Bool space = FALSE;
      int i, c;

      smToken->fpos = ftell(txtfil); /* Remember where it starts */
      smThis->smText[smThis->smLength-1] = '\0';
      if (charset != 0) /* Convert string from non ISO characters if needed */
        toIso((char *)&smThis->smText[1], (char *)&smThis->smText[1], charset);

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
    
}
    break;

  case  94:		/* 'import'*/ 
    {
      Srcp srcp, start;
      Token token;
      int i;
      char c;

      smThis->smScanner = sm_MAIN_FILENAME_Scanner;
      smScan(smThis, &token);		/* Get file name */
      smThis->smScanner = sm_MAIN_MAIN_Scanner;
      if (token.code == sm_MAIN_IDENTIFIER_Token) {
	/* Found an ID which is a file name */

	/* Skip all whitespace */
	do {
	  i = smScSkip(smThis, 1);
	  c = smThis->smText[smThis->smLength-1];
	} while (isspace(c));

	if (c == '.') {
	  i = smScSkip(smThis, 1);
	} else {
	  lmLog(&token.srcp, 109, sevERR, "expected terminating '.'");
	  i = smScSkip(smThis, -1);
	}

	srcp = token.srcp;	/* Insert the file before next line */
	srcp.line++;
	srcp.col = 1;

	if (smScanEnter(token.chars, TRUE)) {
	  start.file = fileNo-1;
	  start.line = 0;	/* Start at beginning */
	  lmLiEnter(&srcp, &start, lexContext->fileName);
	} else
	  lmLog(&token.srcp, 199, sevFAT, token.chars);
      } else
	lmLog(&token.srcp, 151, sevFAT, token.chars); /* Not a file name */
  
}
    break;

  case 127:		/* 'location'*/ 
    {
  smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  90:		/* 'actor'*/ 
    {
  smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  95:		/* 'opaque'*/ 
    {
  smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  24:		/* 'on'*/ 
    {
  smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  27:		/* 'of'*/ 
    {
  smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  86:		/* 'first'*/ 
    {
  smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  67:		/* 'into'*/ 
    {
  smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  96:		/* 'taking'*/ 
    {
  smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case 145:		/* INCLUDE*/ 
    {
      Srcp srcp, start;
      Token token;
      int i;
      char c;

      smThis->smScanner = sm_MAIN_FILENAME_Scanner;
      smToken->srcp.startpos = smThis->smPosition;
      smToken->srcp.endpos = smThis->smNextPosition;
      smScan(smThis, &token);		/* Get file name */
      smThis->smScanner = sm_MAIN_MAIN_Scanner;
      if (token.code == sm_MAIN_IDENTIFIER_Token) {
	/* Found an ID which is a file name */
	srcp = token.srcp;	/* Insert the file before next line */
	srcp.line++;
	srcp.col = 1;

	/* Skip to end of line or EOF */
	do {
	  i = smScSkip(smThis, 1);
	  c = smThis->smText[smThis->smLength-1];
	} while (c != '\n' && i != 0);

      	lmLog(&smToken->srcp, 154, sevWAR, token.chars); /* INCLUDE is deprecated */
	if (smScanEnter(token.chars, TRUE)) {
	  smToken->srcp.file = fileNo-1;
	  start.file = fileNo-1;
	  start.line = 0;	/* Start at beginning */
	  lmLiEnter(&srcp, &start, lexContext->fileName);
	  /* Use the new scanner to get next token and return it */
	  return smScan(lexContext, smToken);
	} else /* Did not find the file */
	  lmLog(&token.srcp, 199, sevFAT, token.chars);
      } else  /* Did not scan any file name */
	lmLog(&token.srcp, 151, sevFAT, token.chars);
  
}
    break;

  case 146:		/* IDENTIFIER*/ 
    {{
	smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 1, COPYMAX-1)] = '\0';
    }
}
    break;
  }
  return smToken->code;
}

int smScPostHook(
     smScContext smThis,
     Token *smToken)
{
  enum {
    smSkipToken		= -1
  };


  smToken->srcp.file = smThis->fileNo;
  smToken->srcp.startpos = smThis->smPosition;
  smToken->srcp.endpos = smThis->smNextPosition;
  if (smToken->code == sm_MAIN_ENDOFTEXT_Token) {
    lines += smThis->smLine;
    close(smThis->fd);
    if (smThis->previous) {
      lexContext = smThis->previous;
      smScDelete(smThis);
      return smScan(lexContext, smToken);
      }
  }


  return smToken->code;
}

