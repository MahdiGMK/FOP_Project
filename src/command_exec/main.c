#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <shared/FileHandling.h>
#include <shared/IOLib.h>
#include <shared/StringLib.h>
#include <shared/RawCommand.h>
#include <shared/CommandReader.h>

static void ReadCMD(FILE *istream)
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
        res = CMD_AutoIndent(stream, istream, inputstream);
        break;
    case 11:
        res = CMD_Grep(stream, istream, inputstream);
        break;
    case 12:
        res = CMD_Find(stream, istream, inputstream);
        break;
    case 13:
        res = CMD_Replace(stream, istream, inputstream);
        break;
    case NWLINE:
        break;

    default:
        ConsumeSTDIN(istream);
        OUTPUT("Invalid Format");
        break;
    }
    fclose(stream);

    if (res == 1)
        ReadCMD(istream);

    // PrintOStream();

    //    LOG("RES : %d", res);
}

int main()
{
    while (1)
    {
        fclose(CreateOStream());
        ReadCMD(stdin);
        PrintOStream();
    }
}
