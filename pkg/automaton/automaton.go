package automaton

import (
	"errors"
	"fmt"
)

type Transition struct {
    FromState string
    Symbol    rune
    ToState   string
}

type Automaton struct {
    States       []string
    Alphabet     []rune

    Transitions  []Transition
    StartState   string
    AcceptStates []string
}

func containsRune(arr []rune, str rune) bool {
    for _, a := range arr {
        if a == str {
            return true
        }
    }
    return false
}

func contains(arr []string, str string) bool {
    for _, a := range arr {
        if a == str {
            return true
        }
    }
    return false
}

func isStatesValid(states []string) error {
    for index, state := range states {
        if state == "" {
            return errors.New(fmt.Sprintf("State of index %d invalid, can not be empty", index))
        }
    }

    return nil
}

func isTransitionsValid(states []string, alphabet []rune, transitions []Transition) error {
    for index, transition := range transitions {
        if !contains(states, transition.FromState) {
            return errors.New(fmt.Sprintf("Transition of index %d is invalid! FromState '%s' is not in the list of states", index, transition.FromState))
        } else if !containsRune(alphabet, transition.Symbol) {
            return errors.New(fmt.Sprintf("Transition of index %d is invalid! Symbol '%c' is not in alphabet", index, transition.Symbol))
        } else if !contains(states, transition.ToState) {
            return errors.New(fmt.Sprintf("Transition of index %d is invalid! ToState '%s' is not in the list of states", index, transition.ToState))
        }
    }

    return nil
}

func isAcceptStatesValid(states []string, acceptStates []string) error {
    for _, state := range acceptStates {
        if !contains(states, state) {
            return errors.New(fmt.Sprintf("Accept state '%s' is not in the list of states", state))
        }
    }

    return nil
}

func NewAutomaton(states []string, alphabet []rune, transitions []Transition, startState string, acceptStates []string) (*Automaton, error) {

    errorStates := isStatesValid(states)
    if errorStates != nil {
        return nil, errorStates
    }

    errorTransitions := isTransitionsValid(states, alphabet, transitions)
    if errorTransitions != nil {
        return nil, errorTransitions
    }

    if !contains(states, startState) {
        return nil, errors.New(fmt.Sprintf("Start state '%s' is not in the list of states", startState))
    }

    errorAcceptStates := isAcceptStatesValid(states, acceptStates)
    if errorAcceptStates != nil {
        return nil, errorAcceptStates
    }

    return &Automaton{
        States:       states,
        Alphabet:     alphabet,
        Transitions:  transitions,
        StartState:   startState,
        AcceptStates: acceptStates,
    }, nil
}

func (aut Automaton) GetNextState(currentState string, symbol rune) (string, error) {
    for _, transition := range aut.Transitions {
        if transition.FromState == currentState && transition.Symbol == symbol {
            return transition.ToState, nil
        }
    }

    return "", errors.New(fmt.Sprintf("There is no transition for the state '%s' and symbol '%c'", currentState, symbol))
}

func (aut Automaton) CheckInput(input string) (bool, error) {
    currentState := aut.StartState

    for _, symbol := range input {
        if !containsRune(aut.Alphabet, symbol) {
            return false, errors.New(fmt.Sprintf("Symbol '%c' is not in the alphabet", symbol))
        }

        newState, err := aut.GetNextState(currentState, symbol)

        if err != nil {
            return false, err
        }

        currentState = newState
    }

    return contains(aut.AcceptStates, currentState), nil
}

