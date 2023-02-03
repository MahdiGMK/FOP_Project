#include <ncurses.h>
#include <shared/CommandReader.h>
#include <shared/StringLib.h>
#include <shared/FileHandling.h>
#include <shared/LibProject.h>
#include <string.h>
#include <assert.h>
#include <shared/RawCommand.h>
#include <shared/IOLib.h>

#define CMDSIZE (1000)

Enum(ActionMode){
    NORMAL,
    COMMAND,
    INSERT,
    VISUAL,
    FIND,
    FINDHL,
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
static char cmdres[CMDSIZE] = {};
static char *cmdptr;
static char fnd[CMDSIZE] = {};
static char *fndptr;
static char file[FILESIZE] = {};
static char fileAddress[ADDRSIZE] = {};
static char clipboard[FILESIZE] = {};
static FilePos cursor, visBeg;
static int wln = 1;
static FILE *logstream;

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
        if (!(clipboard[i] = ptr[i]))
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

FileHandlingStatus SafeSave(char *address)
{
    return SafeWriteFile(address, file);
}
FileHandlingStatus SaveTMP()
{
    return WriteFile(".tmp.viim", file);
}
FileHandlingStatus Load(char *address)
{
    FileHandlingStatus status = ReadFileNoAlloc(address, file);
    cursor.ptr = GetPtrAt(file, cursor.ln, cursor.cn);
    cursor.ln = GetLnNum(file, cursor.ptr);
    cursor.cn = GetChNum(file, cursor.ptr);
    return status;
}
FileHandlingStatus LoadTMP()
{
    return Load(".tmp.viim");
}

void AutoIndent()
{
    SaveTMP();
    logstream = fopen(".ostream.txt", "w");
    _AutoIndent(logstream, "/.tmp.viim");
    fclose(logstream);
    LoadTMP();
}

static int CMDAutoIndent(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE] = {};
    ReadStrSTDIN(istream, address);
    int dotmp = 0;
    if (address[0] == 0)
    {
        dotmp = 1;
        SaveTMP();
        strcpy(address, "/.tmp.viim");
    }

    int opt = ReadOption(istream, (char *[]){
                                      NULL});
    switch (opt)
    {
    case NWLINE:
    case ARMAN:
        if (_AutoIndent(stream, address))
            goto invalid;
        if (dotmp)
            LoadTMP();
        return opt == ARMAN;

    default:
        ConsumeSTDIN(istream);
    invalid:
        OUTPUT("Invalid Format");
        return -1;
    }

    return 0;
}
static int CMDReplace(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE] = {}, pattern[IOSIZE] = {}, replace[IOSIZE] = {};
    int at = 0, atn = 0, all = 0;
    SaveTMP();
    strcpy(address, "/.tmp.viim");

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          "-str1",
                                          "-str2",
                                          "-at",
                                          "-all",
                                          NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case 1:
            ReadStrSTDIN(istream, pattern);
            break;
        case 2:
            ReadStrSTDIN(istream, replace);
            break;
        case 3:
            at = 1;
            fscanf(istream, "%d", &atn);
            break;
        case 4:
            all = 1;
            break;
        case NWLINE:
        case ARMAN:
            if (_Replace(stream, address, pattern, replace, at, atn, all))
                goto invalid;
            LoadTMP();
            return opt == ARMAN;

        default:
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}
static int CMDSave(FILE *stream, FILE *istream, char *inputstream)
{
    if (fileAddress[0] != '/')
    {
        OUTPUT("Empty File! Use saveas Instead");
        return -1;
    }
    SafeSave(fileAddress + 1);
    OUTPUT("Saved Successfully");
    return 0;
}
static int CMDSaveas(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE];
    ReadStrSTDIN(istream, address);
    if (address[0] != '/')
    {
        OUTPUT("Invalid Format");
        return -1;
    }
    strcpy(fileAddress, address);
    SafeSave(fileAddress + 1);
    OUTPUT("Saved Successfully");
    return 0;
}
static int CMDOpen(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE];
    ReadStrSTDIN(istream, address);
    if (address[0] != '/')
    {
        OUTPUT("Invalid Format");
        return -1;
    }
    Load(address + 1);
    strcpy(fileAddress, address);
    OUTPUT("File Opened Successfully");
    return 0;
}

