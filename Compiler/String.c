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


void
String_Escape (char *s)
{
  if (s == NULL)
    return;

  char *i = s, *j = s;

  while (*i != '\0')
    {
      if (*i == '\\')
        {
          i++;
          switch (*i)
            {
            case 'a':
              *j = '\a';
              break;
            case 'b':
              *j = '\b';
              break;
            case 'f':
              *j = '\f';
              break;
            case 'n':
              *j = '\n';
              break;
            case 'r':
              *j = '\r';
              break;
            case 't':
              *j = '\t';
              break;
            case 'v':
              *j = '\v';
              break;
            case '\\':
              *j = '\\';
              break;
            case '\'':
              *j = '\'';
              break;
            case '\"':
              *j = '\"';
              break;
            case '0':
              *j = '\0';
              break;
            default:
              *j = '\\';
              *++j = *i;
              break;
            }

          i++;
        }
      else
        *j = *i++;

      j++;
    }

  *j = '\0';
}


