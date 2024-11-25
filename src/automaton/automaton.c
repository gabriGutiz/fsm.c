#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "automaton.h"

#define MAX_STATES 256
#define MAX_ALPHABET 36
#define MAX_TRANSITIONS MAX_STATES * MAX_ALPHABET

typedef struct STransition {
    char *from;
    char c;
    char *to;
} Transition;

struct SAutomaton {
    char *states[MAX_STATES];
    size_t statesCount;
    char alphabet[MAX_ALPHABET];
    size_t alphabetCount;
    Transition transitions[MAX_TRANSITIONS];
    size_t transitionsCount;
    char *startState;
    char *acceptStates[MAX_STATES];
    size_t acceptStatesCount;
};

int _stringInArray(char *array[], size_t size, char *value);
int _charInArray(char array[], size_t size, char value);
int _automatonStateExists(Automaton *automaton, char *state);
int _automatonSymbolExists(Automaton *automaton, char c);
char *_automatonGetNextState(Automaton *automaton, char *state, char c);
int _automatonCountTransitions(Automaton *automaton, char *state, char c);

/*****************************************************************************
*                              PUBLIC FUNCTIONS                              *
******************************************************************************/

Automaton *automatonCreate() {
    size_t len = sizeof(Automaton);

    Automaton *automaton = malloc(len);
    memset(automaton, 0, len);

    automaton->statesCount = 0;
    automaton->alphabetCount = 0;
    automaton->transitionsCount = 0;
    automaton->startState = NULL;
    automaton->acceptStatesCount = 0;

    return automaton;
}

void automatonDestroy(Automaton **automaton) {
    if (*automaton) {
        free(*automaton);
    }

    automaton = NULL;
}

void automatonAddState(Automaton *automaton, char *state) {
    if (automaton->statesCount >= MAX_STATES) {
        fprintf(stderr, "Error max size of states is %d\n", MAX_STATES);
        exit(EXIT_FAILURE);
    } else if (_automatonStateExists(automaton, state)) {
        fprintf(stderr, "Error state '%s' is already setted\n", state);
        exit(EXIT_FAILURE);
    }

    automaton->states[automaton->statesCount] = state;
    automaton->statesCount++;
}

void automatonAddToAlphabet(Automaton *automaton, char c) {
    if (automaton->alphabetCount >= MAX_ALPHABET) {
        fprintf(stderr, "Error max size of alphabet is %d\n", MAX_ALPHABET);
        exit(EXIT_FAILURE);
    } else if (_automatonSymbolExists(automaton, c)) {
        fprintf(stderr, "Error symbol '%c' is already in alphabet\n", c);
        exit(EXIT_FAILURE);
    }

    automaton->alphabet[automaton->alphabetCount] = c;
    automaton->alphabetCount++;
}

void automatonAddTransition(Automaton *automaton, char *from, char c, char *to) {
    if (automaton->transitionsCount >= MAX_TRANSITIONS) {
        fprintf(stderr, "Error max size of transitions is %d\n", MAX_TRANSITIONS);
        exit(EXIT_FAILURE);
    } else if (!_automatonStateExists(automaton, from)) {
        fprintf(stderr, "Error state '%s' does not exist\n", from);
        exit(EXIT_FAILURE);
    } else if (!_automatonSymbolExists(automaton, c)) {
        fprintf(stderr, "Error symbol '%c' does not exist in the alphabet\n", c);
        exit(EXIT_FAILURE);
    } else if (!_automatonStateExists(automaton, to)) {
        fprintf(stderr, "Error state '%s' does not exist\n", to);
        exit(EXIT_FAILURE);
    }

    Transition t;
    t.from = from;
    t.c = c;
    t.to = to;

    automaton->transitions[automaton->transitionsCount] = t;
    automaton->transitionsCount++;
}

