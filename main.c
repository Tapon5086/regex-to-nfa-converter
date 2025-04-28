#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfa.h"
#include "graphviz.h"

// Declared in parser.y
extern NFA* result_nfa;
extern FILE* yyin;
extern int yyparse();

void print_usage(const char* program_name) {
    printf("Usage: %s [options] [regex]\n\n", program_name);
    printf("Options:\n");
    printf("  -f, --file FILE    Read regex from FILE\n");
    printf("  -o, --output FILE  Output base filename (default: nfa)\n");
    printf("  -t, --type FORMAT  Output format (dot, png, svg, pdf) (default: png)\n");
    printf("  -h, --help         Display this help message\n\n");
    printf("If regex is not provided and no input file is specified, reads from stdin.\n");
}

int main(int argc, char* argv[]) {
    char* input_file = NULL;
    char* output_file = "nfa";
    char* output_format = "png";
    char* regex = NULL;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            if (++i < argc) {
                input_file = argv[i];
            } else {
                fprintf(stderr, "Error: Missing filename after %s\n", argv[i-1]);
                return 1;
            }
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (++i < argc) {
                output_file = argv[i];
            } else {
                fprintf(stderr, "Error: Missing filename after %s\n", argv[i-1]);
                return 1;
            }
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--type") == 0) {
            if (++i < argc) {
                output_format = argv[i];
            } else {
                fprintf(stderr, "Error: Missing format after %s\n", argv[i-1]);
                return 1;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (regex == NULL) {
            regex = argv[i];
        } else {
            fprintf(stderr, "Warning: Ignoring extra argument: %s\n", argv[i]);
        }
    }
    
    // Set up input source
    if (regex != NULL) {
        // Use string as input
        yyin = fmemopen(regex, strlen(regex), "r");
        if (!yyin) {
            fprintf(stderr, "Error: Failed to process regex string\n");
            return 1;
        }
    } else if (input_file != NULL) {
        // Open input file
        yyin = fopen(input_file, "r");
        if (!yyin) {
            fprintf(stderr, "Error: Failed to open input file: %s\n", input_file);
            return 1;
        }
    } else {
        // Use stdin
        yyin = stdin;
        printf("Enter regex pattern (press Ctrl+D when done): ");
    }
    
    // Parse the regex
    if (yyparse() != 0) {
        fprintf(stderr, "Error: Failed to parse regex\n");
        if (yyin != stdin) fclose(yyin);
        return 1;
    }
    
    // Close input if it's not stdin
    if (yyin != stdin) fclose(yyin);
    
    // Check if parsing was successful
    if (!result_nfa) {
        fprintf(stderr, "Error: No NFA generated\n");
        return 1;
    }
    
    printf("Successfully created NFA with %d states\n", result_nfa->state_count);
    
    // Export the NFA
    export_nfa_to_image(result_nfa, output_file, output_format);
    printf("NFA exported to %s.%s\n", output_file, output_format);
    
    // Clean up
    free_nfa(result_nfa);
    
    return 0;
}