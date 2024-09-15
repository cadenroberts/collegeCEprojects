CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = -lm
EXEC = tests
header = operators

all: calc test


calc: calc.o stack.o mathlib.o operators.o
	$(CC) $^ $(LFLAGS) -o $@ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf calc *.o
	rm -rf test *.o

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format

test: $(EXEC).o operators.o stack.o mathlib.o
	$(CC) $^ $(LFLAGS) -o $@

