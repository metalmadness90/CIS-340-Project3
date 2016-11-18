CC = gcc
CFLAGS = -std=gnu99 -Wall -g -I.
DEPS = main.h
OBJ = main.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run:
	./shell

.PHONY: clean
clean:
	rm -f *.o *~ shell
