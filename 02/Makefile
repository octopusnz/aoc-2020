INPUTS=sample.txt finaldata.txt
GCC_CFLAGS=-pedantic -std=c89 -Wall -Werror=missing-braces -Wextra -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes -pg
CLANG_CFLAGS=-pedantic -std=c89 -Wall -Wextra -Wformat -Werror=format-security -Werror=array-bounds
F_SANITIZE=-g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
VAL_PATH=/usr/local/valgrind-latest/bin/valgrind
GCC_PATH=/usr/bin/gcc
CLANG_PATH=clang
UNITY_PATH=../libs/unity/unity.c
TEST_PATH=../tests/test.c
SOURCES:=main.c ../libs/eight_files.c ../libs/eight_algorithms.c
TEST_SOURCES:=$(UNITY_PATH) $(TEST_PATH) ../libs/eight_files.c ../libs/eight_algorithms.c

.PHONY: test clean latest unity

test: 	latest $(INPUTS)
	./asan.out $(INPUTS)
	$(VAL_PATH) -s --tool=memcheck --leak-check=full ./clang.out $(INPUTS)
	

latest: $(SOURCES)
	$(GCC_PATH) $(GCC_CFLAGS) $(F_SANITIZE) $(SOURCES) -static-libasan -o asan.out
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
