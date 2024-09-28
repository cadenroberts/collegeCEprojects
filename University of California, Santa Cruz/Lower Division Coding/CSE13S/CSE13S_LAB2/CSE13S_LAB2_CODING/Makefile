CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = 
EXEC = hangman
HEADERS = hangman_helpers.h

all: $(EXEC)

$(EXEC): $(EXEC).o hangman_helpers.o
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) *.o

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format

tests: test_functionality test_helpers

test_functionality:
	sh test_functionality.sh

test_hgelpers: test_helpers.o hangman_helpers.o
	$(CC) $^ $(LFLAGS) -o $@
	./test_helpers

test_helpers.o: test_helpers.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<
