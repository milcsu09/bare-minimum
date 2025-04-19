#include "Checker.h"
#include <stdio.h>


struct Type *Checker_Check_Program (struct AST *);

struct Type *Checker_Check_Prototype (struct AST *);

struct Type *Checker_Check_Function (struct AST *);

// struct Type *Checker_Check_Alias (struct AST *);

struct Type *Checker_Check_Defer (struct AST *);

struct Type *Checker_Check_Variable (struct AST *);

struct Type *Checker_Check_If (struct AST *);

struct Type *Checker_Check_While (struct AST *);

struct Type *Checker_Check_Unary (struct AST *);

struct Type *Checker_Check_Binary (struct AST *);

struct Type *Checker_Check_Cast (struct AST *);

struct Type *Checker_Check_Access (struct AST *);

struct Type *Checker_Check_Compound (struct AST *);

struct Type *Checker_Check_Identifier (struct AST *);

struct Type *Checker_Check_Call (struct AST *);

struct Type *Checker_Check_I64 (struct AST *);

struct Type *Checker_Check_F64 (struct AST *);

struct Type *Checker_Check_String (struct AST *);

struct Type *Checker_Check_Initializer (struct AST *);


struct Type *
Checker_Check_Program (struct AST *ast)
{
  struct AST *current = ast->child;

  while (current)
    {
      Checker_Check (current);
      current = current->next;
    }

  return ast->type;
}

struct Type *
Checker_Check_Prototype (struct AST *ast)
{
  if (ast->type->kind != TYPE_FUNCTION)
    {
      Diagnostic (ast->location, D_ERROR, "function cannot be of type '%s'",
                  Type_Kind_String (ast->type->kind));
      Diagnostic (ast->location, D_NOTE, "use the syntax '(T1, ...) -> T'");
      Halt ();
    }

  return NULL;
}


struct Type *
Checker_Check_Function (struct AST *ast)
{
  Checker_Check (ast->child);
  return Checker_Check (ast->child->next);
}


struct Type *
Checker_Check_Defer (struct AST *ast)
{
  Checker_Check (ast->child);
  return NULL;
}


struct Type *
Checker_Check_Variable (struct AST *ast)
{
  switch (ast->type->kind)
    {
    case TYPE_VOID:
    case TYPE_FUNCTION:
      Diagnostic (ast->location, D_ERROR, "variable cannot be of type '%s'",
                  Type_Kind_String (ast->type->kind));
      Halt ();
    default:
      break;
    }

  if (ast->child)
    Checker_Check (ast->child);
  return NULL;
}


struct Type *
Checker_Check_If (struct AST *ast)
{
  Checker_Check (ast->child);
  Checker_Check (ast->child->next);
  if (ast->child->next->next)
    Checker_Check (ast->child->next->next);
  return NULL;
}


struct Type *
Checker_Check_While (struct AST *ast)
{
  Checker_Check (ast->child);
  Checker_Check (ast->child->next);
  return NULL;
}


struct Type *
Checker_Check_Unary (struct AST *ast)
{
  switch (ast->token.kind)
    {
    case TOKEN_AMPERSAND:
      if (!AST_Is_LV (ast->child))
        {
          Diagnostic (ast->child->location, D_ERROR,
                      "cannot take address of right-value");
          Halt ();
        }
      break;
    default:
      break;
    }

  return ast->type;
}


struct Type *
Checker_Check_Binary (struct AST *ast)
{
  switch (ast->token.kind)
    {
    case TOKEN_EQUALS:
      if (!AST_Is_LV (ast->child))
        {
          Diagnostic (ast->child->location, D_ERROR,
                      "cannot assign right-value to right-value");
          Halt ();
        }
      break;
    default:
      break;
    }

  Checker_Check (ast->child);
  Checker_Check (ast->child->next);

  return ast->type;
}


struct Type *
Checker_Check_Cast (struct AST *ast)
{
  struct Type *type = Checker_Check (ast->child);

  if (!Type_Castable (type, ast->type))
    {
      Diagnostic (ast->child->location, D_ERROR, "cannot cast '%s' to '%s'",
                  Type_Kind_String (type->kind),
                  Type_Kind_String (ast->type->kind));
      Halt ();
    }

  return ast->type;
}


struct Type *
Checker_Check_Access (struct AST *ast)
{
  Checker_Check (ast->child);
  return ast->type;
}


struct Type *
Checker_Check_Compound (struct AST *ast)
{
  struct AST *current = ast->child;

  while (current)
    {
      Checker_Check (current);
      current = current->next;
    }

  return ast->type;
}


struct Type *
Checker_Check_Identifier (struct AST *ast)
{
  return ast->type;
}


struct Type *
Checker_Check_Call (struct AST *ast)
{
  struct AST *current = ast->child->next;

  while (current)
    {
      struct Type *type = Checker_Check (current);

      switch (type->kind)
        {
        case TYPE_VOID:
        case TYPE_FUNCTION:
          Diagnostic (current->location, D_ERROR,
                      "'%s' as function parameter",
                      Type_Kind_String (type->kind));
          Halt ();
        default:
          break;
        }

      switch (type->kind)
        {
        case TYPE_STRUCTURE:
        case TYPE_INITIALIZER:
          Diagnostic (current->location, D_WARNING,
                      "'%s' as function parameter",
                      Type_Kind_String (type->kind));
          break;
        default:
          break;
        }


      current = current->next;
    }

  return ast->type;
}


struct Type *
Checker_Check_I64 (struct AST *ast)
{
  return ast->type;
}


struct Type *
Checker_Check_F64 (struct AST *ast)
{
  return ast->type;
}


struct Type *
Checker_Check_String (struct AST *ast)
{
  return ast->type;
}


struct Type *
Checker_Check_Initializer (struct AST *ast)
{
  if (ast->type->kind == TYPE_INITIALIZER)
    {
      Diagnostic (ast->location, D_ERROR,
                  "cannot infer type of initializer-list");
      Halt ();
    }

  struct AST *current = ast->child;

  while (current)
    {
      Checker_Check (current);
      current = current->next;
    }

  return ast->type;
}


struct Type *
Checker_Check (struct AST *ast)
{
  // printf ("%s\n", AST_Kind_String (ast->kind));
  switch (ast->kind)
    {
    case AST_PROGRAM:
      return Checker_Check_Program (ast);

    case AST_PROTOTYPE:
      return Checker_Check_Prototype (ast);
    case AST_FUNCTION:
      return Checker_Check_Function (ast);

    case AST_ALIAS:
      /* No need. */
      return NULL;
    case AST_DEFER:
      return Checker_Check_Defer (ast);

    case AST_VARIABLE:
      return Checker_Check_Variable (ast);
    case AST_IF:
      return Checker_Check_If (ast);
    case AST_WHILE:
      return Checker_Check_While (ast);

    case AST_UNARY:
      return Checker_Check_Unary (ast);
    case AST_BINARY:
      return Checker_Check_Binary (ast);
    case AST_CAST:
      return Checker_Check_Cast (ast);

    case AST_ACCESS:
      return Checker_Check_Access (ast);
    case AST_COMPOUND:
      return Checker_Check_Compound (ast);
    case AST_IDENTIFIER:
      return Checker_Check_Identifier (ast);
    case AST_CALL:
      return Checker_Check_Call (ast);
    case AST_I64:
      return Checker_Check_I64 (ast);
    case AST_F64:
      return Checker_Check_F64 (ast);
    case AST_STRING:
      return Checker_Check_String (ast);
    case AST_INITIALIZER:
      return Checker_Check_Initializer (ast);
    }
}


