#include "../src/scanner.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>

typedef struct Expected {
  TokenType type;
} Expected;

Expected newExpected(TokenType type) {
  Expected e;
  e.type = type;
  return e;
}

Test(nextToken, basic) {
  Scanner scanner;
  initScanner(&scanner, "( ) { } , . - + ; / * ! != = == > >= < <= && ||"
                        "else false for fn if nil return true var while");

  Expected tests[] = {newExpected(TOKEN_LEFT_PAREN),
                      newExpected(TOKEN_RIGHT_PAREN),
                      newExpected(TOKEN_LEFT_BRACE),
                      newExpected(TOKEN_RIGHT_BRACE),
                      newExpected(TOKEN_COMMA),
                      newExpected(TOKEN_DOT),
                      newExpected(TOKEN_MINUS),
                      newExpected(TOKEN_PLUS),
                      newExpected(TOKEN_SEMICOLON),
                      newExpected(TOKEN_SLASH),
                      newExpected(TOKEN_STAR),
                      newExpected(TOKEN_BANG),
                      newExpected(TOKEN_BANG_EQUAL),
                      newExpected(TOKEN_EQUAL),
                      newExpected(TOKEN_EQUAL_EQUAL),
                      newExpected(TOKEN_GREATER),
                      newExpected(TOKEN_GREATER_EQUAL),
                      newExpected(TOKEN_LESS),
                      newExpected(TOKEN_LESS_EQUAL),
                      newExpected(TOKEN_DOUBLE_AMPERSAND),
                      newExpected(TOKEN_DOUBLE_PIPE),
                      newExpected(TOKEN_ELSE),
                      newExpected(TOKEN_FALSE),
                      newExpected(TOKEN_FOR),
                      newExpected(TOKEN_FN),
                      newExpected(TOKEN_IF),
                      newExpected(TOKEN_NIL),
                      newExpected(TOKEN_RETURN),
                      newExpected(TOKEN_TRUE),
                      newExpected(TOKEN_VAR),
                      newExpected(TOKEN_WHILE)};

  size_t testCount = sizeof(tests) / sizeof(tests[0]);

  for (size_t i = 0; i < testCount; i++) {
    Token t = scanToken(&scanner);
    cr_assert_eq(tests[i].type, t.type,
                 "(%zu) Not equal, expected type: %d, actual type: %d", i,
                 tests[i].type, t.type);
  }
}
