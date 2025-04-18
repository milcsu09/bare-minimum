#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>

#include "Resolver.h"
#include "Checker.h"

char *
Read_File (const char *filename)
{
  FILE *file = fopen (filename, "r");

  fseek (file, 0, SEEK_END);
  size_t file_size = ftell (file);
  fseek (file, 0, SEEK_SET);

  char *buffer = (char *)malloc (file_size + 1);
  (void)fread (buffer, 1, file_size, file);

  buffer[file_size] = '\0';

  fclose (file);
  return buffer;
}


void
Main_Scope_Add_Type (struct Scope *scope, char *name, struct Type *type)
{
  Scope_Add (scope, Symbol_Create (name, type));
  Type_Destroy (type);
}


int
main (void)
{
  const char *path = "Tests/Main.txt";
  char *source = Read_File (path);

  struct Lexer lexer = Lexer_Create (source, path);

  struct Parser parser = Parser_Create (&lexer);

  struct AST *ast = Parser_Parse (&parser);

  fprintf (stderr, "----------------\n");
  AST_Diagnostic (ast);
  fprintf (stderr, "----------------\n");

  struct Scope *type_scope;

  type_scope = Scope_Create (NULL);

  Main_Scope_Add_Type (type_scope, "Void", Type_Create (TYPE_VOID));
  Main_Scope_Add_Type (type_scope, "I8", Type_Create (TYPE_I8));
  Main_Scope_Add_Type (type_scope, "I16", Type_Create (TYPE_I16));
  Main_Scope_Add_Type (type_scope, "I32", Type_Create (TYPE_I32));
  Main_Scope_Add_Type (type_scope, "I64", Type_Create (TYPE_I64));
  Main_Scope_Add_Type (type_scope, "U8", Type_Create (TYPE_U8));
  Main_Scope_Add_Type (type_scope, "U16", Type_Create (TYPE_U16));
  Main_Scope_Add_Type (type_scope, "U32", Type_Create (TYPE_U32));
  Main_Scope_Add_Type (type_scope, "U64", Type_Create (TYPE_U64));
  Main_Scope_Add_Type (type_scope, "F32", Type_Create (TYPE_F32));
  Main_Scope_Add_Type (type_scope, "F64", Type_Create (TYPE_F64));
  Main_Scope_Add_Type (type_scope, "Bool", Type_Create (TYPE_BOOL));

  Resolver_Resolve (ast, type_scope);

  fprintf (stderr, "----------------\n");
  AST_Diagnostic (ast);
  fprintf (stderr, "----------------\n");

  Checker_Check (ast);


  Scope_Destroy (type_scope);

  AST_Destroy (ast);

  free (source);

  /*
  struct Lexer lexer = Lexer_Create ("42", "__anon");
  struct Token token;

  while ((token = Lexer_Next (&lexer)).kind != TOKEN_EOF)
    {
      Token_Debug_Print (token);
      printf ("\n");
      Token_Destroy (token);
    }
  */

  return 0;
}

