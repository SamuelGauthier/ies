CC=gcc
CFLAGS= -I.
LDFLAGS=
HFILES= utils.h
EXEC= ies

all: $(EXEC)

ies: ies.c utils.c
	$(CC) -o $@ $^ $(CFLAGS)
ies.o: ies.c $(HFILES)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)
