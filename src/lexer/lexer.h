#ifndef _LEXER_H_
#define _LEXER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef enum {
    TokenLParen,
    TokenRParen,
    TokenLSquirly,
    TokenRSquirly,
    TokenComma,
    TokenSemicolon,
    TokenPipe,
    TokenIdent,
    TokenAssign,
    TokenEof,
    TokenIllegal
} TokenType;

char *tokenTypeToString(TokenType type);
char *tokenTypeToLiteral(TokenType type);

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
void lexerDestroy(Lexer **lexer);

Token *tokenCreate(TokenType type, char *literal, int line, int column, size_t size);
void tokenDestroy(Token **token);

#ifdef __cplusplus
}
#endif

#endif // _LEXER_H_
