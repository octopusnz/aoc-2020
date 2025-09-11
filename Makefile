## Root Makefile for Advent of Code 2020 in C

# Common flags
COMMON_CFLAGS = -Ilibs \
                 -pedantic \
                 -std=c89 \
                 -Wall \
                 -Werror=missing-braces \
                 -Wextra \
                 -Wshadow \
                 -Wstrict-prototypes \
                 -pg

# GCC-only warnings that clang doesn't support
GCC_ONLY_CFLAGS = -Wduplicated-cond \
                  -Wduplicated-branches

# Clang-only flags
CLANG_ONLY_CFLAGS = \
         -Wnewline-eof \
         -Wmissing-variable-declarations \
         -Wshorten-64-to-32 \
         -Wpointer-bool-conversion \
         -Wconditional-uninitialized

# Allow external overrides
EXTRA_CFLAGS ?=

CFLAGS_gcc   = $(COMMON_CFLAGS) $(GCC_ONLY_CFLAGS) $(EXTRA_CFLAGS)
CFLAGS_clang = $(COMMON_CFLAGS) $(CLANG_ONLY_CFLAGS) $(EXTRA_CFLAGS)

LIBS = libs/eight_files.c libs/eight_algorithms.c
BIN_DIR = bin
UNITY_PATH = libs/unity/unity.c
TEST_PATH  = tests/test.c
TEST_SOURCES = $(UNITY_PATH) $(TEST_PATH) $(LIBS)

# Compilers to attempt
COMPILERS := gcc clang
AVAILABLE_COMPILERS := $(foreach C,$(COMPILERS),$(if $(shell command -v $(C) 2>/dev/null),$(C),))

# Find all problem folders with main.c
PROBLEM_SRCS := $(wildcard [0-9][0-9]/main.c)
PROBLEM_DIRS := $(sort $(dir $(PROBLEM_SRCS)))
PROBLEMS := $(patsubst %/,%, $(PROBLEM_DIRS))

# Executables per available compiler
EXES := $(foreach P,$(PROBLEMS),$(foreach C,$(AVAILABLE_COMPILERS),$(BIN_DIR)/$(P)-$(C)))

.PHONY: all clean $(PROBLEMS) gcc clang unity

all: $(BIN_DIR) $(EXES)

# Per-problem phony target builds all available compiler variants
$(PROBLEMS):
	@$(MAKE) $(foreach C,$(AVAILABLE_COMPILERS),$(BIN_DIR)/$@-$(C))

# Convenience targets for a single compiler
gcc: $(filter %-gcc,$(EXES))
	@if [ -z "$(filter gcc,$(AVAILABLE_COMPILERS))" ]; then echo "gcc not found"; exit 1; fi

clang: $(filter %-clang,$(EXES))
	@if [ -z "$(filter clang,$(AVAILABLE_COMPILERS))" ]; then echo "clang not found"; exit 1; fi

# Pattern rules per compiler
$(BIN_DIR)/%-gcc: %/main.c $(LIBS) | $(BIN_DIR)
	gcc $(CFLAGS_gcc) $< $(LIBS) -o $@

$(BIN_DIR)/%-clang: %/main.c $(LIBS) | $(BIN_DIR)
	clang $(CFLAGS_clang) $< $(LIBS) -o $@

# Ensure bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Unity test executables per compiler
UNITY_EXES = $(foreach C,$(AVAILABLE_COMPILERS),$(BIN_DIR)/unity-$(C))

# Run unity tests automatically after building them
unity test: $(UNITY_EXES)
	@echo "Running Unity tests..."
	@set -e; for exe in $(UNITY_EXES); do \
		echo "==> $$exe"; \
		./$$exe; \
	done
	@echo "All Unity tests completed."

$(BIN_DIR)/unity-gcc: $(TEST_SOURCES) | $(BIN_DIR)
	gcc $(CFLAGS_gcc) $(TEST_SOURCES) -o $@

$(BIN_DIR)/unity-clang: $(TEST_SOURCES) | $(BIN_DIR)
	clang $(CFLAGS_clang) $(TEST_SOURCES) -o $@

# Clean all built executables
clean:
	rm -rf $(BIN_DIR)