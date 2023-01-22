#ifndef _LIB_PROJECT
#define _LIB_PROJECT

#define Struct(Name)          \
    struct Name;              \
    typedef struct Name Name; \
    struct Name

#define Enum(Name)          \
    enum Name;              \
    typedef enum Name Name; \
    enum Name

#endif