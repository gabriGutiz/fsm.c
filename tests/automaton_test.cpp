#include <gtest/gtest.h>

#include "automaton/automaton.h"

TEST(TestAutomaton, TestAutomaton_One) {
    Automaton *automaton = automatonCreate(strdup("One"));

    automatonAddState(automaton, strdup("s0"));
    automatonAddState(automaton, strdup("s1"));

    automatonAddToAlphabet(automaton, '0');
    automatonAddToAlphabet(automaton, '1');

    automatonAddTransition(automaton, strdup("s0"), '0', strdup("s0"));
    automatonAddTransition(automaton, strdup("s0"), '1', strdup("s1"));
    automatonAddTransition(automaton, strdup("s1"), '0', strdup("s0"));
    automatonAddTransition(automaton, strdup("s1"), '1', strdup("s1"));

    automatonAddStartState(automaton, strdup("s0"));

    automatonAddAcceptState(automaton, strdup("s1"));

    ASSERT_EQ(automatonCheck(automaton, strdup("0")), 0);
    ASSERT_EQ(automatonCheck(automaton, strdup("a1")), 0);
    ASSERT_EQ(automatonCheck(automaton, strdup("01")), 1);
    ASSERT_EQ(automatonCheck(automaton, strdup("011111")), 1);

    automatonDestroy(&automaton);
}
