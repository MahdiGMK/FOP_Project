#ifndef _FILE_HANDLING
#define _FILE_HANDLING

char *ReadFile(const char *fileAddress, int bufferSize);

void ReadFileNoAlloc(const char *fileAddress, char *str);

void WriteFile(const char *fileAddress, char *str);

#endif