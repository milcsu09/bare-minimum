#include "Type.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


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


int
Type_Kind_Is_Integer (enum Type_Kind kind)
{
  switch (kind)
    {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
    case TYPE_BOOL:
      return 1;
    default:
      return 0;
    }
}

int
Type_Kind_Is_Float (enum Type_Kind kind)
{
  switch (kind)
    {
    case TYPE_F32:
    case TYPE_F64:
      return 1;
    default:
      return 0;
    }
}

int
Type_Kind_Is_Signed (enum Type_Kind kind)
{
  switch (kind)
    {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
      return 1;
    default:
      return 0;
    }
}

int
Type_Kind_Width (enum Type_Kind kind)
{
  switch (kind)
    {
    case TYPE_I8:
    case TYPE_U8:
      return 8;
    case TYPE_I16:
    case TYPE_U16:
      return 16;
    case TYPE_I32:
    case TYPE_U32:
      return 32;
    case TYPE_I64:
    case TYPE_U64:
      return 64;
    case TYPE_F32:
      return 32;
    case TYPE_F64:
      return 64;
    case TYPE_BOOL:
      return 1;
    default:
      return 0;
    }
}


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


LLVMTypeRef
Type_Function_As_LLVM (struct Type_Function function, LLVMContextRef context)
{
  LLVMTypeRef in[function.in_n];

  for (size_t i = 0; i < function.in_n; ++i)
    in[i] = Type_As_LLVM (function.in[i], context);

  LLVMTypeRef out = Type_As_LLVM (function.out, context);

  return LLVMFunctionType (out, in, function.in_n, function.variadic);
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


LLVMTypeRef
Type_As_LLVM (struct Type *type, LLVMContextRef context)
{
  switch (type->kind)
    {
    case TYPE_VOID:
      return LLVMVoidTypeInContext (context);
    case TYPE_I8:
      return LLVMInt8TypeInContext (context);
    case TYPE_I16:
      return LLVMInt16TypeInContext (context);
    case TYPE_I32:
      return LLVMInt32TypeInContext (context);
    case TYPE_I64:
      return LLVMInt64TypeInContext (context);
    case TYPE_U8:
      return LLVMInt8TypeInContext (context);
    case TYPE_U16:
      return LLVMInt16TypeInContext (context);
    case TYPE_U32:
      return LLVMInt32TypeInContext (context);
    case TYPE_U64:
      return LLVMInt64TypeInContext (context);
    case TYPE_F32:
      return LLVMFloatTypeInContext (context);
    case TYPE_F64:
      return LLVMDoubleTypeInContext (context);
    case TYPE_BOOL:
      return LLVMInt1TypeInContext (context);
    case TYPE_FUNCTION:
      return Type_Function_As_LLVM (type->value.function, context);
    default:
      assert (0);
    }
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

