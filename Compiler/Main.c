#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>

#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>

#include "CG.h"
#include "Checker.h"
#include "Resolver.h"
#include "Scope.h"

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
  Scope_Add (scope, Symbol_Create_Type (name, type));
  Type_Destroy (type);
}

int
main (void)
{
  LLVMInitializeAllTargetInfos ();
  LLVMInitializeAllTargets ();
  LLVMInitializeAllTargetMCs ();
  LLVMInitializeAllAsmParsers ();
  LLVMInitializeAllAsmPrinters ();

  const char *path = "Tests/Main.txt";
  char *source = Read_File (path);

  struct Lexer lexer = Lexer_Create (source, path);

  struct Parser parser = Parser_Create (&lexer);

  struct AST *ast = Parser_Parse (&parser);

  // fprintf (stderr, "----------------\n");
  // AST_Diagnostic (ast);
  // fprintf (stderr, "----------------\n");

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

  // fprintf (stderr, "----------------\n");
  // AST_Diagnostic (ast);
  // fprintf (stderr, "----------------\n");

  Checker_Check (ast);

  struct CG *cg = CG_Create ();

  struct Scope *cg_scope;

  cg_scope = Scope_Create (NULL);

  CG_Generate (cg, ast, cg_scope);

  // ASDASD

  char *error = NULL;

  char *triple = LLVMGetDefaultTargetTriple();

  LLVMTargetRef target;
  if (LLVMGetTargetFromTriple(triple, &target, &error) != 0) {
      fprintf(stderr, "Error getting target: %s\n", error);
      LLVMDisposeMessage(error);
      exit(1);
  }

  LLVMTargetMachineRef machine = LLVMCreateTargetMachine (
      target, triple, "", "", LLVMCodeGenLevelDefault, LLVMRelocPIC,
      LLVMCodeModelDefault);

  LLVMSetTarget (cg->module, triple);

  LLVMTargetDataRef data_layout = LLVMCreateTargetDataLayout(machine);
  char *layout_str = LLVMCopyStringRepOfTargetData(data_layout);
  LLVMSetDataLayout(cg->module, layout_str);
  LLVMDisposeMessage(layout_str);

  if (LLVMTargetMachineEmitToFile (machine, cg->module, "Tests/Main.o", LLVMObjectFile,
                                   &error))
    {
      fprintf (stderr, "Error emitting object file: %s\n", error);
      LLVMDisposeMessage (error);
      return 1;
    }

  if (LLVMTargetMachineEmitToFile (machine, cg->module, "Tests/Main.s", LLVMAssemblyFile,
                                   &error))
    {
      fprintf (stderr, "Error emitting assembly file: %s\n", error);
      LLVMDisposeMessage (error);
      return 1;
    }

  error = NULL;
  if (LLVMPrintModuleToFile (cg->module, "Tests/Main.ll", &error))
    {
      fprintf (stderr, "Error writing IR file: %s\n", error);
      LLVMDisposeMessage (error);
    }


  LLVMDisposeTargetMachine (machine);
  LLVMDisposeMessage (triple);
 
  // ASDASD

  /*
  char *error = NULL;
  LLVMPrintModuleToFile (cg->module, "Tests/Main.ll", &error);

 char *triple = LLVMGetDefaultTargetTriple();

  LLVMTargetRef target;
  if (LLVMGetTargetFromTriple(triple, &target, &error) != 0) {
      fprintf(stderr, "Error getting target: %s\n", error);
      LLVMDisposeMessage(error);
      exit(1);
  }

  // Create target machine
  LLVMTargetMachineRef target_machine = LLVMCreateTargetMachine(
      target,
      triple,
      "",     // CPU
      "",     // Features
      LLVMCodeGenLevelDefault,
      LLVMRelocPIC,
      LLVMCodeModelDefault
  );

  // Set triple and datalayout on module
  LLVMSetTarget(cg->module, triple);

  LLVMTargetDataRef data_layout = LLVMCreateTargetDataLayout(target_machine);
  char *layout_str = LLVMCopyStringRepOfTargetData(data_layout);
  LLVMSetDataLayout(cg->module, layout_str);
  LLVMDisposeMessage(layout_str);

  // Verify module (optional but useful)
  if (LLVMVerifyModule(cg->module, LLVMAbortProcessAction, &error) != 0) {
      fprintf(stderr, "Module verification failed: %s\n", error);
      LLVMDisposeMessage(error);
      exit(1);
  }

  LLVMDisposeMessage(error);

  // Emit object file
  if (LLVMTargetMachineEmitToFile(
          target_machine,
          cg->module,
          "Tests/Main.o",
          LLVMObjectFile,
          &error) != 0) {
      fprintf(stderr, "Error emitting object file: %s\n", error);
      LLVMDisposeMessage(error);
      exit(1);
  }

  if (LLVMTargetMachineEmitToFile(
        target_machine,
          cg->module,
          "Tests/Main.s",
        LLVMAssemblyFile,
        &error) != 0) {
    fprintf(stderr, "Error emitting assembly file: %s\n", error);
    LLVMDisposeMessage(error);
    exit(1);
  }

  LLVMDumpModule (cg->module);
  */

  CG_Destroy (cg);

  Scope_Destroy_Value (cg_scope);
  Scope_Destroy_Type (type_scope);

  AST_Destroy (ast);

  free (source);

  return 0;
}

