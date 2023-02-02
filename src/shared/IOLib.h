#ifndef _IOLIB
#define _IOLIB

#define ARMAN (-2)
#define NWLINE (-1)
#define IOSIZE (100)

int ReadOption(FILE *istream, char **options);

int ReadStrSTDIN(FILE *istream, char *str);

void ConsumeSTDIN(FILE *istream);

#endif