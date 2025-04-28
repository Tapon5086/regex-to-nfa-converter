# Regex to NFA Converter

This project implements a regular expression to NFA (Non-deterministic Finite Automaton) converter using Flex, Bison, and Graphviz. It parses regular expressions and generates visual representations of the corresponding NFA.

## Features

- Parses regular expressions using Flex (lexical analyzer) and Bison (parser)
- Constructs NFA data structures for the regex
- Visualizes the NFA using Graphviz
- Supports common regex operations:
  - Concatenation
  - Alternation (|)
  - Kleene star (*)
  - Plus (+)
  - Question mark (?)
  - Character classes [a-z]
  - Dot operator (.)
  - Escape sequences

## Requirements

- Flex (lexical analyzer generator)
- Bison (parser generator)
- Graphviz (graph visualization software)
- GCC or other C compiler

## Installation

### On Ubuntu/Debian

```bash
sudo apt-get install flex bison graphviz build-essential
```

### On macOS

```bash
brew install flex bison graphviz
```

### On Windows

Use WSL (Windows Subsystem for Linux) or MinGW/MSYS2 with similar packages.

## Building the Project

```bash
make
```

This will generate the `regex2nfa` executable.

## Usage

```bash
./regex2nfa [options] [regex]
```

### Options

- `-f, --file FILE` : Read regex from FILE
- `-o, --output FILE` : Output base filename (default: nfa)
- `-t, --type FORMAT` : Output format (dot, png, svg, pdf) (default: png)
- `-h, --help` : Display help message

### Examples

Convert a simple regex to an NFA and save as PNG:
```bash
./regex2nfa "a(b|c)*"
```

Read regex from a file and save as SVG:
```bash
./regex2nfa -f regex.txt -t svg -o output
```

## Regex Syntax

- `a` : Single character
- `ab` : Concatenation
- `a|b` : Alternation
- `a*` : Kleene star (zero or more)
- `a+` : Plus (one or more)
- `a?` : Question mark (zero or one)
- `.` : Any character
- `[a-z]` : Character class
- `\c` : Escaped character

## Implementation Details

The converter works in several stages:
1. **Lexical Analysis**: Flex tokenizes the input regex
2. **Parsing**: Bison parses the tokens and builds an NFA structure
3. **NFA Construction**: Each regex operation is translated to NFA states and transitions
4. **Visualization**: Graphviz converts the NFA to a visual graph

## Project Structure

- `src/lexer.l`: Flex lexer for regex tokens
- `src/parser.y`: Bison parser for regex grammar
- `src/nfa.h/c`: NFA data structures and operations
- `src/graphviz.h/c`: Graphviz output generation
- `src/main.c`: Main program and command-line interface
- `Makefile`: Build instructions

## License

This project is provided as-is with no warranties.