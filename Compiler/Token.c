#include "Token.h"
#include "String.h"
#include <stdio.h>
#include <stdlib.h>


static const char *const TOKEN_KIND_STRING[] = {
  "End of File",

  "Whole Number",
  "Real Number",

  "Identifier",
  "Type",

  "then",
  "if",
  "else",
  "while",
  "alias",
  "as",

  ";",
  "(",
  ")",
  "{",
  "}",
  "=",
  ":=",
  ":",
  ",",
  "->",
  "...",

  "&",
  "+",
  "-",
  "*",
  "/",
  "<",
  "<=",
  ">",
  ">=",
};


const char *
Token_Kind_String (enum Token_Kind kind)
{
  return TOKEN_KIND_STRING[kind];
}


struct Token
Token_Create (struct Location location, enum Token_Kind kind)
{
  struct Token token;

  token.location = location;
  token.kind = kind;

  return token;
}


struct Token
Token_Create_I64 (struct Location location, enum Token_Kind kind, int64_t x)
{
  struct Token token;

  token = Token_Create (location, kind);
  token.value.i64 = x;

  return token;
}


struct Token
Token_Create_F64 (struct Location location, enum Token_Kind kind, double x)
{
  struct Token token;

  token = Token_Create (location, kind);
  token.value.f64 = x;

  return token;
}


struct Token
Token_Create_S (struct Location location, enum Token_Kind kind, char *x)
{
  struct Token token;

  token = Token_Create (location, kind);
  token.value.s = x;

  return token;
}


struct Token
Token_Copy (struct Token token)
{
  struct Token copy;

  copy = Token_Create (token.location, token.kind);

  switch (token.kind)
    {
    case TOKEN_IDENTIFIER:
    case TOKEN_TYPE:
      copy.value.s = String_Copy (token.value.s);
      break;
    default:
      break;
    }

  return copy;
}


void
Token_Destroy (struct Token token)
{
  switch (token.kind)
    {
    case TOKEN_IDENTIFIER:
    case TOKEN_TYPE:
      free (token.value.s);
      break;
    default:
      break;
    }
}


void
Token_Diagnostic (struct Token token)
{
  switch (token.kind)
    {
    case TOKEN_I64:
      fprintf (stderr, "i64 '%li'", token.value.i64);
      break;
    case TOKEN_F64:
      fprintf (stderr, "f64 '%g'", token.value.f64);
      break;
    case TOKEN_IDENTIFIER:
      fprintf (stderr, "identifier '%s'", token.value.s);
      break;
    case TOKEN_TYPE:
      fprintf (stderr, "type '%s'", token.value.s);
      break;
    default:
      fprintf (stderr, "%s", Token_Kind_String (token.kind));
      break;
    }
}

