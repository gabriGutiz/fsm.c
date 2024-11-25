#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "../lexer/lexer.h"
#include "../automaton/automaton.h"

struct SParser {
    const char *input;
    const Token **tokens;
    size_t position;
};

Token _getToken(Parser *parser);
Token _consume(Parser *parser, TokenType type);
int _consumeOptional(Parser *parser, TokenType type);
void _parseStates(Parser *parser, Automaton *automaton, int (*stateHelper)(Automaton *, char *));
void _parseAlphabet(Parser *parser, Automaton *automaton);
void _parseTransitions(Parser *parser, Automaton *automaton);
void _printInputLocation(int line, int col, size_t size, const char *input);
void _printInputLocationFromToken(Token token, const char *input);

/*****************************************************************************
*                              PUBLIC FUNCTIONS                              *
******************************************************************************/

Parser *parserCreate(Token **tokens, char *input) {
    size_t len = sizeof(Parser);

    Parser *parser = malloc(len);
    memset(parser, 0, len);

    parser->tokens = tokens;
    parser->input = input;
    parser->position = 0;

    return parser;
}

Automaton *parserParse(Parser *parser) {
    Automaton *automaton = automatonCreate();

    _parseStates(parser, automaton, automatonAddState);
    _consume(parser, TokenSemicolon);
    _parseAlphabet(parser, automaton);
    _consume(parser, TokenSemicolon);
    _parseTransitions(parser, automaton);
    automatonValidateTransitions(automaton);
    _consume(parser, TokenSemicolon);

    Token start = _consume(parser, TokenIdent);
    if (automatonAddStartState(automaton, start.literal) != 0) {
        _printInputLocationFromToken(start, parser->input);
        exit(EXIT_FAILURE);
    }

    _consume(parser, TokenSemicolon);
    _parseStates(parser, automaton, automatonAddAcceptState);

    return automaton;
}

void parserDestroy(Parser **parser) {
}

/*****************************************************************************
*                              PRIVATE FUNCTIONS                             *
******************************************************************************/

Token _getToken(Parser *parser) {
    Token tok = *parser->tokens[parser->position];
    return tok;
}

Token _consume(Parser *parser, TokenType type) {
    Token token = _getToken(parser);

    if (token.type != type) {
        _printInputLocationFromToken(token, parser->input);
        fprintf(stderr, "Expected token type '%s', got '%s'\n", tokenTypeToLiteral(type), tokenTypeToLiteral(token.type));
        exit(EXIT_FAILURE);
    }
    parser->position++;
    return token;
}

int _consumeOptional(Parser *parser, TokenType type) {
    Token token = _getToken(parser);

    if (token.type == type) {
        parser->position++;
        return 1;
    }
    return 0;
}

void _parseStates(Parser *parser, Automaton *automaton, int (*stateHandler)(Automaton *, char *)) {
    int consumed = _consumeOptional(parser, TokenLSquirly);

    do {
        Token tok = _consume(parser, TokenIdent);
        if (stateHandler(automaton, tok.literal) != 0) {
            _printInputLocationFromToken(tok, parser->input);
            exit(EXIT_FAILURE);
        }
    } while (_consumeOptional(parser, TokenComma));

    if (consumed == 1) {
        _consume(parser, TokenRSquirly);
    }
}

void _parseAlphabet(Parser *parser, Automaton *automaton) {
    int consumed = _consumeOptional(parser, TokenLSquirly);

    do {
        Token tok = _consume(parser, TokenChar);
        if (automatonAddToAlphabet(automaton, tok.literal[0]) != 0) {
            _printInputLocationFromToken(tok, parser->input);
            exit(EXIT_FAILURE);
        }
    } while (_consumeOptional(parser, TokenComma));

    if (consumed == 1) {
        _consume(parser, TokenRSquirly);
    }
}

void _parseTransitions(Parser *parser, Automaton *automaton) {
    int consumed = _consumeOptional(parser, TokenLSquirly);

    do {
        Token from = _consume(parser, TokenIdent);
        _consume(parser, TokenComma);
        Token c = _consume(parser, TokenChar);
        _consume(parser, TokenComma);
        Token to = _consume(parser, TokenIdent);

        int res = automatonAddTransition(automaton, from.literal, c.literal[0], to.literal);

        if (res == 2) {
            _printInputLocationFromToken(from, parser->input);
            exit(EXIT_FAILURE);
        } else if (res == 3) {
            _printInputLocationFromToken(c, parser->input);
            exit(EXIT_FAILURE);
        } else if (res == 4) {
            _printInputLocationFromToken(to, parser->input);
            exit(EXIT_FAILURE);
        } else if (res != 0) {
            _printInputLocation(from.line, from.column, from.size + c.size + to.size + 2, parser->input);
            exit(EXIT_FAILURE);
        }

    } while (_consumeOptional(parser, TokenPipe));

    if (consumed == 1) {
        _consume(parser, TokenRSquirly);
    }
}

void _printInputLocation(int line, int col, size_t size, const char *input) {
    fprintf(stderr, "Error at line %d:%d\n", line, col);

    int lineCounter = 0, lastNewLine = 0, startIndex = 0, index = 0;
    do {
        if (input[index] == '\n' || input[index] == '\0' || input[index] == '\r') {
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
    memset(lineStr, 0, i);
    strncpy(lineStr, input + startIndex, i - 1);
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