void automatonValidateTransitions(Automaton *automaton) {
    for (size_t i = 0; i < automaton->statesCount; i++) {
        char *state = automaton->states[i];

        for (size_t j = 0; j < automaton->alphabetCount; j++) {
            char c = automaton->alphabet[j];
            int totalTransitions = _automatonCountTransitions(automaton, state, c);

            if (totalTransitions < 1) {
                fprintf(stderr, "Error transition from state '%s' with symbol '%c' does not exist\n", state, c);
                exit(EXIT_FAILURE);
            } else if (totalTransitions > 1) {
                fprintf(stderr, "Error a total of %d transitions were found from state '%s' with symbol '%c'\n", totalTransitions, state, c);
                exit(EXIT_FAILURE);
            }
        }

        int found = 0;
        for (size_t i = 0; i < automaton->transitionsCount; i++) {
            Transition t = automaton->transitions[i];

            if (strcmp(t.to, state) == 0) {
                found = 1;
            }
        }

        if (!found) {
            fprintf(stderr, "Error there is no transitions to state '%s'\n", state);
            exit(EXIT_FAILURE);
        }
    }
}

void automatonAddStartState(Automaton *automaton, char *state) {
    if (automaton->startState) {
        fprintf(stderr, "Error start state is already setted\n");
        exit(EXIT_FAILURE);
    } else if (!_automatonStateExists(automaton, state)) {
        fprintf(stderr, "Error state '%s' does not exist\n", state);
        exit(EXIT_FAILURE);
    }

    automaton->startState = state;
}

void automatonAddAcceptState(Automaton *automaton, char *state) {
    if (automaton->acceptStatesCount >= MAX_STATES) {
        fprintf(stderr, "Error max size of accept states is %d\n", MAX_STATES);
        exit(EXIT_FAILURE);
    }else if (!_stringInArray(automaton->states, automaton->statesCount, state)) {
        fprintf(stderr, "Error state '%s' does not exist\n", state);
        exit(EXIT_FAILURE);
    }

    automaton->acceptStates[automaton->acceptStatesCount] = state;
    automaton->acceptStatesCount++;
}

int automatonCheck(Automaton *automaton, char *input) {
    char *state = automaton->startState;

    for (size_t i = 0; i < strlen(input); i++) {
        char c = input[i];
        char *nextState;

        if (!_charInArray(automaton->alphabet, automaton->alphabetCount, c)) {
            return 0;
        } else if (!(nextState = _automatonGetNextState(automaton, state, c))) {
            return 0;
        }

        state = nextState;
    }

    return _stringInArray(automaton->acceptStates, automaton->acceptStatesCount, state);
}

/*****************************************************************************
*                              PRIVATE FUNCTIONS                             *
******************************************************************************/

int _stringInArray(char *array[], size_t size, char *value) {
    for (size_t i = 0; i < size; i++) {
        if (strcmp(array[i], value) == 0) {
            return 1;
        }
    }

    return 0;
}

int _charInArray(char array[], size_t size, char value) {
    for (size_t i = 0; i < size; i++) {
        if (array[i] == value) {
            return 1;
        }
    }

    return 0;
}

int _automatonStateExists(Automaton *automaton, char *state) {
    return _stringInArray(automaton->states, automaton->statesCount, state);
}

int _automatonSymbolExists(Automaton *automaton, char c) {
    return _charInArray(automaton->alphabet, automaton->alphabetCount, c);
}

char *_automatonGetNextState(Automaton *automaton, char *state, char c) {
    for (size_t i = 0; i < automaton->transitionsCount; i++) {
        Transition t = automaton->transitions[i];

        if (strcmp(t.from, state) == 0 && t.c == c) {
            return t.to;
        }
    }

    return NULL;
}

int _automatonCountTransitions(Automaton *automaton, char *state, char c) {
    int total = 0;

    for (size_t i = 0; i < automaton->transitionsCount; i++) {
        Transition t = automaton->transitions[i];

        if (strcmp(t.from, state) == 0 && t.c == c) {
            total++;
        }
    }

    return total;
}

