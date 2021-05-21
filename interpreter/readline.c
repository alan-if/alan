/*----------------------------------------------------------------------

  Read line from user, with editing and history

  ----------------------------------------------------------------------*/

#include "readline.h"

#include "sysdep.h"
#include "output.h"
#include "term.h"
#include "exe.h"
#include "save.h"
#include "Location.h"
#include "converter.h"

#include "options.h"


#define LINELENGTH 1000

#ifdef UNITTESTING
/* Transform some stdlib functions to mockable functions */
#define read(fd, buf, n) mocked_read(fd, buf, n)
extern ssize_t mocked_read (int __fd, char *__buf, size_t __nbytes);

#define write(fd, buf, n) mocked_write(fd, buf, n)
extern ssize_t write (int __fd, const void *__buf, size_t __n);

#endif


// TODO Try to split this into more obvious GLK and non-GLK modules
#ifdef HAVE_GLK

#include "options.h"

#include "glk.h"
#include "glkio.h"

#include "resources.h"


#ifdef HAVE_WINGLK
#include "WinGlk.h"

extern HINSTANCE myInstance;	/* Catched by winglk.c */


BOOL CALLBACK AboutDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            EndDialog(hwndDlg, wParam);
            return TRUE;
        }
    }
    return FALSE;
}

#endif



/*======================================================================

  readline() - GLK case

  Read a line from the user, with history and editing

*/

/* TODO - length of user buffer should be used */
bool readline(char buffer[])
{
    event_t event;
    static bool readingCommands = FALSE;
    static frefid_t commandFileRef;
    static strid_t commandFile;
#ifdef HAVE_WINGLK
    static frefid_t commandLogFileRef;
    static frefid_t transcriptFileRef;
    INT_PTR e;
#endif

    if (readingCommands) {
        if (glk_get_line_stream(commandFile, buffer, 255) == 0) {
            glk_stream_close(commandFile, NULL);
            readingCommands = FALSE;
            goto endOfCommandFile;
        } else {
            char *converted = ensureInternalEncoding(buffer);
            glk_set_style(style_Input);
            printf(converted);
            glk_set_style(style_Normal);
            free(converted);
        }
    } else {
    endOfCommandFile:
        glk_request_line_event(glkMainWin, buffer, 255, 0);
        /* FIXME: buffer size should be infallible: all existing calls use 256 or
           80 character buffers, except parse which uses LISTLEN (currently 100)
        */
        do
            {
                glk_select(&event);
                switch (event.type) {
                case evtype_Arrange:
                    statusline();
                    break;
#ifdef HAVE_WINGLK
                case winglk_evtype_GuiInput:
                    switch (event.val1) {
                    case ID_MENU_RESTART:
                        restartGame();
                        break;
                    case ID_MENU_SAVE:
                        glk_set_style(style_Input);
                        printf("save\n");
                        glk_set_style(style_Normal);
                        save();
                        para();
                        printf("> ");
                        break;
                    case ID_MENU_RESTORE:
                        glk_set_style(style_Input);
                        printf("restore\n");
                        glk_set_style(style_Normal);
                        restore();
                        look();
                        para();
                        printf("> ");
                        break;
                    case ID_MENU_RECORD:
                        if (commandLogOption) {
                            glk_stream_close(commandLogFile, NULL);
                            commandLogOption = FALSE;
                        }
                        commandLogFileRef = glk_fileref_create_by_prompt(fileusage_InputRecord+fileusage_TextMode, filemode_Write, 0);
                        if (commandLogFileRef == NULL) break;
                        commandLogFile = glk_stream_open_file(commandLogFileRef, filemode_Write, 0);
                        if (commandLogFile != NULL)
                            commandLogOption = TRUE;
                        break;
                    case ID_MENU_PLAYBACK:
                        commandFileRef = glk_fileref_create_by_prompt(fileusage_InputRecord+fileusage_TextMode, filemode_Read, 0);
                        if (commandFileRef == NULL) break;
                        commandFile = glk_stream_open_file(commandFileRef, filemode_Read, 0);
                        if (commandFile != NULL)
                            if (glk_get_line_stream(commandFile, buffer, 255) != 0) {
                                readingCommands = TRUE;
                                printf(buffer);
                                return TRUE;
                            }
                        break;
                    case ID_MENU_TRANSCRIPT:
                        if (transcriptOption) {
                            glk_stream_close(transcriptFile, NULL);
                            transcriptOption = FALSE;
                        }
                        transcriptFileRef = glk_fileref_create_by_prompt(fileusage_Transcript+fileusage_TextMode, filemode_Write, 0);
                        if (transcriptFileRef == NULL) break;
                        transcriptFile = glk_stream_open_file(transcriptFileRef, filemode_Write, 0);
                        if (transcriptFile != NULL) {
                            transcriptOption = TRUE;
                            glk_put_string_stream(transcriptFile, "> ");
                        }
                        break;
                    case ID_MENU_ABOUT:
                        e = DialogBox(myInstance, MAKEINTRESOURCE(IDD_ABOUT), NULL, &AboutDialogProc);
                        (void)e;
                        break;
                    }
                    break;
#endif
                }
            } while (event.type != evtype_LineInput);
        if (buffer[0] == '@') {
            buffer[event.val1] = 0;
            commandFileRef = glk_fileref_create_by_name(fileusage_InputRecord+fileusage_TextMode, &buffer[1], 0);
            commandFile = glk_stream_open_file(commandFileRef, filemode_Read, 0);
            if (commandFile != NULL)
                if (glk_get_line_stream(commandFile, buffer, 255) != 0) {
                    char *converted = ensureInternalEncoding(buffer);
                    glk_set_style(style_Input);
                    printf(converted);
                    glk_set_style(style_Normal);
                    free(converted);
                    readingCommands = TRUE;
                }
        } else
            buffer[event.val1] = 0;
    }
    return TRUE;
}

