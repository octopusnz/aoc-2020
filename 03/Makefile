INPUTS=sample.txt input.txt
GCC_CFLAGS=-pedantic -std=c89 -Wall -Werror=missing-braces -Wextra -Wformat=2 -Werror=implicit-function-declaration -Wmissing-prototypes -Wmissing-declarations -Wundef -Wpointer-arith -Wcast-qual -Wwrite-strings -Wswitch-enum -Wswitch-default -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes -pg
CLANG_CFLAGS=-pedantic -std=c89 -Wall -Wextra -Wformat -Wformat=2 -Werror=implicit-function-declaration -Wmissing-prototypes -Wmissing-declarations -Wundef -Wpointer-arith -Wcast-qual -Wwrite-strings -Wswitch-enum -Wswitch-default -Werror=format-security -Werror=array-bounds
UNAME_S:=$(shell uname -s)

# Address/Leak sanitizer flags differ by platform. macOS clang does not support
# -fsanitize=leak as a separate flag nor linking a static libasan.
ifeq ($(UNAME_S),Darwin)
	F_SANITIZE=-g -fsanitize=address -fno-omit-frame-pointer
	SAN_LIB_FLAG=
	SAN_CC=$(CLANG_PATH)
	SAN_CFLAGS=$(CLANG_CFLAGS)
	GCC_PATH=/opt/homebrew/bin/gcc-15
	# Homebrew GCC on macOS typically can't link profiling startup objects; drop -pg.
	GCC_CFLAGS := $(filter-out -pg,$(GCC_CFLAGS))
else
	F_SANITIZE=-g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
	SAN_LIB_FLAG=-static-libasan
	GCC_PATH=gcc
	SAN_CC=$(GCC_PATH)
	SAN_CFLAGS=$(GCC_CFLAGS)
endif
VAL_PATH=/usr/local/bin/valgrind
CLANG_PATH=clang
LEAKS ?= leaks

# If "gcc" is aliased/wrapped to clang, drop GCC-only warning flags.
GCC_IS_CLANG := $(shell $(GCC_PATH) --version 2>/dev/null | head -n 1 | grep -qi clang && echo 1 || echo 0)
ifeq ($(GCC_IS_CLANG),1)
	GCC_CFLAGS := $(filter-out -Wduplicated-cond -Wduplicated-branches,$(GCC_CFLAGS))
endif

UNITY_PATH=../libs/unity/unity.c
TEST_PATH=../tests/test.c
SOURCES:=main.c ../libs/eight_files.c ../libs/eight_algorithms.c
TEST_SOURCES:=$(UNITY_PATH) $(TEST_PATH) ../libs/eight_files.c ../libs/eight_algorithms.c

.PHONY: test clean latest unity

test: 	latest $(INPUTS)
	./asan.out $(INPUTS)
	@set -e; \
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
	

latest: $(SOURCES)
	$(SAN_CC) $(SAN_CFLAGS) $(F_SANITIZE) $(SOURCES) $(SAN_LIB_FLAG) -o asan.out
	$(GCC_PATH) $(GCC_CFLAGS) $(SOURCES) -o gcc.out
	$(CLANG_PATH) $(CLANG_CFLAGS) $(SOURCES) -o clang.out

unity: $(TEST_SOURCES)
	$(GCC_PATH) $(GCC_CFLAGS) $(TEST_SOURCES) -o unity.out

# Coverage build for tests (Linux/GCC)
unity-coverage: $(TEST_SOURCES)
	$(GCC_PATH) -std=c89 -Wall -Wextra -O0 --coverage $(TEST_SOURCES) -o unity_coverage.out

clean:	
	rm -f gcc.out
	rm -f  asan.out
	rm -f gmon.out
	rm -f clang.out
	rm -f unity.out unity_coverage.out
	rm -f *.gcno *.gcda ../libs/*.gcno ../libs/*.gcda
	rm -f main.s
