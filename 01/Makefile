INPUTS=sample.txt
GCC_CFLAGS=-pedantic -std=c89 -Wall -Werror=missing-braces -Wextra -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes -pg
F_SANITIZE=-g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
VAL_PATH=/usr/local/valgrind-latest/bin/valgrind
GCC_PATH=/usr/local/gcc-12.1.0/bin/gcc-12.1
GCC_PERF=gperf
SOURCES:=main.c ../libs/eight_files.c ../libs/eight_algorithims.c

.PHONY: test clean SOURCES

test: 	latest $(INPUTS)
	./asan $(INPUTS)
	$(VAL_PATH) -s ./a.out $(INPUTS)

latest: $(SOURCES)
	$(GCC_PATH) $(GCC_CFLAGS) $(F_SANITIZE) $(SOURCES) $(F_SANITIZE) -static-libasan -o asan
	$(GCC_PATH) $(GCC_CFLAGS) $(SOURCES)

clean:	a.out asan gmon.out
	rm a.out
	rm asan
	rm gmon.out
