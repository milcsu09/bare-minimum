#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <stddef.h>


#define D_ERROR "Error"
#define D_WARNING "Warning"
#define D_NOTE "Note"


struct Location
{
  const char *context;
  size_t line;
  size_t column;
};


void Location_Diagnostic (struct Location);

_Noreturn void Halt ();

void Diagnostic (struct Location, const char *, const char *, ...);

struct AST;

void Diagnostic_AST (struct AST *);


#endif // DIAGNOSTIC_H

