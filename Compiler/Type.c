#include "Type.h"
#include "String.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Symbol *
Symbol_Create (const char *name, struct Type *type)
{
  struct Symbol *symbol;

  symbol = calloc (1, sizeof (struct Symbol));

  symbol->name = String_Copy (name);
  symbol->type = Type_Copy (type);

  return symbol;
}


void
Symbol_Destroy (struct Symbol *symbol)
{
  Type_Destroy (symbol->type);
  free (symbol->name);
  free (symbol);
}


struct Scope *
Scope_Create (struct Scope *parent)
{
  struct Scope *scope;

  scope = calloc (1, sizeof (struct Scope));

  scope->parent = parent;

  scope->data_capacity = 4;
  scope->data_n = 0;

  scope->data = calloc (scope->data_capacity, sizeof (struct Symbol *));

  return scope;
}


void
Scope_Destroy (struct Scope *scope)
{
  Scope_Clear (scope);
  free (scope->data);
  free (scope);
}


void
Scope_Clear (struct Scope *scope)
{
  for (size_t i = 0; i < scope->data_n; ++i)
    Symbol_Destroy (scope->data[i]);
  scope->data_n = 0;
}


void
Scope_Add (struct Scope *scope, struct Symbol *symbol)
{
  if (scope->data_n >= scope->data_capacity)
    {
      scope->data_capacity *= 2;
      scope->data = realloc (scope->data,
                             scope->data_capacity * sizeof (struct Symbol *));
    }

  scope->data[scope->data_n++] = symbol;
}


struct Symbol *
Scope_Find_Base (struct Scope *scope, const char *name, int parent)
{
  for (size_t i = 0; i < scope->data_n; ++i)
    if (strcmp (scope->data[i]->name, name) == 0)
      return scope->data[i];

  if (scope->parent && parent)
    return Scope_Find_Base (scope->parent, name, parent);

  return NULL;
}


struct Symbol *
Scope_Find (struct Scope *scope, const char *name)
{
  return Scope_Find_Base (scope, name, 1);
}

struct Symbol *
Scope_Find_Shallow (struct Scope *scope, const char *name)
{
  return Scope_Find_Base (scope, name, 0);
}


static const char *const TYPE_KIND_STRING[] = {
  "DUMMY",
  "Void",
  "I8",
  "I16",
  "I32",
  "I64",
  "U8",
  "U16",
  "U32",
  "U64",
  "F32",
  "F64",
  "Bool",
  "Function"
};


const char *
Type_Kind_String (enum Type_Kind kind)
{
  return TYPE_KIND_STRING[kind];
}


struct Type_Function
Type_Function_Create (size_t in_n, struct Type **in, struct Type *out,
                      int variadic)
{
  struct Type_Function function;

  function.in_n = in_n;
  function.in = calloc (in_n, sizeof (struct Type *));

  for (size_t i = 0; i < in_n; ++i)
    function.in[i] = in[i];

  function.out = out;
  function.variadic = variadic;

  return function;
}


struct Type_Function
Type_Function_Copy (struct Type_Function function)
{
  struct Type **in = calloc (function.in_n, sizeof (struct Type *));

  for (size_t i = 0; i < function.in_n; ++i)
    in[i] = Type_Copy (function.in[i]);

  struct Type *out = Type_Copy (function.out);

  struct Type_Function copy ;
  copy = Type_Function_Create (function.in_n, in, out, function.variadic);

  free (in);

  return copy;
}


void
Type_Function_Destroy (struct Type_Function function)
{
  for (size_t i = 0; i < function.in_n; ++i)
    Type_Destroy (function.in[i]);
  Type_Destroy (function.out);
  free (function.in);
}


int
Type_Function_Match (struct Type_Function f1, struct Type_Function f2)
{
  if (f1.in_n != f2.in_n)
    return 0;

  for (size_t i = 0; i < f1.in_n; ++i)
    if (!Type_Match (f1.in[i], f2.in[i]))
      return 0;

  if (!Type_Match (f1.out, f2.out))
    return 0;

  if (f1.variadic != f2.variadic)
    return 0;

  return 1;
}


