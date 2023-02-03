#include <ncurses.h>
#include <shared/CommandReader.h>
#include <shared/StringLib.h>
#include <shared/FileHandling.h>
#include <shared/LibProject.h>
#include <string.h>
#include <assert.h>

#define CMDSIZE (1000)

Enum(ActionMode){
    NORMAL,
    COMMAND,
    INSERT,
    VISUAL,
    FIND,
};

Struct(FilePos)
{
    char *ptr;
    int ln, cn;
};

static int min(int a, int b)
{
    return (a < b ? a : b);
}
static int max(int a, int b)
{
    return (a < b ? b : a);
}

static ActionMode actionMode = NORMAL;
static char cmd[CMDSIZE] = {};
static char *cmdptr;
static char file[FILESIZE] = {};
static char clipboard[FILESIZE] = {};
static FilePos cursor, visBeg;
static int wln = 1;

void FilePrv()
{
    if (cursor.ptr == file)
        return;
    cursor.ptr--;
    cursor.ln = GetLnNum(file, cursor.ptr);
    cursor.cn = GetChNum(file, cursor.ptr);
}
void FileNxt()
{
    if (cursor.ptr[0] == 0)
        return;
    cursor.ptr++;
    cursor.ln = GetLnNum(file, cursor.ptr);
    cursor.cn = GetChNum(file, cursor.ptr);
}
void FileCursurLeft()
{
    if (cursor.cn == 0)
        return;
    cursor.ptr = GetPtrAt(file, cursor.ln, cursor.cn - 1);
    cursor.ln = GetLnNum(file, cursor.ptr);
    cursor.cn = GetChNum(file, cursor.ptr);
}
void FileCursurRight()
{
    cursor.ptr = GetPtrAt(file, cursor.ln, cursor.cn + 1);
    cursor.ln = GetLnNum(file, cursor.ptr);
    cursor.cn = GetChNum(file, cursor.ptr);
}
void FileCursurDown()
{
    cursor.ptr = GetPtrAt(file, cursor.ln + 1, cursor.cn);
    cursor.ln = GetLnNum(file, cursor.ptr);
    cursor.cn = GetChNum(file, cursor.ptr);
}
void FileCursurUp()
{
    cursor.ptr = GetPtrAt(file, cursor.ln - 1, cursor.cn);
    cursor.ln = GetLnNum(file, cursor.ptr);
    cursor.cn = GetChNum(file, cursor.ptr);
}

void Copy(char *ptr, int sz)
{
    for (int i = 0; i < sz; i++)
        if ((clipboard[i] = ptr[i]))
            return;
    clipboard[sz] = 0;
}
void Paste(char *ptr)
{
    InsertPattern(ptr, clipboard);
    cursor.ptr += strlen(clipboard);
    cursor.ln = GetLnNum(file, cursor.ptr);
    cursor.cn = GetChNum(file, cursor.ptr);
}

int CommandMode(int inp)
{
    switch (inp)
    {
    case 27:
        return 1;

    case KEY_LEFT:
        if (cmdptr > cmd)
            cmdptr--;
        break;
    case KEY_RIGHT:
        if (cmdptr[0])
            cmdptr++;
        break;
    case KEY_HOME:
        cmdptr = cmd;
        break;
    case KEY_END:
        cmdptr = GetEndLine(cmdptr);
        break;

    case KEY_BACKSPACE:
        if (cmd[0] == 0)
            return 1;
        if (cmdptr > cmd)
            EraseSubstring(cmdptr - 1, 1), cmdptr--;
        break;
    case KEY_DC:
        EraseSubstring(cmdptr, 1);
        break;

    case '\n':
        return 1;

    default:
        char ch[2] = "a";
        ch[0] = inp;
        InsertPattern(cmdptr, ch);
        cmdptr++;
        break;
    }
    return 0;
}

int NormalMode(int inp)
{
    switch (inp)
    {
    case ':':
        actionMode = COMMAND;
        cmdptr = cmd;
        break;
    case 'i':
        actionMode = INSERT;
        break;
    case 'v':
        actionMode = VISUAL;
        visBeg = cursor;
        break;

    case KEY_LEFT:
    case 'h':
        FileCursurLeft();
        break;
    case KEY_DOWN:
    case 'j':
        FileCursurDown();
        break;
    case KEY_UP:
    case 'k':
        FileCursurUp();
        break;
    case KEY_RIGHT:
    case 'l':
        FileCursurRight();
        break;
    case KEY_HOME:
        cursor.ptr = GetPtrAt(file, cursor.ln, 0);
        cursor.cn = GetChNum(file, cursor.ptr);
        break;
    case KEY_END:
        cursor.ptr = GetEndLine(cursor.ptr);
        cursor.cn = GetChNum(file, cursor.ptr);
        break;
    case ' ':
        FileNxt();
        break;
    case KEY_BACKSPACE:
        FilePrv();
        break;

    case 'd':
    case KEY_DC:
        Copy(cursor.ptr, 1);
        EraseSubstring(cursor.ptr, 1);
        break;
    case 'y':
        Copy(cursor.ptr, 1);
        break;
    case 'p':
        Paste(cursor.ptr);
        break;

    default:
        break;
    }
    return 0;
}

