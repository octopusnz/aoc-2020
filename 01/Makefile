INPUTS=sample.txt finaldata.txt
GCC_CFLAGS=-pedantic -std=c89 -Wall -Werror=missing-braces -Wextra -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes -pg
CLANG_CFLAGS=-pedantic -std=c89 -Wall -Wextra -Wformat -Werror=format-security -Werror=array-bounds
F_SANITIZE=-g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
VAL_PATH=/usr/local/valgrind-latest/bin/valgrind
GCC_PATH=/usr/bin/gcc
CLANG_PATH=clang
SOURCES:=main.c mainwrappers.c ../libs/eight_files.c ../libs/eight_algorithims.c ../libs/eight_time.c

.PHONY: test clean SOURCES

test: 	latest $(INPUTS)
	./asan.out $(INPUTS)
	$(VAL_PATH) -s --tool=memcheck --leak-check=full ./clang.out $(INPUTS)

latest: $(SOURCES)
	$(GCC_PATH) $(GCC_CFLAGS) $(F_SANITIZE) $(SOURCES) $(F_SANITIZE) -static-libasan -o asan.out
	$(GCC_PATH) $(GCC_CFLAGS) $(SOURCES) -o gcc.out
	$(CLANG_PATH) $(CLANG_CFLAGS) $(SOURCES) -o clang.out

clean:	gcc.out asan.out gmon.out clang.out
	rm gcc.out
	rm asan.out
	rm gmon.out
	rm clang.out