#else
/*---------------------------------------------------------------------------------------*/
/* Non-GLK terminal I/O using simple movements and overwrites with spaces and backspaces */
/*---------------------------------------------------------------------------------------*/

#include "sysdep.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#ifdef HAVE_TERMIOS
#include <termios.h>
#endif

#include <unistd.h>

#include "readline.h"

#include "memory.h"


#ifdef HAVE_TERMIOS
/*----------------------------------------------------------------------

  termio handling

  ----------------------------------------------------------------------*/

static struct termios term;

static void newtermio()
{
    struct termios newterm;
    tcgetattr(0, &term);
    newterm=term;
    newterm.c_lflag&=~(ECHO|ICANON);
    newterm.c_cc[VMIN]=1;
    newterm.c_cc[VTIME]=0;
    tcsetattr(0, TCSANOW, &newterm);
}

static void restoretermio()
{
    tcsetattr(0, TCSANOW, &term);
}

#endif


/*----------------------------------------------------------------------*\

  Global character buffers etc.

  ----------------------------------------------------------------------*/

static char buffer[LINELENGTH+1];
static int bufidx;

static unsigned char *history[HISTORYLENGTH];
static int histidx;		/* Index where to store next history */
static int histp;		/* Points to the history recalled last */

static unsigned char ch;
static int endOfInput = 0;
static bool change;
static bool insert = TRUE;


/*----------------------------------------------------------------------

  Character map types and maps

  ----------------------------------------------------------------------*/

typedef struct {unsigned char min, max; void (*hook)(char ch);} KeyMap;

/* Forward declaration of hooks */
static void escHook(char ch);
static void insertCh(char ch);
static void arrowHook(char ch);
static void upArrow(char ch);
static void downArrow(char ch);
static void rightArrow(char ch);
static void leftArrow(char ch);
static void insertToggle(char ch);
static void newLine(char ch);
static void delFwd(char ch);
static void delBwd(char ch);
static void execute(KeyMap map[], unsigned char ch);

#ifdef __windows__

