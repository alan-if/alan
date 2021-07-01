/*----------------------------------------------------------------------*\

    smScSema.c

    ScannerMaker generated semantic actions

\*----------------------------------------------------------------------*/

/* %%IMPORT */


  /* For open, read & close */
#include <fcntl.h>

  /* For iconv conversion */
#include <errno.h>
#include "converter.h"
#include "charset.h"

#include "sysdep.h"

#include "types.h"

#include "alan.h"
#include "lmlog.h"
#include "encode.h"
#include "util.h"

/* END %%IMPORT */
#include "smScan.h"

/* %%DECLARATION */


#include "str.h"

#define COPYMAX (smThis->smLength>256?256:smThis->smLength)


/* PUBLIC */
smScContext lexContext = NULL;	/* Scanner context */

int scannedLines();


/* PRIVATE */
static int currentCharSet;
static int lines = 0;		/* Updated at end of each file */
#ifndef O_BINARY
#define O_BINARY 0
#endif

static Bool find_and_open_in_path_element(smScContext this, List *ip, char fnm[]) {
    char fnmbuf[PATH_MAX] = "";

    if (ip != NULL) {
        strcpy(fnmbuf, ip->member.str);
        if (fnmbuf[strlen(fnmbuf)-1] != '/')
            strcat(fnmbuf, "/");
    }
    strcat(fnmbuf, fnm);
    /* We need to convert filenames to NATIVE charset/encoding */
    if (currentCharSet == CHARSET_UTF8) {
        char *converted = ensureExternalEncoding(fnmbuf);
        strcpy(fnmbuf, converted);
        free(converted);
    }

    return (this->fd = open(fnmbuf, O_RDONLY|O_BINARY)) > 0;
}

static char *open_import(smScContext this, char fnm[], Bool search) {
    List *ip;

    if (search) {
        if (!find_and_open_in_path_element(this, NULL, fnm)) {
            for (ip = importPaths; ip != NULL; ip = ip->next) {
                if (find_and_open_in_path_element(this, ip, fnm))
                    break;
            }
            if (ip)
                return ip->member.str;    /* Return the prefix found */
            else
                return NULL;
        }

    } else {
        if (!find_and_open_in_path_element(this, NULL, fnm))
            return NULL;              /* Return not found */
    }
    return "";
}

static void register_filename(smScContext this, char *prefix, char filename[]) {
    char *full_name = allocate(strlen(prefix)+strlen(filename)+2);
    if (prefix != NULL) {
        strcpy(full_name, prefix);
        if (strlen(prefix) > 0 && prefix[strlen(prefix)-1] != '/')
            strcat(full_name, "/");
    }
    strcat(full_name, filename);
    this->fileName = newString(full_name);
    fileNames = concat(fileNames, this->fileName, STRING_LIST);
    free(full_name);
}


static void switch_scanner(smScContext this) {
    /* Remember encoding for the current file */
    this->previousCharSet = currentCharSet;

    this->fileNo = fileNo++;
    this->previous = lexContext;
    lexContext = this;

    /* Pre-read to see if there is UTF BOM... */
    uchar BOM[3] = {0xEF,0xBB,0xBF};
    uchar leading[3];
    int len = read(this->fd, leading, 3);
    if (len == 3 && memcmp(leading, BOM, 3) == 0)
       /* Enforce UTF-8 */
       currentCharSet = CHARSET_UTF8;
    else
       /* Not a BOM, restart reading from beginning */
       lseek(this->fd, 0, SEEK_SET);
}


void setCharacterSet(CharSet set)
{
  currentCharSet = set;
}


Bool smScanEnter(Srcp srcp,     /* IN - The source position of the import statement */
                 char fnm[],	/* IN - Name of file to open */
                 Bool search	/* IN - Search the import paths? */
){
    smScContext this;
    List *p;
    char *prefix = "";

    for (p = fileNames; p != NULL; p = p->next) {
        if (strcmp(fnm, p->member.str) == 0) {
            lmlog(&srcp, 197, sevWAR, fnm);
            return false;
        }
    }

    this = smScNew(sm_MAIN_MAIN_Scanner);
    if (fnm == NULL)
        this->fd = 0;
    else {
        if (!(prefix = open_import(this, fnm, search))) {
            lmlog(&srcp, 199, sevFAT, fnm);
            return false;
        }
    }

    register_filename(this, prefix, fnm);
    switch_scanner(this);
    if (currentCharSet == CHARSET_UTF8) {
        this->conversionDescriptor = initUtf8Conversion();
    }

    return true;
}

int scannedLines(void)
{
  return(lines - 1);
}



/* END %%DECLARATION */

