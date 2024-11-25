#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "automaton/automaton.h"

char *readFile(const char *filename);
Token **tokenize(char *input, size_t *size);

int main(int argc, char *argv[]) {
    char *fileContent;
    size_t totalTokens = 0;
    Token **tokens;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <filename> <test_string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!(fileContent = readFile(argv[1]))) {
        fprintf(stderr, "Error reading file or file is empty");
        return EXIT_FAILURE;
    }
    printf("File content:\n%s\n", fileContent);

    if (!(tokens = tokenize(fileContent, &totalTokens))) {
        fprintf(stderr, "Error tokenizing file content");
        return EXIT_FAILURE;
    }
    printf("Total tokens: %ld\n", totalTokens);

    Parser *parser = parserCreate(tokens);
    Automaton *aut = parserParse(parser);

    if (automatonCheck(aut, argv[2]) == 1) {
        printf("String '%s' is accepted by the automaton\n", argv[2]);
    } else {
        printf("String '%s' is not accepted by the automaton\n", argv[2]);
    }

    parserDestroy(&parser);
    automatonDestroy(&aut);

    return EXIT_SUCCESS;
}

char *readFile(const char *filename) {
    FILE *file;
    char *buffer;
    long size;

    if (!(file = fopen(filename, "r"))) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

Token **tokenize(char *input, size_t *size) {
    Lexer *lexer = NULL;
    Token **tokens = NULL, **tmp = NULL;
    size_t currSize = 40;

    lexer = lexerCreate(input);
    *size = 0;

    if (!(tokens = (Token **)malloc(currSize * sizeof(Token *)))) {
        fprintf(stderr, "Error allocating memory for tokens");
        exit(EXIT_FAILURE);
    }

    do {
        if (*size > currSize) {
            currSize *= 2;
            if (!(tmp = (Token **)realloc(tokens, currSize * sizeof(Token *)))) {
                for (size_t i = 0; i < currSize; i++) {
                    tokenDestroy(tokens + i);
                }
                free(tokens);
                fprintf(stderr, "Error reallocating memory for tokens");
                exit(EXIT_FAILURE);
            }
            tokens = tmp;
        }

        tokens[*size] = lexerNext(lexer);
        (*size)++;
    } while (tokens[*size - 1]->type != TokenEof);

    lexerDestroy(&lexer);

    if (!(tmp = (Token **)realloc(tokens, *size * sizeof(Token *)))) {
        for (size_t i = 0; i < *size; i++) {
            tokenDestroy(tokens + i);
        }
        free(tokens);
        fprintf(stderr, "Error reallocating memory for tokens");
        exit(EXIT_FAILURE);
    }
    tokens = tmp;

    return tokens;
}
