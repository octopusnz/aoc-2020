## Root Makefile for Advent of Code 2020 in C

CC ?= cc
# Common flags
COMMON_CFLAGS = -Ilibs \
                 -pedantic \
                 -std=c89 \
                 -Wall \
                 -Werror=missing-braces \
                 -Wextra \
                 -Wpadded \
                 -Wshadow \
                 -Wstrict-prototypes \
                 -pg

# GCC-only warnings that clang doesn't support
GCC_ONLY_CFLAGS = -Wduplicated-cond -Wduplicated-branches

# Detect compiler and compose CFLAGS
IS_CLANG := $(findstring clang,$(shell $(CC) --version 2>/dev/null | head -n1))
# Clang-only flags (safe, minimal); extend if you want stricter clang checks
CLANG_ONLY_CFLAGS = \
         -Wnewline-eof \
         -Wmissing-variable-declarations \
         -Wshorten-64-to-32 \
         -Wpointer-bool-conversion \
         -Wconditional-uninitialized

# Allow external overrides
EXTRA_CFLAGS ?=

ifeq ($(IS_CLANG),)
    CFLAGS = $(COMMON_CFLAGS) $(GCC_ONLY_CFLAGS) $(EXTRA_CFLAGS)
else
    CFLAGS = $(COMMON_CFLAGS) $(CLANG_ONLY_CFLAGS) $(EXTRA_CFLAGS)
endif

LIBS = libs/eight_files.c libs/eight_algorithms.c
BIN_DIR = bin

# Find all problem folders with main.c (e.g., 01/main.c, 02/main.c, ...)
PROBLEM_SRCS := $(wildcard [0-9][0-9]/main.c)
PROBLEM_DIRS := $(sort $(dir $(PROBLEM_SRCS)))
# Strip trailing slashes to get directory names like 01, 02, ...
PROBLEMS := $(patsubst %/,%, $(PROBLEM_DIRS))
EXES := $(addprefix $(BIN_DIR)/, $(PROBLEMS))

.PHONY: all clean $(PROBLEMS) $(BIN_DIR) clang gcc FORCE

all: $(BIN_DIR) $(EXES)

# Force-compiler convenience targets
clang: FORCE
    $(MAKE) CC=clang all

gcc: FORCE
    $(MAKE) CC=gcc all

# Always-executed helper
FORCE:

# Pattern rule: build each problem's executable
$(BIN_DIR)/%: %/main.c $(LIBS)
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

# Ensure bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean all built executables
clean:
	rm -rf $(BIN_DIR)