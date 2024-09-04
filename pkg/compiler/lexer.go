package compiler

type TokenType string

const (
    Comma     TokenType = ","
	Semicolon TokenType = ";"
	LParen    TokenType = "("
	RParen    TokenType = ")"
	LSquirly  TokenType = "{"
	RSquirly  TokenType = "}"
	Pipe      TokenType = "|"
    Eof       TokenType = "EOF"
    Illegal   TokenType = "ILLEGAL"
    Ident     TokenType = "IDENT"
)

type Token struct {
    type_   TokenType
    literal string
}

func CreateToken(type_ TokenType, literal string) Token {
	return Token{type_: type_, literal: literal}
}

const _0 = int('0')
const _9 = int('9')

const a = int('a')
const z = int('z')

const A = int('A')
const Z = int('Z')

const __ = int('_')

func isLetter(character rune) bool {
	var char = int(character)
	return a <= char && z >= char ||
		A <= char && Z >= char ||
		char == __
}

func isNumber(character rune) bool {
	var char = int(character)
	return _0 <= char && _9 >= char
}

type Tokenizer struct {
	position     int
	readPosition int
	ch           rune
	input        string
	tokens       []Token
}

func NewTokenizer(input string) Tokenizer {
	var lexer = Tokenizer{
		position:     0,
		readPosition: 0,
		input:        input,
	}
	lexer.readChar()
	return lexer
}

func (tokenizer *Tokenizer) GetNextToken() Token {
	tokenizer.skipWhitespace()

	var tok Token
	var tokNil bool = true
	switch tokenizer.ch {
	case '{':
		tok = CreateToken(LSquirly, string(tokenizer.ch))
		tokNil = false
	case '}':
		tok = CreateToken(RSquirly, string(tokenizer.ch))
		tokNil = false
	case '(':
		tok = CreateToken(LParen, string(tokenizer.ch))
		tokNil = false
	case ')':
		tok = CreateToken(RParen, string(tokenizer.ch))
		tokNil = false
	case ',':
		tok = CreateToken(Comma, string(tokenizer.ch))
		tokNil = false
	case ';':
		tok = CreateToken(Semicolon, string(tokenizer.ch))
		tokNil = false
	case '|':
		tok = CreateToken(Pipe, string(tokenizer.ch))
		tokNil = false
	case '\x00':
		tok = CreateToken(Eof, "eof")
		tokNil = false
	}

	if isLetter(tokenizer.ch) || isNumber(tokenizer.ch) {
		var ident = tokenizer.readIdent()
        return CreateToken(Ident, ident)
	} else if tokNil {
		return CreateToken(Illegal, string(tokenizer.ch))
	}

	tokenizer.readChar()
	return tok
}

func (tokenizer *Tokenizer) Tokenize() (tokens []Token) {
    var token Token

    for token.type_ != Eof {
        token = tokenizer.GetNextToken()
        tokens = append(tokens, token)
    }

    return
}

func (tokenizer *Tokenizer) skipWhitespace() {
	for tokenizer.ch == ' ' || tokenizer.ch == '\t' || tokenizer.ch == '\n' || tokenizer.ch == '\r' {
		tokenizer.readChar()
	}
}

func (tokenizer *Tokenizer) readChar() {
	if tokenizer.readPosition >= len(tokenizer.input) {
		tokenizer.ch = '\x00'
	} else {
		tokenizer.ch = rune(tokenizer.input[tokenizer.readPosition])
	}

	tokenizer.position = tokenizer.readPosition
	tokenizer.readPosition++
}

func (tokenizer *Tokenizer) readIdent() string {
	var position = tokenizer.position

	for isLetter(tokenizer.ch) || isNumber(tokenizer.ch) {
		tokenizer.readChar()
	}

	return tokenizer.input[position:tokenizer.position]
}

