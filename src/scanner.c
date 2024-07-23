#include "scanner.h"

#include <string.h>

#include "common.h"

void initScanner(Scanner* s, const char* source) {
  s->start = source;
  s->current = source;
  s->line = 1;
}

static bool isAtEnd(Scanner* s) { return *s->current == '\0'; }

static Token makeToken(Scanner* s, TokenType type) {
  Token token;
  token.type = type;
  token.start = s->start;
  token.len = (int)(s->current - s->start);
  token.line = s->line;

  return token;
}

static Token errorToken(Scanner* s, const char* msg) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = msg;
  token.len = (int)strlen(msg);
  token.line = s->line;

  return token;
}

static char nextChar(Scanner* s) {
  s->current++;
  return s->current[-1];
}

static char peek(Scanner* s) { return *s->current; }
static char peekNext(Scanner* s) {
  if (isAtEnd(s)) return '\0';
  return s->current[1];
}

static bool match(Scanner* s, char expected) {
  if (isAtEnd(s)) return false;
  if (*s->current != expected) return false;
  s->current++;
  return true;
}

static void skipWhitespace(Scanner* s) {
  for (;;) {
    char c = peek(s);
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        nextChar(s);
        break;
      case '\n':
        s->line++;
        nextChar(s);
        break;
      case '/':
        if (peekNext(s) == '/') {
          while (peek(s) != '\n' && !isAtEnd(s)) nextChar(s);
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

static bool isDigit(char c) { return c >= '0' && c <= '9'; }
static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static TokenType checkKeyword(Scanner* s, int start, int length,
                              const char* rest, TokenType type) {
  if (s->current - s->start == start + length &&
      memcmp(s->start + start, rest, length) == 0) {
    return type;
  }
  return TOKEN_IDENTIFIER;
}

static TokenType identifierType(Scanner* s) {
  switch (s->start[0]) {
    case 'e':
      return checkKeyword(s, 1, 3, "lse", TOKEN_ELSE);
    case 'f':
      if (s->current - s->start > 1) {
        switch (s->start[1]) {
          case 'a':
            return checkKeyword(s, 2, 3, "lse", TOKEN_FALSE);
          case 'o':
            return checkKeyword(s, 2, 1, "r", TOKEN_FOR);
          case 'n':
            return TOKEN_FN;
        }
      }
      break;
    case 'i':
      return checkKeyword(s, 1, 1, "f", TOKEN_IF);
    case 'n':
      return checkKeyword(s, 1, 2, "il", TOKEN_NIL);
    case 'r':
      return checkKeyword(s, 1, 5, "eturn", TOKEN_RETURN);
    case 't':
      if (s->current - s->start > 1) {
        return checkKeyword(s, 2, 2, "ue", TOKEN_TRUE);
      }
      break;
    case 'v':
      return checkKeyword(s, 1, 2, "ar", TOKEN_VAR);
    case 'w':
      return checkKeyword(s, 1, 4, "hile", TOKEN_WHILE);
  }
  return TOKEN_IDENTIFIER;
}

static Token number(Scanner* s) {
  while (isDigit(peek(s))) nextChar(s);
  // TODO: floats
  return makeToken(s, TOKEN_NUMBER);
}

static Token identifier(Scanner* s) {
  while (isAlpha(peek(s)) || isDigit(peek(s))) nextChar(s);
  return makeToken(s, identifierType(s));
}

static Token string(Scanner* s) {
  while (peek(s) != '"' && !isAtEnd(s)) {
    if (peek(s) == '\n') s->line++;
    nextChar(s);
  }
  if (isAtEnd(s)) return errorToken(s, "Unterminated string");
  // The closing quote.
  nextChar(s);
  return makeToken(s, TOKEN_STRING);
}

Token scanToken(Scanner* s) {
  skipWhitespace(s);

  s->start = s->current;

  if (isAtEnd(s)) {
    return makeToken(s, TOKEN_EOF);
  }

  char c = nextChar(s);

  if (isAlpha(c)) return identifier(s);
  if (isDigit(c)) return number(s);

  switch (c) {
    case '(':
      return makeToken(s, TOKEN_LEFT_PAREN);
    case ')':
      return makeToken(s, TOKEN_RIGHT_PAREN);
    case '{':
      return makeToken(s, TOKEN_LEFT_BRACE);
    case '}':
      return makeToken(s, TOKEN_RIGHT_BRACE);
    case ';':
      return makeToken(s, TOKEN_SEMICOLON);
    case ',':
      return makeToken(s, TOKEN_COMMA);
    case '.':
      return makeToken(s, TOKEN_DOT);
    case '-':
      return makeToken(s, TOKEN_MINUS);
    case '+':
      return makeToken(s, TOKEN_PLUS);
    case '/':
      return makeToken(s, TOKEN_SLASH);
    case '*':
      return makeToken(s, TOKEN_STAR);
    case '!':
      return makeToken(s, match(s, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(s, match(s, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(s, match(s, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(s, match(s, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '&':
      if (match(s, '&')) {
        return makeToken(s, TOKEN_DOUBLE_AMPERSAND);
      }
    case '|':
      if (match(s, '|')) {
        return makeToken(s, TOKEN_DOUBLE_PIPE);
      }
    case '"':
      return string(s);
  }

  return errorToken(s, "Unexpected char");
}
