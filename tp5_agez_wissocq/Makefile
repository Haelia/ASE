CC=gcc
CFLAGS=-m32 -g
EXEC=dmps write format
CLIB=-L./lib -lhardware

all: $(EXEC)

drive.o: drive.c drive.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LIB)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

dmps: dmps.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

write: write.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

format: format.o drive.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)


clean:
	rm -rf *~ *.o *.bin format dmps write