int smScReader(
     smScContext smThis,
     unsigned char *smBuffer,
     unsigned int smLength)
{


  if (currentCharSet == CHARSET_UTF8) {
      return readWithConversionFromUtf8(smThis->fd, smThis->conversionDescriptor, smBuffer, smLength);
  } else
      return read(smThis->fd, (char *)smBuffer, smLength);



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
  case 146:		/* INTEGER*/ 
    {
    smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, COPYMAX)] = '\0';
    
}
    break;

  case 147:		/* IDENTIFIER*/ 
    {
    smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, COPYMAX)] = '\0';
    
}
    break;

  case 148:		/* IDENTIFIER*/ 
    {{
    char *c;

    /* If terminated by \n illegal! */
    if (smThis->smText[smThis->smLength-1] == '\n')
      lmlog(&smToken->srcp, 152, sevERR, "");

    smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 1, COPYMAX-1)] = '\0';
    /* Replace any doubled quotes by single */
    for (c = strchr(smToken->chars, '\''); c; c = strchr(c, '\'')) {
        strmov(c, &c[1]);
        c++;
    }
    }
}
    break;

  case 149:		/* STRING*/ 
    {
        int len = 0;		/* The total length of the copied data */
        Bool space = false;
        int i, c;

        smToken->fpos = ftell(txtfil); /* Remember where it starts */
        smThis->smText[smThis->smLength-1] = '\0';

        for (i = 1; i < smThis->smLength-1; i++) {
            /* Write the character */
            if (isspace(c = smThis->smText[i])) {
                if (!space) {		/* Are we looking at spaces? */
                    /* No, so output a space and remember */
                    putc(' ', txtfil);
                    incFreq(' ');
                    space = true;
                    len++;
                }
            } else {
                putc(c, txtfil);
                incFreq(c);
                space = false;
                len++;
                if (c == '"') i++;	/* skip second '"' */
            }
        }
        smToken->len = len;
    
}
    break;

  case  98:		/* 'import'*/ 
    {
        Srcp srcp, start;
        Token token;
        static int i;
        static char c;

        smThis->smScanner = sm_MAIN_FILENAME_Scanner;
        smScan(smThis, &token);		/* Get file name */
        smThis->smScanner = sm_MAIN_MAIN_Scanner;
        if (token.code == sm_MAIN_IDENTIFIER_Token) {
            /* Found an ID which is a file name */

            /* Skip all whitespace */
            do {
                i = smScSkip(smThis, 1);
                c = smThis->smText[smThis->smLength-1];
            } while (isspace((int)c) && i > 0);

            if (c != '.') {
                lmlog(&token.srcp, 109, sevERR, "expected terminating '.'");
                i = smScSkip(smThis, -1);
            }

            srcp = token.srcp;	/* Insert the file before next line */
            srcp.line++;
            srcp.col = 1;

            if (smScanEnter(token.srcp, token.chars, true)) {
                start.file = fileNo-1;
                start.line = 0;	/* Start at beginning */
                lmLiEnter(&srcp, &start, lexContext->fileName);
            }
        } else
            lmlog(&token.srcp, 151, sevFAT, token.chars); /* Not a file name */
  
}
    break;

  case 133:		/* 'location'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  94:		/* 'actor'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case 101:		/* 'opaque'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case 100:		/* 'visits'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  25:		/* 'on'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  21:		/* 'it'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case 132:		/* 'contains'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  28:		/* 'of'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  90:		/* 'first'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  72:		/* 'into'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case 102:		/* 'taking'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case  46:		/* 'off'*/ 
    {
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    
}
    break;

  case 154:		/* BLOCK_COMMENT*/ 
    {
       int i;
       int c;
       Srcp srcp = smToken->srcp;

       srcp.line=smThis->smNextLine;
       srcp.col = smThis->smNextColumn-4;
       if (smThis->smColumn != 1)
              lmlog(&srcp, 156, sevERR, "");

       /* We are reading files in binary mode so take care for CRLF:s */
       do {
          // Skip rest of line
          do {
              i = smScSkip(smThis, 1);
              c = smThis->smText[smThis->smLength-1];
          } while (c != '\n' && i != 0); // not newline and not end-of-file (actually read a character = 1)
          if (i == 0) {
              // end-of-file!
              lmlog(&srcp, 155, sevERR, "");
              break;
          }

          // Did next line start with four forward slashes
          for (int n=0; n<4; n++) {
              i = smScSkip(smThis, 1);
              c = smThis->smText[smThis->smLength-1];
              if (c != '/')
                 break;
          }

          // Was the last from the previous loop a slash then we had four of them
          if (c == '/') {
              // if the rest was also slashes, this was the last line of the block comment
              do {
                  i = smScSkip(smThis, 1);
                  c = smThis->smText[smThis->smLength-1];
              } while (c == '/' && i != 0);
              if (c != '\n' && c != '\r')
                  // we found something else on this line, so ...
                  continue;
              else {
                  if (c == '\r') { /* A CR so look for LF */
                      i = smScSkip(smThis, 1);
                      c = smThis->smText[smThis->smLength-1];
                      if (c != '\n')  /* Followed by newline? */
                          /* If not that was a spurrious CR, so ... */
                          continue;
                  }
                  // end-of-line and only slashes. Done!
                  break;         
              }
          } else
              continue;
       } while (1);
    
}
    break;

  case 155:		/* INCLUDE*/ 
    {
      Srcp srcp, start;
      Token token;
      int i;
      int c;

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

          lmlog(&smToken->srcp, 154, sevERR, token.chars); /* INCLUDE is deprecated */

          if (smScanEnter(token.srcp, token.chars, true)) {
              smToken->srcp.file = fileNo-1;
              start.file = fileNo-1;
              start.line = 0;	/* Start at beginning */
              lmLiEnter(&srcp, &start, lexContext->fileName);
              /* Use the new scanner to get next token and return it */
              return smScan(lexContext, smToken);
          }
      } else  /* Did not scan any file name */
          lmlog(&token.srcp, 151, sevFAT, token.chars);
  
}
    break;

  case 156:		/* IDENTIFIER*/ 
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
    if (currentCharSet == CHARSET_UTF8)
        finishUtf8Conversion(smThis->conversionDescriptor);
    currentCharSet = smThis->previousCharSet;
    if (smThis->previous) {
      lexContext = smThis->previous;
      smScDelete(smThis);
      return smScan(lexContext, smToken);
      }
  }


  return smToken->code;
}

