#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "../lexer/lexer.h"
#include "../automaton/automaton.h"

struct SParser {
    const Token **tokens;
    size_t position;
};

Token _getToken(Parser *parser);
char *_consume(Parser *parser, TokenType type);
int _consumeOptional(Parser *parser, TokenType type);
void _parseStates(Parser *parser, Automaton *automaton, void (*stateHelper)(Automaton *, char *));
void _parseAlphabet(Parser *parser, Automaton *automaton);
void _parseTransitions(Parser *parser, Automaton *automaton);

/*****************************************************************************
*                              PUBLIC FUNCTIONS                              *
******************************************************************************/

Parser *parserCreate(Token **tokens) {
    size_t len = sizeof(Parser);

    Parser *parser = malloc(len);
    memset(parser, 0, len);

    parser->tokens = tokens;
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
    automatonAddStartState(automaton, _consume(parser, TokenIdent));
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

char *_consume(Parser *parser, TokenType type) {
    Token token = _getToken(parser);

    if (token.type != type) {
        fprintf(stderr, "Expected token type %d, got %d\n", type, token.type);
        exit(EXIT_FAILURE);
    }
    parser->position++;
    return token.literal;
}

int _consumeOptional(Parser *parser, TokenType type) {
    Token token = _getToken(parser);

    if (token.type == type) {
        parser->position++;
        return 1;
    }
    return 0;
}

void _parseStates(Parser *parser, Automaton *automaton, void (*stateHandler)(Automaton *, char *)) {
    int consumed = _consumeOptional(parser, TokenLSquirly);

    do {
        char *state = _consume(parser, TokenIdent);
        stateHandler(automaton, state);
    } while (_consumeOptional(parser, TokenComma));

    if (consumed == 1) {
        _consume(parser, TokenRSquirly);
    }
}

void _parseAlphabet(Parser *parser, Automaton *automaton) {
    int consumed = _consumeOptional(parser, TokenLSquirly);

    do {
        char *c = _consume(parser, TokenChar);
        automatonAddToAlphabet(automaton, c[0]);
    } while (_consumeOptional(parser, TokenComma));

    if (consumed == 1) {
        _consume(parser, TokenRSquirly);
    }
}

void _parseTransitions(Parser *parser, Automaton *automaton) {
    int consumed = _consumeOptional(parser, TokenLSquirly);

    do {
        char *from = _consume(parser, TokenIdent);
        _consume(parser, TokenComma);
        char *c = _consume(parser, TokenChar);
        _consume(parser, TokenComma);
        char *to = _consume(parser, TokenIdent);
        automatonAddTransition(automaton, from, c[0], to);
    } while (_consumeOptional(parser, TokenPipe));

    if (consumed == 1) {
        _consume(parser, TokenRSquirly);
    }
}
