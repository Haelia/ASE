CC=gcc
CFLAGS=-m32 -g
CLIB=-I./include -L./lib -lhardware
EXEC=handler


all: $(EXEC)

handler: handler.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

.PHONY: clean realclean test

clean:
	rm -rf *~ *.o

realclean:
	make clean
	rm -rf $(EXEC) vdisk*.bin
