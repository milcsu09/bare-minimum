
all:
	clang -std=c11 -Wall -Wextra -Wpedantic -o C Compiler/*.c # `llvm-config --cflags --libs core analysis`

