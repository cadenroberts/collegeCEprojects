CC= clang
CFLAGS= -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
OBJS= pq.o bitwriter.o bitreader.o brtest.o bwtest.o node.o pqtest.o nodetest.o
HEAD= node.h pq.h bitwriter.h bitreader.h
EXEC=
TESTS= huff dehuff pqtest nodetest brtest bwtest
.PHONY: clean format scan-build

all: huff dehuff bwtest brtest pqtest nodetest

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

huff: huff.o node.o pq.o bitwriter.o bitreader.o
	$(CC) $^ -o $@

dehuff: dehuff.o node.o pq.o bitwriter.o bitreader.o
	$(CC) $^ -o $@

bwtest: bwtest.o node.o pq.o bitwriter.o bitreader.o
	$(CC) $^ -o $@

pqtest: pqtest.o pq.o node.o
	$(CC) $^ -o $@

nodetest: nodetest.o node.o
	$(CC) $^ -o $@

brtest: brtest.o node.o pq.o bitwriter.o bitreader.o
	$(CC) $^ -o $@

clean: 
	rm -f $(TESTS) *.o
	rm -f brtest.in bwtest.out

scan-build: clean
	scan-build --use-cc=clang make

format:
	clang-format -i -style=file *.[ch]
