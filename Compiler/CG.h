#ifndef CG_H
#define CG_H

#include "AST.h"
#include "Scope.h"
#include <llvm-c/Core.h>
#include <llvm-c/Transforms/Utils.h>

struct CG
{
  LLVMContextRef context;
  LLVMModuleRef module;
  LLVMBuilderRef builder;
  LLVMPassManagerRef pass;
  LLVMValueRef function;
};


struct CG *CG_Create (const char *);

void CG_Destroy (struct CG *);

LLVMValueRef CG_Generate (struct CG *, struct AST *, struct Scope *);


#endif // CG_H

