#ifndef TYPE_H
#define TYPE_H

#include "Token.h"
#include <stddef.h>


struct Symbol
{
  char *name;
  struct Type *type;
};


struct Symbol *Symbol_Create (const char *, struct Type *);

void Symbol_Destroy (struct Symbol *);


struct Scope
{
  struct Scope *parent;
  size_t data_capacity;
  size_t data_n;
  struct Symbol **data;
};


struct Scope *Scope_Create (struct Scope *);

void Scope_Destroy (struct Scope *);

void Scope_Clear (struct Scope *);

void Scope_Add (struct Scope *, struct Symbol *);

struct Symbol *Scope_Find (struct Scope *, const char *);

struct Symbol *Scope_Find_Shallow (struct Scope *, const char *);


enum Type_Kind
{
  TYPE_DUMMY,

  TYPE_VOID,
  TYPE_I8,
  TYPE_I16,
  TYPE_I32,
  TYPE_I64,
  TYPE_U8,
  TYPE_U16,
  TYPE_U32,
  TYPE_U64,
  TYPE_F32,
  TYPE_F64,
  TYPE_BOOL,
  TYPE_FUNCTION
};


const char *Type_Kind_String (enum Type_Kind);


struct Type_Function
{
  size_t in_n;
  struct Type **in;
  struct Type *out;
  int variadic;
};


struct Type_Function Type_Function_Create (size_t, struct Type **,
                                           struct Type *, int);

struct Type_Function Type_Function_Copy (struct Type_Function);

void Type_Function_Destroy (struct Type_Function);

int Type_Function_Match (struct Type_Function, struct Type_Function);

void Type_Function_Diagnostic (struct Type_Function);


union Type_Value
{
  struct Token dummy;
  struct Type_Function function;
};


struct Type
{
  enum Type_Kind kind;
  union Type_Value value;
};


struct Type *Type_Create (enum Type_Kind);

struct Type *Type_Create_Dummy (struct Token);

struct Type *Type_Create_Function (struct Type_Function);

struct Type *Type_Copy (struct Type *);

void Type_Destroy (struct Type *);

int Type_Match (struct Type *, struct Type *);

int Type_Castable (struct Type *, struct Type *);

void Type_Diagnostic (struct Type *);


#endif // TYPE_H

