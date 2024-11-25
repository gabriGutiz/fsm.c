#ifndef _AUTOMATON_H_
#define _AUTOMATON_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SAutomaton Automaton;
Automaton *automatonCreate();
void automatonDestroy(Automaton **automaton);
void automatonAddState(Automaton *automaton, char *state);
void automatonAddToAlphabet(Automaton *automaton, char c);
void automatonAddTransition(Automaton *automaton, char *from, char c, char *to);
void automatonValidateTransitions(Automaton *automaton);
void automatonAddStartState(Automaton *automaton, char *state);
void automatonAddAcceptState(Automaton *automaton, char *state);
int automatonCheck(Automaton *automaton, char *input);

#ifdef __cplusplus
}
#endif

#endif // _AUTOMATON_H_
