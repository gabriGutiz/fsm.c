#include <gtest/gtest.h>

#include "fsm/fsm.h"

TEST(TestFsm, TestFsm_One) {
    Fsm *fsm = fsmCreate(strdup("One"));

    fsmAddState(fsm, strdup("s0"));
    fsmAddState(fsm, strdup("s1"));

    fsmAddToAlphabet(fsm, '0');
    fsmAddToAlphabet(fsm, '1');

    fsmAddTransition(fsm, strdup("s0"), '0', strdup("s0"));
    fsmAddTransition(fsm, strdup("s0"), '1', strdup("s1"));
    fsmAddTransition(fsm, strdup("s1"), '0', strdup("s0"));
    fsmAddTransition(fsm, strdup("s1"), '1', strdup("s1"));

    fsmAddStartState(fsm, strdup("s0"));

    fsmAddAcceptState(fsm, strdup("s1"));

    ASSERT_EQ(fsmCheck(fsm, strdup("0")), 0);
    ASSERT_EQ(fsmCheck(fsm, strdup("a1")), 0);
    ASSERT_EQ(fsmCheck(fsm, strdup("01")), 1);
    ASSERT_EQ(fsmCheck(fsm, strdup("011111")), 1);

    fsmDestroy(&fsm);
}
