#include "parser/parser.h"
#include <gtest/gtest.h>

TEST(TestParser, TestParser_One) {
    const char *input = "(){},;|";
    Lexer *lexer = NULL;
    TokenType tests[8];
    int testLength = 8;
    int i;

    tests[0] = TokenLParen;
    tests[1] = TokenRParen;
    tests[2] = TokenLSquirly;
    tests[3] = TokenRSquirly;
    tests[4] = TokenComma;
    tests[5] = TokenSemicolon;
    tests[6] = TokenPipe;
    tests[7] = TokenEof;

    lexer = lexerCreate(input);

    for (i = 0; i < testLength; i++) {
        Token *token = lexerNext(lexer);
        ASSERT_EQ(token->type, tests[i]);
        tokenDestroy(&token);
    }

    lexerDestroy(&lexer);
}

TEST(TestLexer, TestLexer_Complete) {
    const char *input = "s1,s2;0,1;s1,0,s1|s1,1,s2|s2,0,s1|s2,1,s2;s1;s2";

    Lexer *lexer = NULL;
    Token *tests[36];
    int testLength = 36;
    int i;

    tests[0] = tokenCreate(TokenIdent, strdup("s1"), 1, 1);
    tests[1] = tokenCreate(TokenComma, NULL);
    tests[2] = tokenCreate(TokenIdent, strdup("s2"));

    tests[3] = tokenCreate(TokenSemicolon, NULL);

    tests[4] = tokenCreate(TokenChar, strdup("0"));
    tests[5] = tokenCreate(TokenComma, NULL);
    tests[6] = tokenCreate(TokenChar, strdup("1"));

    tests[7] = tokenCreate(TokenSemicolon, NULL);

    tests[8] = tokenCreate(TokenIdent, strdup("s1"));
    tests[9] = tokenCreate(TokenComma, NULL);
    tests[10] = tokenCreate(TokenChar, strdup("0"));
    tests[11] = tokenCreate(TokenComma, NULL);
    tests[12] = tokenCreate(TokenIdent, strdup("s1"));

    tests[13] = tokenCreate(TokenPipe, NULL);

    tests[14] = tokenCreate(TokenIdent, strdup("s1"));
    tests[15] = tokenCreate(TokenComma, NULL);
    tests[16] = tokenCreate(TokenChar, strdup("1"));
    tests[17] = tokenCreate(TokenComma, NULL);
    tests[18] = tokenCreate(TokenIdent, strdup("s2"));

    tests[19] = tokenCreate(TokenPipe, NULL);

    tests[20] = tokenCreate(TokenIdent, strdup("s2"));
    tests[21] = tokenCreate(TokenComma, NULL);
    tests[22] = tokenCreate(TokenChar, strdup("0"));
    tests[23] = tokenCreate(TokenComma, NULL);
    tests[24] = tokenCreate(TokenIdent, strdup("s1"));

    tests[25] = tokenCreate(TokenPipe, NULL);

    tests[26] = tokenCreate(TokenIdent, strdup("s2"));
    tests[27] = tokenCreate(TokenComma, NULL);
    tests[28] = tokenCreate(TokenChar, strdup("1"));
    tests[29] = tokenCreate(TokenComma, NULL);
    tests[30] = tokenCreate(TokenIdent, strdup("s2"));

    tests[31] = tokenCreate(TokenSemicolon, NULL);

    tests[32] = tokenCreate(TokenIdent, strdup("s1"));

    tests[33] = tokenCreate(TokenSemicolon, NULL);

    tests[34] = tokenCreate(TokenIdent, strdup("s2"));

    tests[35] = tokenCreate(TokenEof, NULL);

    lexer = lexerCreate(input);

    for (i = 0; i < testLength; i++) {
        Token *token = lexerNext(lexer);
        EXPECT_STREQ(token->literal, tests[i]->literal);
        ASSERT_EQ(token->type, tests[i]->type);
        tokenDestroy(&token);

        tokenDestroy(&tests[i]);
    }

    lexerDestroy(&lexer);
}
