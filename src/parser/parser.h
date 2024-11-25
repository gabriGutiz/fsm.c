#ifndef _PARSER_H_
#define _PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../lexer/lexer.h"
#include "../automaton/automaton.h"

typedef struct SParser Parser;
Parser *parserCreate(Token **tokens, char *input);
Automaton *parserParse(Parser *parser);
void parserDestroy(Parser **parser);

#ifdef __cplusplus
}
#endif

#endif // _PARSER_H_