int VisualMode(int inp)
{
    char *from = (cursor.ptr < visBeg.ptr ? cursor.ptr : visBeg.ptr);
    char *to = (cursor.ptr > visBeg.ptr ? cursor.ptr : visBeg.ptr);
    switch (inp)
    {
    case 27:
        return 1;

    case KEY_LEFT:
    case 'h':
        FileCursurLeft();
        break;
    case KEY_DOWN:
    case 'j':
        FileCursurDown();
        break;
    case KEY_UP:
    case 'k':
        FileCursurUp();
        break;
    case KEY_RIGHT:
    case 'l':
        FileCursurRight();
        break;
    case KEY_HOME:
        cursor.ptr = GetPtrAt(file, cursor.ln, 0);
        cursor.cn = GetChNum(file, cursor.ptr);
        break;
    case KEY_END:
        cursor.ptr = GetEndLine(cursor.ptr);
        cursor.cn = GetChNum(file, cursor.ptr);
        break;
    case ' ':
        FileNxt();
        break;
    case KEY_BACKSPACE:
        FilePrv();
        break;

    case 'd':
    case KEY_DC:
        Copy(from, to - from + 1);
        EraseSubstring(from, to - from + 1);
        cursor.ptr = from;
        cursor.ln = GetLnNum(file, cursor.ptr);
        cursor.cn = GetChNum(file, cursor.ptr);
        return 1;
    case 'y':
        Copy(from, to - from + 1);
        return 1;

    default:
        break;
    }
    return 0;
}

int InsertMode(int inp)
{
    switch (inp)
    {
    case 27:
        return 1;

    case KEY_LEFT:
        FileCursurLeft();
        break;
    case KEY_DOWN:
        FileCursurDown();
        break;
    case KEY_UP:
        FileCursurUp();
        break;
    case KEY_RIGHT:
        FileCursurRight();
        break;
    case KEY_HOME:
        cursor.ptr = GetPtrAt(file, cursor.ln, 0);
        cursor.cn = GetChNum(file, cursor.ptr);
        break;
    case KEY_END:
        cursor.ptr = GetEndLine(cursor.ptr);
        cursor.cn = GetChNum(file, cursor.ptr);
        break;

    case KEY_BACKSPACE:
        if (cursor.ptr > file)
            FilePrv(), EraseSubstring(cursor.ptr, 1);
        break;
    case KEY_DC:
        EraseSubstring(cursor.ptr, 1);
        break;

    default:
        char ch[2] = "a";
        ch[0] = inp;
        InsertPattern(cursor.ptr, ch), FileNxt();
        break;
    }
    return 0;
}

int FindMode(int inp)
{
}

int tick(int inp)
{
    if (inp)
        switch (actionMode)
        {
        case COMMAND:
            int rs = CommandMode(inp);
            if (rs == 1)
                actionMode = NORMAL;
            break;
        case NORMAL:
            return NormalMode(inp);
        case INSERT:
            if (InsertMode(inp))
                actionMode = NORMAL;
            break;
        case VISUAL:
            if (VisualMode(inp))
                actionMode = NORMAL;
            break;
        case FIND:
            if (FindMode(inp))
                actionMode = NORMAL;
            break;
        }
    return 0;
}

void renderLine(char *ptr, int ln, int y)
{
    move(y, 0);
    // colorize?
    printw("%d", ln);
    move(y, 4);

    for (; ptr[0] && ptr[0] != '\n'; ptr++)
    {
        // colorize?
        printw("%c", ptr[0]);
    }
    printw("\n");
}

void render()
{
    int lnc = CountLines(file);
    // printw("%d", lnc);

    for (int i = 0; i < min(lnc - wln + 1, LINES - 2); i++)
    {
        char *wptr = GetPtrAt(file, wln + i, 0);
        renderLine(wptr, wln + i, i);
    }

    move(LINES - 2, 0);
    switch (actionMode)
    {
    case NORMAL:
        printw("NORMAL");
        break;
    case COMMAND:
        printw("COMMAND");
        break;
    case INSERT:
        printw("INSERT");
        break;
    case VISUAL:
        printw("VISUAL");
        break;
    case FIND:
        printw("FIND");
        break;

    default:
        break;
    }
    move(LINES - 2, 8);
    move(LINES - 1, 0);
    if (actionMode == COMMAND)
    {
        printw(":%s", cmd);
        move(LINES - 1, cmdptr - cmd + 1);
    }
    else
        move(cursor.ln - wln, cursor.cn + 4);
}

int main()
{
    WINDOW *wnd = initscr();
    keypad(wnd, 1);
    cbreak();
    noecho();

    strcpy(file, "testtet tea t at\n teat \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nss ate\n etast");

    cursor.ln = 1, cursor.cn = 0, cursor.ptr = file;

    render();
    refresh();
    while (1)
    {
        int k = getch();
        if (tick(k))
            break;
        wln = min(wln, cursor.ln - 4);
        wln = max(wln, cursor.ln - LINES + 7);
        wln = max(1, wln);

        clear();
        render();
        refresh();
    }
    endwin();

    return 0;
}