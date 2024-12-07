#ifndef _PARSER_H_
#define _PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../lexer/lexer.h"
#include "../fsm/fsm.h"

typedef struct SParser Parser;
Parser *parserCreate(Lexer *lexer);
Fsm *parserParse(Parser *parser);
void parserDestroy(Parser **parser);

#ifdef __cplusplus
}
#endif

#endif // _PARSER_H_
