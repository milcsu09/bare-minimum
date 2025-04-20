#include "Scope.h"
#include "String.h"
#include "Type.h"
#include <stdlib.h>
#include <string.h>


struct Symbol *
Symbol_Create (const char *name)
{
  struct Symbol *symbol;

  symbol = calloc (1, sizeof (struct Symbol));

  symbol->name = String_Copy (name);
  // symbol->type = Type_Copy (type);

  return symbol;
}


struct Symbol *
Symbol_Create_Type (const char *name, struct Type *type, int raw_type)
{
  struct Symbol *symbol;

  symbol = Symbol_Create (name);

  symbol->type = Type_Copy (type);
  symbol->raw_type = raw_type;

  return symbol;
}


struct Symbol *
Symbol_Create_Value (const char *name, LLVMValueRef value)
{
  struct Symbol *symbol;

  symbol = Symbol_Create (name);

  symbol->value = value;

  return symbol;
}


void
Symbol_Destroy (struct Symbol *symbol)
{
  free (symbol->name);
  free (symbol);
}


void
Symbol_Destroy_Type (struct Symbol *symbol)
{
  Type_Destroy (symbol->type);
  Symbol_Destroy (symbol);
}


void
Symbol_Destroy_Value (struct Symbol *symbol)
{
  Symbol_Destroy (symbol);
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
  free (scope->data);
  free (scope);
}


void
Scope_Destroy_Type (struct Scope *scope)
{
  Scope_Clear_Type (scope);
  Scope_Destroy (scope);
}


void
Scope_Destroy_Value (struct Scope *scope)
{
  Scope_Clear_Value (scope);
  Scope_Destroy (scope);
}


void
Scope_Clear (struct Scope *scope)
{
  scope->data_n = 0;
}


void
Scope_Clear_Type (struct Scope *scope)
{
  for (size_t i = 0; i < scope->data_n; ++i)
    Symbol_Destroy_Type (scope->data[i]);
  Scope_Clear (scope);
}


void
Scope_Clear_Value (struct Scope *scope)
{
  for (size_t i = 0; i < scope->data_n; ++i)
    Symbol_Destroy_Value (scope->data[i]);
  Scope_Clear (scope);
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

