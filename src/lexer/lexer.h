#ifndef _LEXER_H_
#define _LEXER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/*
* WARNING: if you change the order of this enumeration,
* grep -rn "ORDER RESERVED" ./src
*/
typedef enum {
    TK_LPAREN,
    TK_RPAREN,
    TK_LSQUIRLY,
    TK_RSQUIRLY,
    TK_COMMA,
    TK_SEMICOLON,
    TK_PIPE,
    TK_ASSIGN,
    TK_IDENT,
    TK_EOF,
    TK_ILLEGAL
} TokenType;

const char *tokenTypeToLiteral(TokenType type);

typedef struct SToken {
    TokenType type;
    char *literal;
    int line;
    int column;
    size_t size;
} Token;

typedef struct SLexer Lexer;
Lexer *lexerCreate(const char *input);

Token *lexerNext(Lexer *lexer);
const char *lexerGetInput(Lexer *lexer);
void lexerDestroy(Lexer **lexer);

Token *tokenCreate(TokenType type, char *literal, int line, int column, size_t size);
void tokenDestroy(Token **token);

#ifdef __cplusplus
}
#endif

#endif // _LEXER_H_
