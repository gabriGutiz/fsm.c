#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fsm.h"

#define MAX_STATES 256
#define MAX_ALPHABET 36
#define MAX_TRANSITIONS MAX_STATES * MAX_ALPHABET

typedef struct STransition {
    char *from;
    char c;
    char *to;
} Transition;

struct SFsm {
    char *name;
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
int _fsmStateExists(Fsm *fsm, char *state);
int _fsmSymbolExists(Fsm *fsm, char c);
char *_fsmGetNextState(Fsm *fsm, char *state, char c);
int _fsmCountTransitions(Fsm *fsm, char *state, char c);

/*****************************************************************************
*                              PUBLIC FUNCTIONS                              *
******************************************************************************/

Fsm *fsmCreate(char *name) {
    size_t len = sizeof(Fsm);

    Fsm *fsm = malloc(len);
    memset(fsm, 0, len);

    fsm->name = name;
    fsm->statesCount = 0;
    fsm->alphabetCount = 0;
    fsm->transitionsCount = 0;
    fsm->startState = NULL;
    fsm->acceptStatesCount = 0;

    return fsm;
}

char *fsmGetName(Fsm *fsm) {
    return fsm->name;
}

void fsmDestroy(Fsm **fsm) {
    if (*fsm) {
        free(*fsm);
    }

    fsm = NULL;
}

int fsmAddState(Fsm *fsm, char *state) {
    if (fsm->statesCount >= MAX_STATES) {
        fprintf(stderr, "Error max size of states is %d\n", MAX_STATES);
        return 1;
    } else if (_fsmStateExists(fsm, state)) {
        fprintf(stderr, "Error state '%s' is already setted\n", state);
        return 1;
    }

    fsm->states[fsm->statesCount] = state;
    fsm->statesCount++;
    return 0;
}

int fsmAddToAlphabet(Fsm *fsm, char c) {
    if (fsm->alphabetCount >= MAX_ALPHABET) {
        fprintf(stderr, "Error max size of alphabet is %d\n", MAX_ALPHABET);
        return 1;
    } else if (_fsmSymbolExists(fsm, c)) {
        fprintf(stderr, "Error symbol '%c' is already in alphabet\n", c);
        return 1;
    }

    fsm->alphabet[fsm->alphabetCount] = c;
    fsm->alphabetCount++;
    return 0;
}

int fsmAddTransition(Fsm *fsm, char *from, char c, char *to) {
    if (fsm->transitionsCount >= MAX_TRANSITIONS) {
        fprintf(stderr, "Error max size of transitions is %d\n", MAX_TRANSITIONS);
        return 1;
    } else if (!_fsmStateExists(fsm, from)) {
        fprintf(stderr, "Error state '%s' does not exist\n", from);
        return 2; // Error code for error in state from
    } else if (!_fsmSymbolExists(fsm, c)) {
        fprintf(stderr, "Error symbol '%c' does not exist in the alphabet\n", c);
        return 3; // Error code for error in symbol
    } else if (!_fsmStateExists(fsm, to)) {
        fprintf(stderr, "Error state '%s' does not exist\n", to);
        return 4; // Error code for error in state to
    }

    Transition t;
    t.from = from;
    t.c = c;
    t.to = to;

    fsm->transitions[fsm->transitionsCount] = t;
    fsm->transitionsCount++;
    return 0;
}

void fsmValidateTransitions(Fsm *fsm) {
    for (size_t i = 0; i < fsm->statesCount; i++) {
        char *state = fsm->states[i];

        for (size_t j = 0; j < fsm->alphabetCount; j++) {
            char c = fsm->alphabet[j];
            int totalTransitions = _fsmCountTransitions(fsm, state, c);

            if (totalTransitions < 1) {
                fprintf(stderr, "Error transition from state '%s' with symbol '%c' does not exist\n", state, c);
                exit(EXIT_FAILURE);
            } else if (totalTransitions > 1) {
                fprintf(stderr, "Error a total of %d transitions were found from state '%s' with symbol '%c'\n", totalTransitions, state, c);
                exit(EXIT_FAILURE);
            }
        }

        int found = 0;
        for (size_t i = 0; i < fsm->transitionsCount; i++) {
            Transition t = fsm->transitions[i];

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

int fsmAddStartState(Fsm *fsm, char *state) {
    if (fsm->startState) {
        fprintf(stderr, "Error start state is already setted\n");
        return 1;
    } else if (!_fsmStateExists(fsm, state)) {
        fprintf(stderr, "Error state '%s' does not exist\n", state);
        return 1;
    }

    fsm->startState = state;
    return 0;
}

int fsmAddAcceptState(Fsm *fsm, char *state) {
    if (fsm->acceptStatesCount >= MAX_STATES) {
        fprintf(stderr, "Error max size of accept states is %d\n", MAX_STATES);
        return 1;
        exit(EXIT_FAILURE);
    }else if (!_stringInArray(fsm->states, fsm->statesCount, state)) {
        fprintf(stderr, "Error state '%s' does not exist\n", state);
        return 1;
    }

    fsm->acceptStates[fsm->acceptStatesCount] = state;
    fsm->acceptStatesCount++;
    return 0;
}

int fsmCheck(Fsm *fsm, char *input) {
    char *state = fsm->startState;

    for (size_t i = 0; i < strlen(input); i++) {
        char c = input[i];
        char *nextState;

        if (!_charInArray(fsm->alphabet, fsm->alphabetCount, c)) {
            return 0;
        } else if (!(nextState = _fsmGetNextState(fsm, state, c))) {
            return 0;
        }

        state = nextState;
    }

    return _stringInArray(fsm->acceptStates, fsm->acceptStatesCount, state);
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

int _fsmStateExists(Fsm *fsm, char *state) {
    return _stringInArray(fsm->states, fsm->statesCount, state);
}

int _fsmSymbolExists(Fsm *fsm, char c) {
    return _charInArray(fsm->alphabet, fsm->alphabetCount, c);
}

char *_fsmGetNextState(Fsm *fsm, char *state, char c) {
    for (size_t i = 0; i < fsm->transitionsCount; i++) {
        Transition t = fsm->transitions[i];

        if (strcmp(t.from, state) == 0 && t.c == c) {
            return t.to;
        }
    }

    return NULL;
}

int _fsmCountTransitions(Fsm *fsm, char *state, char c) {
    int total = 0;

    for (size_t i = 0; i < fsm->transitionsCount; i++) {
        Transition t = fsm->transitions[i];

        if (strcmp(t.from, state) == 0 && t.c == c) {
            total++;
        }
    }

    return total;
}

