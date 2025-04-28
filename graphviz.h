#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "nfa.h"

// Export NFA to Graphviz DOT format
void export_nfa_to_dot(struct NFA* nfa, const char* filename);

// Export NFA directly to image using Graphviz
void export_nfa_to_image(struct NFA* nfa, const char* filename, const char* format);

#endif