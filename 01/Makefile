INPUTS=sample.txt
GCC:=gcc
CLANG:=clang
GCC_CFLAGS=-pedantic -std=c89 -Wall -Werror=missing-braces -Wextra -Wpadded -Wduplicated-cond -Wduplicated-branches -Wshadow -Wstrict-prototypes
CLANG_CFLAGS=-pedantic -std=c17 -Wall -Wextra
SOURCES:=  $(wildcard *.c)
LDFLAGS:=-fsanitize=address -fno-omit-frame-pointer

.PHONY: test clean
.PHONY: test

test: 01_gcc $(INPUTS)
	./a.out $(INPUTS)

latest: $(SOURCES)
	/usr/local/gcc-11.1.0/bin/gcc $(GCC_CFLAGS) -fsanitize=address -fno-omit-frame-pointer $(SOURCES) $(LDFLAGS)

01_gcc: $(SOURCES)
	$(GCC) $(GCC_CFLAGS) $(SOURCES)

01_clang: $(SOURCES)
	$(CLANG) $(CLANG_CFLAGS) $(SOURCES)

clean:	a.out
	rm a.out
