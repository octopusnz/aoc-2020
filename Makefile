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
# Preferred compiler commands (can be overridden at invocation)
GCC_COMPILER ?= gcc-latest
CLANG_COMPILER ?= clang-latest

# Detect available compiler commands with fallback to standard names
GCC_CANDIDATES   := $(GCC_COMPILER) gcc
CLANG_CANDIDATES := $(CLANG_COMPILER) clang

DETECTED_GCC   := $(firstword $(foreach c,$(GCC_CANDIDATES),$(if $(shell command -v $(c) 2>/dev/null),$(c))))
DETECTED_CLANG := $(firstword $(foreach c,$(CLANG_CANDIDATES),$(if $(shell command -v $(c) 2>/dev/null),$(c))))

# Environment overrides for command paths (shells can't export vars with dots)
ENV_COMPILER_CMD_GCC   := $(or $(COMPILER_cmd_gcc),$(COMPILER_CMD_GCC))
ENV_COMPILER_CMD_CLANG := $(or $(COMPILER_cmd_clang),$(COMPILER_CMD_CLANG))

# Compilers to attempt
# Stable labels for file suffixes
GCC_LABEL := gcc
CLANG_LABEL := clang

# Available compilers (by label) and their commands
COMPILER_cmd.gcc ?= $(ENV_COMPILER_CMD_GCC)
COMPILER_cmd.gcc ?= $(DETECTED_GCC)
COMPILER_cmd.clang ?= $(ENV_COMPILER_CMD_CLANG)
COMPILER_cmd.clang ?= $(DETECTED_CLANG)
AVAILABLE_COMPILERS := $(strip $(if $(COMPILER_cmd.gcc),$(GCC_LABEL)) $(if $(COMPILER_cmd.clang),$(CLANG_LABEL)))

# Find all problem folders with main.c
PROBLEM_SRCS := $(wildcard [0-9][0-9]/main.c)
PROBLEM_DIRS := $(sort $(dir $(PROBLEM_SRCS)))
PROBLEMS := $(patsubst %/,%, $(PROBLEM_DIRS))

# Executables per available compiler
EXES := $(foreach P,$(PROBLEMS),$(foreach C,$(AVAILABLE_COMPILERS),$(BIN_DIR)/$(P)-$(C)))

.PHONY: all clean $(PROBLEMS) gcc clang unity test

all: $(BIN_DIR) $(EXES)

# Per-problem phony target builds all available compiler variants
$(PROBLEMS):
	@$(MAKE) $(foreach C,$(AVAILABLE_COMPILERS),$(BIN_DIR)/$@-$(C))

# Convenience targets for a single compiler
gcc: $(filter %-$(GCC_LABEL),$(EXES))
	@if [ -z "$(COMPILER_cmd.$(GCC_LABEL))" ]; then echo "gcc not available"; exit 1; fi

clang: $(filter %-$(CLANG_LABEL),$(EXES))
	@if [ -z "$(COMPILER_cmd.$(CLANG_LABEL))" ]; then echo "clang not available"; exit 1; fi

# Pattern rules per compiler
$(BIN_DIR)/%-$(GCC_LABEL): %/main.c $(LIBS) | $(BIN_DIR)
	$(COMPILER_cmd.$(GCC_LABEL)) $(CFLAGS_gcc) $< $(LIBS) -o $@

$(BIN_DIR)/%-$(CLANG_LABEL): %/main.c $(LIBS) | $(BIN_DIR)
	$(COMPILER_cmd.$(CLANG_LABEL)) $(CFLAGS_clang) $< $(LIBS) -o $@

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
	@if [ -n "$(COMPILER_cmd.$(GCC_LABEL))" ]; then echo "GCC Version Used: $$($(COMPILER_cmd.$(GCC_LABEL)) --version | head -n 1)"; fi
	@if [ -n "$(COMPILER_cmd.$(CLANG_LABEL))" ]; then echo "Clang Version Used: $$($(COMPILER_cmd.$(CLANG_LABEL)) --version | head -n 1)"; fi

$(BIN_DIR)/unity-$(GCC_LABEL): $(TEST_SOURCES) | $(BIN_DIR)
	$(COMPILER_cmd.$(GCC_LABEL)) $(CFLAGS_gcc) $(TEST_SOURCES) -o $@

$(BIN_DIR)/unity-$(CLANG_LABEL): $(TEST_SOURCES) | $(BIN_DIR)
	$(COMPILER_cmd.$(CLANG_LABEL)) $(CFLAGS_clang) $(TEST_SOURCES) -o $@

# Clean all built executables
clean:
	rm -rf $(BIN_DIR)