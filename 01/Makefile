INPUTS=sample.txt finaldata.txt test1.txt

# Base flags
GCC_CFLAGS=-pedantic -std=c89 -Wall -Werror=missing-braces -Wextra -Wformat=2 -Werror=implicit-function-declaration -Wmissing-prototypes -Wmissing-declarations -Wundef -Wpointer-arith -Wcast-qual -Wwrite-strings -Wswitch-enum -Wswitch-default -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes -pg
CLANG_CFLAGS=-pedantic -std=c89 -Wall -Wextra -Wformat -Wformat=2 -Werror=implicit-function-declaration -Wmissing-prototypes -Wmissing-declarations -Wundef -Wpointer-arith -Wcast-qual -Wwrite-strings -Wswitch-enum -Wswitch-default -Werror=format-security -Werror=array-bounds

# Detect host OS
UNAME_S := $(shell uname -s)

# Default tool paths (overridable by environment)
ifeq ($(UNAME_S),Darwin)
GCC_PATH ?= /opt/homebrew/bin/gcc-15
else
GCC_PATH ?= gcc
endif
CLANG_PATH ?= clang
VAL_PATH ?= /usr/local/bin/valgrind
LEAKS ?= leaks

# Sources
SOURCES := main.c ../libs/eight_files.c ../libs/eight_algorithms.c

# Sanitizer flags by OS
ifeq ($(UNAME_S),Darwin)
  # macOS + Apple Clang: no -fsanitize=leak, no static ASan
  F_SANITIZE := -g -fno-omit-frame-pointer -fsanitize=address
  STATIC_ASAN :=
  # On macOS, the default "gcc" may be Clang; make that explicit to avoid surprises
  GCC_PATH := clang
else
  # Linux (GCC/Clang): address + leak usually OK; static ASan only for GCC
  F_SANITIZE := -g -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak
  # Static ASan can be flaky on some distros; leave enabled by default, override with STATIC_ASAN=
  STATIC_ASAN := -static-libasan
endif

# If "gcc" is aliased/wrapped to clang (common on macOS), drop GCC-only warning flags.
GCC_IS_CLANG := $(shell $(GCC_PATH) --version 2>/dev/null | head -n 1 | grep -qi clang && echo 1 || echo 0)
ifeq ($(GCC_IS_CLANG),1)
  GCC_CFLAGS := $(filter-out -Wduplicated-cond -Wduplicated-branches,$(GCC_CFLAGS))
endif

.PHONY: test clean latest unity unity-coverage

test: latest $(INPUTS) ; @./asan.out $(INPUTS); \
  set -e; \
  if [ "$(UNAME_S)" = "Darwin" ]; then \
    LEAKS_PATH=$$(command -v "$(LEAKS)" 2>/dev/null || true); \
    if [ -n "$$LEAKS_PATH" ]; then \
      "$$LEAKS_PATH" --atExit -- ./clang.out $(INPUTS); \
      exit 0; \
    fi; \
  fi; \
  VAL=$$(command -v "$(VAL_PATH)" 2>/dev/null || true) ; \
  if [ -n "$$VAL" ]; then \
    "$$VAL" -s --tool=memcheck --leak-check=full ./clang.out $(INPUTS) ; \
  else \
    echo "valgrind not found (VAL_PATH=$(VAL_PATH)); skipping memcheck" ; \
  fi

latest: $(SOURCES) ; $(GCC_PATH) $(GCC_CFLAGS) $(F_SANITIZE) $(SOURCES) $(STATIC_ASAN) -o asan.out ; $(GCC_PATH) $(GCC_CFLAGS) $(SOURCES) -o gcc.out ; $(CLANG_PATH) $(CLANG_CFLAGS) $(SOURCES) -o clang.out

UNITY_PATH=../libs/unity/unity.c
TEST_PATH=../tests/test.c
TEST_SOURCES=$(UNITY_PATH) $(TEST_PATH) ../libs/eight_files.c ../libs/eight_algorithms.c

unity: $(TEST_SOURCES) ; $(GCC_PATH) $(GCC_CFLAGS) $(TEST_SOURCES) -o unity.out

# Coverage build for tests (Linux/GCC)
unity-coverage: $(TEST_SOURCES) ; $(GCC_PATH) -std=c89 -Wall -Wextra -O0 --coverage $(TEST_SOURCES) -o unity_coverage.out

clean: ; rm -f gcc.out asan.out gmon.out clang.out ; rm -f unity.out unity_coverage.out ; rm -f *.gcno *.gcda ../libs/*.gcno ../libs/*.gcda main.s
