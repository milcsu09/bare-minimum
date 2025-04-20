#include "Lexer.h"
#include "String.h"
#include "Token.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Lexer
Lexer_Create (char *const source, const char *const context)
{
  struct Lexer lexer;

  lexer.location.context = context;
  lexer.location.line = 1;
  lexer.location.column = 1;

  lexer.current = source;

  return lexer;
}


void
Lexer_Advance (struct Lexer *lexer)
{
  char c = *lexer->current;

  if (c == '\0')
    return;

  lexer->current++;
  lexer->location.column++;

  if (c == '\n')
    {
      lexer->location.line++;
      lexer->location.column = 1;
    }
}


void
Lexer_Advance_N (struct Lexer *lexer, size_t n)
{
  while (n--)
    Lexer_Advance (lexer);
}


struct Token
Lexer_Advance_Token (struct Lexer *lexer, enum Token_Kind kind)
{
  struct Token token;

  token = Token_Create (lexer->location, kind);

  Lexer_Advance (lexer);

  return token;
}


struct Token
Lexer_Advance_N_Token (struct Lexer *lexer, size_t n, enum Token_Kind kind)
{
  struct Token token;

  token = Token_Create (lexer->location, kind);

  Lexer_Advance_N (lexer, n);

  return token;
}


int
Lexer_Match_Start (struct Lexer *lexer, const char *s)
{
  size_t length = strlen (s);
  return strncmp (lexer->current, s, length) == 0;
}


int
Lexer_Is_I (char c, int base)
{
  switch (base)
    {
    case 2:
      return c == '0' || c == '1';
    case 8:
      return c >= '0' && c <= '7';
    case 16:
      return isxdigit (c);
    }

  return 0;
}


struct Token
Lexer_Lex_I (struct Lexer *lexer, int base)
{
  struct Location location = lexer->location;
  const char *start = lexer->current;

  while (Lexer_Is_I (*lexer->current, base))
    Lexer_Advance (lexer);

  int64_t x = strtol (start, NULL, base);

  return Token_Create_I64 (location, TOKEN_I64, x);
}


struct Token
Lexer_Lex_Number (struct Lexer *lexer)
{
  if (*lexer->current == '0')
    switch (*(lexer->current + 1))
      {
      case 'b':
      case 'B':
        Lexer_Advance_N (lexer, 2);
        return Lexer_Lex_I (lexer, 2);
      case 'o':
      case 'O':
        Lexer_Advance_N (lexer, 2);
        return Lexer_Lex_I (lexer, 8);
      case 'x':
      case 'X':
        Lexer_Advance_N (lexer, 2);
        return Lexer_Lex_I (lexer, 16);
      }

  struct Location location = lexer->location;
  const char *start = lexer->current;

  int is_float = 0;

  while (isdigit (*lexer->current))
    Lexer_Advance (lexer);

  if (*lexer->current == '.')
    {
      is_float = 1;
      Lexer_Advance (lexer);

      while (isdigit (*lexer->current))
        Lexer_Advance (lexer);
    }

  if (*lexer->current == 'e' || *lexer->current == 'E')
    {
      is_float = 1;
      Lexer_Advance (lexer);

      if (*lexer->current == '+' || *lexer->current == '-')
        Lexer_Advance (lexer);

      while (isdigit (*lexer->current))
        Lexer_Advance (lexer);
    }

  if (is_float)
    {
      double x = strtod (start, NULL);
      return Token_Create_F64 (location, TOKEN_F64, x);
    }
  else
    {
      int64_t x = strtol (start, NULL, 10);
      return Token_Create_I64 (location, TOKEN_I64, x);
    }
}


struct Keyword
{
  const char *key;
  enum Token_Kind kind;
};


static struct Keyword KEYWORD_TABLE[] = {
  { "then", TOKEN_THEN },
  { "if", TOKEN_IF },
  { "else", TOKEN_ELSE },
  { "while", TOKEN_WHILE },
  { "alias", TOKEN_ALIAS },
  { "as", TOKEN_AS },
  { "defer", TOKEN_DEFER },
  { "sizeof", TOKEN_SIZEOF },
};


struct Token
Lexer_Lex_Identifier (struct Lexer *lexer)
{
  struct Location location = lexer->location;
  const char *start = lexer->current;

  while (isalnum (*lexer->current) || *lexer->current == '_')
    Lexer_Advance (lexer);

  size_t length = lexer->current - start;

  for (size_t i = 0; i < sizeof KEYWORD_TABLE / sizeof (struct Keyword); ++i)
    {
      if (strlen (KEYWORD_TABLE[i].key) != length)
        continue;

      if (strncmp (start, KEYWORD_TABLE[i].key, length) == 0)
        return Token_Create (location, KEYWORD_TABLE[i].kind);
    }

  char *s = String_Copy_N (start, length);

  return Token_Create_S (location, TOKEN_IDENTIFIER, s);
}


