INPUTS=sample.txt finaldata.txt
GCC_CFLAGS=-pedantic -std=c89 -Wall -Werror=missing-braces -Wextra -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes -pg
CLANG_CFLAGS=-pedantic -std=c89 -Wall -Wextra -Wformat -Werror=format-security -Werror=array-bounds
UNAME_S:=$(shell uname -s)
# Address/Leak sanitizer flags differ by platform. macOS clang does not support
# -fsanitize=leak as a separate flag nor linking a static libasan.
ifeq ($(UNAME_S),Darwin)
	F_SANITIZE=-g -fsanitize=address -fno-omit-frame-pointer
	SAN_LIB_FLAG=
else
	F_SANITIZE=-g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
	SAN_LIB_FLAG=-static-libasan
endif
VAL_PATH=/usr/bin/valgrind
GCC_PATH=/usr/bin/gcc
CLANG_PATH=clang
UNITY_PATH=../libs/unity/unity.c
TEST_PATH=../tests/test.c
SOURCES:=main.c ../libs/eight_files.c ../libs/eight_algorithms.c
TEST_SOURCES:=$(UNITY_PATH) $(TEST_PATH) ../libs/eight_files.c ../libs/eight_algorithms.c

.PHONY: test clean latest unity

test: 	latest $(INPUTS)
	./asan.out $(INPUTS)
	@VAL=$$(command -v "$(VAL_PATH)" 2>/dev/null || true) ; \
	if [ -n "$$VAL" ]; then \
		"$$VAL" -s --tool=memcheck --leak-check=full ./clang.out $(INPUTS) ; \
	else \
		echo "valgrind not found (VAL_PATH=$(VAL_PATH)); skipping memcheck" ; \
	fi

latest: $(SOURCES)
	$(GCC_PATH) $(GCC_CFLAGS) $(F_SANITIZE) $(SOURCES) $(SAN_LIB_FLAG) -o asan.out
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
