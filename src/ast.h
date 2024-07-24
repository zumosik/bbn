#ifndef AST_H
#define AST_H

#include "scanner.h"

// Ast will have 3 types of nodes:
// 1. Expressions (Exp)
// 2. Statements (Stmt)

typedef struct Exp Exp;
typedef struct Stmt Stmt;

// ----------------------------------------------------------------
// EXPRESSIONS
// ----------------------------------------------------------------

typedef enum {
  EXP_BINARY,
  // TODO
} ExpType;

typedef struct BinaryExp {
  TokenType op;
  Exp *left, *right;
} BinaryExp;

struct Exp {  // basic expression
  ExpType type;
  union {
    BinaryExp binary;
  } as;
};

// ----------------------------------------------------------------
// STATEMENTS
// ----------------------------------------------------------------

typedef enum {
  STMT_IF,
  STMT_EXP,
  // TODO
} StmtType;

typedef struct IfStmt {
  Exp* condition;
  Stmt *thenStmt, *elseStmt;
} IfStmt;

struct Stmt {
  StmtType type;
  union {
    IfStmt ifStmt;
    Exp* expr;
  } as;
};

#endif  // !AST_H
