#include "AST.h"
#include <stdio.h>
#include <stdlib.h>


static const char *const AST_KIND_STRING[] = {
  "PROGRAM",

  "PROTOTYPE",
  "FUNCTION",

  "ALIAS",
  "DEFER",

  "VARIABLE",
  "IF",
  "WHILE",

  "UNARY",
  "BINARY",
  "CAST",

  "ACCESS",
  "COMPOUND",
  "IDENTIFIER",
  "CALL",
  "I64",
  "F64",
  "STRING",
  "INITIALIZER",
  "SIZEOF",
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
    case AST_BINARY:
      switch (ast->token.kind)
        {
        case TOKEN_EQUALS:
          return 1;
        default:
          return 0;
        }
    case AST_ACCESS:
      return 1;
    case AST_CAST:
      return AST_Is_LV (ast->child);
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

  fprintf (stderr, " (state %d)", root->state);
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


void
AST_Dump_Indent (size_t depth)
{
  for (size_t i = depth * 2; i--;)
    fprintf (stderr, " ");
}


void
AST_Dump_String (const char *s)
{
  fprintf(stderr, "\"");
  while (*s)
    {
      switch (*s)
        {
        case '\n':
          fprintf (stderr, "\\n");
          break;
        case '\t':
          fprintf (stderr, "\\t");
          break;
        case '\r':
          fprintf (stderr, "\\r");
          break;
        case '\b':
          fprintf (stderr, "\\b");
          break;
        case '\f':
          fprintf (stderr, "\\f");
          break;
        case '\\':
          fprintf (stderr, "\\\\");
          break;
        case '\"':
          fprintf (stderr, "\\\"");
          break;
        case '\a':
          fprintf (stderr, "\\a");
          break;
        case '\v':
          fprintf (stderr, "\\v");
          break;
        default:
          if ((unsigned char)*s < 32 || (unsigned char)*s == 127)
            fprintf (stderr, "\\x%02x", (unsigned char)*s);
          else
            putc (*s, stderr);
        }
      s++;
    }
  fprintf(stderr, "\"");
}


void
AST_Dump_Base (struct AST *root, size_t depth)
{
  switch (root->kind)
    {
    case AST_PROGRAM:
      {
        struct AST *current = root->child;
        while (current)
          {
            AST_Dump_Indent (depth);

            AST_Dump_Base (current, depth);
            fprintf (stderr, ";");
            fprintf (stderr, "\n");
            current = current->next;
            if (current)
              fprintf (stderr, "\n\n");
          }
      }
      break;
    case AST_PROTOTYPE:
      {
        fprintf (stderr, "%s (", root->child->token.value.s);
        struct AST *current = root->child->next;
        while (current)
          {
            fprintf (stderr, "%s", current->token.value.s);
            current = current->next;
            if (current)
              fprintf (stderr, ", ");
          }
        fprintf (stderr, ") : ");
        Type_Diagnostic (root->type);
      }
      break;
    case AST_FUNCTION:
      {
        AST_Dump_Base (root->child, depth);
        fprintf (stderr, " = ");
        AST_Dump_Base (root->child->next, depth);
      }
      break;
    case AST_ALIAS:
      {
        fprintf (stderr, "alias ");
        Type_Diagnostic (root->type);
        fprintf (stderr, " as %s", root->token.value.s);
      }
      break;
    case AST_DEFER:
      {
        fprintf (stderr, "defer ");
        AST_Dump_Base (root->child, depth);
      }
      break;
    case AST_VARIABLE:
      {
        fprintf (stderr, "%s", root->token.value.s);

        if (root->child)
          fprintf (stderr, " := ");
        else
          {
            fprintf (stderr, " : ");
            Type_Diagnostic (root->type);
          }

        if (root->child)
          AST_Dump_Base (root->child, depth);

        /*
        AST_Dump_Base (root->child, depth);

        if (!root->type || (root->child && root->child->kind != AST_CAST))
          fprintf (stderr, " := ");
        else
          {
            fprintf (stderr, " : ");
            Type_Diagnostic (root->type);
          }

        if (root->child)
          {
            if (root->child->kind == AST_CAST)
              fprintf (stderr, " = ");

            AST_Dump_Base (root->child, depth);
          }
        */
      }
      break;
    case AST_IF:
      {
        fprintf (stderr, "if ");
        AST_Dump_Base (root->child, depth);
        fprintf (stderr, " then");

        if (root->child->next->kind != AST_COMPOUND)
          {
            printf ("\n");
            AST_Dump_Indent (depth + 1);
            AST_Dump_Base (root->child->next, depth + 1);
          }
        else
          AST_Dump_Base (root->child->next, depth);

        if (root->child->next->next)
          {
            fprintf (stderr, ";");
            fprintf (stderr, "\n");
            AST_Dump_Indent (depth);
            fprintf (stderr, "else");
            if (root->child->next->next->kind != AST_COMPOUND)
              {
                printf ("\n");
                AST_Dump_Indent (depth + 1);
                AST_Dump_Base (root->child->next->next, depth + 1);
              }
            else
              AST_Dump_Base (root->child->next->next, depth);
          }
      }
      break;
    case AST_WHILE:
      {
        fprintf (stderr, "while ");
        AST_Dump_Base (root->child, depth);
        fprintf (stderr, " then");
        if (root->child->next->kind != AST_COMPOUND)
          {
            printf ("\n");
            AST_Dump_Indent (depth + 1);
            AST_Dump_Base (root->child->next, depth + 1);
          }
        else
          AST_Dump_Base (root->child->next, depth);
      }
      break;
    case AST_UNARY:
      {
        int need_paren;

        switch (root->child->kind)
          {
          case AST_BINARY:
          case AST_ACCESS:
            need_paren = 1;
            break;
          default:
            need_paren = 0;
            break;
          }

        fprintf (stderr, "%s", Token_Kind_String (root->token.kind));
        if (need_paren)
          fprintf (stderr, "(");
        AST_Dump_Base (root->child, depth);
        if (need_paren)
          fprintf (stderr, ")");
      }
      break;
    case AST_BINARY:
      {
        AST_Dump_Base (root->child, depth);
        fprintf (stderr, " %s ", Token_Kind_String (root->token.kind));
        AST_Dump_Base (root->child->next, depth);
      }
      break;
    case AST_CAST:
      {
        int need_paren;

        switch (root->child->kind)
          {
          case AST_BINARY:
          case AST_SIZEOF:
          case AST_CAST:
            need_paren = 1;
            break;
          default:
            need_paren = 0;
            break;
          }

        if (need_paren)
          fprintf (stderr, "(");
        AST_Dump_Base (root->child, depth);

        if (need_paren)
          fprintf (stderr, ")");

        fprintf (stderr, " as ");
        Type_Diagnostic (root->type);
      }
      break;
    case AST_ACCESS:
      {
        AST_Dump_Base (root->child, depth);
        fprintf (stderr, ".%s", root->token.value.s);
      }
      break;
    case AST_COMPOUND:
      {
        fprintf (stderr, "\n");

        AST_Dump_Indent (depth + 1);

        fprintf (stderr, "{\n");

        struct AST *current = root->child;
        while (current)
          {
            AST_Dump_Indent (depth + 2);

            AST_Dump_Base (current, depth + 2);
            if (current->next || root->state)
              fprintf (stderr, ";");
            fprintf (stderr, "\n");
            current = current->next;
            if (current)
              fprintf (stderr, "\n");
          }

        AST_Dump_Indent (depth + 1);
        fprintf (stderr, "}");
      }
      break;
    case AST_IDENTIFIER:
      {
        fprintf (stderr, "%s", root->token.value.s);
      }
      break;
    case AST_CALL:
      {
        AST_Dump_Base (root->child, depth);
        fprintf (stderr, " (");

        struct AST *current = root->child->next;
        while (current)
          {
            AST_Dump_Base (current, depth);
            current = current->next;
            if (current)
              fprintf (stderr, ", ");
          }

        fprintf (stderr, ")");
      }
      break;
    case AST_I64:
      {
        fprintf (stderr, "%ld", root->token.value.i64);
      }
      break;
    case AST_F64:
      {
        fprintf (stderr, "%g", root->token.value.f64);
      }
      break;
    case AST_STRING:
      {
        AST_Dump_String (root->token.value.s);
        // fprintf (stderr, "\"%s\"", );
      }
      break;
    case AST_INITIALIZER:
      {
        fprintf (stderr, "[");

        struct AST *current = root->child;
        while (current)
          {
            AST_Dump_Base (current, depth);
            current = current->next;
            if (current)
              fprintf (stderr, ", ");
          }

        fprintf (stderr, "]");
      }
      break;
    case AST_SIZEOF:
      {
        fprintf (stderr, "sizeof ");
        Type_Diagnostic (root->type);
      }
      break;
    }
}


void
AST_Dump (struct AST *root)
{
  AST_Dump_Base (root, 0);
}

