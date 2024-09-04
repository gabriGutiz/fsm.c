package compiler

import (
    "testing"
)

func TestGetNextToken(t *testing.T) {

    t.Run("simple input", func(t *testing.T) {
        const input = "({s1};|)"

        var tokens []TokenType

        tokens = append(tokens, LParen, LSquirly, Ident, RSquirly,
            Semicolon, Pipe, RParen)

        var tokenizer = NewTokenizer(input)

        for _, token := range tokens {
            var nextToken = tokenizer.GetNextToken()
            if nextToken.type_ != token {
                t.Errorf("Token %s expected %s", nextToken.type_, token)
            }
        }
	})

    t.Run("complete input", func(t *testing.T) {
        const input = "({s1},{0,1},{(s1,0,s1),(s1,1,s1)},s1,{s1})"

        var tokenizer = NewTokenizer(input)

        var tokens []Token

        tokens = append(tokens,
            CreateToken(LParen, "("),

            CreateToken(LSquirly, "{"),
            CreateToken(Ident, "s1"),
            CreateToken(RSquirly, "}"),

            CreateToken(Comma, ","),

            CreateToken(LSquirly, "{"),
            CreateToken(Ident, "0"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "1"),
            CreateToken(RSquirly, "}"),

            CreateToken(Comma, ","),

            CreateToken(LSquirly, "{"),
            CreateToken(LParen, "("),
            CreateToken(Ident, "s1"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "0"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "s1"),
            CreateToken(RParen, ")"),
            CreateToken(Comma, ","),
            CreateToken(LParen, "("),
            CreateToken(Ident, "s1"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "1"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "s1"),
            CreateToken(RParen, ")"),
            CreateToken(RSquirly, "}"),

            CreateToken(Comma, ","),

            CreateToken(Ident, "s1"),

            CreateToken(Comma, ","),

            CreateToken(LSquirly, "{"),
            CreateToken(Ident, "s1"),
            CreateToken(RSquirly, "}"),

            CreateToken(RParen, ")"),

            CreateToken(Eof, "eof"))

        for _, token := range tokens {
            var nextToken = tokenizer.GetNextToken()
            if nextToken != token {
                t.Errorf("Token %s expected %s", nextToken, token)
            }
        }
    })
}

func TestTokenize(t *testing.T) {

    t.Run("complete input", func(t *testing.T) {
        const input = "({s1},{0,1},{(s1,0,s1),(s1,1,s1)},s1,{s1})"

        var tokenizer = NewTokenizer(input)

        var tokens []Token

        tokens = append(tokens,
            CreateToken(LParen, "("),

            CreateToken(LSquirly, "{"),
            CreateToken(Ident, "s1"),
            CreateToken(RSquirly, "}"),

            CreateToken(Comma, ","),

            CreateToken(LSquirly, "{"),
            CreateToken(Ident, "0"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "1"),
            CreateToken(RSquirly, "}"),

            CreateToken(Comma, ","),

            CreateToken(LSquirly, "{"),
            CreateToken(LParen, "("),
            CreateToken(Ident, "s1"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "0"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "s1"),
            CreateToken(RParen, ")"),
            CreateToken(Comma, ","),
            CreateToken(LParen, "("),
            CreateToken(Ident, "s1"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "1"),
            CreateToken(Comma, ","),
            CreateToken(Ident, "s1"),
            CreateToken(RParen, ")"),
            CreateToken(RSquirly, "}"),

            CreateToken(Comma, ","),

            CreateToken(Ident, "s1"),

            CreateToken(Comma, ","),

            CreateToken(LSquirly, "{"),
            CreateToken(Ident, "s1"),
            CreateToken(RSquirly, "}"),

            CreateToken(RParen, ")"),

            CreateToken(Eof, "eof"))

        var testTokens []Token
        testTokens = tokenizer.Tokenize()

        if testTokens == nil {
            t.Errorf("Tokenize failed")
        } else if len(testTokens) != len(tokens) {
            t.Errorf("Returned tokens length %d expected %d", len(testTokens), len(tokens))
        }
        
        for i, token := range tokens {
            if testTokens[i] != token {
                t.Errorf("Token %s expected %s", testTokens[i], token)
            }
        }
    })
}

