ROOTDIR=.

CC	= gcc
CFLAGS	= -Wall -ansi -pedantic -g
LIBDIR	= $(ROOTDIR)/lib
LIBS	= -L$(LIBDIR) -lhardware


EXEC = dmps write format mkhd


all: $(EXEC)


### Objects files
drive.o: drive.c drive.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LIBS)

mkhd.o: mkhd.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LIBS)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)


### Executables files
mkhd: mkhd.o
	$(CC) -o $@ $< $(CFLAGS) $(LIBS)

dmps: dmps.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

write: write.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

format: format.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -rf *~ *.o *.bin $(EXEC)