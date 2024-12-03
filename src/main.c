#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "automaton/automaton.h"

char *readFile(const char *filename);

int main(int argc, char *argv[]) {
    char *fileContent;
    size_t totalTokens = 0;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <filename> <test_string>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (!(fileContent = readFile(argv[1]))) {
        fprintf(stderr, "Error reading file or file is empty");
        return EXIT_FAILURE;
    }

    Lexer *lexer = lexerCreate(fileContent);
    Parser *parser = parserCreate(lexer);
    Automaton *aut = parserParse(parser);

    if (automatonCheck(aut, argv[2]) == 1) {
        printf("String '%s' is accepted by automaton %s\n", argv[2], automatonGetName(aut));
    } else {
        printf("String '%s' is NOT accepted by automaton %s\n", argv[2], automatonGetName(aut));
    }

    lexerDestroy(&lexer);
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
