#ifndef NFA_H
#define NFA_H

#include <stdbool.h>

// Define epsilon transition character
#define EPSILON '\0'

// State structure
typedef struct State {
    int id;
    bool is_accepting;
    struct Transition* transitions;
    struct State* next;  // For state list management
} State;

// Transition structure
typedef struct Transition {
    char symbol;
    State* next_state;
    struct Transition* next;  // For transition list management
} Transition;

// NFA structure
typedef struct NFA {
    State* start;
    State* accept;
    int state_count;
} NFA;

// NFA creation functions
struct NFA* create_char_nfa(char c);
struct NFA* create_any_char_nfa();
struct NFA* create_epsilon_nfa();
struct NFA* create_union_nfa(struct NFA* nfa1, struct NFA* nfa2);
struct NFA* create_concat_nfa(struct NFA* nfa1, struct NFA* nfa2);
struct NFA* create_kleene_star_nfa(struct NFA* nfa);
struct NFA* create_plus_nfa(struct NFA* nfa);
struct NFA* create_question_nfa(struct NFA* nfa);
struct NFA* create_char_range_nfa(char start, char end);

// NFA utility functions
void add_transition(State* from, char symbol, State* to);
State* create_state(int id, bool is_accepting);
void free_nfa(struct NFA* nfa);

#endif