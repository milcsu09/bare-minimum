#ifndef STRING_H
#define STRING_H

#include <stddef.h>


char *String_Copy (const char *);

char *String_Copy_N (const char *, size_t);

char *String_Copy_Until (const char *, const char *);


#endif // STRING_H

