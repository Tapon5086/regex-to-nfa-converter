#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"

// Global state ID counter
static int next_state_id = 0;

// Create a new state
State* create_state(int id, bool is_accepting) {
    State* state = (State*)malloc(sizeof(State));
    if (!state) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    state->id = id;
    state->is_accepting = is_accepting;
    state->transitions = NULL;
    state->next = NULL;
    
    return state;
}

// Add a transition between states
void add_transition(State* from, char symbol, State* to) {
    Transition* trans = (Transition*)malloc(sizeof(Transition));
    if (!trans) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    trans->symbol = symbol;
    trans->next_state = to;
    trans->next = from->transitions;
    from->transitions = trans;
}

// Create a basic NFA that accepts a single character
struct NFA* create_char_nfa(char c) {
    struct NFA* nfa = (struct NFA*)malloc(sizeof(struct NFA));
    if (!nfa) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    nfa->start = create_state(next_state_id++, false);
    nfa->accept = create_state(next_state_id++, true);
    nfa->state_count = 2;
    
    add_transition(nfa->start, c, nfa->accept);
    
    return nfa;
}

// Create an NFA that accepts any character (.)
struct NFA* create_any_char_nfa() {
    struct NFA* nfa = (struct NFA*)malloc(sizeof(struct NFA));
    if (!nfa) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    nfa->start = create_state(next_state_id++, false);
    nfa->accept = create_state(next_state_id++, true);
    nfa->state_count = 2;
    
    // Add transitions for all ASCII printable characters (32-126)
    for (char c = 32; c <= 126; c++) {
        add_transition(nfa->start, c, nfa->accept);
    }
    
    // Add transitions for common escape sequences
    add_transition(nfa->start, '\n', nfa->accept);
    add_transition(nfa->start, '\t', nfa->accept);
    add_transition(nfa->start, '\r', nfa->accept);
    
    return nfa;
}

// Create an NFA with epsilon transition
struct NFA* create_epsilon_nfa() {
    struct NFA* nfa = (struct NFA*)malloc(sizeof(struct NFA));
    if (!nfa) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    nfa->start = create_state(next_state_id++, false);
    nfa->accept = create_state(next_state_id++, true);
    nfa->state_count = 2;
    
    add_transition(nfa->start, EPSILON, nfa->accept);
    
    return nfa;
}

// Create union of two NFAs (|)
struct NFA* create_union_nfa(struct NFA* nfa1, struct NFA* nfa2) {
    struct NFA* nfa = (struct NFA*)malloc(sizeof(struct NFA));
    if (!nfa) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    nfa->start = create_state(next_state_id++, false);
    nfa->accept = create_state(next_state_id++, true);
    nfa->state_count = nfa1->state_count + nfa2->state_count + 2;
    
    // Connect new start to both NFA's start states
    add_transition(nfa->start, EPSILON, nfa1->start);
    add_transition(nfa->start, EPSILON, nfa2->start);
    
    // Connect both NFA's accept states to new accept state
    nfa1->accept->is_accepting = false;
    nfa2->accept->is_accepting = false;
    add_transition(nfa1->accept, EPSILON, nfa->accept);
    add_transition(nfa2->accept, EPSILON, nfa->accept);
    
    return nfa;
}

// Create concatenation of two NFAs
struct NFA* create_concat_nfa(struct NFA* nfa1, struct NFA* nfa2) {
    nfa1->state_count += nfa2->state_count;
    
    // Connect nfa1's accept state to nfa2's start state
    nfa1->accept->is_accepting = false;
    add_transition(nfa1->accept, EPSILON, nfa2->start);
    
    // Set nfa1's accept state to nfa2's accept state
    nfa1->accept = nfa2->accept;
    
    // Free the nfa2 struct but not its states
    free(nfa2);
    
    return nfa1;
}

// Create Kleene star NFA (*)
struct NFA* create_kleene_star_nfa(struct NFA* nfa) {
    struct NFA* new_nfa = (struct NFA*)malloc(sizeof(struct NFA));
    if (!new_nfa) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    new_nfa->start = create_state(next_state_id++, false);
    new_nfa->accept = create_state(next_state_id++, true);
    new_nfa->state_count = nfa->state_count + 2;
    
    // Connect new start to original start
    add_transition(new_nfa->start, EPSILON, nfa->start);
    
    // Connect new start to new accept (empty string case)
    add_transition(new_nfa->start, EPSILON, new_nfa->accept);
    
    // Connect original accept to original start (loop back)
    nfa->accept->is_accepting = false;
    add_transition(nfa->accept, EPSILON, nfa->start);
    
    // Connect original accept to new accept
    add_transition(nfa->accept, EPSILON, new_nfa->accept);
    
    return new_nfa;
}

// Create plus NFA (+)
struct NFA* create_plus_nfa(struct NFA* nfa) {
    // Create a copy for repetition
    struct NFA* star_nfa = create_kleene_star_nfa(nfa);
    
    // Remove the epsilon transition from start to accept
    // (This is a simplified approach; in a real implementation,
    // you'd need to actually remove the transition)
    
    return star_nfa;
}

// Create question mark NFA (?)
struct NFA* create_question_nfa(struct NFA* nfa) {
    struct NFA* new_nfa = (struct NFA*)malloc(sizeof(struct NFA));
    if (!new_nfa) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    new_nfa->start = create_state(next_state_id++, false);
    new_nfa->accept = create_state(next_state_id++, true);
    new_nfa->state_count = nfa->state_count + 2;
    
    // Connect new start to original start
    add_transition(new_nfa->start, EPSILON, nfa->start);
    
    // Connect new start to new accept (empty string case)
    add_transition(new_nfa->start, EPSILON, new_nfa->accept);
    
    // Connect original accept to new accept
    nfa->accept->is_accepting = false;
    add_transition(nfa->accept, EPSILON, new_nfa->accept);
    
    return new_nfa;
}

// Create character range NFA [a-z]
struct NFA* create_char_range_nfa(char start, char end) {
    struct NFA* nfa = (struct NFA*)malloc(sizeof(struct NFA));
    if (!nfa) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    nfa->start = create_state(next_state_id++, false);
    nfa->accept = create_state(next_state_id++, true);
    nfa->state_count = 2;
    
    // Add transitions for all characters in the range
    for (char c = start; c <= end; c++) {
        add_transition(nfa->start, c, nfa->accept);
    }
    
    return nfa;
}

// Free NFA memory
void free_nfa(struct NFA* nfa) {
    // This would require traversing the entire NFA structure
    // and freeing all states and transitions
    // Not implemented for brevity
}