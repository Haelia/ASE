ROOTDIR	= .

CC	= gcc
CFLAGS	= -Wall -ansi -pedantic -g -m32
LIBDIR	= $(ROOTDIR)/lib
LIBS	= -L$(LIBDIR) -lhardware


EXEC = dmps write format


all: $(EXEC)


### Objects files
drive.o: drive.c drive.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LIBS)


%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)


### Executables files

dmps: dmps.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

write: write.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

format: format.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean realclean

clean:
	rm -rf *~ *.o $(EXEC)

realclean:
	rm -rf *~ *.o *.bin $(EXEC)
