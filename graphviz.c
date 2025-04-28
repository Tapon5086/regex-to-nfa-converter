#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphviz.h"

// Helper function to get readable representation of symbol
char* get_symbol_label(char symbol) {
    static char label[10];
    
    if (symbol == EPSILON) {
        strcpy(label, "ε");
    } else if (symbol == '\n') {
        strcpy(label, "\\n");
    } else if (symbol == '\t') {
        strcpy(label, "\\t");
    } else if (symbol == '\r') {
        strcpy(label, "\\r");
    } else {
        label[0] = symbol;
        label[1] = '\0';
    }
    
    return label;
}

// Export NFA to DOT format
void export_nfa_to_dot(struct NFA* nfa, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Failed to open file %s for writing\n", filename);
        return;
    }
    
    // Write DOT file header
    fprintf(file, "digraph NFA {\n");
    fprintf(file, "    rankdir=LR;\n");
    fprintf(file, "    node [shape=circle];\n");
    
    // Create a queue for BFS traversal
    State** queue = (State**)malloc(sizeof(State*) * nfa->state_count);
    if (!queue) {
        fprintf(stderr, "Memory allocation failed for queue\n");
        fclose(file);
        return;
    }
    
    bool* visited = (bool*)calloc(nfa->state_count, sizeof(bool));
    if (!visited) {
        fprintf(stderr, "Memory allocation failed for visited array\n");
        free(queue);
        fclose(file);
        return;
    }
    
    int front = 0, rear = 0;
    
    // Add start state to queue
    queue[rear++] = nfa->start;
    visited[nfa->start->id] = true;
    
    // Mark start state
    fprintf(file, "    \"start\" [shape=point];\n");
    fprintf(file, "    \"start\" -> \"%d\";\n", nfa->start->id);
    
    // BFS traversal
    while (front < rear) {
        State* current = queue[front++];
        
        // If accepting state, make it double circle
        if (current->is_accepting) {
            fprintf(file, "    \"%d\" [shape=doublecircle];\n", current->id);
        }
        
        // Process all transitions
        Transition* trans = current->transitions;
        while (trans) {
            // Add transition to DOT file
            fprintf(file, "    \"%d\" -> \"%d\" [label=\"%s\"];\n", 
                    current->id, trans->next_state->id, get_symbol_label(trans->symbol));
            
            // Add next state to queue if not visited
            if (!visited[trans->next_state->id]) {
                queue[rear++] = trans->next_state;
                visited[trans->next_state->id] = true;
            }
            
            trans = trans->next;
        }
    }
    
    // Write DOT file footer
    fprintf(file, "}\n");
    
    // Clean up
    fclose(file);
    free(queue);
    free(visited);
    
    printf("DOT file successfully written to %s\n", filename);
}

// Check if a command exists in PATH
int command_exists(const char* cmd) {
    char which_cmd[256];
    sprintf(which_cmd, "which %s > /dev/null 2>&1", cmd);
    return system(which_cmd) == 0;
}

// Export NFA to image using Graphviz
void export_nfa_to_image(struct NFA* nfa, const char* filename, const char* format) {
    // First export to DOT
    char dot_filename[256];
    sprintf(dot_filename, "%s.dot", filename);
    export_nfa_to_dot(nfa, dot_filename);
    
    // Check if dot command exists
    if (!command_exists("dot")) {
        fprintf(stderr, "Error: Graphviz 'dot' command not found. Please install Graphviz.\n");
        fprintf(stderr, "You can still use the generated DOT file (%s) manually.\n", dot_filename);
        return;
    }
    
    // Then use system call to convert DOT to image
    char command[512];
    sprintf(command, "dot -T%s %s -o %s.%s", format, dot_filename, filename, format);
    
    printf("Executing command: %s\n", command);
    int result = system(command);
    
    if (result != 0) {
        fprintf(stderr, "Failed to execute Graphviz command (error code: %d)\n", result);
        fprintf(stderr, "Command was: %s\n", command);
        
        if (result == 127) {
            fprintf(stderr, "Error 127 usually means the command was not found.\n");
            fprintf(stderr, "Make sure Graphviz is installed and in your PATH.\n");
        }
    } else {
        printf("NFA successfully exported to %s.%s\n", filename, format);
    }
}