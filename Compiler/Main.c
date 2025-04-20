#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include <llvm-c/Analysis.h>
#include <llvm-c/ExecutionEngine.h>

#include "CG.h"
#include "Checker.h"
#include "Lexer.h"
#include "Parser.h"
#include "Resolver.h"
#include "String.h"
#include "Scope.h"
#include "Type.h"

void Usage(const char *name)
{
  fprintf (stderr, "Usage: %s [options] <file>\n", name);
  fprintf (stderr, "Options:\n");
  fprintf (stderr, "  -h, --help           Display this information.\n");
  fprintf (stderr, "  -d, --dump           Dump module into '.ll' file.\n");
  fprintf (stderr, "  -a, --ast            Dump AST.\n");
  fprintf (stderr, "  -f, --fast           Dump formatted AST.\n");
}


char *
Read_File (const char *filename)
{
  FILE *file = fopen (filename, "r");
  if (file == NULL)
    {
      perror (filename);
      exit (1);
    }

  fseek (file, 0, SEEK_END);
  size_t file_size = ftell (file);
  fseek (file, 0, SEEK_SET);

  char *buffer = (char *)malloc (file_size + 1);
  size_t read_n = fread (buffer, 1, file_size, file);

  if (read_n != file_size)
    {
      perror(filename);
      exit (1);
    }

  buffer[file_size] = '\0';

  fclose (file);
  return buffer;
}


char *
Change_Extension (const char *org, const char *new_ext)
{
  char *copy = String_Copy (org);

  char *ext = strrchr (copy, '.');

  if (ext)
    *ext = '\0';

  size_t new_size = strlen (copy) + strlen (new_ext) + 1;

  char *new_name = malloc (new_size);

  sprintf (new_name, "%s%s", copy, new_ext);

  free (copy);

  return new_name;
}


void
Main_Scope_Add_Type (struct Scope *scope, char *name, struct Type *type)
{
  Scope_Add (scope, Symbol_Create_Type (name, type));
  Type_Destroy (type);
}


int
main (int argc, char *argv[])
{
  static struct option long_options[]
      = { { "help", no_argument, 0, 'h' },
          { "dump", no_argument, 0, 'd' },
          { "ast", no_argument, 0, 'a' },
          { "fast", no_argument, 0, 'f' },
          { 0, 0, 0, 0 } };

  int dump_module = 0;
  int dump_ast = 0;
  int dump_fmt = 0;

  int opt;

  while ((opt = getopt_long (argc, argv, "hdaf", long_options, NULL)) != -1)
    switch (opt)
      {
      case 'h':
        Usage (argv[0]);
        return 1;
      case 'd':
        dump_module = 1;
        break;
      case 'a':
        dump_ast = 1;
        break;
      case 'f':
        dump_fmt = 1;
        break;
      default:
        fprintf (stderr, "Use --help for usage information.\n");
        return 1;
      };

  if (optind >= argc)
    {
      fprintf (stderr, "%s: no input files specified.\n", argv[0]);
      return 1;
    }

  LLVMInitializeAllTargetInfos();
  LLVMInitializeAllTargets();
  LLVMInitializeAllTargetMCs();
  LLVMInitializeAllAsmPrinters();
  LLVMInitializeAllAsmParsers();

  for (int i = optind; i < argc; i++)
    {
      char *path = argv[i];
      char *file_ll = Change_Extension (path, ".ll");
      char *file_o = Change_Extension (path, ".o");

      char *source = Read_File (path);

      struct Lexer lexer = Lexer_Create (source, path);

      struct Parser parser = Parser_Create (&lexer);

      struct AST *ast = Parser_Parse (&parser);

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

      if (dump_ast)
        {
          fprintf (stderr, "\n");
          AST_Diagnostic (ast);
          fprintf (stderr, "\n");
        }

      if (dump_fmt)
        {
          fprintf (stderr, "\n");
          AST_Dump (ast);
          fprintf (stderr, "\n");
        }

      Checker_Check (ast);

      struct CG *cg = CG_Create (path); // Use file path as module name.

      struct Scope *cg_scope;

      cg_scope = Scope_Create (NULL);

      CG_Generate (cg, ast, cg_scope);

      char *error = NULL;

      if (LLVMVerifyModule (cg->module, LLVMAbortProcessAction, &error) != 0)
        {
          fprintf (stderr, "%s: Error: %s\n", path, error);
          LLVMDisposeMessage (error);
          return 1;
        }

      LLVMDisposeMessage (error);

      if (dump_module)
        LLVMPrintModuleToFile (cg->module, file_ll, NULL);

      const char *targetTriple = LLVMGetDefaultTargetTriple ();
      LLVMTargetRef target;

      if (LLVMGetTargetFromTriple (targetTriple, &target, &error) != 0)
        {
          fprintf (stderr, "%s: Error: Unable to get target for %s\n", path,
                   targetTriple);
          return 1;
        }

      LLVMTargetMachineRef targetMachine = LLVMCreateTargetMachine (
          target, targetTriple, "", "", LLVMCodeGenLevelNone, LLVMRelocPIC,
          LLVMCodeModelDefault);

      if (LLVMTargetMachineEmitToFile (targetMachine, cg->module, file_o,
                                       LLVMObjectFile, &error))
        {
          fprintf (stderr, "%s: Error: %s\n", path, error);
          LLVMDisposeMessage (error);
          return 1;
        }

      printf ("%s => %s", path, file_o);
      if (dump_module)
        printf (", %s", file_ll);
      printf ("\n");

      LLVMDisposeTargetMachine (targetMachine);
      LLVMDisposeMessage ((void *)targetTriple);

      CG_Destroy (cg);
      Scope_Destroy_Value (cg_scope);
      Scope_Destroy_Type (type_scope);

      AST_Destroy (ast);

      free (source);

      free (file_ll);
      free (file_o);
    }
}

