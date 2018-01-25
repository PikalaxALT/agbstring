.PHONY: all agbstring clean

CC := gcc
CFLAGS := -O3

all: agbstring

agbstring: main.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f agbstring
