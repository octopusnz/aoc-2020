INPUTS=sample.txt
GCC_CFLAGS=-pedantic -std=c17 -Wall -Wextra -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes
CLANG_CFLAGS=-pedantic -std=c17 -Wall -Wextra
SOURCES:=  $(wildcard *.c)

.PHONY: test

test: 01_gcc $(INPUTS)
	./a.out $(INPUTS)

01_gcc: $(SOURCES)
	$(CC) $(GCC_CFLAGS) $(SOURCES)

01_clang: $(SOURCES)
	$(CC) $(CLANG_CFLAGS) $(SOURCES)
