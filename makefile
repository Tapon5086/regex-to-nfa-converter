# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Flex and Bison
FLEX = flex
BISON = bison

# Source and build directories
SRC_DIR = src
BUILD_DIR = build

# Target executable
TARGET = regex2nfa

# Source files
SRCS = $(SRC_DIR)/nfa.c $(SRC_DIR)/graphviz.c $(SRC_DIR)/main.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS)) $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.tab.o

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile .c files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Generate lexer.c from lexer.l
$(BUILD_DIR)/lexer.c: $(SRC_DIR)/lexer.l $(BUILD_DIR)/parser.tab.h
	$(FLEX) -o $@ $<

# Generate parser.tab.c and parser.tab.h from parser.y
$(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/parser.tab.h: $(SRC_DIR)/parser.y
	$(BISON) -d -o $(BUILD_DIR)/parser.tab.c $<

# Compile lexer
$(BUILD_DIR)/lexer.o: $(BUILD_DIR)/lexer.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile parser
$(BUILD_DIR)/parser.tab.o: $(BUILD_DIR)/parser.tab.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link everything
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Clean and rebuild
rebuild: clean all

.PHONY: all clean rebuild