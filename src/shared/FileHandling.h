#ifndef _FILE_HANDLING
#define _FILE_HANDLING

#define FILESIZE (10000)

#include "LibProject.h"

Enum(FileHandlingStatus){
    FILE_CREATED,
    FILE_EXISTED,
    FILE_DIDNT_EXIST,
};

FileHandlingStatus ReadFileNoAlloc(const char *fileAddress, char *str);

FileHandlingStatus WriteFile(const char *fileAddress, char *str);

FileHandlingStatus CreateFile(const char *fileAddress);

FileHandlingStatus CreateDirectory(const char *fileAddress);

FileHandlingStatus CreateBackup(const char *fileAddress);

FileHandlingStatus Undo(const char *fileAddress);

#endif