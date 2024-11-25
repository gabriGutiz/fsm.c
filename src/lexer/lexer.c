#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

struct SLexer {
    const char *input;
    size_t inputLength;
    size_t position;
    size_t readPosition;
    int line;
    int column;
    char ch;
};

static void _lexerReadChar(Lexer *lexer);
static void _lexerSkipWhitespace(Lexer *lexer);
static const char *_lexerReadIdent(Lexer *lexer, size_t *len);

static int _isLetter(char ch);
static int _isNumber(char ch);

/*****************************************************************************
*                              PUBLIC FUNCTIONS                              *
******************************************************************************/

Lexer *lexerCreate(const char *input) {
    size_t len = sizeof(Lexer);

    Lexer *lexer = malloc(len);
    memset(lexer, 0, len);

    lexer->input = input;
    lexer->inputLength = strlen(input);
    lexer->position = 0;
    lexer->readPosition = 0;
    lexer->line = 1;
    lexer->column = 0;

    _lexerReadChar(lexer);

    return lexer;
}

void lexerDestroy(Lexer **lexer) {
    if (*lexer) {
        free(*lexer);
    }

    *lexer = NULL;
}

Token *lexerNext(Lexer *lexer) {
    Token *tok = NULL;

    _lexerSkipWhitespace(lexer);

    switch (lexer->ch) {
        case '(':
            tok = tokenCreate(TokenLParen, NULL, lexer->line, lexer->column, 1);
            break;
        case ')':
            tok = tokenCreate(TokenRParen, NULL, lexer->line, lexer->column, 1);
            break;
        case '{':
            tok = tokenCreate(TokenLSquirly, NULL, lexer->line, lexer->column, 1);
            break;
        case '}':
            tok = tokenCreate(TokenRSquirly, NULL, lexer->line, lexer->column, 1);
            break;
        case ',':
            tok = tokenCreate(TokenComma, NULL, lexer->line, lexer->column, 1);
            break;
        case ';':
            tok = tokenCreate(TokenSemicolon, NULL, lexer->line, lexer->column, 1);
            break;
        case '|':
            tok = tokenCreate(TokenPipe, NULL, lexer->line, lexer->column, 1);
            break;
        case '=':
            tok = tokenCreate(TokenAssign, NULL, lexer->line, lexer->column, 1);
            break;
        case '\0':
            tok = tokenCreate(TokenEof, NULL, lexer->line, lexer->column, 1);
            break;
    }

    if (_isLetter(lexer->ch) || _isNumber(lexer->ch)) {
        size_t len = 0;
        char *literal = NULL;
        const char *ident = _lexerReadIdent(lexer, &len);

        literal = strndup(ident, len);

        tok = tokenCreate(TokenIdent, literal, lexer->line, lexer->column - len, len);
        return tok;
    }

    if (!tok) {
        tok = tokenCreate(TokenIllegal, NULL, lexer->line, lexer->column, 1);
    }

    _lexerReadChar(lexer);

    return tok;
}

char *tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenIllegal:
            return strdup("ILLEGAL");
        case TokenEof:
            return strdup("EOF");
        case TokenIdent:
            return strdup("IDENT");
        case TokenLParen:
            return strdup("LPAREN");
        case TokenRParen:
            return strdup("RPAREN");
        case TokenLSquirly:
            return strdup("LSQUIRLY");
        case TokenRSquirly:
            return strdup("RSQUIRLY");
        case TokenComma:
            return strdup("COMMA");
        case TokenSemicolon:
            return strdup("SEMICOLON");
        case TokenPipe:
            return strdup("PIPE");
        case TokenAssign:
            return strdup("ASSIGN");
        default:
            return NULL;
    }
}

char *tokenTypeToLiteral(TokenType type) {
    switch (type) {
        case TokenIllegal:
            return tokenTypeToString(type);
        case TokenEof:
            return tokenTypeToString(type);
        case TokenIdent:
            return tokenTypeToString(type);
        case TokenLParen:
            return strdup("(");
        case TokenRParen:
            return strdup(")");
        case TokenLSquirly:
            return strdup("{");
        case TokenRSquirly:
            return strdup("}");
        case TokenComma:
            return strdup(",");
        case TokenSemicolon:
            return strdup(";");
        case TokenPipe:
            return strdup("|");
        case TokenAssign:
            return strdup("=");
        default:
            return NULL;
    }
}

Token *tokenCreate(TokenType type, char *literal, int line, int column, size_t size) {
    size_t len = sizeof(Token);

    Token *token = malloc(len);
    memset(token, 0, len);

    token->literal = literal;
    token->type = type;
    token->line = line;
    token->column = column;
    token->size = size;

    return token;
}

void tokenDestroy(Token **token) {
    if (*token && (*token)->literal) {
        free((*token)->literal);
    }

    if (*token) {
        free(*token);
    }

    *token = NULL;
}

/*****************************************************************************
*                              PRIVATE FUNCTIONS                             *
******************************************************************************/

static void _lexerReadChar(Lexer *lexer) {
    if (lexer->readPosition >= lexer->inputLength) {
        lexer->ch = '\0';
    } else {
        lexer->ch = lexer->input[lexer->readPosition];
    }

    if (lexer->ch == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }

    lexer->position = lexer->readPosition;
    lexer->readPosition++;
}

static void _lexerSkipWhitespace(Lexer *lexer) {
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' || lexer->ch == '\r') {
        _lexerReadChar(lexer);
    }
}

static int _isLetter(char ch) {
  return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z';
}

static int _isNumber(char ch) {
    return '0' <= ch && '9' >= ch;
}

static const char *_lexerReadIdent(Lexer *lexer, size_t *len) {
    char *result = NULL;
    size_t position = lexer->position;

    while (_isLetter(lexer->ch) || _isNumber(lexer->ch)) {
        _lexerReadChar(lexer);
    }

    if (len) {
        *len = lexer->position - position;
    }

    return lexer->input + position;
}

