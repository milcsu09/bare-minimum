#include "Diagnostic.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void
Location_Diagnostic (struct Location location)
{
  fprintf (stderr, "%s:%ld:%ld", location.context, location.line,
           location.column);
}

_Noreturn void
Halt ()
{
  exit (1);
}

void
Diagnostic (struct Location location, const char *type, const char *fmt, ...)
{
  Location_Diagnostic (location);

  fprintf (stderr, ": %s: ", type);

  va_list args;
  va_start (args, fmt);
  vfprintf (stderr, fmt, args);
  va_end (args);

  fprintf (stderr, "\n");
}

