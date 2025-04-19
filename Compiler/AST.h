#ifndef AST_H
#define AST_H

#include "Token.h"
#include "Type.h"


enum AST_Kind
{
  AST_PROGRAM,

  AST_PROTOTYPE,
  AST_FUNCTION,

  AST_ALIAS,
  AST_DEFER,

  AST_VARIABLE,
  AST_IF,
  AST_WHILE,

  AST_UNARY,
  AST_BINARY,
  AST_CAST,

  AST_ACCESS,
  AST_COMPOUND,
  AST_IDENTIFIER,
  AST_CALL,
  AST_I64,
  AST_F64,
  AST_STRING,
  AST_INITIALIZER,
};


const char *AST_Kind_String (enum AST_Kind);


struct AST
{
  struct Token token;
  struct Location location;
  struct Type *type;
  // struct Scope *scope;
  struct AST *child;
  struct AST *next;
  enum AST_Kind kind;
  int state;
};


struct AST *AST_Create (struct Location, enum AST_Kind);

void AST_Destroy (struct AST *);

void AST_Attach (struct AST *, struct AST *);

void AST_Append (struct AST *, struct AST *);

void AST_Switch_Type (struct AST *, struct Type *);

int AST_Is_LV (struct AST *);

void AST_Diagnostic (struct AST *);


#endif // AST_H

