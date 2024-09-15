CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = 
EXEC = uniqcounter
HEADERS = item.h ll.h hash.h dictwords.h

all: toy bench1 bench2 uniqq

$(EXEC): main.o ll.o item.o bench1.o bench2.o uniqq.o toy.o
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

toy: toy.o ll.o item.o
	$(CC) $^ $(LFLAGS) -o $@

bench1: dictwords_list.o abstractwords.o bench1.o ll.o item.o
	$(CC) $^ $(LFLAGS) -o $@

bench2: dictwords_hash.o abstractwords.o bench2.o hash.o ll.o item.o badhash.o
	$(CC) $^ $(LFLAGS) -o $@
	
uniqq: uniqq.o hash.o ll.o item.o badhash.o
	$(CC) $^ $(LFLAGS) -o $@

clean:
	rm -rf $(EXEC) *.o
	rm -rf toy bench1 bench2 uniqq

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format
