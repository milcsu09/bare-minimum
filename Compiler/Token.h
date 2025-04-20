#ifndef TOKEN_H
#define TOKEN_H

#include "Diagnostic.h"
#include <stdint.h>


union Token_Value
{
  int64_t i64;
  double f64;
  char *s;
};


enum Token_Kind
{
  TOKEN_EOF,

  TOKEN_I64,
  TOKEN_F64,
  TOKEN_STRING,

  TOKEN_IDENTIFIER,
  // TOKEN_TYPE,

  TOKEN_THEN,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_WHILE,
  TOKEN_ALIAS,
  TOKEN_AS,
  TOKEN_DEFER,
  TOKEN_SIZEOF,

  TOKEN_SEMICOLON,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_LBRACE,
  TOKEN_RBRACE,
  TOKEN_EQUALS,
  TOKEN_WALRUS,
  TOKEN_COLON,
  TOKEN_COMMA,
  TOKEN_ARROW,
  TOKEN_3DOT,

  TOKEN_DOT,
  TOKEN_AMPERSAND,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_LT,
  TOKEN_LE,
  TOKEN_GT,
  TOKEN_GE,
  TOKEN_DEQ,
  TOKEN_NEQ,
};


const char *Token_Kind_String (enum Token_Kind);


struct Token
{
  struct Location location;
  union Token_Value value;
  enum Token_Kind kind;
};


struct Token Token_Create (struct Location, enum Token_Kind);

struct Token Token_Create_I64 (struct Location, enum Token_Kind, int64_t);

struct Token Token_Create_F64 (struct Location, enum Token_Kind, double);

struct Token Token_Create_S (struct Location, enum Token_Kind, char *);

struct Token Token_Copy (struct Token);

void Token_Destroy (struct Token);

void Token_Diagnostic (struct Token);


#endif // TOKEN_H