static KeyMap keymap[] = {
    {0x00, 0x01, NULL},
    {0x02, 0x02, leftArrow},
    {0x03, 0x05, NULL},
    {0x06, 0x06, rightArrow},
    {0x07, 0x07, NULL},
    {0x08, 0x08, delBwd},
    {0x09, 0x09, NULL},
    {0x0a, 0x0a, newLine},
    {0x1b, 0x1b, escHook},
    {0x1c, 0x7e, insertCh},
    {0x7f, 0x7f, delFwd},
    {0x80, 0xff, insertCh},
    {0x00, 0x00, NULL}
};

static KeyMap escmap[] = {
    {0x00, 0x5a, NULL},
    {0x5b, 0x5b, arrowHook},
    {0x5c, 0xff, NULL},
    {0x00, 0x00, NULL}
};

static KeyMap arrowmap[] = {
    {0x00, 0x31, NULL},
    {0x32, 0x32, insertToggle},
    {0x33, 0x40, NULL},
    {0x41, 0x41, upArrow},
    {0x42, 0x42, downArrow},
    {0x43, 0x43, rightArrow},
    {0x44, 0x44, leftArrow},
    {0x45, 0xff, NULL},
    {0x00, 0x00, NULL}
};

#else

/* Not windows */

static void escapeBracket3Hook(char ch);
static void ignoreCh(char ch) {}

static KeyMap keymap[] = {
    {0x00, 0x03, NULL},
    {0x04, 0x04, delFwd},
    {0x05, 0x07, delBwd},
    {0x08, 0x08, delBwd},
    {0x09, 0x09, NULL},
    {0x0a, 0x0a, newLine},
    {0x0d, 0x0d, ignoreCh},
    {0x1b, 0x1b, escHook},
    {0x1c, 0x7e, insertCh},
    {0x7f, 0x7f, delBwd},
    {0x80, 0xff, insertCh},
    {0x00, 0x00, NULL}
};

static KeyMap escmap[] = {
    {0x00, 0x5a, NULL},
    {0x5b, 0x5b, arrowHook},
    {0x5c, 0xff, NULL},
    {0x00, 0x00, NULL}
};

static KeyMap arrowmap[] = {
    {0x00, 0x31, NULL},
    {0x32, 0x32, insertToggle},
    {0x33, 0x33, escapeBracket3Hook},
    {0x33, 0x40, NULL},
    {0x41, 0x41, upArrow},
    {0x42, 0x42, downArrow},
    {0x43, 0x43, rightArrow},
    {0x44, 0x44, leftArrow},
    {0x45, 0xff, NULL},
    {0x00, 0x00, NULL}
};

static KeyMap escapeBracket3map[] = {
    {0x7e, 0x7e, delFwd},
    {0x00, 0x00, NULL}
};


static void escapeBracket3Hook(char ch) {
    int rc;
    (void)rc;                   /* UNUSED */
    rc = read(0, &ch, 1);
    execute(escapeBracket3map, ch);
}

#endif


static void stripNewline(char *buffer) {
    int len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
        buffer[len-1] = '\0';
}

static bool is_utf8_prefix(uchar ch) {
    return (ch&0xC0) == 0xC0;
}

static bool is_utf8_follow(uchar ch) {
    return (ch&0xC0) == 0x80;
}

/* Number of characters counting multi-byte characters as one (as opposed to bytes) in a null-terminated array */
int character_length(uchar *string) {
    int count = 0;

    for (int i = 0; string[i] != '\0'; i++) {
        if (encodingOption == ENCODING_UTF && is_utf8_prefix(string[i])) {
            count++;
            i++;
            do {
                i++;
            } while ((string[i] != '\0') && is_utf8_follow(string[i]));
            i--;
        } else {
            count++;
        }
    }
    return count;
}

/* Number of bytes (as opposed to characters e.g. UTF-8) in a null-terminated array */
static int byte_length(char *bytes) {
    return strlen(bytes);
}



#ifdef UNITTESTING
#include <cgreen/mocks.h>
static void doBeep(void) { mock(); }
#else
static void doBeep(void)
{
    int rc;
    (void)rc;                   /* UNUSED */
    rc = write(1, "\7", 1);
}
#endif

