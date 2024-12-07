#ifndef _FSM_H_
#define _FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SFsm Fsm;
Fsm *fsmCreate(char *name);
char *fsmGetName(Fsm *fsm);
void fsmDestroy(Fsm **fsm);
int fsmAddState(Fsm *fsm, char *state);
int fsmAddToAlphabet(Fsm *fsm, char c);
int fsmAddTransition(Fsm *fsm, char *from, char c, char *to);
void fsmValidateTransitions(Fsm *fsm);
int fsmAddStartState(Fsm *fsm, char *state);
int fsmAddAcceptState(Fsm *fsm, char *state);
int fsmCheck(Fsm *fsm, char *input);

#ifdef __cplusplus
}
#endif

#endif // _FSM_H_
