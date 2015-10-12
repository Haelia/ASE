all:main


%.o : %.c %.h
	gcc -c $<

main : main.c semaphore.o
	gcc -o main hw.c semaphore.c ordonnancement.c main.c -m32

clean:
	rm -f *~ *.o *#
	rm main
