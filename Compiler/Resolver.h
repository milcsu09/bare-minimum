#ifndef RESOLVER_H
#define RESOLVER_H

#include "AST.h"
#include "Scope.h"


void Resolver_Resolve (struct AST *, struct Scope *);


#endif // RESOLVER_H

