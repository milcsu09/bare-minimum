#include "String.h"
#include <stdlib.h>
#include <string.h>


char *
String_Copy (const char *s)
{
  if (s == NULL)
    return NULL;

  size_t length = strlen (s);

  char *copy;

  copy = calloc (length + 1, 1);

  if (copy)
    memcpy (copy, s, length);

  return copy;
}


char *
String_Copy_N (const char *s, size_t length)
{
  if (s == NULL)
    return NULL;

  char *copy;

  copy = calloc (length + 1, 1);

  if (copy)
    memcpy (copy, s, length);

  return copy;
}


char *
String_Copy_Until (const char *s, const char * e)
{
  return String_Copy_N (s, e - s);
}

