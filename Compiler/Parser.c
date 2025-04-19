#include "Diagnostic.h"
#include "Lexer.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>


struct Parser
Parser_Create (struct Lexer *lexer)
{
  struct Parser parser;

  parser.lexer = lexer;

  return parser;
}


void
Parser_Advance (struct Parser *parser)
{
  parser->current = Lexer_Next (parser->lexer);
  parser->location = parser->current.location;
}


int
Parser_Match (struct Parser *parser, enum Token_Kind kind)
{
  return parser->current.kind == kind;
}


void
Parser_Diagnostic_Expect_Base (struct Location location, const char *a,
                               const char *b)
{
  Diagnostic (location, D_ERROR, "expected '%s' recieved '%s'", a, b);
}


void
Parser_Diagnostic_Expect (struct Parser *parser, enum Token_Kind kind)
{
  const char *a = Token_Kind_String (kind);
  const char *b = Token_Kind_String (parser->current.kind);
  Parser_Diagnostic_Expect_Base (parser->location, a, b);
}


void
Parser_Expect (struct Parser *parser, enum Token_Kind kind)
{
  if (Parser_Match (parser, kind))
    ;
  else
    {
      Parser_Diagnostic_Expect (parser, kind);
      Halt ();
    }
}


void
Parser_Expect_Advance (struct Parser *parser, enum Token_Kind kind)
{
  Parser_Expect (parser, kind);
  Parser_Advance (parser);
}

struct Type *
Parser_Parse_Type_Primary (struct Parser *parser)
{
  struct Token token = parser->current;

  switch (token.kind)
    {
    case TOKEN_TYPE:
      Parser_Advance (parser);
      return Type_Create_Dummy (token);
    default:
      Parser_Diagnostic_Expect (parser, TOKEN_TYPE);
      Halt ();
    }
}


struct Type *
Parser_Parse_Type (struct Parser *parser)
{
  switch (parser->current.kind)
    {
    case TOKEN_LPAREN:
      {
        Parser_Advance (parser);

        struct Type **in;
        size_t in_capacity = 4;
        size_t in_n = 0;
        int variadic = 0;

        in = calloc (in_capacity, sizeof (struct Type *));

        if (!Parser_Match (parser, TOKEN_RPAREN))
          while (1)
            {
              if (Parser_Match (parser, TOKEN_3DOT))
                {
                  Parser_Advance (parser);
                  variadic = 1;
                  break;
                }

              struct Type *argument;

              argument = Parser_Parse_Type (parser);

              if (in_n >= in_capacity)
                {
                  in_capacity *= 2;
                  in = realloc (in, in_capacity * sizeof (struct Type *));
                }

              in[in_n++] = argument;

              if (Parser_Match (parser, TOKEN_RPAREN))
                break;

              if (!Parser_Match (parser, TOKEN_COMMA))
                Parser_Expect (parser, TOKEN_RPAREN);
              else
                Parser_Advance (parser);
            }

        Parser_Expect_Advance (parser, TOKEN_RPAREN);
        Parser_Expect_Advance (parser, TOKEN_ARROW);

        struct Type *out = Parser_Parse_Type (parser);

        struct Type_Function function;

        function = Type_Function_Create (in_n, in, out, variadic);

        // Type_Function_Create allocates it's own array.
        free (in);

        return Type_Create_Function (function);
      }
      break;
    case TOKEN_STAR:
      {
        Parser_Advance (parser);
        struct Type *base = Parser_Parse_Type (parser);
        return Type_Create_Pointer (base);
      }
      break;
    case TOKEN_LBRACKET:
      {
        Parser_Advance (parser);

        struct Type_Field *fields;
        size_t fields_capacity = 4;
        size_t fields_n = 0;

        fields = calloc (fields_capacity, sizeof (struct Type_Field));

        while (1)
          {
            Parser_Expect (parser, TOKEN_IDENTIFIER);
            const char *name = parser->current.value.s;
            Parser_Advance (parser);

            Parser_Expect_Advance (parser, TOKEN_COLON);

            struct Type *type;

            type = Parser_Parse_Type (parser);

            if (fields_n >= fields_capacity)
              {
                fields_capacity *= 2;
                fields = realloc (fields,
                                  fields_capacity * sizeof (struct Type *));
              }

            fields[fields_n++] = Type_Field_Create (name, type);

            if (Parser_Match (parser, TOKEN_RBRACKET))
              break;

            if (!Parser_Match (parser, TOKEN_COMMA))
              Parser_Expect (parser, TOKEN_RBRACKET);
            else
              Parser_Advance (parser);
          }

        Parser_Expect_Advance (parser, TOKEN_RBRACKET);

        struct Type_Structure structure;

        structure = Type_Structure_Create (fields_n, fields);

        free (fields);

        return Type_Create_Structure (structure);
      }
      break;
    default:
      return Parser_Parse_Type_Primary (parser);
    }
}


