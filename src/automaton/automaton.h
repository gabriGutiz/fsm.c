#ifndef _AUTOMATON_H_
#define _AUTOMATON_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SAutomaton Automaton;
Automaton *automatonCreate();
void automatonDestroy(Automaton **automaton);
int automatonAddState(Automaton *automaton, char *state);
int automatonAddToAlphabet(Automaton *automaton, char c);
int automatonAddTransition(Automaton *automaton, char *from, char c, char *to);
void automatonValidateTransitions(Automaton *automaton);
int automatonAddStartState(Automaton *automaton, char *state);
int automatonAddAcceptState(Automaton *automaton, char *state);
int automatonCheck(Automaton *automaton, char *input);

#ifdef __cplusplus
}
#endif

#endif // _AUTOMATON_H_