static int ReadCMD(FILE *istream)
{

    int opt = ReadOption(istream, (char *[]){
                                      "createfile",
                                      "insertstr",
                                      "cat",
                                      "removestr",
                                      "copy",
                                      "cut",
                                      "paste",
                                      "undo",
                                      "tree",
                                      "compare",
                                      "auto-indent",
                                      "grep",
                                      "find",
                                      "replace",
                                      "open",
                                      "save",
                                      "saveas",
                                      "quit",
                                      NULL});

    char inputstream[FILESIZE] = {};
    ReadOStream(inputstream);

    FILE *stream = CreateOStream();
    int res = 0;
    switch (opt)
    {
    case 0:
        res = CMD_CreateFile(stream, istream, inputstream);
        break;
    case 1:
        res = CMD_InsertStr(stream, istream, inputstream);
        break;
    case 2:
        res = CMD_CAT(stream, istream, inputstream);
        break;
    case 3:
        res = CMD_RemoveStr(stream, istream, inputstream);
        break;
    case 4:
        res = CMD_Copy(stream, istream, inputstream);
        break;
    case 5:
        res = CMD_Cut(stream, istream, inputstream);
        break;
    case 6:
        res = CMD_Paste(stream, istream, inputstream);
        break;
    case 7:
        res = CMD_Undo(stream, istream, inputstream);
        break;
    case 8:
        res = CMD_Tree(stream, istream, inputstream);
        break;
    case 9:
        res = CMD_Compare(stream, istream, inputstream);
        break;
    case 10:
        res = CMDAutoIndent(stream, istream, inputstream);
        break;
    case 11:
        res = CMD_Grep(stream, istream, inputstream);
        break;
    case 12:
        res = CMD_Find(stream, istream, inputstream);
        break;
    case 13:
        res = CMDReplace(stream, istream, inputstream);
        break;
    case 14:
        res = CMDOpen(stream, istream, inputstream);
        break;
    case 15:
        res = CMDSave(stream, istream, inputstream);
        break;
    case 16:
        res = CMDSaveas(stream, istream, inputstream);
        break;
    case 17:
        return 1; // quit
    case NWLINE:
        break;

    default:
        OUTPUT("Invalid Format");
        break;
    }
    fclose(stream);

    if (res == 1)
        ReadCMD(istream);

    return 0;
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
        while (cmdptr[0])
            cmdptr++;
        cmdptr++[0] = '\n';
        cmdptr[0] = 0;

        WriteFile(".istream.txt", cmd);
        FILE *istream = fopen(".istream.txt", "r");
        int rs = ReadCMD(istream);
        fclose(istream);
        if (rs)
            return -1;

        char ostream[FILESIZE];
        ReadOStream(ostream);
        char *ptr = GetPtrAt(ostream, 2, 1);
        if (ptr[0])
        {
            strcpy(file, ostream);
            fileAddress[0] = 0;
        }
        else
        {
            if (ptr > ostream)
                ptr[-1] = 0;
            strcpy(cmdres, ostream);
            if (fileAddress[0] == '/')
                Load(fileAddress + 1);
        }

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
    case '/':
        actionMode = FIND;
        fndptr = fnd;
        break;
    case ':':
        actionMode = COMMAND;
        cmdptr = cmd;
        cmd[0] = 0;
        break;
    case 'i':
        actionMode = INSERT;
        break;
    case 'v':
        actionMode = VISUAL;
        visBeg = cursor;
        break;
    case '=':
        AutoIndent();
        break;

    case KEY_LEFT:
    case 'h':
        FileCursurLeft();
        break;
    case '\n':
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
    case '\n':
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
    char ch[5] = "a";
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

    case '\t':
        strcpy(ch, "    ");
        InsertPattern(cursor.ptr, ch), cursor.ptr += 3, FileNxt();
        break;
    case '{':
        strcpy(ch, "{}");
        InsertPattern(cursor.ptr, ch), FileNxt();
        break;
    case '[':
        strcpy(ch, "[]");
        InsertPattern(cursor.ptr, ch), FileNxt();
        break;
    case '(':
        strcpy(ch, "()");
        InsertPattern(cursor.ptr, ch), FileNxt();
        break;
    case '<':
        strcpy(ch, "<>");
        InsertPattern(cursor.ptr, ch), FileNxt();
        break;
    default:
        ch[0] = inp;
        InsertPattern(cursor.ptr, ch), FileNxt();
        break;
    }
    return 0;
}

