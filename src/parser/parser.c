#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "../lexer/lexer.h"
#include "../automaton/automaton.h"

struct SParser {
    Lexer *lexer;
    Token *currentToken;
};

Token _consume(Parser *parser, TokenType type);
int _consumeOptional(Parser *parser, TokenType type);
void _parseStates(Parser *parser, Automaton *automaton, int (*stateHelper)(Automaton *, char *));
void _parseAlphabet(Parser *parser, Automaton *automaton);
void _parseTransitions(Parser *parser, Automaton *automaton);
void _validateTokenSizeIsOne(Token token, const char *input);
void _printInputLocation(int line, int col, size_t size, const char *input);
void _printInputLocationFromToken(Token token, const char *input);

/*****************************************************************************
*                              PUBLIC FUNCTIONS                              *
******************************************************************************/

Parser *parserCreate(Lexer *lexer) {
    size_t len = sizeof(Parser);
    Parser *parser = malloc(len);
    memset(parser, 0, len);
    parser->lexer = lexer;
    parser->currentToken = NULL;
    return parser;
}

Automaton *parserParse(Parser *parser) {
    Token name = _consume(parser, TK_IDENT);
    Automaton *automaton = automatonCreate(name.literal);

    _consume(parser, TK_ASSIGN);

    int consumed = _consumeOptional(parser, TK_LPAREN);

    _parseStates(parser, automaton, automatonAddState);
    _consume(parser, TK_SEMICOLON);
    _parseAlphabet(parser, automaton);
    _consume(parser, TK_SEMICOLON);
    _parseTransitions(parser, automaton);
    automatonValidateTransitions(automaton);
    _consume(parser, TK_SEMICOLON);

    Token start = _consume(parser, TK_IDENT);
    if (automatonAddStartState(automaton, start.literal) != 0) {
        _printInputLocationFromToken(start, lexerGetInput(parser->lexer));
        exit(EXIT_FAILURE);
    }

    _consume(parser, TK_SEMICOLON);
    _parseStates(parser, automaton, automatonAddAcceptState);

    if (consumed == 1) {
        _consume(parser, TK_RPAREN);
    }

    return automaton;
}

void parserDestroy(Parser **parser) {
    if (*parser && (*parser)->currentToken) {
        tokenDestroy(&(*parser)->currentToken);
    }

    if (*parser) {
        free(*parser);
    }

    *parser = NULL;
}

/*****************************************************************************
*                              PRIVATE FUNCTIONS                             *
******************************************************************************/

Token _getToken(Parser *parser) {
    if (parser->currentToken == NULL) {
        return *lexerNext(parser->lexer);
    } else {
        Token token = *parser->currentToken;
        free(parser->currentToken);
        parser->currentToken = NULL;
        return token;
    }
}

Token _consume(Parser *parser, TokenType type) {
    Token token = _getToken(parser);
    if (token.type != type) {
        fprintf(stderr, "Expected token type '%s', got '%s'\n", tokenTypeToLiteral(type), tokenTypeToLiteral(token.type));
        _printInputLocationFromToken(token, lexerGetInput(parser->lexer));
        exit(EXIT_FAILURE);
    }
    return token;
}

int _consumeOptional(Parser *parser, TokenType type) {
    Token token = _getToken(parser);
    if (token.type == type) {
        return 1;
    }
    
    Token *tok;
    if (!(tok = (Token *)malloc(sizeof(Token)))) {
        fprintf(stderr, "Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    (*tok) = token;
    parser->currentToken = tok;

    return 0;
}

void _parseStates(Parser *parser, Automaton *automaton, int (*stateHandler)(Automaton *, char *)) {
    int consumed = _consumeOptional(parser, TK_LSQUIRLY);

    do {
        Token tok = _consume(parser, TK_IDENT);
        if (stateHandler(automaton, tok.literal) != 0) {
            _printInputLocationFromToken(tok, lexerGetInput(parser->lexer));
            exit(EXIT_FAILURE);
        }
    } while (_consumeOptional(parser, TK_COMMA));

    if (consumed == 1) {
        _consume(parser, TK_RSQUIRLY);
    }
}

void _parseAlphabet(Parser *parser, Automaton *automaton) {
    int consumed = _consumeOptional(parser, TK_LSQUIRLY);

    do {
        Token tok = _consume(parser, TK_IDENT);
        _validateTokenSizeIsOne(tok, lexerGetInput(parser->lexer));

        if (automatonAddToAlphabet(automaton, tok.literal[0]) != 0) {
            _printInputLocationFromToken(tok, lexerGetInput(parser->lexer));
            exit(EXIT_FAILURE);
        }
    } while (_consumeOptional(parser, TK_COMMA));

    if (consumed == 1) {
        _consume(parser, TK_RSQUIRLY);
    }
}

void _parseTransitions(Parser *parser, Automaton *automaton) {
    int consumed = _consumeOptional(parser, TK_LSQUIRLY);

    do {
        Token from = _consume(parser, TK_IDENT);
        _consume(parser, TK_COMMA);
        Token c = _consume(parser, TK_IDENT);
        _consume(parser, TK_COMMA);
        Token to = _consume(parser, TK_IDENT);

        int res = automatonAddTransition(automaton, from.literal, c.literal[0], to.literal);

        if (res == 2) {
            _printInputLocationFromToken(from, lexerGetInput(parser->lexer));
            exit(EXIT_FAILURE);
        } else if (res == 3) {
            _printInputLocationFromToken(c, lexerGetInput(parser->lexer));
            exit(EXIT_FAILURE);
        } else if (res == 4) {
            _printInputLocationFromToken(to, lexerGetInput(parser->lexer));
            exit(EXIT_FAILURE);
        } else if (res != 0) {
            _printInputLocation(from.line, from.column, from.size + c.size + to.size + 2, lexerGetInput(parser->lexer));
            exit(EXIT_FAILURE);
        }

    } while (_consumeOptional(parser, TK_PIPE));

    if (consumed == 1) {
        _consume(parser, TK_RSQUIRLY);
    }
}

void _validateTokenSizeIsOne(Token token, const char *input) {
    if (token.size > 1) {
        fprintf(stderr, "Expected alphabet symbol to be a single character\n");
        _printInputLocationFromToken(token, input);
        exit(EXIT_FAILURE);
    }
}

void _printInputLocation(int line, int col, size_t size, const char *input) {
    fprintf(stderr, "\nError at line %d:%d\n", line, col);

    int lineCounter = 0, lastNewLine = 0, startIndex = 0, index = 0;
    do {
        if (input[index] == '\n' || input[index] == '\0' || input[index] == '\r') {
            if (input[lastNewLine] == '\n') {
                lastNewLine++;
            }
            startIndex = lastNewLine;
            lastNewLine = index;
            lineCounter++;
        }
        index++;
    } while (lineCounter < line && input[index] != '\0');

    size_t i = lastNewLine - startIndex;
    char *lineStr;
    if (!(lineStr = (char *)malloc(i))) {
        fprintf(stderr, "Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    memset(lineStr, 0, i + 1);
    strncpy(lineStr, input + startIndex, i);
    fprintf(stderr, "%s\n", lineStr);

    for (int i = 0; i < col - 1; i++) {
        fprintf(stderr, " ");
    }
    for (int i = 0; i < size; i++) {
        fprintf(stderr, "^");
    }

    fprintf(stderr, "\n");

    free(lineStr);
}

void _printInputLocationFromToken(Token token, const char *input) {
    _printInputLocation(token.line, token.column, token.size, input);
}