void
Type_Function_Diagnostic (struct Type_Function function)
{
  fprintf (stderr, "(");

  for (size_t i = 0; i < function.in_n; ++i)
    {
      Type_Diagnostic (function.in[i]);
      if (i < function.in_n - 1 || function.variadic)
        fprintf (stderr, ", ");
    }

  if (function.variadic)
    fprintf (stderr, "...");

  fprintf (stderr, ")");
  fprintf (stderr, " -> ");

  Type_Diagnostic (function.out);
}


struct Type *
Type_Create (enum Type_Kind kind)
{
  struct Type *type;

  type = calloc (1, sizeof (struct Type));

  type->kind = kind;

  return type;
}


struct Type *
Type_Create_Dummy (struct Token dummy)
{
  struct Type *type;

  type = Type_Create (TYPE_DUMMY);

  type->value.dummy = dummy;

  return type;
}


struct Type *
Type_Create_Function (struct Type_Function function)
{
  struct Type *type;

  type = Type_Create (TYPE_FUNCTION);

  type->value.function = function;

  return type;
}


struct Type *
Type_Copy (struct Type *type)
{
  struct Type *copy;

  copy = Type_Create (type->kind);

  switch (type->kind)
    {
    case TYPE_DUMMY:
      {
        copy->value.dummy = Token_Copy (type->value.dummy);
      }
      break;

    case TYPE_VOID:
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
    case TYPE_F32:
    case TYPE_F64:
    case TYPE_BOOL:
      break;
    case TYPE_FUNCTION:
      copy->value.function = Type_Function_Copy (type->value.function);
      break;
    }

  return copy;
}


void
Type_Destroy (struct Type *type)
{
  switch (type->kind)
    {
    case TYPE_DUMMY:
      Token_Destroy (type->value.dummy);
      break;
    case TYPE_FUNCTION:
      Type_Function_Destroy (type->value.function);
      break;
    default:
      break;
    }

  free (type);
}


int
Type_Match (struct Type *t1, struct Type * t2)
{
  if (t1->kind != t2->kind)
    return 0;

  switch (t1->kind)
    {
    case TYPE_DUMMY:
    case TYPE_VOID:
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
    case TYPE_F32:
    case TYPE_F64:
    case TYPE_BOOL:
      return 1;
    case TYPE_FUNCTION:
      return Type_Function_Match (t1->value.function, t2->value.function);
    }
}


int
Type_Castable (struct Type *t1, struct Type *t2)
{
  if (t1->kind == TYPE_VOID || t2->kind == TYPE_VOID)
    return t1->kind == t2->kind;

  if (t1->kind == TYPE_FUNCTION || t2->kind == TYPE_FUNCTION)
    {
      if (t1->kind != t2->kind)
        return 0;


      struct Type_Function *f1 = &t1->value.function;
      struct Type_Function *f2 = &t2->value.function;

      if (f1->in_n != f2->in_n || f1->variadic != f2->variadic)
        return 0;

      for (size_t i = 0; i < f1->in_n; ++i)
        if (!Type_Castable (f1->in[i], f2->in[i]))
          return 0;

      return Type_Castable (f1->out, f2->out);
    }

  int is_1_primitive = t1->kind >= TYPE_I8 && t1->kind <= TYPE_BOOL;
  int is_2_primitive = t2->kind >= TYPE_I8 && t2->kind <= TYPE_BOOL;

  return is_1_primitive && is_2_primitive;
}


void
Type_Diagnostic (struct Type *type)
{
  switch (type->kind)
    {
    case TYPE_DUMMY:
      fprintf (stderr, "DUMMY ");
      Token_Diagnostic (type->value.dummy);
      break;
    case TYPE_FUNCTION:
      Type_Function_Diagnostic (type->value.function);
      break;
    default:
      fprintf (stderr, "%s", Type_Kind_String (type->kind));
      break;
    }
}