int FindMode(int inp)
{
    switch (inp)
    {
    case 27:
        fnd[0] = 0;
        return 1;

    case KEY_LEFT:
        if (fndptr > fnd)
            fndptr--;
        break;
    case KEY_RIGHT:
        if (fndptr[0])
            fndptr++;
        break;
    case KEY_HOME:
        fndptr = fnd;
        break;
    case KEY_END:
        fndptr = GetEndLine(fndptr);
        break;

    case KEY_BACKSPACE:
        if (fnd[0] == 0)
            return 1;
        if (fndptr > fnd)
            EraseSubstring(fndptr - 1, 1), fndptr--;
        break;
    case KEY_DC:
        EraseSubstring(fndptr, 1);
        break;

    case '\n':
        char *fdptr = FindPattern(cursor.ptr + (cursor.ptr[0] != 0), fnd);
        if (!fdptr)
            fdptr = FindPattern(file, fnd);
        if (fdptr)
        {
            cursor.ptr = fdptr;
            cursor.ln = GetLnNum(file, cursor.ptr);
            cursor.cn = GetChNum(file, cursor.ptr);
        }
        actionMode = FINDHL;
        break;

    default:
        char ch[2] = "a";
        ch[0] = inp;
        InsertPattern(fndptr, ch);
        fndptr++;
        break;
    }
    return 0;
}

int FindHL(int inp)
{
    switch (inp)
    {
    case 'n':
        char *fdptr = FindPattern(cursor.ptr + (cursor.ptr[0] != 0), fnd);
        if (!fdptr)
            fdptr = FindPattern(file, fnd);
        if (fdptr)
        {
            cursor.ptr = fdptr;
            cursor.ln = GetLnNum(file, cursor.ptr);
            cursor.cn = GetChNum(file, cursor.ptr);
        }
        break;

    default:
        fnd[0] = 0;
        return 1;
    }
    return 0;
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
            else if (rs == -1)
                return 1;
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
        case FINDHL:
            if (FindHL(inp))
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

    char *highBeg = (visBeg.ptr < cursor.ptr ? visBeg.ptr : cursor.ptr);
    char *highEnd = (visBeg.ptr > cursor.ptr ? visBeg.ptr : cursor.ptr) + 1;
    int fndlen = strlen(fnd);
    if (actionMode == FIND || actionMode == FINDHL)
    {
        highBeg = FindPattern(ptr, fnd);
        highEnd = highBeg + fndlen;
    }

    for (; ptr[0] && ptr[0] != '\n'; ptr++)
    {
        switch (actionMode)
        {
        case VISUAL:
            if (highBeg <= ptr && ptr < highEnd)
                attron(COLOR_PAIR(1));
            break;
        case FINDHL:
        case FIND:
            if (ptr == highEnd)
            {
                highBeg = FindPattern(highBeg + 1, fnd);
                highEnd = highBeg + fndlen;
            }
            if (highBeg <= ptr && ptr < highEnd)
                attron(COLOR_PAIR(2));
            break;
        }
        printw("%c", ptr[0]);

        attroff(COLOR_PAIR(1));
        attroff(COLOR_PAIR(2));
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
    attron(COLOR_PAIR(1));
    switch (actionMode)
    {
    case NORMAL:
        printw(" NORMAL ");
        break;
    case COMMAND:
        printw("COMMAND ");
        break;
    case INSERT:
        printw(" INSERT ");
        break;
    case VISUAL:
        printw(" VISUAL ");
        break;
    case FIND:
    case FINDHL:
        printw("  FIND  ");
        break;

    default:
        break;
    }
    attroff(COLOR_PAIR(1));
    if (fileAddress[0] == '/')
        printw(" %s", fileAddress);
    else
        printw(" Empty");
    move(LINES - 1, 0);

    if (actionMode == COMMAND)
    {
        printw(":%s", cmd);
    }
    else if (actionMode == FIND)
    {
        printw("/%s", fnd);
    }
    else if (actionMode == NORMAL)
    {
        printw("%s", cmdres);
    }

    if (actionMode == FIND)
        move(LINES - 1, fndptr - fnd + 1);
    else if (actionMode == COMMAND)
        move(LINES - 1, cmdptr - cmd + 1);
    else
        move(cursor.ln - wln, cursor.cn + 4);
}

int main()
{
    WINDOW *wnd = initscr();
    keypad(wnd, 1);
    cbreak();
    noecho();
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_RED);

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