static void moveCursorLeft(void)
{
    int rc;
    (void)rc;                   /* UNUSED */
    rc = write(1, "\b", 1);
}


static void writeBlank() {
    int rc;
    (void)rc;               /* UNUSED */
    rc = write(1, " ", 1);
}



static void erase()
{
    int i;

    /* Backup to beginning of text */
    /* TODO: This is wrong! Should not use strlen, but instead ...*/
    /* TODO: moveCursorBackOver(n); including UTF-8 chars */
    for (i = 0; i < character_length((uchar *)&buffer[bufidx]); i++)
        moveCursorLeft();

    /* Overwrite with spaces */
    for (i = 0; i < character_length((uchar *)buffer); i++) {
        writeBlank();
    }

    /* Backup to beginning of text */
    for (i = 0; i < character_length((uchar *)buffer); i++)
        moveCursorLeft();
}

/*----------------------------------------------------------------------*\

  Character handling hook functions

  ----------------------------------------------------------------------*/

static void execute(KeyMap map[], unsigned char ch)
{
    int i = 0;

    // TODO Make this loop until end of KeyMap (0,0,NULL) is found instead
    // then we can remove any NULL entries in the keymaps
    for (i = 0; i <= 256; i++) {
        if (i > 0 && map[i].min == 0x00) break; /* End marker is a 0,0,NULL */
        if (map[i].min <= ch && ch <= map[i].max) {
            if (map[i].hook != NULL) {
                map[i].hook(ch);
                return;
            } else
                doBeep();
        }
    }
    doBeep();
}


static void upArrow(char ch)
{
    int rc;
    (void)rc;                   /* UNUSED */
    /* Is there more history ? */
    if (history[(histp+HISTORYLENGTH-1)%HISTORYLENGTH] == NULL ||
        (histp+HISTORYLENGTH-1)%HISTORYLENGTH == histidx) {
        rc = write(1, "\7", 1);
        return;
    }

    erase();

    /* Backup history pointer */
    histp = (histp+HISTORYLENGTH-1)%HISTORYLENGTH;

    /* Copy the history and write it */
    strcpy((char *)buffer, (char *)history[histp]);
    bufidx = strlen((char *)buffer);
    rc = write(1, (void *)buffer, strlen((char *)buffer));
}


static void downArrow(char ch)
{
    /* Is there more history ? */
    if (histp == histidx) {
        int rc;
        (void)rc;                   /* UNUSED */
        rc = write(1, "\7", 1);
        return;
    }

    erase();

    /* Advance history pointer */
    histp = (histp+1)%HISTORYLENGTH;

    /* If we are not at the most recent history entry, copy history and write it */
    if (histp != histidx) {
        int rc;
        (void)rc;                   /* UNUSED */
        strcpy((char *)buffer, (char *)history[histp]);
        bufidx = strlen((char *)buffer);
        rc = write(1, (void *)buffer, strlen((char *)buffer));
    } else {
        bufidx = 0;
        buffer[0] = '\0';
    }
}


static void rightArrow(char ch)
{
    if (bufidx > LINELENGTH || buffer[bufidx] == '\0')
        doBeep();
    else {
        int rc;
        (void)rc;                   /* UNUSED */
        int count = 1;
        int startidx = bufidx;
        if (encodingOption == ENCODING_UTF) {
            if (is_utf8_prefix((uchar)buffer[bufidx])) {
                /* Next character is a multi-byte */
                while (is_utf8_follow((uchar)buffer[bufidx+1])) {
                    count++;
                    bufidx++;
                }
            }
        }
        bufidx++;
        rc = write(1, (void *)&buffer[startidx], count);
    }
}


static void leftArrow(char ch)
{
    if (bufidx == 0)
        doBeep();
    else {
        if (encodingOption == ENCODING_UTF) {
            /* This is moving backwards over UTF-8 characters */
            if (is_utf8_follow(buffer[bufidx-1])) {
                /* If top two bits are 10 then it's a UTF-8 follow-up byte, so backup till we find prefix */
                while (is_utf8_follow(buffer[--bufidx]))
                    ;
            } else
                bufidx--;       /* For an "ASCII" char just backup the single byte */
        } else
            bufidx--;
        moveCursorLeft();       /* And it's still just one character on the screen */
    }
}


