CC     = c99
CFLAGS = -Wall -Wextra -O0 -g3

all: simple sem setjmp ucp

simple: main.o tree.o iterator.o
	$(CC) $(LDFLAGS) -o $@ main.o tree.o iterator.o $(LDLIBS)

sem: main.o tree.o iterator_sem.o
	$(CC) $(LDFLAGS) -pthread -o $@ main.o tree.o iterator_sem.o $(LDLIBS)

setjmp: main.o tree.o iterator_setjmp.o
	$(CC) $(LDFLAGS) -o $@ main.o tree.o iterator_setjmp.o $(LDLIBS)

ucp: main.o tree.o iterator_ucp.o
	$(CC) $(LDFLAGS) -o $@ main.o tree.o iterator_ucp.o $(LDLIBS)

main.o: main.c tree.h
tree.o: tree.c tree.h
iterator.o: iterator.c tree.h
iterator_ucp.o: iterator_ucp.c tree.h
iterator_sem.o: iterator_sem.c tree.h
iterator_setjmp.o: iterator_setjmp.c tree.h

clean:
	rm -f main.o tree.o simple sem setjmp ucp
	rm -f iterator.o iterator_sem.o iterator_ucp.o iterator_setjmp.o
