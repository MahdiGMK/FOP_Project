#ifndef _IOLIB
#define _IOLIB

#define NWLINE (-1)
#define IOSIZE (100)

int ReadOption(char **options);

int ReadStrSTDIN(char *str);

void ConsumeSTDIN();

#endif