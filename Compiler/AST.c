#include "AST.h"
#include <stdio.h>
#include <stdlib.h>


static const char *const AST_KIND_STRING[] = {
  "PROGRAM",

  "PROTOTYPE",
  "FUNCTION",

  "ALIAS",

  "VARIABLE",
  "IF",
  "WHILE",

  "UNARY",
  "BINARY",
  "CAST",

  "COMPOUND",
  "IDENTIFIER",
  "CALL",
  "I64",
  "F64",
};


const char *
AST_Kind_String (enum AST_Kind kind)
{
  return AST_KIND_STRING[kind];
}


struct AST *
AST_Create (struct Location location, enum AST_Kind kind)
{
  struct AST *ast;

  ast = calloc (1, sizeof (struct AST));

  ast->location = location;
  ast->kind = kind;

  return ast;
}


void
AST_Destroy (struct AST *ast)
{
  if (ast->child)
    AST_Destroy (ast->child);

  if (ast->next)
    AST_Destroy (ast->next);

  Token_Destroy (ast->token);

  if (ast->type)
    Type_Destroy (ast->type);

  // if (ast->scope)
  //   Scope_Destroy (ast->scope);

  free (ast);
}


void
AST_Attach (struct AST *root, struct AST *node)
{
  struct AST *current = root;

  while (current->next != NULL)
    current = current->next;

  current->next = node;
}


void
AST_Append (struct AST *root, struct AST *node)
{
  if (root->child == NULL)
    root->child = node;
  else
    AST_Attach (root->child, node);
}

void
AST_Switch_Type (struct AST *ast, struct Type *type)
{
  if (ast->type)
    Type_Destroy (ast->type);

  ast->type = type;
}


int
AST_Is_LV (struct AST *ast)
{
  switch (ast->kind)
    {
    case AST_IDENTIFIER:
      return 1;
    case AST_UNARY:
      switch (ast->token.kind)
        {
        case TOKEN_STAR:
          return 1;
        default:
          return 0;
        }
    default:
      return 0;
    }
}


void
AST_Diagnostic_Base (struct AST *root, size_t depth)
{
  for (size_t i = depth * 4; i--;)
    if (i % 4 == 3)
      fprintf (stderr, "┊");
    else
      fprintf (stderr, " ");

  fprintf (stderr, "%s", AST_Kind_String (root->kind));

  if (root->token.kind != TOKEN_EOF)
    {
      fprintf (stderr, " ");
      Token_Diagnostic (root->token);
    }

  if (root->type != NULL)
    {
      fprintf (stderr, " : ");
      Type_Diagnostic (root->type);
    }

  // fprintf (stderr, " (state %d)", root->state);
  // fprintf (stderr, " at ");
  // Location_Diagnostic (root->location);

  fprintf (stderr, "\n");

  if (root->child)
    AST_Diagnostic_Base (root->child, depth + 1);

  if (root->next)
    AST_Diagnostic_Base (root->next, depth);
}


void
AST_Diagnostic (struct AST *root)
{
  AST_Diagnostic_Base (root, 0);
}

