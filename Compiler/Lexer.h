#ifndef LEXER_H
#define LEXER_H

#include "Diagnostic.h"


struct Lexer
{
  struct Location location;
  char *current;
};


struct Lexer Lexer_Create (char *const, const char *const);

struct Token Lexer_Next (struct Lexer *);

struct Token Lexer_Peek (struct Lexer *);


#endif // LEXER_H

