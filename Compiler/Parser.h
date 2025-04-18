#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Token.h"
#include "Lexer.h"


struct Parser
{
  struct Lexer *lexer;
  struct Token current;
  struct Location location;
};


struct Parser Parser_Create (struct Lexer *);

struct AST *Parser_Parse (struct Parser *);


#endif // PARSER_H