struct Token
Lexer_Lex_String (struct Lexer *lexer)
{
  struct Location location = lexer->location;
  const char *start = lexer->current;

  Lexer_Advance (lexer);

  while (strchr ("\"\n", *lexer->current) == NULL)
    {
      if (*lexer->current == '\\')
        Lexer_Advance (lexer);
      Lexer_Advance (lexer);
    }

  if (*lexer->current != '"')
    {
      Diagnostic (location, D_ERROR, "unterimated string-literal");
      Halt ();
    }

  Lexer_Advance (lexer);

  char *s = String_Copy_Until (start + 1, lexer->current - 1);

  String_Escape (s);

  return Token_Create_S (location, TOKEN_STRING, s);
}


struct Token
Lexer_Lex_Character (struct Lexer *lexer)
{
  struct Location location = lexer->location;
  const char *start = lexer->current;

  Lexer_Advance (lexer);

  size_t length = 0;
  while (strchr ("\'\n", *lexer->current) == NULL)
    {
      if (*lexer->current == '\\')
        Lexer_Advance (lexer);
      Lexer_Advance (lexer);
      length++;
    }

  if (*lexer->current != '\'')
    {
      Diagnostic (location, D_ERROR, "unterimated character-literal");
      Halt ();
    }

  if (length != 1)
    {
      Diagnostic (location, D_ERROR, "character-literal must be length of one 1");
      Halt ();
    }

  Lexer_Advance (lexer);

  char *s = String_Copy_Until (start + 1, lexer->current - 1);

  String_Escape (s);

  struct Token token;

  token = Token_Create_I64 (location, TOKEN_I64, (char)*s);

  free (s);

  return token;
}


struct Token
Lexer_Next (struct Lexer *lexer)
{
  while (isspace (*lexer->current))
    Lexer_Advance (lexer);

  while (*lexer->current == '#')
    {
      while (*lexer->current && *lexer->current != '\n')
        Lexer_Advance (lexer);

      while (isspace (*lexer->current))
        Lexer_Advance (lexer);
    }

  char c = *lexer->current;

  switch (c)
    {
    case ';':
      return Lexer_Advance_Token (lexer, TOKEN_SEMICOLON);
    case '(':
      return Lexer_Advance_Token (lexer, TOKEN_LPAREN);
    case ')':
      return Lexer_Advance_Token (lexer, TOKEN_RPAREN);
    case '[':
      return Lexer_Advance_Token (lexer, TOKEN_LBRACKET);
    case ']':
      return Lexer_Advance_Token (lexer, TOKEN_RBRACKET);
    case '{':
      return Lexer_Advance_Token (lexer, TOKEN_LBRACE);
    case '}':
      return Lexer_Advance_Token (lexer, TOKEN_RBRACE);
    case '=':
      if (Lexer_Match_Start (lexer, "=="))
        return Lexer_Advance_N_Token (lexer, 2, TOKEN_DEQ);
      return Lexer_Advance_Token (lexer, TOKEN_EQUALS);
    case ':':
      if (Lexer_Match_Start (lexer, ":="))
        return Lexer_Advance_N_Token (lexer, 2, TOKEN_WALRUS);
      return Lexer_Advance_Token (lexer, TOKEN_COLON);
    case ',':
      return Lexer_Advance_Token (lexer, TOKEN_COMMA);
    case '-':
      if (Lexer_Match_Start (lexer, "->"))
        return Lexer_Advance_N_Token (lexer, 2, TOKEN_ARROW);
      return Lexer_Advance_Token (lexer, TOKEN_MINUS);
    case '.':
      if (Lexer_Match_Start (lexer, "..."))
        return Lexer_Advance_N_Token (lexer, 3, TOKEN_3DOT);
      return Lexer_Advance_Token (lexer, TOKEN_DOT);

    case '&':
      return Lexer_Advance_Token (lexer, TOKEN_AMPERSAND);
    case '+':
      return Lexer_Advance_Token (lexer, TOKEN_PLUS);
    case '*':
      return Lexer_Advance_Token (lexer, TOKEN_STAR);
    case '/':
      return Lexer_Advance_Token (lexer, TOKEN_SLASH);
    case '<':
      if (Lexer_Match_Start (lexer, "<="))
        return Lexer_Advance_N_Token (lexer, 2, TOKEN_LE);
      return Lexer_Advance_Token (lexer, TOKEN_LT);
    case '>':
      if (Lexer_Match_Start (lexer, ">="))
        return Lexer_Advance_N_Token (lexer, 2, TOKEN_GE);
      return Lexer_Advance_Token (lexer, TOKEN_GT);
    case '!':
      if (Lexer_Match_Start (lexer, "!="))
        return Lexer_Advance_N_Token (lexer, 2, TOKEN_NEQ);
      break;

    case '\0':
      return Token_Create (lexer->location, TOKEN_EOF);
    }

  if (isdigit (c))
    return Lexer_Lex_Number (lexer);

  if (isalpha (c) || c == '_')
    return Lexer_Lex_Identifier (lexer);

  if (c == '"')
    return Lexer_Lex_String (lexer);

  if (c == '\'')
    return Lexer_Lex_Character (lexer);

  Diagnostic (lexer->location, D_ERROR, "unexpected character '%c'", c);
  Halt ();
}


struct Token
Lexer_Peek (struct Lexer *lexer)
{
  struct Lexer copy = *lexer;
  return Lexer_Next (&copy);
}