struct AST *
Parser_Parse_Identifier (struct Parser *parser)
{
  Parser_Expect (parser, TOKEN_IDENTIFIER);

  struct AST *result;

  result = AST_Create (parser->location, AST_IDENTIFIER);

  result->token = parser->current;

  Parser_Advance (parser);

  return result;
}

struct AST *Parser_Parse_Program (struct Parser *);

struct AST *Parser_Parse_Top_Level_Statement (struct Parser *);

struct AST *Parser_Parse_Function_Prototype (struct Parser *);

struct AST *Parser_Parse_Statement (struct Parser *);

struct AST *Parser_Parse_Alias_Statement (struct Parser *);

struct AST *Parser_Parse_Defer_Statement (struct Parser *);

struct AST *Parser_Parse_Control_Statement (struct Parser *);

struct AST *Parser_Parse_If_Statement (struct Parser *);

struct AST *Parser_Parse_While_Statement (struct Parser *);

struct AST *Parser_Parse_Expression (struct Parser *);

struct AST *Parser_Parse_Primary_Expression (struct Parser *);

struct AST *Parser_Parse_Call_Expression (struct Parser *);

struct AST *Parser_Parse_Access_Expression (struct Parser *);

struct AST *Parser_Parse_Atom_Expression (struct Parser *);

struct AST *Parser_Parse_Group_Expression (struct Parser *);

struct AST *Parser_Parse_Compound_Expression (struct Parser *);

struct AST *Parser_Parse_Number (struct Parser *);

struct AST *Parser_Parse_String (struct Parser *);

struct AST *Parser_Parse_Initializer (struct Parser *);


struct AST *
Parser_Parse_Program (struct Parser *parser)
{
  struct AST *result;

  result = AST_Create (parser->location, AST_PROGRAM);

  while (1)
    switch (parser->current.kind)
      {
      case TOKEN_EOF:
        return result;
      default:
        {
          struct AST *statement;

          statement = Parser_Parse_Top_Level_Statement (parser);

          Parser_Expect_Advance (parser, TOKEN_SEMICOLON);

          AST_Append (result, statement);
        }
        break;
      }
}


struct AST *
Parser_Parse_Top_Level_Statement (struct Parser *parser)
{
  if (Parser_Match (parser, TOKEN_ALIAS))
    return Parser_Parse_Alias_Statement (parser);

  struct AST *prototype;

  prototype = Parser_Parse_Function_Prototype (parser);

  if (Parser_Match (parser, TOKEN_EQUALS))
    Parser_Advance (parser);
  else
    return prototype;

  struct AST *expression;

  expression = Parser_Parse_Expression (parser);

  struct AST *function;

  function = AST_Create (prototype->location, AST_FUNCTION);

  AST_Append (function, prototype);
  AST_Append (function, expression);

  return function;
}


struct AST *
Parser_Parse_Function_Prototype (struct Parser *parser)
{
  struct AST *name;

  name = Parser_Parse_Identifier (parser);

  Parser_Expect_Advance (parser, TOKEN_LPAREN);

  struct AST *result;

  // int variadic = 0; // result->state

  result = AST_Create (name->location, AST_PROTOTYPE);

  AST_Append (result, name);

  if (!Parser_Match (parser, TOKEN_RPAREN))
    while (1)
      {
        // if (Parser_Match (parser, TOKEN_3DOT))
        //   {
        //     variadic = 1;

        //     Parser_Advance (parser);

        //     break;
        //   }

        struct AST *argument;

        argument = Parser_Parse_Identifier (parser);

        AST_Append (result, argument);

        if (Parser_Match (parser, TOKEN_RPAREN))
          break;

        if (!Parser_Match (parser, TOKEN_COMMA))
          Parser_Expect (parser, TOKEN_RPAREN);
        else
          Parser_Advance (parser);

        // Parser_Expect_Advance (parser, TOKEN_COMMA);
      }

  Parser_Expect_Advance (parser, TOKEN_RPAREN);

  Parser_Expect_Advance (parser, TOKEN_COLON);

  struct Type *type;

  type = Parser_Parse_Type (parser);

  result->type = type;
  // result->state = variadic;

  return result;
}


