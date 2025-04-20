#ifndef SCOPE_H
#define SCOPE_H

#include <stddef.h>
#include <llvm-c/Types.h>


struct Symbol
{
  char *name;
  int raw_type; // Is this symbol a raw-type? (I64, Void, etc.)
  union
  {
    struct Type *type;
    LLVMValueRef value;
  };
};


struct Symbol *Symbol_Create_Type (const char *, struct Type *, int);

struct Symbol *Symbol_Create_Value (const char *, LLVMValueRef);

void Symbol_Destroy_Type (struct Symbol *);

void Symbol_Destroy_Value (struct Symbol *);


struct Scope
{
  struct Scope *parent;
  size_t data_capacity;
  size_t data_n;
  struct Symbol **data;
};


struct Scope *Scope_Create (struct Scope *);

void Scope_Destroy_Type (struct Scope *);

void Scope_Destroy_Value (struct Scope *);

void Scope_Clear_Type (struct Scope *);

void Scope_Clear_Value (struct Scope *);

void Scope_Add (struct Scope *, struct Symbol *);

struct Symbol *Scope_Find (struct Scope *, const char *);

struct Symbol *Scope_Find_Shallow (struct Scope *, const char *);



#endif // SCOPE_H

