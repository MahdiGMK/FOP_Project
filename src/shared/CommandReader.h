#include <stdio.h>
int CMD_CreateFile(FILE *stream, FILE *istream, char *inputstream);

int CMD_InsertStr(FILE *stream, FILE *istream, char *inputstream);

int CMD_CAT(FILE *stream, FILE *istream, char *inputstream);

int CMD_RemoveStr(FILE *stream, FILE *istream, char *inputstream);

int CMD_Copy(FILE *stream, FILE *istream, char *inputstream);

int CMD_Cut(FILE *stream, FILE *istream, char *inputstream);

int CMD_Paste(FILE *stream, FILE *istream, char *inputstream);

int CMD_Undo(FILE *stream, FILE *istream, char *inputstream);

int CMD_Tree(FILE *stream, FILE *istream, char *inputstream);

int CMD_Compare(FILE *stream, FILE *istream, char *inputstream);

int CMD_AutoIndent(FILE *stream, FILE *istream, char *inputstream);

int CMD_Grep(FILE *stream, FILE *istream, char *inputstream);

int CMD_Find(FILE *stream, FILE *istream, char *inputstream);

int CMD_Replace(FILE *stream, FILE *istream, char *inputstream);