struct AST *
Parser_Parse_Statement (struct Parser *parser)
{
  if (Parser_Match (parser, TOKEN_IDENTIFIER))
    {
      struct Token name = parser->current;

      struct Token peek = Lexer_Peek (parser->lexer);

      Token_Destroy (peek); // In-case it allocated memory.

      if (peek.kind != TOKEN_COLON && peek.kind != TOKEN_WALRUS)
        return Parser_Parse_Expression (parser);

      Parser_Advance (parser);

      struct AST *result;

      result = AST_Create (name.location, AST_VARIABLE);

      result->token = name;

      if (Parser_Match (parser, TOKEN_COLON))
        {
          Parser_Advance (parser);

          result->type = Parser_Parse_Type (parser);

          if (!Parser_Match (parser, TOKEN_EQUALS))
            {
              Parser_Expect (parser, TOKEN_SEMICOLON);
              return result;
            }

          Parser_Expect_Advance (parser, TOKEN_EQUALS);
        }
      else // Must be WALRUS
        Parser_Advance (parser);

      struct AST *expression;

      expression = Parser_Parse_Expression (parser);

      AST_Append (result, expression);

      Parser_Expect (parser, TOKEN_SEMICOLON);
      return result;
    }

  return Parser_Parse_Alias_Statement (parser);
}


struct AST *
Parser_Parse_Alias_Statement (struct Parser *parser)
{
  if (Parser_Match (parser, TOKEN_ALIAS))
    {
      struct Location location = parser->location;

      Parser_Advance (parser);

      struct Type *type = Parser_Parse_Type (parser);

      Parser_Expect_Advance (parser, TOKEN_AS);

      Parser_Expect (parser, TOKEN_TYPE);

      struct Token name = parser->current;

      Parser_Advance (parser);

      struct AST *result;

      result = AST_Create (location, AST_ALIAS);

      result->token = name;
      result->type = type;

      Parser_Expect (parser, TOKEN_SEMICOLON);

      return result;
    }

  return Parser_Parse_Defer_Statement (parser);
}


struct AST *
Parser_Parse_Defer_Statement (struct Parser *parser)
{
  struct Location location = parser->location;

  if (Parser_Match (parser, TOKEN_DEFER))
    {
      Parser_Expect_Advance (parser, TOKEN_DEFER);

      struct AST *expression;

      expression = Parser_Parse_Expression (parser);

      struct AST *result;

      result = AST_Create (location, AST_DEFER);

      AST_Append (result, expression);

      Parser_Expect (parser, TOKEN_SEMICOLON);

      return result;
    }

  return Parser_Parse_Control_Statement (parser);
}


struct AST *
Parser_Parse_Control_Statement (struct Parser *parser)
{
  switch (parser->current.kind)
    {
    case TOKEN_IF:
      return Parser_Parse_If_Statement (parser);
    case TOKEN_WHILE:
      return Parser_Parse_While_Statement (parser);
    default:
      return Parser_Parse_Expression (parser);
    }
}


struct AST *
Parser_Parse_If_Statement (struct Parser *parser)
{
  struct Location location = parser->location;

  Parser_Expect_Advance (parser, TOKEN_IF);

  struct AST *result;

  result = AST_Create (location, AST_IF);

  struct AST *condition;

  condition = Parser_Parse_Expression (parser);

  AST_Append (result, condition);

  Parser_Expect_Advance (parser, TOKEN_THEN);

  struct AST *then;

  then = Parser_Parse_Control_Statement (parser);

  AST_Append (result, then);

  // Little hacky, but works.
  struct Token peek = Lexer_Peek (parser->lexer);

  Token_Destroy (peek);

  Parser_Expect (parser, TOKEN_SEMICOLON);

  if (peek.kind == TOKEN_ELSE)
    Parser_Advance (parser);

  if (!Parser_Match (parser, TOKEN_ELSE))
    return result;

  Parser_Advance (parser);

  struct AST *otherwise;

  otherwise = Parser_Parse_Control_Statement (parser);

  AST_Append (result, otherwise);

  Parser_Expect (parser, TOKEN_SEMICOLON);

  return result;
}


struct AST *
Parser_Parse_While_Statement (struct Parser *parser)
{
  struct Location location = parser->location;

  Parser_Expect_Advance (parser, TOKEN_WHILE);

  struct AST *result;

  result = AST_Create (location, AST_WHILE);

  struct AST *condition;

  condition = Parser_Parse_Expression (parser);

  AST_Append (result, condition);

  Parser_Expect_Advance (parser, TOKEN_THEN);

  struct AST *then;

  then = Parser_Parse_Control_Statement (parser);

  AST_Append (result, then);

  Parser_Expect (parser, TOKEN_SEMICOLON);

  return result;
}


