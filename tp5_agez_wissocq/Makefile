ROOTDIR=/home/enseign/ASE

CC	= gcc
CFLAGS	= -Wall -ansi -pedantic 
CFLAGS  += -g -m32
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware


EXEC = mkhd dmps write format


all: $(EXEC)


### Objects files
drive.o: drive.c drive.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LIBS)

mkhd.o: mkhd.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCDIR)

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
	rm -rf *~ *.o *.bin format dmps write
# CC=gcc
# CFLAGS=-m32 -g
# EXEC=dmps write format
# CLIB=-L./lib -lhardware
# 
# all: $(EXEC)
# 
# drive.o: drive.c drive.h
# 	$(CC) -o $@ -c $< $(CFLAGS) $(LIB)
# 
# %.o : %.c
# 	$(CC) -o $@ -c $< $(CFLAGS)
# 
# dmps: dmps.o drive.o
# 	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)
# 
# write: write.o drive.o
# 	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)
# 
# format: format.o drive.o
# 	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)
# 
# 
# clean:
# 	rm -rf *~ *.o *.bin format dmps write
# 


