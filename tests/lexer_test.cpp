#include <gtest/gtest.h>

#include "lexer/lexer.h"

TEST(TestLexer, TestLexer_One) {
    const char *input = "(){},;|";
    Lexer *lexer = NULL;
    TokenType tests[8];
    int testLength = 8;
    int i;

    tests[0] = TK_LPAREN;
    tests[1] = TK_RPAREN;
    tests[2] = TK_LSQUIRLY;
    tests[3] = TK_RSQUIRLY;
    tests[4] = TK_COMMA;
    tests[5] = TK_SEMICOLON;
    tests[6] = TK_PIPE;
    tests[7] = TK_EOF;

    lexer = lexerCreate(input);

    for (i = 0; i < testLength; i++) {
        Token *token = lexerNext(lexer);
        ASSERT_EQ(token->type, tests[i]);
        tokenDestroy(&token);
    }

    lexerDestroy(&lexer);
}

TEST(TestLexer, TestLexer_Complete) {
    const char *input = "s1,s2;0,1;s1,0,s1|s1,1,s2|s2,0,s1|s2,1,s2;s1;{s2}";

    Lexer *lexer = NULL;
    Token *tests[38];
    int testLength = 38;
    int i;

    tests[0] = tokenCreate(TK_IDENT, strdup("s1"), 1, 1, 2);
    tests[1] = tokenCreate(TK_COMMA, NULL, 1, 3, 1);
    tests[2] = tokenCreate(TK_IDENT, strdup("s2"), 1, 4, 2);

    tests[3] = tokenCreate(TK_SEMICOLON, NULL, 1, 6, 1);

    tests[4] = tokenCreate(TK_IDENT, strdup("0"), 1, 7, 1);
    tests[5] = tokenCreate(TK_COMMA, NULL, 1, 8, 1);
    tests[6] = tokenCreate(TK_IDENT, strdup("1"), 1, 9, 1);

    tests[7] = tokenCreate(TK_SEMICOLON, NULL, 1, 10, 1);

    tests[8] = tokenCreate(TK_IDENT, strdup("s1"), 1, 11, 2);
    tests[9] = tokenCreate(TK_COMMA, NULL, 1, 13, 1);
    tests[10] = tokenCreate(TK_IDENT, strdup("0"), 1, 14, 1);
    tests[11] = tokenCreate(TK_COMMA, NULL, 1, 15, 1);
    tests[12] = tokenCreate(TK_IDENT, strdup("s1"), 1, 16, 2);

    tests[13] = tokenCreate(TK_PIPE, NULL, 1, 18, 1);

    tests[14] = tokenCreate(TK_IDENT, strdup("s1"), 1, 19, 2);
    tests[15] = tokenCreate(TK_COMMA, NULL, 1, 21, 1);
    tests[16] = tokenCreate(TK_IDENT, strdup("1"), 1, 22, 1);
    tests[17] = tokenCreate(TK_COMMA, NULL, 1, 23, 1);
    tests[18] = tokenCreate(TK_IDENT, strdup("s2"), 1, 24, 2);

    tests[19] = tokenCreate(TK_PIPE, NULL, 1, 26, 1);

    tests[20] = tokenCreate(TK_IDENT, strdup("s2"), 1, 27, 2);
    tests[21] = tokenCreate(TK_COMMA, NULL, 1, 29, 1);
    tests[22] = tokenCreate(TK_IDENT, strdup("0"), 1, 30, 1);
    tests[23] = tokenCreate(TK_COMMA, NULL, 1, 31, 1);
    tests[24] = tokenCreate(TK_IDENT, strdup("s1"), 1, 32, 2);

    tests[25] = tokenCreate(TK_PIPE, NULL, 1, 34, 1);

    tests[26] = tokenCreate(TK_IDENT, strdup("s2"), 1, 35, 2);
    tests[27] = tokenCreate(TK_COMMA, NULL, 1, 37, 1);
    tests[28] = tokenCreate(TK_IDENT, strdup("1"), 1, 38, 1);
    tests[29] = tokenCreate(TK_COMMA, NULL, 1, 39, 1);
    tests[30] = tokenCreate(TK_IDENT, strdup("s2"), 1, 40, 2);

    tests[31] = tokenCreate(TK_SEMICOLON, NULL, 1, 42, 1);

    tests[32] = tokenCreate(TK_IDENT, strdup("s1"), 1, 43, 2);

    tests[33] = tokenCreate(TK_SEMICOLON, NULL, 1, 45, 1);

    tests[34] = tokenCreate(TK_LSQUIRLY, NULL, 1, 46, 1);
    tests[35] = tokenCreate(TK_IDENT, strdup("s2"), 1, 47, 2);
    tests[36] = tokenCreate(TK_RSQUIRLY, NULL, 1, 49, 1);

    tests[37] = tokenCreate(TK_EOF, NULL, 1, 50, 1);

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
