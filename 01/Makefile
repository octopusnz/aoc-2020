INPUTS=sample.txt
GCC:=gcc
CLANG:=clang
GCC_CFLAGS=-pedantic -std=c89 -Wall -Wextra -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes
CLANG_CFLAGS=-pedantic -std=c17 -Wall -Wextra
SOURCES:=  $(wildcard *.c)

.PHONY: test clean
.PHONY: test

test: 01_gcc $(INPUTS)
	./a.out $(INPUTS)

01_gcc: $(SOURCES)

	$(GCC) $(GCC_CFLAGS) $(SOURCES)

01_clang: $(SOURCES)
	$(CLANG) $(CLANG_CFLAGS) $(SOURCES)

clean:	a.out
	rm a.out