static void insertToggle(char ch)
{
    int rc;
    (void)rc;                   /* UNUSED */
    rc = read(0, &ch, 1);
    if (ch != 'z')
        doBeep();
    else
        insert = !insert;
}


static void shiftBufferLeftFrom(int idx, int offset) {
    for (int i = 0; i <= byte_length((char *)&buffer[idx+offset])+1; i++) {
        buffer[idx+i] = buffer[idx+i+offset];
    }
}


static void writeBufferFrom(int idx) {
    int rc;
    (void)rc;                   /* UNUSED */
    rc = write(1, (void *)&buffer[idx], byte_length(&buffer[idx]));
}


static void moveCursorFromEndBackTo(int idx) {
    for (int i = 0; i <= character_length((uchar *)&buffer[idx]); i++)
        moveCursorLeft();
}



static void delBwd(char ch)
{
    (void)ch;                   /* UNUSED - to match other keymap functions */

    if (bufidx == 0)
        doBeep();
    else {
        int rc;
        (void)rc;                   /* UNUSED */
        int deleted_length = 1;

        change = TRUE;

        moveCursorLeft();            /* Move backwards over the deleted char */

        if (encodingOption == ENCODING_UTF && is_utf8_follow(buffer[bufidx-1])) {
            /* For multi-byte characters find the first non-follow, presumably the prefix */
            while (is_utf8_follow(buffer[--bufidx]))
                deleted_length++;
        } else
            bufidx--;       /* For a single-byte char just backup the single byte */

        /* Move up any remaning characters in the buffer ... */
        shiftBufferLeftFrom(bufidx, deleted_length);

        /* ... on the screen, print the rest of the string ... */
        writeBufferFrom(bufidx);
        writeBlank();           /* And erase the character at the end still left on screen */

        moveCursorFromEndBackTo(bufidx);   /* Move back to current position */
    }
}

static void delFwd(char ch)
{
    if (bufidx > LINELENGTH || buffer[bufidx] == '\0')
        doBeep();
    else {
        int i;
        int rc;
        (void)rc;                   /* UNUSED */

        change = TRUE;

        int deleted_length = 1;
        if (encodingOption == ENCODING_UTF && is_utf8_prefix(buffer[bufidx])) {
            /* For multi-byte characters find the first non-follow, presumably another prefix or non-multi-byte character */
            int idx = bufidx+1;
            while (is_utf8_follow(buffer[idx++]))
                deleted_length++;
        }
        shiftBufferLeftFrom(bufidx, deleted_length);
        writeBufferFrom(bufidx);
        writeBlank();
        for (i = 0; i <= character_length((uchar *)&buffer[bufidx]); i++)
            moveCursorLeft();
    }
}

static void escHook(char ch) {
    int rc;
    (void)rc;                   /* UNUSED */
    rc = read(0, &ch, 1);
    execute(escmap, ch);
}

static void arrowHook(char ch) {
    int rc;
    (void)rc;                   /* UNUSED */
    rc = read(0, &ch, 1);
    execute(arrowmap, ch);
}

static void newLine(char ch)
{
    int rc;
    (void)rc;                   /* UNUSED */

    endOfInput = 1;
    rc = write(1, "\n", 1);

    /* If the input is not the same as the previous, save it in the history */
    if (change && strlen((char *)buffer) > 0) {
        if (history[histidx] == NULL)
            history[histidx] = (unsigned char *)allocate(LINELENGTH+1);
        strcpy((char *)history[histidx], (char *)buffer);
        histidx = (histidx+1)%HISTORYLENGTH;
    }
}


static void shift_buffer_right_from(int idx) {
    for (int i = byte_length((char *)buffer); i >= idx; i--)
        buffer[i+1] = buffer[i];
}


static void moveCursorLeftTo(int idx) {
    for (int i = character_length((uchar *)&buffer[idx]); i > 0; i--)
        moveCursorLeft();
}


