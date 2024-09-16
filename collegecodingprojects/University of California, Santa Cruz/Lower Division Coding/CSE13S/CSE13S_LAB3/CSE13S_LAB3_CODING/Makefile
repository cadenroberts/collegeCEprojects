CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS =
EXEC = xd

all: $(EXEC)

$(EXEC): xd.o
	$(CC) $^ $(LFLAGS) -o $@

xxd.o: xd.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXEC) *.o

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format