struct Operator
{
  enum Token_Kind kind;
  int precedence;
  int associvitaty; // 0 = Left, 1 = Right
};


struct Operator OPERATOR_TABLE[] = {
  {TOKEN_EQUALS, 10, 1},
  {TOKEN_PLUS, 90, 0},
  {TOKEN_MINUS, 90, 0},
  {TOKEN_STAR, 100, 0},
  {TOKEN_SLASH, 100, 0},
  {TOKEN_LT, 80, 0},
  {TOKEN_LE, 80, 0},
  {TOKEN_GT, 80, 0},
  {TOKEN_GE, 80, 0},
};


struct Operator
Parser_Get_Operator (enum Token_Kind kind)
{
  size_t size = sizeof OPERATOR_TABLE / sizeof (struct Operator);

  for (size_t i = 0; i < size; ++i)
    if (kind == OPERATOR_TABLE[i].kind)
      return OPERATOR_TABLE[i];

  return (struct Operator){ 0 };
}


struct AST *
Parser_Parse_Expression_Base (struct Parser *parser, int previous)
{
  struct AST *left;

  left = Parser_Parse_Primary_Expression (parser);

  struct Operator operator;

  operator = Parser_Get_Operator (parser->current.kind);

  while ((operator.associvitaty == 0 && operator.precedence > previous) ||
         (operator.associvitaty == 1 && operator.precedence >= previous))
    {
      struct Token middle = parser->current;

      Parser_Advance (parser);

      struct AST *right;

      if (operator.associvitaty == 0)
        right = Parser_Parse_Expression_Base (parser, operator.precedence);
      else
        right = Parser_Parse_Expression_Base (parser, operator.precedence - 1);

      struct AST *binary;

      binary = AST_Create (left->location, AST_BINARY);
      binary->token = middle;

      AST_Append (binary, left);
      AST_Append (binary, right);

      left = binary;

      operator = Parser_Get_Operator (parser->current.kind);
    }

  return left;
}


struct AST *
Parser_Parse_Expression (struct Parser *parser)
{
  return Parser_Parse_Expression_Base (parser, 0);
}


struct AST *
Parser_Parse_Primary_Expression (struct Parser *parser)
{
  struct AST *call;

  call = Parser_Parse_Call_Expression (parser);

  if (!Parser_Match (parser, TOKEN_AS))
    return call;

  Parser_Advance (parser);

  struct AST *cast;

  cast = AST_Create (call->location, AST_CAST);

  cast->type = Parser_Parse_Type (parser);

  AST_Append (cast, call);

  return cast;
}


struct AST *
Parser_Parse_Call_Expression (struct Parser *parser)
{
  struct AST *atom;

  atom = Parser_Parse_Access_Expression (parser);

  if (!Parser_Match (parser, TOKEN_LPAREN))
    return atom;

  struct AST *result;

  result = AST_Create (atom->location, AST_CALL);

  AST_Append (result, atom);

  Parser_Advance (parser);

  if (!Parser_Match (parser, TOKEN_RPAREN))
    while (1)
      {
        struct AST *expression;

        expression = Parser_Parse_Expression (parser);

        AST_Append (result, expression);

        if (Parser_Match (parser, TOKEN_RPAREN))
          break;

        if (!Parser_Match (parser, TOKEN_COMMA))
          Parser_Expect (parser, TOKEN_RPAREN);
        else
          Parser_Advance (parser);
      }

  Parser_Expect_Advance (parser, TOKEN_RPAREN);

  return result;
}


struct AST *
Parser_Parse_Access_Expression (struct Parser *parser)
{
  struct AST *expression;

  expression = Parser_Parse_Atom_Expression (parser);

  if (!Parser_Match (parser, TOKEN_DOT))
    return expression;

  while (Parser_Match (parser, TOKEN_DOT))
    {
      struct AST *result = AST_Create (expression->location, AST_ACCESS);

      Parser_Advance (parser);

      Parser_Expect (parser, TOKEN_IDENTIFIER);

      result->token = parser->current;

      AST_Append (result, expression);

      Parser_Advance (parser);

      expression = result;
    }

  return expression;
}


