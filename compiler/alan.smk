%%OPTIONS

    Prefix 'sm';
    Pack RDS, GCS, LES, ERROR;

%%IMPORT

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

%%EXPORT

#include <iconv.h>

#include "sysdep.h"
#include "types.h"
#include "charset.h"

#include "lst_x.h"
#include "options.h"


extern smScContext lexContext;

/* List of file names */
extern List *fileNames;

extern bool smScanEnter(Srcp srcp, char fnm[], bool search);
extern int scannedLines(void);
extern void setCharacterSet(CharSet charset);

%%DECLARATIONS

#include <limits.h>

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

static bool find_and_open_in_path_element(smScContext this, List *ip, char fnm[]) {
    char fnmbuf[PATH_MAX+1] = "";

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

static char *open_import(smScContext this, char fnm[], bool search) {
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


bool smScanEnter(Srcp srcp,     /* IN - The source position of the import statement */
                 char fnm[],	/* IN - Name of file to open */
                 bool search	/* IN - Search the import paths? */
){
    smScContext this;
    char *prefix = "";

    for (List *p = fileNames; p != NULL; p = p->next) {
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


%%CONTEXT

  smScContext previous;
  int fd;
  char *fileName;
  int fileNo;
  int previousCharSet;
  iconv_t conversionDescriptor;


%%READER

  if (currentCharSet == CHARSET_UTF8) {
      return readWithConversionFromUtf8(smThis->fd, smThis->conversionDescriptor, smBuffer, smLength);
  } else
      return read(smThis->fd, (char *)smBuffer, smLength);


%%POSTHOOK

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

%%MAP

  [A-Z]		= [a-z];
  [\xC0-\xD6]	= [\xe0-\xf6];
  [\xD8-\xDE]	= [\xf8-\xfe];

%%DEFINITIONS

  letter	= [a-z\xdf\xe0-\xf6\xf8-\xfe];
  digit		= [0-9];

------------------------------------------------------------------------------
%%VOCABULARY main

%%SCANNER main

%%RULES

  Integer = digit+
    %%
    smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, COPYMAX)] = '\0';
    %%;


  IDENTIFIER = letter (letter ! digit ! '_')*			-- normal id
    %%
    smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, COPYMAX)] = '\0';
    %%;

  IDENTIFIER = '\'' ([^\'\n]!'\'''\'')* ('\'' ! '\n')		-- quoted id
    %%{
    /* If terminated by \n illegal! */
    if (smThis->smText[smThis->smLength-1] == '\n')
      lmlog(&smToken->srcp, 152, sevERR, "");

    smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 1, COPYMAX-1)] = '\0';
    /* Replace any doubled quotes by single */
    for (char *c = strchr(smToken->chars, '\''); c; c = strchr(c, '\'')) {
        strmov(c, &c[1]);
        c++;
    }
    }%%;


  STRING = '"' ([^"]!'"''"')* '"'
    %%
        int len = 0;		/* The total length of the copied data */
        bool space = false;

        smToken->fpos = ftell(txtfil); /* Remember where it starts */
        smThis->smText[smThis->smLength-1] = '\0';

        for (int i = 1; i < smThis->smLength-1; i++) {
            int c = smThis->smText[i];

            /* Write the character */
            if (isspace(c)) {
                if (!space) {		/* Are we looking at spaces? */
                    space = true;
                    /* No, so output a space and remember */
                    putc(' ', txtfil);
                    incFreq(' ');
                    len++;
                }
            } else {
                space = false;
                putc(c, txtfil);
                incFreq(c);
                len++;
                if (c == '"') i++;	/* skip second '"' */
            }
        }
        smToken->len = len;
    %%;

  'import' = 'import'
    %%
        Srcp srcp, start;
        Token token;
        static int i;
        static char c;

        smThis->smScanner = sm_MAIN_FILENAME_Scanner;
        smScan(smThis, `&token);		/* Get file name */
        smThis->smScanner = sm_MAIN_MAIN_Scanner;
        if (token.code == sm_MAIN_IDENTIFIER_Token) {
            /* Found an ID which is a file name */

            /* Skip all whitespace */
            do {
                i = smScSkip(smThis, 1);
                c = smThis->smText[smThis->smLength-1];
            } while (isspace((int)c) && i > 0);

            if (c != '.') {
                lmlog(`&token.srcp, 109, sevERR, "expected terminating '.'");
                i = smScSkip(smThis, -1);
            }

            srcp = token.srcp;	/* Insert the file before next line */
            srcp.line++;
            srcp.col = 1;

            if (smScanEnter(token.srcp, token.chars, true)) {
                start.file = fileNo-1;
                start.line = 0;	/* Start at beginning */
                lmLiEnter(`&srcp, `&start, lexContext->fileName);
            }
        } else
            lmlog(`&token.srcp, 151, sevFAT, token.chars); /* Not a file name */
  %%;

  'location' = 'location'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'actor' = 'actor'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'opaque' = 'opaque'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'visits' = 'visits'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'on' = 'on'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'it' = 'it'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'contains' = 'contains'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'of' = 'of'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'first' = 'first'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'into' = 'into'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'taking' = 'taking'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  'off' = 'off'
    %%
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 0, smThis->smLength)] = '\0';
    %%;

  Unknown = _Unknown;

  EndOfText = _EndOfText;

%%SKIP

  blanks = [ \r\n\t\x0d]+;

  comment = '--' [^\n\r]*;

  block_comment = '////'
    %%
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
    %%;

  include = '$include'
    %%
      Srcp srcp, start;
      Token token;
      int i;
      int c;

      smThis->smScanner = sm_MAIN_FILENAME_Scanner;
      smToken->srcp.startpos = smThis->smPosition;
      smToken->srcp.endpos = smThis->smNextPosition;
      smScan(smThis, `&token);		/* Get file name */
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

          lmlog(`&smToken->srcp, 154, sevERR, token.chars); /* INCLUDE is deprecated */

          if (smScanEnter(token.srcp, token.chars, true)) {
              smToken->srcp.file = fileNo-1;
              start.file = fileNo-1;
              start.line = 0;	/* Start at beginning */
              lmLiEnter(`&srcp, `&start, lexContext->fileName);
              /* Use the new scanner to get next token and return it */
              return smScan(lexContext, smToken);
          }
      } else  /* Did not scan any file name */
          lmlog(`&token.srcp, 151, sevFAT, token.chars);
  %%;


%%SCANNER filename

%%RULES

  Identifier = '\'' ([^\']!'\'''\'')* '\''	-- quoted id
    %%{
        smToken->chars[smScCopy(smThis, (unsigned char *)smToken->chars, 1, COPYMAX-1)] = '\0';
    }%%;

  Unknown = _Unknown;

  EndOfText = _EndOfText;

%%SKIP

  blanks = [ \n\t]+;
