#ifndef TYPE_H
#define TYPE_H

#include "Token.h"
#include <stddef.h>
#include <llvm-c/Core.h>


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
  TYPE_FUNCTION,
  TYPE_POINTER,
  TYPE_INITIALIZER,
  TYPE_STRUCTURE,
};


int Type_Kind_Is_Integer (enum Type_Kind);

int Type_Kind_Is_Float (enum Type_Kind);

int Type_Kind_Is_Signed (enum Type_Kind);

int Type_Kind_Width (enum Type_Kind);

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

LLVMTypeRef Type_Function_As_LLVM (struct Type_Function, LLVMContextRef);

void Type_Function_Diagnostic (struct Type_Function);


struct Type_Field
{
  const char *name;
  struct Type *type;
};


struct Type_Field Type_Field_Create (const char *, struct Type *);

struct Type_Field Type_Field_Copy (struct Type_Field);

void Type_Field_Destroy (struct Type_Field);

int Type_Field_Match (struct Type_Field, struct Type_Field);

int Type_Function_Sizeof (struct Type_Function);

void Type_Field_Diagnostic (struct Type_Field);


struct Type_Structure
{
  size_t fields_n;
  struct Type_Field *fields;
};


struct Type_Structure Type_Structure_Create (size_t, struct Type_Field *);

struct Type_Structure Type_Structure_Copy (struct Type_Structure);

void Type_Structure_Destroy (struct Type_Structure);

int Type_Structure_Match (struct Type_Structure, struct Type_Structure);

LLVMTypeRef Type_Structure_As_LLVM (struct Type_Structure, LLVMContextRef);

int Type_Structure_Sizeof (struct Type_Structure);

void Type_Structure_Diagnostic (struct Type_Structure);


union Type_Value
{
  struct Token dummy;
  struct Type *base;
  struct Type_Function function;
  struct Type_Structure structure;
};


struct Type
{
  enum Type_Kind kind;
  union Type_Value value;
};


struct Type *Type_Create (enum Type_Kind);

struct Type *Type_Create_Dummy (struct Token);

struct Type *Type_Create_Function (struct Type_Function);

struct Type *Type_Create_Pointer (struct Type *);

struct Type *Type_Create_Structure (struct Type_Structure);

struct Type *Type_Copy (struct Type *);

void Type_Destroy (struct Type *);

int Type_Match (struct Type *, struct Type *);

int Type_Castable (struct Type *, struct Type *);

LLVMTypeRef Type_As_LLVM (struct Type *, LLVMContextRef);

int Type_Sizeof (struct Type *);

void Type_Diagnostic (struct Type *);


#endif // TYPE_H