struct AST *
Parser_Parse_Unary (struct Parser *parser)
{
  struct Location location = parser->location;

  struct Token operator = parser->current;

  Parser_Advance (parser);

  struct AST *atom;

  atom = Parser_Parse_Atom_Expression (parser);

  struct AST *result;

  result = AST_Create (location, AST_UNARY);

  result->token = operator;

  AST_Append (result, atom);

  return result;
}


struct AST *
Parser_Parse_Atom_Expression (struct Parser *parser)
{
  switch (parser->current.kind)
    {
    case TOKEN_PLUS:
    case TOKEN_MINUS:
    case TOKEN_AMPERSAND:
    case TOKEN_STAR:
      return Parser_Parse_Unary (parser);

    case TOKEN_LPAREN:
      return Parser_Parse_Group_Expression (parser);
    case TOKEN_LBRACE:
      return Parser_Parse_Compound_Expression (parser);
    case TOKEN_IDENTIFIER:
      return Parser_Parse_Identifier (parser);
    case TOKEN_I64:
    case TOKEN_F64:
      return Parser_Parse_Number (parser);
    case TOKEN_STRING:
      return Parser_Parse_String (parser);
    case TOKEN_LBRACKET:
      return Parser_Parse_Initializer (parser);
    default:
      {
        const char *b = Token_Kind_String (parser->current.kind);
        Parser_Diagnostic_Expect_Base (parser->location, "expression", b);
        Halt ();
      }
    }
}


struct AST *
Parser_Parse_Group_Expression (struct Parser *parser)
{
  struct Location location = parser->location;

  Parser_Expect_Advance (parser, TOKEN_LPAREN);

  struct AST *result;

  result = Parser_Parse_Expression (parser);

  result->location = location;

  Parser_Expect_Advance (parser, TOKEN_RPAREN);

  return result;
}


struct AST *
Parser_Parse_Compound_Expression (struct Parser *parser)
{
  struct Location location = parser->location;

  Parser_Expect_Advance (parser, TOKEN_LBRACE);

  struct AST *result;

  int void_block = 0; // result->state

  result = AST_Create (location, AST_COMPOUND);

  struct AST *statement = NULL;

  if (Parser_Match (parser, TOKEN_RBRACE))
    void_block = 1;
  else
    while (1)
      {
        if (Parser_Match (parser, TOKEN_RBRACE))
          {
            void_block = 1;
            break;
          }

        statement = Parser_Parse_Statement (parser);

        AST_Append (result, statement);

        if (Parser_Match (parser, TOKEN_RBRACE))
          break;

        Parser_Expect_Advance (parser, TOKEN_SEMICOLON);
      }

  Parser_Expect_Advance (parser, TOKEN_RBRACE);

  result->state = void_block;

  return result;
}


struct AST *
Parser_Parse_Number (struct Parser *parser)
{
  switch (parser->current.kind)
    {
    case TOKEN_I64:
      {
        struct AST *result;

        result = AST_Create (parser->location, AST_I64);

        result->token = parser->current;

        Parser_Advance (parser);

        return result;
      }
    case TOKEN_F64:
      {
        struct AST *result;

        result = AST_Create (parser->location, AST_F64);

        result->token = parser->current;

        Parser_Advance (parser);

        return result;
      }
    default:
      {
        const char *b = Token_Kind_String (parser->current.kind);
        Parser_Diagnostic_Expect_Base (parser->location, "number", b);
        Halt ();
      }
    }
}


struct AST *
Parser_Parse_String (struct Parser *parser)
{
  Parser_Expect (parser, TOKEN_STRING);

  struct AST *result;

  result = AST_Create (parser->location, AST_STRING);

  result->token = parser->current;

  Parser_Advance (parser);

  return result;
}


struct AST *
Parser_Parse_Initializer (struct Parser *parser)
{
  struct Location location = parser->location;

  Parser_Expect_Advance (parser, TOKEN_LBRACKET);

  struct AST *result;

  result = AST_Create (location, AST_INITIALIZER);

  while (1)
    {
      struct AST *expression;

      expression = Parser_Parse_Statement (parser);

      AST_Append (result, expression);

      if (Parser_Match (parser, TOKEN_RBRACKET))
        break;

      if (!Parser_Match (parser, TOKEN_COMMA))
        Parser_Expect (parser, TOKEN_RBRACKET);
      else
        Parser_Advance (parser);
    }

  Parser_Expect_Advance (parser, TOKEN_RBRACKET);

  return result;
}


struct AST *
Parser_Parse (struct Parser *parser)
{
  Parser_Advance (parser);

  struct AST *result;

  result = Parser_Parse_Program (parser);

  Parser_Expect_Advance (parser, TOKEN_EOF);

  return result;
}

