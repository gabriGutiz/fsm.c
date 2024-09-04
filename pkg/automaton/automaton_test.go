package automaton

import (
    "testing"
)

func TestNewAutomaton(t *testing.T) {

    // States Validation
    t.Run("should return error if have state empty", func(t *testing.T) {
        aut, err := NewAutomaton([]string{"s1", ""}, []rune{'0', '1'}, []Transition{}, "s1", []string{"s1"})

        assertError(t, err, "State of index 1 invalid, can not be empty")

        if aut != nil {
            t.Errorf("got %v, want nil", aut)
        }
	})

    // Transitions Validation
    t.Run("should return error if FromState not in states", func(t *testing.T) {

        transitions := []Transition{
            { FromState: "abc", Symbol: '0', ToState: "s2" },
        }

        aut, err := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "s1", []string{"s1"})

        assertError(t, err, "Transition of index 0 is invalid! FromState 'abc' is not in the list of states")

        if aut != nil {
            t.Errorf("got %v, want nil", aut)
        }
	})

    t.Run("should return error if ToState not in states", func(t *testing.T) {

        transitions := []Transition{
            { FromState: "s1", Symbol: '0', ToState: "abc" },
        }

        aut, err := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "s1", []string{"s1"})

        assertError(t, err, "Transition of index 0 is invalid! ToState 'abc' is not in the list of states")

        if aut != nil {
            t.Errorf("got %v, want nil", aut)
        }
	})

    t.Run("should return error if Symbol not in alphabet", func(t *testing.T) {

        transitions := []Transition{
            { FromState: "s1", Symbol: '3', ToState: "s2" },
        }

        aut, err := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "s1", []string{"s1"})

        assertError(t, err, "Transition of index 0 is invalid! Symbol '3' is not in alphabet")

        if aut != nil {
            t.Errorf("got %v, want nil", aut)
        }
	})

    // StartState Validation
    t.Run("should return error if StartState not in states", func(t *testing.T) {

        transitions := []Transition{
            { FromState: "s1", Symbol: '0', ToState: "s2" },
        }

        aut, err := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "abc", []string{"s1"})

        assertError(t, err, "Start state 'abc' is not in the list of states")

        if aut != nil {
            t.Errorf("got %v, want nil", aut)
        }
	})

    // AcceptStates Validation
    t.Run("should return error if state in AcceptStates is not in states", func(t *testing.T) {

        transitions := []Transition{
            { FromState: "s1", Symbol: '0', ToState: "s2" },
        }

        aut, err := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "s1", []string{"abc"})

        assertError(t, err, "Accept state 'abc' is not in the list of states")

        if aut != nil {
            t.Errorf("got %v, want nil", aut)
        }
	})

    // Success
    t.Run("should return error nil and automaton", func(t *testing.T) {

        transitions := []Transition{
            { FromState: "s1", Symbol: '0', ToState: "s2" },
        }

        aut, err := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "s1", []string{"s1"})

        assertNoError(t, err)

        if aut == nil {
            t.Errorf("got nil, want not nil")
        }
	})

}

func TestGetNextState(t *testing.T) {

    transitions := []Transition{
        { FromState: "s1", Symbol: '0', ToState: "s2" },
    }

    aut, _ := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "s1", []string{"s1"})

    t.Run("should return error if transition not found", func(t *testing.T) {

        state, err := aut.GetNextState("s1", '5')

        assertError(t, err, "There is no transition for the state 's1' and symbol '5'")

        if state != "" {
            t.Errorf("got %s, want empty", state)
        }
	})

    t.Run("should return state if found", func(t *testing.T) {

        state, err := aut.GetNextState("s1", '0')

        assertNoError(t, err)

        if state == "" {
            t.Errorf("got empty, want %s", "s2")
        }
	})

}

func TestCheckInput(t *testing.T) {
    
    transitions := []Transition{
        { FromState: "s1", Symbol: '0', ToState: "s1" },
        { FromState: "s1", Symbol: '1', ToState: "s2" },
        { FromState: "s2", Symbol: '0', ToState: "s1" },
        { FromState: "s2", Symbol: '1', ToState: "s2" },
    }

    // automaton that accepts strings that end with 1
    aut, _ := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "s1", []string{"s2"})

    t.Run("should return error if symbol not in alphabet", func(t *testing.T) {

        result, err := aut.CheckInput("12")

        assertError(t, err, "Symbol '2' is not in the alphabet")
        assertEqualBoolean(t, result, false)
	})

    t.Run("should return error if transition not found", func(t *testing.T) {
        transitions := []Transition{
            { FromState: "s1", Symbol: '0', ToState: "s1" },
            { FromState: "s2", Symbol: '0', ToState: "s1" },
            { FromState: "s2", Symbol: '1', ToState: "s2" },
        }

        aut, _ := NewAutomaton([]string{"s1", "s2"}, []rune{'0', '1'}, transitions, "s1", []string{"s2"})

        result, err := aut.CheckInput("1")

        assertError(t, err, "There is no transition for the state 's1' and symbol '1'")
        assertEqualBoolean(t, result, false)
	})

    t.Run("should return error nil and false if input invalid", func(t *testing.T) {

        result, err := aut.CheckInput("10")

        assertNoError(t, err)
        assertEqualBoolean(t, result, false)
	})

    t.Run("should return error nil and true if input invalid", func(t *testing.T) {

        result, err := aut.CheckInput("101")

        assertNoError(t, err)
        assertEqualBoolean(t, result, true)
	})

}

func assertEqualBoolean(t testing.TB, got bool, want bool) {
    t.Helper()
    if got != want {
        t.Errorf("got %t, want %t", got, want)
    }
}

func assertNoError(t testing.TB, got error) {
    t.Helper()
    if got != nil {
        t.Fatal("didn't want an error but got one")
    }
}

func assertError(t testing.TB, got error, want string) {
    t.Helper()
    if got == nil {
        t.Fatal("wanted an error but didn't get one")
    }

    if got.Error() != want {
        t.Errorf("got %q, want %q", got, want)
    }
}