static void insertCh(char ch) {
    if (bufidx > LINELENGTH) {
        doBeep();
        return;
    }

    int rc;
    (void)rc;               /* UNUSED */

    /* If at end advance the NULL */
    if (buffer[bufidx] == '\0')
        buffer[bufidx+1] = '\0';

    if (insert) {
        static int bytes_left = 0;
        static int utf_start;

        shift_buffer_right_from(bufidx);

        if (encodingOption == ENCODING_UTF)  {
            buffer[bufidx++] = ch;
            if (is_utf8_prefix(ch)) {
                /* 110xxxxx -> 1 extra byte */
                /* 1110xxxx -> 2 extra bytes */
                /* 11110xxx -> 3 extra bytes */
                bytes_left = 1; /* TODO: For now... */
                utf_start = bufidx-1;
            } else if (bytes_left > 0) {
                bytes_left--;
                if (bytes_left == 0) {
                    writeBufferFrom(utf_start);
                    moveCursorLeftTo(bufidx);
                }
            } else {
                rc = write(1, &ch, 1);
                writeBufferFrom(bufidx);
                moveCursorLeftTo(bufidx);
            }
        } else {
            rc = write(1, (void *)&buffer[bufidx], byte_length(&buffer[bufidx]));
            for (int i = byte_length(&buffer[bufidx]); i > 0; i--)
                moveCursorLeft();
            buffer[bufidx] = ch;
            rc = write(1, &ch, 1);
            bufidx++;
        }
    } else {
        /* TODO: we can't just overwrite, if the new char is a two byte, and the current is a one byte... */
        buffer[bufidx] = ch;
        rc = write(1, &ch, 1);
        bufidx++;
    }
    change = TRUE;
}

#ifdef __win__
#include <windows.h>
#include <winbase.h>
#include <wincon.h>
#endif

/*----------------------------------------------------------------------*/
static void echoOff()
{
#ifdef HAVE_TERMIOS
    newtermio();
#else
#ifdef __win__

    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);

    (void) SetConsoleMode(handle, 0);

#endif
#endif
}


/*----------------------------------------------------------------------*/
static void echoOn()
{
#ifdef HAVE_TERMIOS
    restoretermio();
#else
#ifdef __win__

    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    (void) SetConsoleMode(handle, ENABLE_ECHO_INPUT);

#endif
#endif
}


/*======================================================================

  readline() - non-GLK case

  Read a line from the user, with history, editing and command
  reading from file

  NOTE that the raw characters (and thus the maps) are in the
  native/external encoding for the platform.

*/

/* TODO - length of user buffer should be used */
bool readline(char usrbuf[])
{
    static bool readingCommands = FALSE;
    static FILE *commandFile;

    if (readingCommands) {
        fflush(stdout);
        /* TODO: Arbitrarily using 255 for buffer sife */
        if (!fgets(buffer, 255, commandFile)) {
            fclose(commandFile);
            readingCommands = FALSE;
            buffer[0] = '\0';
            goto endOfCommandFile;
        } else {
            printf("%s", buffer);
        }
    } else {
    endOfCommandFile:
        fflush(stdout);
        bufidx = 0;
        histp = histidx;
        buffer[0] = '\0';
        change = TRUE;
        echoOff();
        endOfInput = 0;
        while (!endOfInput) {
            if (read(0, (void *)&ch, 1) != 1) {
                /* Not returning 1 means we did not get any character at all... */
                echoOn();
                return FALSE;
            }
            execute(keymap, ch);
        }
        echoOn();

        if (buffer[0] == '@')
            if ((commandFile = fopen(&buffer[1], "r")) != NULL)
                if (fgets(buffer, 255, commandFile)) {
                    readingCommands = TRUE;
                    printf("%s", buffer);
                }
        /* Reset line counter only if we read actual player input */
        lin = 1;
    }
    stripNewline(buffer);
    char *converted = ensureInternalEncoding(buffer);
    strcpy(usrbuf, converted);
    free(converted);
    return TRUE;
}

#endif
