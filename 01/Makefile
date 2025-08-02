INPUTS=sample.txt finaldata.txt test1.txt
GCC_CFLAGS=-pedantic -std=c89 -Wall -Werror=missing-braces -Wextra -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes -pg
CLANG_CFLAGS=-pedantic -std=c89 -Wall -Wextra -Wformat -Werror=format-security -Werror=array-bounds
F_SANITIZE=-g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
VAL_PATH=/usr/local/valgrind-latest/bin/valgrind
GCC_PATH=/usr/bin/gcc
CLANG_PATH=clang
SOURCES:=main.c ../libs/eight_files.c ../libs/eight_algorithms.c

.PHONY: test clean 

test: 	latest $(INPUTS)
	./asan.out $(INPUTS)
	$(VAL_PATH) -s --tool=memcheck --leak-check=full ./clang.out $(INPUTS)

latest: $(SOURCES)
	$(GCC_PATH) $(GCC_CFLAGS) $(F_SANITIZE) $(SOURCES) -static-libasan -o asan.out
	$(GCC_PATH) $(GCC_CFLAGS) $(SOURCES) -o gcc.out
	$(CLANG_PATH) $(CLANG_CFLAGS) $(SOURCES) -o clang.out

clean:
	rm gcc.out
	rm asan.out
	rm gmon.out
	rm clang.out
